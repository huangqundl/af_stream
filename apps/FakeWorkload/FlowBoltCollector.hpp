#ifndef __FlowBoltCollector_hpp__
#define __FlowBoltCollector_hpp__

#include "../../Collector/Collector.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "Flow.hpp"

    typedef struct {
        // key
        uint32_t orig_ip;            // originator IP
        uint32_t resp_ip;            // responder IP
        uint16_t orig_port;            // port # of originator IP
        uint16_t resp_port;            // port # of responder IP
        uint8_t proto;                // protocol
        // values
        uint64_t export_workload;
    } HASH_FLOW;


void printFlow(HASH_FLOW& f) {
    char addr1[30];
    char addr2[30];
    printf("origip=%s respip=%s origport=%u respport=%u proto=%u workload: %" PRIu64 "\n", 
        ip2a(f.orig_ip, addr1),
        ip2a(f.resp_ip, addr2),
        f.orig_port,
        f.resp_port,
        f.proto,
        f.export_workload); 
}

class Dummy {
public:
    uint32_t size() { return 0; }
    uint32_t get_size() { return 0; }
};

class FlowBoltCollector : public Collector<FLOW, Dummy> {
public:
private:
    HashTable<HASH_FLOW>* flow_table_;        // hash table for storing states
    uint32_t flow_table_size_;            // flow table size
    double flow_table_timeout_;            // flow table timeout

    unsigned int printCount;
    uint64_t totalWorkload;
    std::vector<HASH_FLOW> unexported_flows;

    static unsigned int flow_hash_fcn(const HASH_FLOW *);
    static bool flow_cmp_fcn(const HASH_FLOW *, const HASH_FLOW *);
    static void flow_init_fcn(HASH_FLOW *, void*); 
    static void flow_free_fcn(HASH_FLOW *, void*); 
    static void flow_travel_do_fcn(HASH_FLOW *, void*); 

    void process(FLOW& f) {
        char key_str[16];
        HASH_FLOW* key_f = (HASH_FLOW*)key_str;    // flow that stores the key
        HASH_FLOW* hf;                        // pointer to the flow state

        // same hash row is returned for the either orig-to-resp or resp-to-orig 
        // direction    
        key_f->orig_ip = f.field.orig_ip;
        key_f->resp_ip = f.field.resp_ip;
        key_f->orig_port = f.field.orig_port;
        key_f->resp_port = f.field.resp_port;
        key_f->proto = f.field.proto;
    
        hf = flow_table_->find(key_f, 0, true);

        if (hf == NULL) {
            LOG_MSG("NULL flow in Load Balancer\n");
        }

        if (hf->orig_ip == 0 && hf->resp_ip == 0) {    // f is new entry
            // set the key
            hf->orig_ip = f.field.orig_ip;
            hf->resp_ip = f.field.resp_ip;
            hf->orig_port = f.field.orig_port;
            hf->resp_port = f.field.resp_port;
            hf->proto = f.field.proto;
        }
        
        hf->export_workload += f.load();
        totalWorkload += f.load();

        if (totalWorkload >= 500000) {
            printf("count = %d\n", printCount++);
            flow_table_->travel_table();
            uint32_t size = unexported_flows.size();
            for (uint32_t i=0; i<size; ++i) {
                HASH_FLOW f = unexported_flows[i];
                printFlow(f);
            }
            flow_table_->reset();
            unexported_flows.clear();
            printf("\n");
            totalWorkload = 0;
        }
    }

public:
    FlowBoltCollector(std::string name, int num): Collector<FLOW, Dummy>(name, num) {
        flow_table_size_ = 200000;
        flow_table_timeout_ = 360000;

        printCount = 0;
    }

    void initialize() {
        LOG_MSG("FlowBoltCollector: flowGroupingTableSize %u\n", flow_table_size_);

        flow_table_ = new HashTable<HASH_FLOW>("FlowBoltCollector",
            flow_table_size_, flow_table_timeout_,
            flow_hash_fcn, flow_cmp_fcn, 
            flow_init_fcn, flow_free_fcn, flow_travel_do_fcn, this);
        if (flow_table_ == NULL) {
            fprintf(stderr, "ERR: out of memory - create flow_table_\n");
            exit(-1);
        }

        Collector<FLOW, Dummy>::initialize();
    }
};

/****************************************************************************
 * Private static member functions
 ****************************************************************************/
unsigned int 
FlowBoltCollector::flow_hash_fcn(const HASH_FLOW* f) {
    char key[7];
    unsigned int a = f->orig_ip + f->resp_ip;
    unsigned short b = f->orig_port + f->resp_port;
    memcpy(key, &a, 4);
    memcpy(key + 4, &b, 2);
    key[6] = (char)(f->proto);
    return HashFunc(key, 7);
}

bool 
FlowBoltCollector::flow_cmp_fcn(const HASH_FLOW* f1, const HASH_FLOW* f2) {
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
FlowBoltCollector::flow_init_fcn(HASH_FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
FlowBoltCollector::flow_free_fcn(HASH_FLOW* f, void* st_arg) {
    LOG_MSG("Warning: free flows in FlowBoltCollector\n");
    memset(f, 0, sizeof(HASH_FLOW));
}

void 
FlowBoltCollector::flow_travel_do_fcn(HASH_FLOW* f, void* context) {
    std::vector<HASH_FLOW>* flows = &(((FlowBoltCollector*) context)->unexported_flows);
    flows->push_back(*f);
}
#endif
