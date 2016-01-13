/*
 * StFlow.cpp
 * - keep track of flow states
 */

#include <inttypes.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "common.hpp"
#include "HashFunc.hpp"
#include "Util.hpp"
#include "StFlow.hpp"

/****************************************************************************
 * Private static member functions
 ****************************************************************************/
unsigned int 
StFlow::flow_hash_fcn(const FLOW* f) {
    char key[7];
    unsigned int a = f->field.orig_ip + f->field.resp_ip;
    unsigned short b = f->field.orig_port + f->field.resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->field.proto);
    return HashFunc(key, 7);
}

bool 
StFlow::flow_cmp_fcn(const FLOW* f1, const FLOW* f2) {
    return ((f1->field.orig_ip == f2->field.orig_ip && f1->field.resp_ip == f2->field.resp_ip && 
                f1->field.orig_port == f2->field.orig_port && 
                f1->field.resp_port == f2->field.resp_port && 
                f1->field.proto == f2->field.proto) ||
            (f1->field.orig_ip == f2->field.resp_ip && f1->field.resp_ip == f2->field.orig_ip && 
             f1->field.orig_port == f2->field.resp_port && 
             f1->field.resp_port == f2->field.orig_port &&
             f1->field.proto == f2->field.proto));
}

void 
StFlow::flow_init_fcn(FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(FLOW));
}

void 
StFlow::flow_free_fcn(FLOW* f, void* st_arg) {
    if (f->field.o2r_pkt_cnt+f->field.r2o_pkt_cnt > f->field.export_pkt_cnt) {
        std::vector<FLOW>* flows = &(((StFlow*) st_arg)->unexported_flows);
        flows->push_back(*f);
    }
    /*
            if ((f->field.resp_ip == 0x7f21700a  && f->field.orig_port == 3128 && f->field.resp_port == 1672) || (f->field.orig_ip == 0x7f21700a  && f->field.resp_port == 3128 && f->field.orig_port == 1672)) {
                char addr1[30], addr2[30];
                LOG_MSG("free flow: %s:%d <-> %s:%d, data: %u\n", ip2a(f->field.orig_ip, addr1), f->field.orig_port, ip2a(f->field.resp_ip, addr2), f->field.resp_port, f->field.o2r_data_cnt+f->field.r2o_data_cnt);
            }
    */

    memset(f, 0, sizeof(FLOW));
}

void 
StFlow::flow_travel_do_fcn(FLOW* f, void* context) {
    if (f->field.o2r_pkt_cnt+f->field.r2o_pkt_cnt > f->field.export_pkt_cnt) {
        std::vector<FLOW>* flows = &(((StFlow*) context)->unexported_flows);
        flows->push_back(*f);
    }
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * Constructor
 *
 * @param flow_table_size - flow table size
 * @param flow_table_timeout - flow table timeout
 * @param pair_port_table_size
 * @param pair_port_table_timeout
 */    
StFlow::StFlow(uint32_t flow_table_size, double flow_table_timeout) {
    
    flow_table_ = NULL;
    flow_table2_ = NULL;
    flow_table_size_ = flow_table_size;
    flow_table_timeout_ = flow_table_timeout;
}

/*
 * Create flowtable
 */
void
StFlow::create_table() {
    flow_table_ = new HashTable<FLOW>("StFlow", 
            flow_table_size_, flow_table_timeout_,
            flow_hash_fcn, flow_cmp_fcn, 
            flow_init_fcn, flow_free_fcn, flow_travel_do_fcn, this);
            //flow_init_fcn, flow_free_fcn, this);
    if (flow_table_ == NULL) {
        fprintf(stderr, "ERR: out of memory - create flow_table_\n");
        exit(-1);
    }
    flow_table2_ = new HashTable<FLOW>("StFlow", 
            flow_table_size_ / 2, flow_table_timeout_ / 5,
            flow_hash_fcn, flow_cmp_fcn, 
            flow_init_fcn, flow_free_fcn, flow_travel_do_fcn, this);
            //flow_init_fcn, flow_free_fcn, this);
    if (flow_table2_ == NULL) {
        fprintf(stderr, "ERR: out of memory - create flow_table2_\n");
        exit(-1);
    }
}

/*
 * Link flowstate to the packet
 *
 * @param p - decoded packet
 */
void 
StFlow::link_state(Packet& p) {
    char key_str[16];
    FLOW* key_f = (FLOW*)key_str;    // flow that stores the key
    FLOW* f;                        // pointer to the flow state

    // same hash row is returned for the either orig-to-resp or resp-to-orig 
    // direction    
    key_f->field.orig_ip = p.get_src_ip();    
    key_f->field.resp_ip = p.get_dst_ip();
    key_f->field.orig_port = p.get_src_port();
    key_f->field.resp_port = p.get_dst_port();
    key_f->field.proto = p.get_proto();
    
    f = flow_table2_->find(key_f, p.get_pkt_ts(), false);
    if (f == NULL)
        f = flow_table_->find(key_f, p.get_pkt_ts(), true);

    if (f->field.orig_ip == 0 && f->field.resp_ip == 0) {    // f is new entry
        // set the key
        f->field.orig_ip = p.get_src_ip();
        f->field.resp_ip = p.get_dst_ip();
        f->field.orig_port = p.get_src_port();
        f->field.resp_port = p.get_dst_port();
        f->field.proto = p.get_proto();
    }

    // set the state, and also the originator/responder IPs, and resp. port
    p.set_flow_state(f);
}
    
/*
 * Update the flow state
 *
 * @param p - decoded packet
 */
FLOW*
StFlow::update_state(Packet& p, unsigned char* data, uint32_t data_len) {
    
    // get the current flow state
    FLOW* f = (FLOW*)(p.get_flow_state());
    if (f == NULL) {        // unexpected error - shouldn't happen
        return NULL;
    }
    
    //bool is_new_uni_flow = false;
    //bool is_new_bi_flow = false;

    // (I) flow management

    // set the start time of a flow    
    if (f->field.flow_start_ts == 0) {
        f->field.flow_start_ts = p.get_pkt_ts();
    }

    // update the volume and direction change in either direction
    if (f->field.orig_ip == p.get_src_ip()) {
        // i) orig-to-resp direction
        if (f->field.o2r_first_ts == 0) {
            f->field.o2r_first_ts = p.get_pkt_ts();
            //is_new_uni_flow = true;
        }
        f->field.o2r_last_ts = p.get_pkt_ts();
        f->field.o2r_pkt_cnt++;
        f->field.o2r_byte_cnt += p.get_ip_dlen();

        // contain payload
        // currently, date = IP header + IP payload
        //if (p.get_data_len() > 0) {
        if (data_len > 0) {
            // should copy ?
            if (MAX_PAYLOAD_LENGTH > (f->field.o2r_data_cnt+f->field.r2o_data_cnt)) {
                uint32_t rest_len = MAX_PAYLOAD_LENGTH - (f->field.o2r_data_cnt + f->field.r2o_data_cnt);
                int cpy_len = rest_len >= data_len ? data_len : rest_len;
                char* addr = (char*)(f->payload);
                memcpy(addr + f->field.o2r_data_cnt + f->field.r2o_data_cnt, data, cpy_len);

                // update payload length
                f->field.o2r_data_cnt += cpy_len;
            }
        }
    } else {
        // ii) resp-to-orig direction
        if (f->field.r2o_first_ts == 0) {    
            f->field.r2o_first_ts = p.get_pkt_ts();
            //is_new_bi_flow = true;
        } 
        f->field.r2o_last_ts = p.get_pkt_ts();
        f->field.r2o_pkt_cnt++;
        f->field.r2o_byte_cnt += p.get_ip_dlen();

        if (data_len > 0) {
            if (MAX_PAYLOAD_LENGTH > (f->field.o2r_data_cnt+f->field.r2o_data_cnt)) {
                uint32_t rest_len = MAX_PAYLOAD_LENGTH - (f->field.o2r_data_cnt + f->field.r2o_data_cnt);
                int cpy_len = (uint32_t)rest_len >= data_len ? data_len : rest_len;
                char* addr = (char*)(f->payload);
                memcpy(addr + f->field.o2r_data_cnt + f->field.r2o_data_cnt, data, cpy_len);
                f->field.r2o_data_cnt += cpy_len;
            }
        }
    }

	struct ip* ip_hdr = (struct ip*) p.get_tf().payload;
	if (ip_hdr->ip_p == IPPROTO_TCP) {
		struct tcphdr* tcp_hdr = (struct tcphdr*)((uint8_t*)ip_hdr + (ip_hdr->ip_hl << 2));
        if (tcp_hdr->fin && flow_table_->del(f)) {
            FLOW tmp = *f;
            memset(f, 0, sizeof(FLOW));
            f = flow_table2_->find(&tmp, p.get_pkt_ts(), true);
            *f = tmp;
            p.set_flow_state(f);
            //char addr1[30], addr2[30];
            //printf("flow (timeout changed): %s:%d -> %s:%d\n", ip2a(f->orig_ip, addr1), f->orig_port, ip2a(f->resp_ip, addr2), f->resp_port);
        }
    }

    // determine if syn/fin flags are set
    /*
    if (is_new_uni_flow) {
        f->is_tcpsyn = p.is_tcpsyn();
        char addr1[30], addr2[30];
        printf("new flow: %s:%d -> %s:%d\n\n", ip2a(f->orig_ip, addr1), f->orig_port, ip2a(f->resp_ip, addr2), f->resp_port);
    }
    if (f->is_tcpfin == false) {
        f->is_tcpfin = p.is_tcpfin(); 
    }
    */

    return f;
}

/*
 * Flush all entries
 */
void
StFlow::flush() {
    flow_table_->reset();
    flow_table2_->reset();
}

/*
 * go through all flows
 */
void
StFlow::travel_flows() {
    flow_table_->travel_table();
    flow_table2_->travel_table();
}
