#ifndef __QueueSizeGrouping_hpp__
#define __QueueSizeGrouping_hpp__

#include <inttypes.h>
#include <stdint.h>

#include "../../Dispatcher/Router/Router.hpp"
#include "common.hpp"
#include "Packet.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"

class QueueSizeGrouping : public LoadBalancer <Packet::TotalField> {
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
    } FLOW;

    HashTable<FLOW>* flow_table_;        // hash table for storing states
    uint32_t flow_table_size_;            // flow table size
    double flow_table_timeout_;            // flow table timeout

    uint64_t* queue_volumn;
    int num_of_rebalance;

    static unsigned int flow_hash_fcn(const FLOW *);
    static bool flow_cmp_fcn(const FLOW *, const FLOW *);
    static void flow_init_fcn(FLOW *, void*); 
    static void flow_free_fcn(FLOW *, void*); 
    static void flow_travel_do_fcn(FLOW *, void*); 

    inline bool is_large_flow(FLOW* f) {
        /*
        if (f->orig_port == 37682 || f->resp_port == 37682) {
            if (f->pkt == 7200) {
                return true;
            }
        }
        */
#ifdef LOAD_BALANCE
        if ((double)f->byte / queue_volumn[f->queue_index] > 0.01 && f->pkt > 10000 && f->rebalance_time==0) {
            return true;
        }
#endif
        return false; 
    }

    // rebalance may fail when other queues are not idle
    inline bool rebalance(FLOW* f, int* old_index) {
        int index = -1;
        uint64_t min_size = 0;
        for (int i=0; i<attachedQueueCount; ++i) {
            if (i == f->queue_index) {
                continue;
            }
            if (index == -1 || min_size > queue_volumn[i]) {
                index = i;
                min_size = queue_volumn[i];
            }
        }
        if (index >= 0) {
            if (index != f->queue_index) {
                *old_index = f->queue_index;
                f->queue_index = index;
                f->pkt = 0;
                f->byte = 0;
                f->rebalance_time++;
            }
            return true;
        }
        return false;
    }
    
public:
    QueueSizeGrouping() {
        flow_table_size_ = 1000000;
        flow_table_timeout_ = 0;
    }

    void initialize(void* context) {
        num_of_rebalance = 0;
        flow_table_ = new HashTable<FLOW>("PacketLoadBalance",
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

    // Version 1: direct hash
    /*
	int balance(Packet::TotalField* tf, int* flags=NULL) {
        uint64_t key = tf->bf.src_ip + tf->bf.dst_ip;
        int ret = HashFunc((const char*)&key, 8) % attachedQueueCount;
        queue_volumn[ret] += tf->bf.ip_dlen;
        return ret;
    }
    */

    // Version 2: shortest queue
    /*
	int balance(Packet::TotalField* tf, int* flags=NULL) {
        char key_str[16];
        FLOW* key_f = (FLOW*)key_str;    // flow that stores the key
        FLOW* f;                        // pointer to the flow state
        int ret;

        // same hash row is returned for the either orig-to-resp or resp-to-orig 
        // direction    
        key_f->orig_ip = tf->bf.src_ip;
        key_f->resp_ip = tf->bf.dst_ip;
        key_f->orig_port = tf->bf.src_port;
        key_f->resp_port = tf->bf.dst_port;
        key_f->proto = tf->bf.proto;
    
        f = flow_table_->find(key_f, tf->bf.pkt_ts, true);

        if (f->orig_ip == 0 && f->resp_ip == 0) {    // f is new entry
            // set the key
            f->orig_ip = tf->bf.src_ip;
            f->resp_ip = tf->bf.dst_ip;
            f->orig_port = tf->bf.src_port;
            f->resp_port = tf->bf.dst_port;
            f->proto = tf->bf.proto;
            ret = -1;
            uint64_t min_size = 0;
            for (int i=0; i<attachedQueueCount; ++i) {
                if (ret == -1 || min_size > queue_volumn[i]) {
                    ret = i;
                    min_size = queue_volumn[i];
                }
            }
            f->queue_index = ret;
        }
        else {
            ret = f->queue_index;
        }

        if (ret < 0) {
            LOG_MSG("Error ret: %d\n", ret);
        }
        f->pkt += 1;
        f->byte += tf->bf.ip_dlen;
        queue_volumn[ret] += tf->bf.ip_dlen;
        return ret;
    }
    */

    // Version 3: migrate large flows
	int balance(Packet::TotalField* tf, int* old_index=NULL) {
        *old_index = -1;

        char key_str[16];
        FLOW* key_f = (FLOW*)key_str;    // flow that stores the key
        FLOW* f;                        // pointer to the flow state
        int ret;

        // same hash row is returned for the either orig-to-resp or resp-to-orig 
        // direction    
        key_f->orig_ip = tf->bf.src_ip;
        key_f->resp_ip = tf->bf.dst_ip;
        key_f->orig_port = tf->bf.src_port;
        key_f->resp_port = tf->bf.dst_port;
        key_f->proto = tf->bf.proto;
    
        f = flow_table_->find(key_f, tf->bf.pkt_ts, true);

        if (f == NULL) {
            LOG_MSG("NULL flow in Load Balancer\n");
        }

        if (f->orig_ip == 0 && f->resp_ip == 0) {    // f is new entry
            // set the key
            f->orig_ip = tf->bf.src_ip;
            f->resp_ip = tf->bf.dst_ip;
            f->orig_port = tf->bf.src_port;
            f->resp_port = tf->bf.dst_port;
            f->proto = tf->bf.proto;
            ret = -1;
            uint64_t min_size = 0;
            for (int i=0; i<attachedQueueCount; ++i) {
                if (ret == -1 || min_size > queue_volumn[i]) {
                    ret = i;
                    min_size = queue_volumn[i];
                }
            }
            f->queue_index = ret;
        }
        else {
            if (is_large_flow(f)) {
                if (rebalance(f, old_index)) {
                    num_of_rebalance++;
                }
            }
            ret = f->queue_index;
        }

        if (ret < 0) {
            LOG_MSG("Error ret: %d\n", ret);
        }
        
        // update flow info
        f->pkt += 1;
        f->byte += tf->bf.ip_dlen;
        queue_volumn[ret] += tf->bf.ip_dlen;
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
QueueSizeGrouping::flow_hash_fcn(const FLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
QueueSizeGrouping::flow_cmp_fcn(const FLOW* f1, const FLOW* f2) {
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
QueueSizeGrouping::flow_init_fcn(FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(FLOW));
}

void 
QueueSizeGrouping::flow_free_fcn(FLOW* f, void* st_arg) {
    LOG_MSG("Warning: free flows in Queuesize Grouping\n");
    memset(f, 0, sizeof(FLOW));
}

void 
QueueSizeGrouping::flow_travel_do_fcn(FLOW* f, void* context) {
}

#endif
