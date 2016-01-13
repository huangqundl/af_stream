#ifndef __FlowTokenGroupingMigrate_hpp__
#define __FlowTokenGroupingMigrate_hpp__

#include <inttypes.h>
#include <stdint.h>

#include "../../Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "../../Config.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "Flow.hpp"

class FlowTokenGroupingMigrate : public LoadBalancer <FLOW> {
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
        unsigned int rebalance_time;
    } HASH_FLOW;

    HashTable<HASH_FLOW>* flow_table_;        // hash table for storing states
    uint32_t flow_table_size_;            // flow table size
    double flow_table_timeout_;            // flow table timeout

    uint64_t* queue_volumn;
    uint64_t* queue_last_time_rebalance;
    int num_of_rebalance;
    unsigned int max_num_rebalance;
    unsigned int last_time_rebalance;
    unsigned int rebalance_ratio;
    int diffRatio1;
    int diffRatio2;

    uint64_t flow_cnt;
    uint64_t total_load;

    static unsigned int flow_hash_fcn(const HASH_FLOW *);
    static bool flow_cmp_fcn(const HASH_FLOW *, const HASH_FLOW *);
    static void flow_init_fcn(HASH_FLOW *, void*); 
    static void flow_free_fcn(HASH_FLOW *, void*); 
    static void flow_travel_do_fcn(HASH_FLOW *, void*); 

    bool is_ignore_flow(FLOW* f) {
        if (f->field.orig_port == 80 || f->field.resp_port == 80) {
            return true;
        }

        return false;
    }
    
public:
    FlowTokenGroupingMigrate() {
        LOG_MSG("LoadBalancer in ZMQMainComputeThread: FlowTokenGroupingMigrate\n");
        flow_table_size_ = config->getint("flowGroupingTableSize", 3000000);
        flow_table_timeout_ = 10*flow_table_size_;
        flow_cnt = 0;

        num_of_rebalance = 0;
        max_num_rebalance = config->getint("maxNumRebalance", 1);
        last_time_rebalance = 0;
        total_load = 0;
        rebalance_ratio = config->getint("rebalance_ratio", 1);
        diffRatio1 = config->getint("diffRatio1", 11);
        diffRatio2 = config->getint("diffRatio2", 10);
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
        queue_last_time_rebalance = (uint64_t*)malloc(c*sizeof(uint64_t));
        memset(queue_volumn, 0, c*sizeof(uint64_t));
        memset(queue_last_time_rebalance, 0, c*sizeof(uint64_t));
    }

    // Version 3: migrate large flows
	int balance(FLOW* f, int* old_index=NULL) {
        *old_index = -1;
        flow_cnt++;

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
    
        hf = flow_table_->find(key_f, flow_cnt, true);

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
        }
        else {
            // decide to migrate a flow
            ret = hf->queue_index;
            if (!f->field.has_export) {
                /*
                uint64_t maxToken = 0;
                int maxQ = 0;
                for (int i=0; i<attachedQueueCount; ++i) {
                    if (maxToken < queue_volumn[i]) {
                        maxQ = i;
                        maxToken = queue_volumn[i];
                    }
                }
                ret = maxQ;
                hf->queue_index = ret;
                */
            }
            else {

                // satisfy constrains on load balance and flow
                /*
                    uint64_t maxToken = 0;
                    int maxQ = 0;
                    for (int i=0; i<attachedQueueCount; ++i) {
                        if (maxToken < queue_volumn[i]) {
                            maxQ = i;
                            maxToken = queue_volumn[i];
                        }
                    }
                    ret = maxQ;
                */
                //if (last_time_rebalance > rebalance_ratio) {
                    uint64_t maxToken = 0;
                    int maxQ = 0;
                    for (int i=0; i<attachedQueueCount; ++i) {
                        if (maxToken < queue_volumn[i]) {
                            maxQ = i;
                            maxToken = queue_volumn[i];
                        }
                    }
                    if (maxQ != ret && flow_cnt - queue_last_time_rebalance[maxQ] > rebalance_ratio && queue_volumn[maxQ] - queue_volumn[ret] > queue_volumn[ret]*diffRatio1/diffRatio2 && hf->rebalance_time < max_num_rebalance) {
                        *old_index = ret;
                        ret = maxQ;
                        hf->queue_index = ret;
                        num_of_rebalance++;
                        last_time_rebalance = 0;
                        hf->rebalance_time++;
                        queue_last_time_rebalance[ret] = flow_cnt;
                    }
                //}
            }
        }
        /*
            if ((f->field.resp_ip == 0x7f21700a  && f->field.orig_port == 3128 && f->field.resp_port == 1672) || (f->field.orig_ip == 0x7f21700a  && f->field.resp_port == 3128 && f->field.orig_port == 1672)) {
                char addr1[30], addr2[30];
                LOG_MSG("flow: %s:%d <-> %s:%d, worker: %d\n", ip2a(f->field.orig_ip, addr1), f->field.orig_port, ip2a(f->field.resp_ip, addr2), f->field.resp_port, ret);
            }
            */

        last_time_rebalance++;
        // update flow info
        hf->pkt = f->field.export_pkt_cnt;
        hf->byte += f->size();

        /*
        total_load += f->size();
        if (total_load > 100000) {
            memset(queue_volumn, 0, attachedQueueCount*sizeof(uint64_t));
            total_load = 0;
        }
        */
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
        flow_table_->travel_table();
    }
};

/****************************************************************************
 * Private static member functions
 ****************************************************************************/
unsigned int 
FlowTokenGroupingMigrate::flow_hash_fcn(const HASH_FLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
FlowTokenGroupingMigrate::flow_cmp_fcn(const HASH_FLOW* f1, const HASH_FLOW* f2) {
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
FlowTokenGroupingMigrate::flow_init_fcn(HASH_FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
FlowTokenGroupingMigrate::flow_free_fcn(HASH_FLOW* f, void* st_arg) {
    LOG_MSG("Warning: free flows in Token Grouping\n");
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
FlowTokenGroupingMigrate::flow_travel_do_fcn(HASH_FLOW* f, void* context) {
    if (f->rebalance_time) {
        printf("flow rebalance time: %u\n", f->rebalance_time);
    }
}

#endif
