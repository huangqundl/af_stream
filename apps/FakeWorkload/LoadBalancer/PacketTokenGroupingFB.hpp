#ifndef __PacketTokenGroupingFB_hpp__
#define __PacketTokenGroupingFB_hpp__

#include <inttypes.h>
#include <stdint.h>

#include "Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "Config.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "../Packet.hpp"

class PacketTokenGroupingFB : public LoadBalancer <Packet::BasicField> {
private:
    typedef struct {
        // key
        uint32_t orig_ip;            // originator IP
        uint32_t resp_ip;            // responder IP
        uint16_t orig_port;            // port # of originator IP
        uint16_t resp_port;            // port # of responder IP
        uint8_t proto;                // protocol
        // values
        uint16_t queue_index;
        uint64_t pkt;
        uint64_t byte;
        uint8_t rebalance_time;
    } HASH_FLOW;

    HashTable<HASH_FLOW>* flow_table_;        // hash table for storing states
    uint32_t flow_table_size_;            // flow table size
    double flow_table_timeout_;            // flow table timeout

    uint64_t* queue_volumn;
    int num_of_rebalance;
    int currentQueueUsed;

    uint64_t flow_cnt;

    double first_ts;
    double last_ts;
    double queue_timeout;
    double* queue_last_update;

    static unsigned int flow_hash_fcn(const HASH_FLOW *);
    static bool flow_cmp_fcn(const HASH_FLOW *, const HASH_FLOW *);
    static void flow_init_fcn(HASH_FLOW *, void*); 
    static void flow_free_fcn(HASH_FLOW *, void*); 
    static void flow_travel_do_fcn(HASH_FLOW *, void*); 
    
public:
    PacketTokenGroupingFB() {
        LOG_MSG("LoadBalancer in ZMQMainAnalyzer: PacketTokenGroupingFB\n");
        flow_table_size_ = config->getint("flowGroupingTableSize", 500000);
        flow_table_timeout_ = 60;
        flow_cnt = 0;
        queue_timeout = config->getdouble("queueTimeout", 1);
        currentQueueUsed = config->getint("initOutQueue", 1);
    }

    void initialize(void* context) {
        LOG_MSG("ZmqMainAnalyzer: flowGroupingTableSize %u\n", flow_table_size_);

        num_of_rebalance = 0;
        flow_table_ = new HashTable<HASH_FLOW>("PacketLoadBalance",
            flow_table_size_, flow_table_timeout_,
            flow_hash_fcn, flow_cmp_fcn, 
            flow_init_fcn, flow_free_fcn, flow_travel_do_fcn, this);
        if (flow_table_ == NULL) {
            fprintf(stderr, "ERR: out of memory - create flow_table_\n");
            exit(-1);
        }
    }

    void setQueueCount(int c) {
        attachedQueueCount = c;
        queue_volumn = (uint64_t*)malloc(c*sizeof(uint64_t));
        queue_last_update = (double*)malloc(c*sizeof(double));
        memset(queue_volumn, 0, c*sizeof(uint64_t));
        memset(queue_last_update, 0, c*sizeof(double));
    }

    // Version 3: migrate large flows
	int balance(Packet::BasicField* bf, int* old_index=NULL) {
        *old_index = -1;

        char key_str[16];
        HASH_FLOW* key_f = (HASH_FLOW*)key_str;    // flow that stores the key
        HASH_FLOW* hf;                        // pointer to the flow state
        int ret;

        // same hash row is returned for the either orig-to-resp or resp-to-orig 
        // direction    
        key_f->orig_ip = bf->src_ip;
        key_f->resp_ip = bf->dst_ip;
        key_f->orig_port = bf->src_port;
        key_f->resp_port = bf->dst_port;
        key_f->proto = bf->proto;
    
        hf = flow_table_->find(key_f, bf->pkt_ts, true);

        if (hf == NULL) {
            LOG_MSG("NULL flow in Load Balancer\n");
        }

        if (hf->orig_ip == 0 && hf->resp_ip == 0) {    // f is new entry
            // set the key
            hf->orig_ip = bf->src_ip;
            hf->resp_ip = bf->dst_ip;
            hf->orig_port = bf->src_port;
            hf->resp_port = bf->dst_port;
            hf->proto = bf->proto;
            ret = 0;
            uint64_t minToken = queue_volumn[0];
            for (int i=1; i<currentQueueUsed; ++i) {
                if (minToken > queue_volumn[i]) {
                    ret = i;
                    minToken = queue_volumn[i];
                }
            }
            hf->queue_index = ret;
            flow_cnt++;
        }
        else {
            ret = hf->queue_index;
        }

        // update flow info
        hf->pkt += 1;
        hf->byte += bf->ip_dlen;

        if (flow_cnt == 1) {
            first_ts = bf->pkt_ts;
        }
        else {
            if (bf->pkt_ts - last_ts > queue_timeout) {
                for (int i=0; i<attachedQueueCount; ++i) {
                    queue_volumn[i] *= 0.2;
                }
                last_ts = bf->pkt_ts;
            }
        }

        queue_volumn[ret] += bf->load() * 0.8;
        queue_last_update[ret] = bf->pkt_ts;

        return ret;
    }

    void notify_hotpoint(int queue_index) {
        if (currentQueueUsed < attachedQueueCount) { 
            currentQueueUsed++;
        }
        uint64_t now_ts = now_us();
        printf("notify queue %d, wall clock %" PRIu64 "\n", queue_index, now_ts);
    }

    void output() {
        LOG_MSG("\n");
        for (int i=0; i<attachedQueueCount; ++i) {
            LOG_MSG("Queue %d, volumn: %" PRIu64 "\n", i, queue_volumn[i]);
        }
        LOG_MSG("num_of_rebalance: %d\n", num_of_rebalance);
        LOG_MSG("\n");
    }
};

/****************************************************************************
 * Private static member functions
 ****************************************************************************/
unsigned int 
PacketTokenGroupingFB::flow_hash_fcn(const HASH_FLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
PacketTokenGroupingFB::flow_cmp_fcn(const HASH_FLOW* f1, const HASH_FLOW* f2) {
    return ((f1->orig_ip == f2->orig_ip && f1->resp_ip == f2->resp_ip && 
                f1->orig_port == f2->orig_port && 
                f1->resp_port == f2->resp_port && 
                f1->proto == f2->proto) ||
            (f1->orig_ip == f2->resp_ip && f1->resp_ip == f2->orig_ip && 
             f1->orig_port == f2->resp_port && 
             f1->resp_port == f2->orig_port &&
             f1->proto == f2->proto));
}

void 
PacketTokenGroupingFB::flow_init_fcn(HASH_FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
PacketTokenGroupingFB::flow_free_fcn(HASH_FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
PacketTokenGroupingFB::flow_travel_do_fcn(HASH_FLOW* f, void* context) {
}

#endif
