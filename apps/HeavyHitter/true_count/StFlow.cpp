/*
 * StFlow.cpp
 * - keep track of flow states
 */

#include <inttypes.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "common.hpp"
#include "HashFunc.hpp"
#include "Util.hpp"
#include "StFlow.hpp"

/****************************************************************************
 * Private static member functions
 ****************************************************************************/
unsigned int 
StFlow::flow_hash_fcn(const FLOW* f) {
    char key[8];
    unsigned int a = f->field.orig_ip;
    memcpy(key, &a, 4);
    unsigned int b = f->field.resp_ip;
    memcpy(key+4, &b, 4);
    return HashFunc(key, 8);
}

bool 
StFlow::flow_cmp_fcn(const FLOW* f1, const FLOW* f2) {
    return (f1->field.orig_ip == f2->field.orig_ip) && (f1->field.resp_ip == f2->field.resp_ip);
}

void 
StFlow::flow_init_fcn(FLOW* f, void* st_arg) {
    memset(f, 0, sizeof(FLOW));
}

void 
StFlow::flow_free_fcn(FLOW* f, void* st_arg) {
    std::vector<FLOW>* flows = &(((StFlow*) st_arg)->flows);
    flows->push_back(*f);
    memset(f, 0, sizeof(FLOW));
}

void 
StFlow::flow_travel_do_fcn(FLOW* f, void* context) {
    std::vector<FLOW>* flows = &(((StFlow*) context)->flows);
    flows->push_back(*f);
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * Constructor
 *
 * @param flow_table_size - flow table size
 * @param flow_table_timeout - flow table timeout
 * @param pair_port_table_size
 * @param pair_port_table_timeout
 */    
StFlow::StFlow(uint32_t flow_table_size, double flow_table_timeout) {
    
    flow_table_ = NULL;
    flow_table_size_ = flow_table_size;
    flow_table_timeout_ = flow_table_timeout;
}

/*
 * Create flowtable
 */
void
StFlow::create_table() {
    flow_table_ = new HashTable<FLOW>("StFlow", 
            flow_table_size_, flow_table_timeout_,
            flow_hash_fcn, flow_cmp_fcn, 
            flow_init_fcn, flow_free_fcn, flow_travel_do_fcn, this);
            //flow_init_fcn, flow_free_fcn, this);
    if (flow_table_ == NULL) {
        fprintf(stderr, "ERR: out of memory - create flow_table_\n");
        exit(-1);
    }
}

/*
 * Update the flow state
 *
 * @param p - decoded packet
 */
FLOW*
StFlow::update_state(Packet& p) {

    char key_str[8];
    FLOW* key_f = (FLOW*)key_str;    // flow that stores the key
    FLOW* f;                        // pointer to the flow state

    // search hash table for flow
    key_f->field.orig_ip = p.get_src_ip();    
    key_f->field.resp_ip = p.get_dst_ip();    
    //f = flow_table_->find(key_f, p.get_pkt_ts(), true);
    f = flow_table_->find(key_f, 0, true);
    if (f == NULL) {        // unexpected error - shouldn't happen
        return NULL;
    }

    if (f->field.orig_ip == 0) {    // f is new entry
        // set the key
        f->field.orig_ip = p.get_src_ip();
        f->field.resp_ip = p.get_dst_ip();
    }

    // set the start time of a flow    
    if (f->field.flow_start_ts == 0) {
        //f->field.flow_start_ts = p.get_pkt_ts();
        f->field.flow_start_ts = 0;
    }

    // update the volume and direction change in either direction
    f->field.event_cnt++;
    f->field.pkt_cnt += p.get_bf().accumulated_pkt;
    f->field.byte_cnt += p.get_bf().accumulated_byte;
    return f;
}

/*
 * Flush all entries
 */
void
StFlow::flush() {
    flow_table_->reset();
    flows.clear();
}

/*
 * go through all flows
 */
void
StFlow::travel_flows() {
    flow_table_->travel_table();
}
