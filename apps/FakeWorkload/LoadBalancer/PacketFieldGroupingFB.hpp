#ifndef __PacketFieldGroupingFB_hpp__
#define __PacketFieldGroupingFB_hpp__

#include <stdint.h>

#include "Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "../Packet.hpp"

class PacketFieldGroupingFB : public LoadBalancer<Packet::BasicField> {
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
    } MyFLOW;

    int currentQueueUsed;

    HashTable<MyFLOW>* flow_table_;
    uint32_t flow_table_size_;
    double flow_table_timeout_;

    static unsigned int flow_hash_fcn(const MyFLOW *);
    static bool flow_cmp_fcn(const MyFLOW *, const MyFLOW *);
    static void flow_init_fcn(MyFLOW *, void*); 
    static void flow_free_fcn(MyFLOW *, void*); 
    static void flow_travel_do_fcn(MyFLOW *, void*); 

    uint64_t event;
    double last_ts;

public:
    PacketFieldGroupingFB() {
        LOG_MSG("LoadBalancer in ZMQMainAnalyzer: PacketFieldGroupingFB\n");
        flow_table_size_ = config->getint("flowGroupingTableSize", 500000);
        flow_table_timeout_ = 60;
        currentQueueUsed = config->getint("initOutQueue", 1);
        event = 0;
    }

    void initialize(void* context) {
        flow_table_ = new HashTable<MyFLOW>("PacketLoadBalance",
            flow_table_size_, flow_table_timeout_,
            flow_hash_fcn, flow_cmp_fcn, 
            flow_init_fcn, flow_free_fcn, flow_travel_do_fcn, this);
        if (flow_table_ == NULL) {
            fprintf(stderr, "ERR: out of memory - create flow_table_\n");
            exit(-1);
        }
    }

	int balance(Packet::BasicField* bf, int* old_index=NULL) {
        event++;
        last_ts = bf->pkt_ts;

        char key_str[16];
        MyFLOW* key_f = (MyFLOW*)key_str;
        MyFLOW* hf;

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
            uint64_t key = hf->orig_ip + hf->resp_ip;
            hf->queue_index = HashFunc((const char*)&key, 8) % currentQueueUsed;
        }

        return hf->queue_index;
    }

    void notify_hotpoint(int queue_index) {
        if (currentQueueUsed < attachedQueueCount) { 
            currentQueueUsed++;
        }
        uint64_t now_ts = now_us();
        printf("notify queue %d at event %" PRIu64 ", time %lf, wall clock %" PRIu64 "\n", queue_index, event, last_ts, now_ts);
    }
};

/****************************************************************************
 * Private static member functions
 ****************************************************************************/
unsigned int 
PacketFieldGroupingFB::flow_hash_fcn(const MyFLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
PacketFieldGroupingFB::flow_cmp_fcn(const MyFLOW* f1, const MyFLOW* f2) {
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
PacketFieldGroupingFB::flow_init_fcn(MyFLOW* f, void* st_arg) {
    memset(f, 0, sizeof(MyFLOW));
}

void 
PacketFieldGroupingFB::flow_free_fcn(MyFLOW* f, void* st_arg) {
    memset(f, 0, sizeof(MyFLOW));
}

void 
PacketFieldGroupingFB::flow_travel_do_fcn(MyFLOW* f, void* context) {
}
#endif
