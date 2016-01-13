#ifndef __FlowFieldGroupingFB_hpp__
#define __FlowFieldGroupingFB_hpp__

#include <stdint.h>

#include "../../Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "Flow.hpp"
#include "HashFunc.hpp"

class FlowFieldGroupingFB : public LoadBalancer<FLOW> {
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


public:
    FlowFieldGroupingFB(int initQueue): currentQueueUsed(initQueue) {
        LOG_MSG("LoadBalancer in ZMQMainComputeThread: FlowFieldGroupingFB\n");
        flow_table_size_ = 200000;
        flow_table_timeout_ = 0;
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

	int balance(FLOW* stf, int* old_index=NULL) {
        char key_str[16];
        MyFLOW* key_f = (MyFLOW*)key_str;
        MyFLOW* f;

        // same hash row is returned for the either orig-to-resp or resp-to-orig 
        // direction    
        key_f->orig_ip = stf->field.orig_ip;
        key_f->resp_ip = stf->field.resp_ip;
        key_f->orig_port = stf->field.orig_port;
        key_f->resp_port = stf->field.resp_port;
        key_f->proto = stf->field.proto;
    
        f = flow_table_->find(key_f, 0, true);

        if (f == NULL) {
            LOG_MSG("NULL flow in Load Balancer\n");
        }

        if (f->orig_ip == 0 && f->resp_ip == 0) {    // f is new entry
            // set the key
            f->orig_ip = stf->field.orig_ip;
            f->resp_ip = stf->field.resp_ip;
            f->orig_port = stf->field.orig_port;
            f->resp_port = stf->field.resp_port;
            f->proto = stf->field.proto;
            uint64_t key = f->orig_ip + f->resp_ip;
            f->queue_index = HashFunc((const char*)&key, 8) % currentQueueUsed;
        }

        return f->queue_index;
    }

    void notify_hotpoint(int queue_index) {
        if (currentQueueUsed < attachedQueueCount) { 
            currentQueueUsed++;
        }
    }
};

/****************************************************************************
 * Private static member functions
 ****************************************************************************/
unsigned int 
FlowFieldGroupingFB::flow_hash_fcn(const MyFLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
FlowFieldGroupingFB::flow_cmp_fcn(const MyFLOW* f1, const MyFLOW* f2) {
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
FlowFieldGroupingFB::flow_init_fcn(MyFLOW* f, void* st_arg) {
    memset(f, 0, sizeof(MyFLOW));
}

void 
FlowFieldGroupingFB::flow_free_fcn(MyFLOW* f, void* st_arg) {
    LOG_MSG("Warning: free flows in FlowFieldGroupingFB\n");
    memset(f, 0, sizeof(MyFLOW));
}

void 
FlowFieldGroupingFB::flow_travel_do_fcn(MyFLOW* f, void* context) {
}
#endif
