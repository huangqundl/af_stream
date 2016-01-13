#ifndef __FlowTokenGrouping_hpp__
#define __FlowTokenGrouping_hpp__

#include <inttypes.h>
#include <stdint.h>

#include "../../Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "../../Config.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "Flow.hpp"

class FlowTokenGrouping : public LoadBalancer <FLOW> {
private:
    typedef struct {
        // key
        uint32_t orig_ip;            // originator IP
        uint32_t resp_ip;            // responder IP
        uint16_t orig_port;            // port # of originator IP
        uint16_t resp_port;            // port # of responder IP
        uint8_t proto;                // protocol
        // values
        uint64_t fid;
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

    uint64_t flow_cnt;

    static unsigned int flow_hash_fcn(const HASH_FLOW *);
    static bool flow_cmp_fcn(const HASH_FLOW *, const HASH_FLOW *);
    static void flow_init_fcn(HASH_FLOW *, void*); 
    static void flow_free_fcn(HASH_FLOW *, void*); 
    static void flow_travel_do_fcn(HASH_FLOW *, void*); 
    
public:
    FlowTokenGrouping() {
        LOG_MSG("LoadBalancer in ZMQMainComputeThread: FlowTokenGrouping\n");
        flow_table_size_ = config->getint("flowGroupingTableSize", 3000000);
        flow_table_timeout_ = 10*flow_table_size_;
        flow_cnt = 0;
    }

    void initialize(void* context) {
        LOG_MSG("ZmqMainComputeThread: flowGroupingTableSize %u\n", flow_table_size_);

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
        memset(queue_volumn, 0, c*sizeof(uint64_t));
    }

    // Version 3: migrate large flows
	int balance(FLOW* f, int* old_index=NULL) {
        *old_index = -1;

        char key_str[16];
        HASH_FLOW* key_f = (HASH_FLOW*)key_str;    // flow that stores the key
        HASH_FLOW* hf;                        // pointer to the flow state
        int ret;

        // same hash row is returned for the either orig-to-resp or resp-to-orig 
        // direction    
        key_f->orig_ip = f->field.orig_ip;
        key_f->resp_ip = f->field.resp_ip;
        key_f->orig_port = f->field.orig_port;
        key_f->resp_port = f->field.resp_port;
        key_f->proto = f->field.proto;
    
        hf = flow_table_->find(key_f, 0, true);

        if (hf == NULL) {
            LOG_MSG("NULL flow in Load Balancer\n");
        }

        if (hf->orig_ip == 0 && hf->resp_ip == 0) {    // f is new entry
            // set the key
            hf->orig_ip = f->field.orig_ip;
            hf->resp_ip = f->field.resp_ip;
            hf->orig_port = f->field.orig_port;
            hf->resp_port = f->field.resp_port;
            hf->proto = f->field.proto;
            ret = 0;
            uint64_t maxToken = 0;
            for (int i=0; i<attachedQueueCount; ++i) {
                if (maxToken < queue_volumn[i]) {
                    ret = i;
                    maxToken = queue_volumn[i];
                }
            }
            hf->queue_index = ret;
            hf->fid = flow_cnt;
            flow_cnt++;
        }
        else {
            ret = hf->queue_index;
        }

        // update flow info
        hf->pkt = f->field.export_pkt_cnt;
        hf->byte = f->field.export_byte_cnt;

        for (int i=0; i<attachedQueueCount; ++i) {
            queue_volumn[i] += f->size()/attachedQueueCount;
        }
        if (queue_volumn[ret] > f->size()) {
            queue_volumn[ret] -= f->size();
        }
        else {
            queue_volumn[ret] = 0;
        }
        return ret;
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
FlowTokenGrouping::flow_hash_fcn(const HASH_FLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
FlowTokenGrouping::flow_cmp_fcn(const HASH_FLOW* f1, const HASH_FLOW* f2) {
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
FlowTokenGrouping::flow_init_fcn(HASH_FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
FlowTokenGrouping::flow_free_fcn(HASH_FLOW* f, void* st_arg) {
    LOG_MSG("Warning: free flows in Token Grouping\n");
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
FlowTokenGrouping::flow_travel_do_fcn(HASH_FLOW* f, void* context) {
}

#endif
