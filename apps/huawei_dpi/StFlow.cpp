/*
 * StFlow.cpp
 * - keep track of flow states
 */

#include <list>
#include <inttypes.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "common.hpp"
#include "HashFunc.hpp"
#include "Util.hpp"
#include "StFlow.hpp"

using namespace std;

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
    f->pkt_list = new list<Packet::TotalField>();
}

void 
StFlow::flow_free_fcn(FLOW* f, void* st_arg) {
    if (f->is_finished == false) {
        std::vector<FLOW>* flows = &(((StFlow*)
                    st_arg)->unexported_flows);
        flows->push_back(*f);
    }
    else
        delete f->pkt_list;

    memset(f, 0, sizeof(FLOW));
    f->pkt_list = new list<Packet::TotalField>();
}

void 
StFlow::flow_travel_do_fcn(FLOW* f, void* st_arg) {
    if (f->is_finished == false) {
        std::vector<FLOW>* flows = &(((StFlow*)
                    st_arg)->unexported_flows);
        flows->push_back(*f);
    }
    else
        delete f->pkt_list;
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
    if (flow_table_ == NULL) {
        fprintf(stderr, "ERR: out of memory - create flow_table_\n");
        exit(-1);
    }
    flow_table2_ = new HashTable<FLOW>("StFlow", 
            flow_table_size_ / 2, flow_table_timeout_ / 5,
            flow_hash_fcn, flow_cmp_fcn, 
            flow_init_fcn, flow_free_fcn, flow_travel_do_fcn, this);
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
        if (p.get_src_ip() < p.get_dst_ip()) {
            f->field.orig_ip = p.get_src_ip();
            f->field.resp_ip = p.get_dst_ip();
            f->field.orig_port = p.get_src_port();
            f->field.resp_port = p.get_dst_port();
            f->field.proto = p.get_proto();
        }
        else {
            f->field.orig_ip = p.get_dst_ip();    
            f->field.resp_ip = p.get_src_ip();
            f->field.orig_port = p.get_dst_port();
            f->field.resp_port = p.get_src_port();
            f->field.proto = p.get_proto();
        }
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
StFlow::update_state(Packet& p) {
    
    // get the current flow state
    FLOW* f = (FLOW*)(p.get_flow_state());
    if (f == NULL) {        // unexpected error - shouldn't happen
        return NULL;
    }

    f->pkt_list->push_back(p.get_tf());

    //static int count = 0;
    //Packet::TotalField first = f->pkt_list->front();
    //if (first.bf.status == STATUS_VALID && p.get_tf().bf.status == STATUS_VALID) {
        //struct ip *ip_hdr = (struct ip *) (first.raw_data + 14);
        //uint32_t t_key = ip_hdr->ip_src.s_addr + ip_hdr->ip_dst.s_addr;
        //ip_hdr = (struct ip *) (p.get_tf().raw_data + 14);
        //uint32_t c_key = ip_hdr->ip_src.s_addr + ip_hdr->ip_dst.s_addr;
        //if (t_key != c_key) {
            //count++;
            //printf("Packets in one flow served by different SGSN: %d, "
                    //"seq = %" PRIu64 "\n", count, p.get_tf().bf.seq);
        //}
    //}
    
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
    } else {
        // ii) resp-to-orig direction
        if (f->field.r2o_first_ts == 0) {    
            f->field.r2o_first_ts = p.get_pkt_ts();
            //is_new_bi_flow = true;
        } 
        f->field.r2o_last_ts = p.get_pkt_ts();
        f->field.r2o_pkt_cnt++;
        f->field.r2o_byte_cnt += p.get_ip_dlen();
    }

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
