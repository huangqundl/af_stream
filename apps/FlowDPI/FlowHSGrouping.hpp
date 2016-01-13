#ifndef __FlowHSGrouping_hpp__
#define __FlowHSGrouping_hpp__

#include <inttypes.h>
#include <stdint.h>

#include "../../Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "../../Config.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "Flow.hpp"

class FlowHSGrouping : public LoadBalancer <FLOW> {
private:
    inline double max(double a, double b) {
        if (a < b) {
            return b;
        }
        return a;
    }

    typedef struct {
        // key
        uint32_t orig_ip;            // originator IP
        uint32_t resp_ip;            // responder IP
        uint16_t orig_port;            // port # of originator IP
        uint16_t resp_port;            // port # of responder IP
        uint8_t proto;                // protocol
        // values
        uint16_t queue_index;
        uint8_t has_export;
        uint64_t byte;
        double last_ts;
    } HASH_FLOW;

    HashTable<HASH_FLOW>* flow_table_;        // hash table for storing states
    uint32_t flow_table_size_;            // flow table size
    double flow_table_timeout_;            // flow table timeout

    typedef struct {
        uint64_t volumn;
        double last_ts;
        HASH_FLOW large_flows;
        uint32_t rebalance_time; 
    
    } QUEUE;

    QUEUE* queues;
    uint64_t flow_cnt;

    int num_of_rebalance;
    int num_of_rebalance_queue;
    int num_of_export;
    int currentQueueUsed;
    int max_num_rebalance;
    int max_num_export;
    int ratio1;
    int ratio2;
    int diffRatio1;
    int diffRatio2;
    unsigned int export_ratio;
    unsigned int rebalance_ratio;

    uint32_t event_cnt;

    //HASH_FLOW large_flow;

    double queueTimeout;
    int maxQ;
    int minQ;

    static unsigned int flow_hash_fcn(const HASH_FLOW *);
    static bool flow_cmp_fcn(const HASH_FLOW *, const HASH_FLOW *);
    static void flow_init_fcn(HASH_FLOW *, void*); 
    static void flow_free_fcn(HASH_FLOW *, void*); 
    static void flow_travel_do_fcn(HASH_FLOW *, void*); 

    inline int get_standby_queue() {
        return attachedQueueCount - 1;
    }
    
public:
    FlowHSGrouping() {
        LOG_MSG("LoadBalancer in ZMQMainComputeThread: FlowHSGrouping\n");
        flow_table_size_ = config->getint("flowGroupingTableSize", 3000000);
        flow_table_timeout_ = 0;
        flow_cnt = 0;

        queueTimeout = config->getdouble("queueTimeout", 5.0);

        currentQueueUsed = config->getint("initOutQueue", 1);

        max_num_rebalance = config->getint("maxNumRebalance", 100);
        max_num_export = config->getint("maxNumExport", 10);
        num_of_rebalance = 0;
        num_of_rebalance_queue = 0;
        num_of_export = 0;

        maxQ = 0;
        minQ = 3;

        ratio1 = config->getint("ratio1", 99);
        ratio2 = config->getint("ratio2", 100);
        diffRatio1 = config->getint("diffRatio1", 11);
        diffRatio2 = config->getint("diffRatio2", 10);
        export_ratio = config->getint("export_ratio", 1);
        rebalance_ratio = config->getint("rebalance_ratio", 1);
    }

    void initialize(void* context) {
        LOG_MSG("ZmqMainComputeThread: flowGroupingTableSize %u\n", flow_table_size_);

        //memset(&large_flow, 0, sizeof(HASH_FLOW));
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
        queues = new QUEUE[c];
        memset(queues, 0, c*sizeof(QUEUE));
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
    
        double cur_ts = max(f->field.o2r_last_ts, f->field.r2o_last_ts);
        hf = flow_table_->find(key_f, 0, false);
        if (hf == NULL) {
            // not specially handled flow
            uint64_t key = f->field.orig_ip + f->field.resp_ip;
            unsigned int hashKey = HashFunc((const char*)&key, 8);
            int queue_index = hashKey % currentQueueUsed;
            ret = queue_index;

            // decide whether to specially handled
            if (!f->field.has_export) {
                flow_cnt++;
                if (num_of_rebalance >= max_num_rebalance || num_of_rebalance_queue == currentQueueUsed) { 
                    ret = hashKey % attachedQueueCount;
                }
                else if (hashKey % (queues[queue_index].rebalance_time+1) !=0 && queues[queue_index].rebalance_time && num_of_rebalance < max_num_rebalance) {
                    if (!num_of_rebalance || flow_cnt/num_of_rebalance > rebalance_ratio) {
                        ret = get_standby_queue();
                        num_of_rebalance++;
                    }
                }
            }
            else {
                if (num_of_export<max_num_export && queue_index == maxQ && f->field.has_export && queues[minQ].last_ts > queueTimeout) {
                    uint64_t diff = queues[maxQ].volumn - queues[minQ].volumn;
                    if (diff > queues[minQ].volumn*diffRatio1/diffRatio2) {
                        if (!num_of_export || flow_cnt/num_of_export > export_ratio) {
                            ret = minQ;
                            *old_index = queue_index;
                            num_of_export++;
                            num_of_rebalance++;
                        }
                    }
                }
            }
                
            // insert specially handled flow
            if (ret != queue_index) {
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

                    hf->queue_index = ret;
                    hf->has_export++;
                }
                else {
                    LOG_MSG("flow has been in Load Balancer\n");
                }
            }

            // update queue info
            queues[ret].last_ts = cur_ts;
            queues[ret].volumn += f->field.o2r_byte_cnt + f->field.r2o_byte_cnt - f->field.export_byte_cnt;
            if (queues[ret].volumn > queues[maxQ].volumn) {
                maxQ = ret;
            }
            // large flow: only non-migrated flows are consider
            if (f->field.o2r_byte_cnt + f->field.r2o_byte_cnt > queues[ret].large_flows.byte && ret==queue_index) {
                queues[ret].large_flows.byte = f->field.o2r_byte_cnt + f->field.r2o_byte_cnt;
                queues[ret].large_flows.last_ts = cur_ts;

                if (!flow_cmp_fcn(key_f, &queues[ret].large_flows)) {
                    queues[ret].large_flows.orig_ip = f->field.orig_ip;
                    queues[ret].large_flows.resp_ip = f->field.resp_ip;
                    queues[ret].large_flows.orig_port = f->field.orig_port;
                    queues[ret].large_flows.resp_port = f->field.resp_port;
                    queues[ret].large_flows.proto = f->field.proto;
                }
            }
        }
        else {
            // specially handled flow
            ret = hf->queue_index;
            queues[ret].last_ts = cur_ts;
            queues[ret].volumn += f->field.o2r_byte_cnt + f->field.r2o_byte_cnt - f->field.export_byte_cnt;
            if (queues[ret].volumn > queues[maxQ].volumn) {
                maxQ = ret;
            }
            if (!hf->has_export) {
                hf->has_export++;
                num_of_rebalance++;
                uint64_t key = f->field.orig_ip + f->field.resp_ip;
                *old_index = HashFunc((const char*)&key, 8) % currentQueueUsed;
                num_of_export++;
                queues[*old_index].rebalance_time++;
                if (queues[*old_index].rebalance_time == 1) {
                    num_of_rebalance_queue++;
                }
            }
        }

        for (int i=0; i<attachedQueueCount; ++i) {
            queues[i].volumn = queues[i].volumn * ratio1;
            queues[i].volumn = queues[i].volumn / ratio2;
        }
        for (int i=0; i<attachedQueueCount; ++i) {
            if (cur_ts - queues[i].large_flows.last_ts > queueTimeout) {
            //if (queues[i].volumn == 0) {
                queues[i].large_flows.byte = 0; 
                queues[i].large_flows.last_ts = 0; 
            }
            if (queues[i].volumn < queues[minQ].volumn) {
                minQ = i;
            }
        }

        return ret;
    }

    void output() {
        LOG_MSG("\n");

        LOG_MSG("total flows: %" PRIu64 "\n", flow_cnt);
        LOG_MSG("num_of_export: %d\n", num_of_export);
        LOG_MSG("num_of_rebalance: %d\n", num_of_rebalance);
        LOG_MSG("num_of_rebalance_queue: %d\n", num_of_rebalance_queue);
        LOG_MSG("\n");
    }

    /*
    void notify_hotpoint(int queue_index) {

        if (queue_index >= currentQueueUsed) {
            return;
        }

        HASH_FLOW* f = &queues[queue_index].large_flows;
        if (f->byte <= 5000) {
            return;
        }

        HASH_FLOW* hf = flow_table_->find(f, 0, true);
        if (hf == NULL) {
            LOG_MSG("NULL flow in Load Balancer\n");
        }

        if (hf->orig_ip == 0 && hf->resp_ip == 0) {    // f is new entry
            if (currentQueueUsed == attachedQueueCount) {
                return;
            }
            // set the key
            hf->orig_ip = f->orig_ip;
            hf->resp_ip = f->resp_ip;
            hf->orig_port = f->orig_port;
            hf->resp_port = f->resp_port;
            hf->proto = f->proto;

            int ret = get_standby_queue();
            hf->queue_index = ret;

            memset(&queues[queue_index].large_flows, 0, sizeof(HASH_FLOW));
        }
        else {
            // do nothing for flows that are already rebalanced before
        }
    }
    */
};

/****************************************************************************
 * Private static member functions
 ****************************************************************************/
unsigned int 
FlowHSGrouping::flow_hash_fcn(const HASH_FLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
FlowHSGrouping::flow_cmp_fcn(const HASH_FLOW* f1, const HASH_FLOW* f2) {
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
FlowHSGrouping::flow_init_fcn(HASH_FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
FlowHSGrouping::flow_free_fcn(HASH_FLOW* f, void* st_arg) {
    LOG_MSG("Warning: free flows in HS Grouping\n");
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
FlowHSGrouping::flow_travel_do_fcn(HASH_FLOW* f, void* context) {
}

#endif
