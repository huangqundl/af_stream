#ifndef __PacketTokenGroupingMigrate_hpp__
#define __PacketTokenGroupingMigrate_hpp__

#include <inttypes.h>
#include <stdint.h>

#include "Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "Config.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "../Packet.hpp"

class PacketTokenGroupingMigrate : public LoadBalancer <Packet::BasicField> {
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
    //uint64_t* queue_last_time_rebalance;
    double* queue_last_time_rebalance;
    int num_of_rebalance;
    //unsigned int rebalance_ratio;
    double rebalance_interval;
    uint64_t large_flow_threshold;
    int diffRatio1;
    int diffRatio2;

    uint64_t flow_cnt;
    uint64_t total_load;

    double first_ts;
    double last_ts;
    double queue_timeout;
    double* queue_last_update;

    static unsigned int flow_hash_fcn(const HASH_FLOW *);
    static bool flow_cmp_fcn(const HASH_FLOW *, const HASH_FLOW *);
    static void flow_init_fcn(HASH_FLOW *, void*); 
    static void flow_free_fcn(HASH_FLOW *, void*); 
    static void flow_travel_do_fcn(HASH_FLOW *, void*); 

    bool is_ignore_flow(FLOW* f) {
        return false;
    }
    
public:
    PacketTokenGroupingMigrate() {
        LOG_MSG("LoadBalancer in ZMQMainAnalyzer: PacketTokenGroupingMigrate\n");
        flow_table_size_ = config->getint("flowGroupingTableSize", 3000000);
        flow_table_timeout_ = 300;
        flow_cnt = 0;

        num_of_rebalance = 0;
        total_load = 0;
        //rebalance_ratio = config->getint("rebalance_ratio", 1);
        rebalance_interval = config->getdouble("rebalance_interval", 1);
        diffRatio1 = config->getint("diffRatio1", 11);
        diffRatio2 = config->getint("diffRatio2", 10);
        large_flow_threshold = config->getint("large_flow_threshold", 4000000);
        queue_timeout = config->getdouble("queueTimeout", 1);
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
        //queue_last_time_rebalance = (uint64_t*)malloc(c*sizeof(uint64_t));
        queue_last_time_rebalance = (double*)malloc(c*sizeof(double));
        queue_last_update = (double*)malloc(c*sizeof(double));
        memset(queue_volumn, 0, c*sizeof(uint64_t));
        //memset(queue_last_time_rebalance, 0, c*sizeof(uint64_t));
        memset(queue_last_time_rebalance, 0, c*sizeof(double));
        memset(queue_last_update, 0, c*sizeof(double));
    }

    // Version 3: migrate large flows
	int balance(Packet::BasicField* bf, int* old_index=NULL) {
        *old_index = -1;
        flow_cnt++;

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
            for (int i=1; i<attachedQueueCount; ++i) {
                if (minToken > queue_volumn[i]) {
                    ret = i;
                    minToken = queue_volumn[i];
                }
            }
            hf->queue_index = ret;
        }
        else {
            // decide to migrate a flow
            ret = hf->queue_index;

            int minQ = 0;
            uint64_t minToken = queue_volumn[0];
            for (int i=1; i<attachedQueueCount; ++i) {
                if (minToken > queue_volumn[i]) {
                    minQ = i;
                    minToken = queue_volumn[i];
                }
            }
            if (minQ != ret && 
                        //flow_cnt - queue_last_time_rebalance[maxQ] > rebalance_ratio
                        bf->pkt_ts - queue_last_time_rebalance[minQ] > rebalance_interval
                        ) {
                
                if (hf->byte > large_flow_threshold
                            //|| (queue_volumn[maxQ] - queue_volumn[ret] > queue_volumn[ret]*diffRatio1/diffRatio2 && hf->byte > large_flow_threshold/2)) {
                            && (queue_volumn[ret] - queue_volumn[minQ] > queue_volumn[minQ]*diffRatio1/diffRatio2)
                                ) {
                *old_index = ret;
                ret = minQ;
                hf->queue_index = ret;
                num_of_rebalance++;
                hf->rebalance_time++;
                hf->byte = 0;
                //queue_last_time_rebalance[ret] = flow_cnt;
                queue_last_time_rebalance[ret] = bf->pkt_ts;
                /*
                char addr1[30], addr2[30];
                printf("migrate flow: %s:%d <-> %s:%d, from %d to %d\n",
                            ip2a(hf->orig_ip, addr1),
                            hf->orig_port,
                            ip2a(hf->resp_ip, addr2),
                            hf->resp_port,
                            *old_index,
                            ret);
                            */
                }
            }
        }

        // update flow info
        hf->pkt += 1;
        hf->byte += bf->load();

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
PacketTokenGroupingMigrate::flow_hash_fcn(const HASH_FLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
PacketTokenGroupingMigrate::flow_cmp_fcn(const HASH_FLOW* f1, const HASH_FLOW* f2) {
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
PacketTokenGroupingMigrate::flow_init_fcn(HASH_FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
PacketTokenGroupingMigrate::flow_free_fcn(HASH_FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
PacketTokenGroupingMigrate::flow_travel_do_fcn(HASH_FLOW* f, void* context) {
    if (f->rebalance_time) {
        printf("flow rebalance time: %u\n", f->rebalance_time);
    }
}

#endif
