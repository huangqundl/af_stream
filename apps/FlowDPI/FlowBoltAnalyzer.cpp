#include "FlowBoltComputeThread.hpp"

#include <inttypes.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

extern "C" {
#include "ip_fragment.h"
}

#include "common.hpp"
#include "Config.hpp"

#define CLEAR_THRESHOLD 0

/****************************************************************************
 * functions from class ComputeThreads
 ****************************************************************************/
void FlowBoltComputeThread::process(Packet::TotalField& tf) {
    count++;
    if (count == 1) {
        last_ts = tf.bf.pkt_ts;
    }
    double cur_ts = tf.bf.pkt_ts;
    if ((uint64_t)cur_ts - (uint64_t)last_ts >= flowBoltTimeout) {
        char tmp[sizeof(FLOW::Field)];
        FLOW* tmpFlow = (FLOW*)tmp;
        tmpFlow->field.has_export = 1;
        emit(*tmpFlow, E_TIMEOUT);
        last_ts = tf.bf.pkt_ts;
    }

    // link flow state
    struct ip *iph;
    int need_free = 0;
    switch (ip_defrag_stub((struct ip *) (&tf.payload), &iph)) {
        case IPF_ISF:
            return;
        case IPF_NOTF:
            need_free = 0;
            iph = (struct ip *) tf.payload;
            break;
        case IPF_NEW:
            need_free = 1;
	        if (iph->ip_p == IPPROTO_TCP) {
                // TCP
		        struct tcphdr* tcp_hdr = (struct tcphdr*)((uint8_t*)iph + (iph->ip_hl << 2));
		        tf.bf.src_port = ntohs(tcp_hdr->source);
		        tf.bf.dst_port = ntohs(tcp_hdr->dest);
	        }
            else if (iph->ip_p == IPPROTO_UDP) {
                // UDP
		        struct udphdr* udp_hdr = (struct udphdr*)((uint8_t*)iph + (iph->ip_hl << 2));
		        tf.bf.src_port = ntohs(udp_hdr->source);
		        tf.bf.dst_port = ntohs(udp_hdr->dest);
	        } else {
                // Other L4
		        tf.bf.src_port = 0;
		        tf.bf.dst_port = 0;
	        }
            break;
        default:
            break;
    }
	p.get_tf() = tf;

    st_flow_->link_state(p);        
    
    // update flow state
    FLOW* f = st_flow_->update_state(p, (unsigned char*)iph, ntohs(iph->ip_len));
    /*
            if ((f->field.resp_ip == 0x7f21700a  && f->field.orig_port == 3128 && f->field.resp_port == 1672) || (f->field.orig_ip == 0x7f21700a  && f->field.resp_port == 3128 && f->field.orig_port == 1672)) {
                char addr1[30], addr2[30];
                LOG_MSG("flow: %s:%d <-> %s:%d, data: %u\n", ip2a(f->field.orig_ip, addr1), f->field.orig_port, ip2a(f->field.resp_ip, addr2), f->field.resp_port, f->field.o2r_data_cnt+f->field.r2o_data_cnt);
            }
            */
    if (f != NULL) {
        if (!(f->field.has_export) && (f->field.o2r_data_cnt+f->field.r2o_data_cnt==MAX_PAYLOAD_LENGTH)) {
            export_pkt += f->field.o2r_pkt_cnt + f->field.r2o_pkt_cnt - f->field.export_pkt_cnt;
            emit(*f);
            f->field.has_export = 1;
            f->field.export_pkt_cnt = f->field.o2r_pkt_cnt + f->field.r2o_pkt_cnt;
            f->field.export_byte_cnt = f->field.o2r_byte_cnt + f->field.r2o_byte_cnt;
        }
        else if (f->field.has_export && (f->field.o2r_byte_cnt+f->field.r2o_byte_cnt-f->field.export_byte_cnt > MAX_PAYLOAD_LENGTH)) {
            export_pkt += f->field.o2r_pkt_cnt + f->field.r2o_pkt_cnt - f->field.export_pkt_cnt;
            emit(*f);
            f->field.export_pkt_cnt = f->field.o2r_pkt_cnt + f->field.r2o_pkt_cnt;
            f->field.export_byte_cnt = f->field.o2r_byte_cnt + f->field.r2o_byte_cnt;
        }
    }
   
    std::vector<FLOW>* unexported_flows = st_flow_->get_unexported_flows();
    if (unexported_flows->size() > CLEAR_THRESHOLD) {
        int size= unexported_flows->size();
        expire_cnt += size;
        for (int i=0; i<size; ++i) {
            uint64_t tmp = (*unexported_flows)[i].field.o2r_pkt_cnt + (*unexported_flows)[i].field.r2o_pkt_cnt - (*unexported_flows)[i].field.export_pkt_cnt;
            export_pkt += tmp;
            expire_pkt += tmp;
            emit((*unexported_flows)[i]);
        }
        // unexported_flows->clear();
        std::vector<FLOW> tmp;
        unexported_flows->swap(tmp);
    }

	p.reset();
    if (need_free)
      free(iph);
}

void FlowBoltComputeThread::finish() {
    std::vector<FLOW>* unexported_flows = st_flow_->get_unexported_flows();
    st_flow_->travel_flows();
    uint32_t size = unexported_flows->size();
    for (uint32_t i=0; i<size; ++i) {
        FLOW f = (*unexported_flows)[i];
        export_pkt += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
        emit(f);
        f.field.export_pkt_cnt = f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt;
        f.field.export_byte_cnt = f.field.o2r_byte_cnt + f.field.r2o_byte_cnt;
    }

    LOG_MSG("ComputeThread %d (%d) processed %" PRIu64 ", export pkt %" PRIu64 " expire cnt %" PRIu64 " pkt %" PRIu64 "\n", subId, getpid(), count, export_pkt, expire_cnt, expire_pkt);
    ComputeThread<Packet::TotalField, FLOW>::finish();
}
    
#ifdef LOAD_BALANCE
int FlowBoltComputeThread::get_exported_data(Packet::TotalField& tf, char* data, uint32_t* len, uint32_t *key) {
	p.get_tf() = tf;
    st_flow_->link_state(p);        
    FLOW* f = (FLOW*)(p.get_flow_state());
    if (f == NULL) {
	    p.reset();
        LOG_MSG("Error: no such flow\n");
        return -1;
    }
    
    memcpy(data, f, sizeof(FLOW::Field));
    *len = sizeof(FLOW::Field);
    *key = get_state_key(tf);
    f->field.export_pkt_cnt = f->field.o2r_pkt_cnt + f->field.r2o_pkt_cnt;
    f->field.export_byte_cnt = f->field.o2r_byte_cnt + f->field.r2o_byte_cnt;
    if (f->field.has_export) {
        export_pkt += f->field.o2r_pkt_cnt + f->field.r2o_pkt_cnt - f->field.export_pkt_cnt;
        emit(*f, E_LOAD_BALANCE_EXPORT);
    }
    else {
        LOG_DEBUG("haven't export\n");
    }
	p.reset();
    return 0;
}
    
int FlowBoltComputeThread::import_and_merge_data(char* data, uint32_t len) {
    FLOW* old_f = (FLOW*)data;
    p.get_tf().bf.src_ip = old_f->field.orig_ip;
    p.get_tf().bf.src_port = old_f->field.orig_port;
    p.get_tf().bf.dst_ip = old_f->field.resp_ip;
    p.get_tf().bf.dst_port = old_f->field.resp_port;
    p.get_tf().bf.proto = old_f->field.proto;
    st_flow_->link_state(p);        
    FLOW* new_f = (FLOW*)(p.get_flow_state());
    if (new_f == NULL) {
	    p.reset();
        LOG_MSG("Error: no such flow\n");
        return -1;
    }
        
    new_f->field.o2r_pkt_cnt += old_f->field.o2r_pkt_cnt;
    new_f->field.r2o_pkt_cnt += old_f->field.r2o_pkt_cnt;
    new_f->field.o2r_byte_cnt += old_f->field.o2r_byte_cnt;
    new_f->field.r2o_byte_cnt += old_f->field.r2o_byte_cnt;
    new_f->field.export_pkt_cnt += old_f->field.export_pkt_cnt;
    new_f->field.export_byte_cnt += old_f->field.export_byte_cnt;
    new_f->field.has_export = old_f->field.has_export;
    new_f->field.o2r_data_cnt = old_f->field.o2r_data_cnt;
    new_f->field.r2o_data_cnt = old_f->field.r2o_data_cnt;
    export_pkt += new_f->field.o2r_pkt_cnt + new_f->field.r2o_pkt_cnt - new_f->field.export_pkt_cnt;
    emit(*new_f, E_LOAD_BALANCE_NEW);
    new_f->field.export_pkt_cnt = new_f->field.o2r_pkt_cnt + new_f->field.r2o_pkt_cnt;
    new_f->field.export_byte_cnt = new_f->field.o2r_byte_cnt + new_f->field.r2o_byte_cnt;

    p.reset();
    return 0;
}
#endif

/****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * Constructor
 */
FlowBoltComputeThread::FlowBoltComputeThread(std::string name, int id) : ComputeThread<Packet::TotalField, FLOW>(name, id) {
    count = 0;
    expire_cnt = 0;
    expire_pkt = 0;
    export_pkt = 0;

    Config *config = Config::getInstance();

    frag_table_size = config->getint("fragTableSize", 100000);
    if (id == 0) {
        LOG_MSG("FlowComputeThread: flow table size: %u\n", config->get_flow_table_size());
        LOG_MSG("FlowComputeThread: reassemble table size: %u\n", frag_table_size);
    }
    st_flow_ = new StFlow(config->get_flow_table_size(), 
            config->get_flow_table_timeout());
        
    flowBoltTimeout = config->getint("flowBoltTimeout", 5);
    LOG_MSG("FlowComputeThread: timeout %u\n", flowBoltTimeout);
}
    
void FlowBoltComputeThread::initialize() {
    st_flow_->create_table();
    ip_frag_init(frag_table_size);

    ComputeThread<Packet::TotalField, FLOW>::initialize();
}
