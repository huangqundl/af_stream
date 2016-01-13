/*
 * StFlow.hpp
 * - keep track of flow states
 */

#ifndef __StFlow_hpp__
#define __StFlow_hpp__

#include <stdint.h>
#include <vector>
#include "Flow.hpp"
#include "HashTable.hpp"
#include "Packet.hpp"

class StFlow {
public:

private:
    // data members    
    HashTable<FLOW>* flow_table_;        // hash table for storing states
    HashTable<FLOW>* flow_table2_;       // hash table for storing states
    uint32_t flow_table_size_;           // flow table size
    double flow_table_timeout_;          // flow table timeout

    // static functions
    static unsigned int flow_hash_fcn(const FLOW *);
    static bool flow_cmp_fcn(const FLOW *, const FLOW *);
    static void flow_init_fcn(FLOW *, void*); 
    static void flow_free_fcn(FLOW *, void*); 
    static void flow_travel_do_fcn(FLOW *, void*); 
    
    std::vector<FLOW> unexported_flows;

public:
    StFlow(uint32_t flow_table_size, double flow_table_timeout);
    void create_table();
    void link_state(Packet& p);
    FLOW* update_state(Packet& p, unsigned char* data, uint32_t data_len);
    void flush(); 
    void travel_flows();
    std::vector<FLOW>* get_unexported_flows() {return &unexported_flows;}
};

#endif
