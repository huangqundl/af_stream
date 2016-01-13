/*
 * StFlow.hpp
 * - keep track of flow states
 */

#ifndef __StFlow_hpp__
#define __StFlow_hpp__

#include <stdint.h>
#include <vector>
#include "HashTable.hpp"
#include "../Packet.hpp"
#include "Flow.hpp"

/**
 * Flow table
 */
class StFlow {
public:

private:
    // data members    
    /// hash table for storing states
    HashTable<FLOW>* flow_table_;        
    /// flow table size
    uint32_t flow_table_size_;           
    /// flow table timeout
    double flow_table_timeout_;          

    // static functions

    /// hash function
    /// @param f the flow
    static unsigned int flow_hash_fcn(const FLOW * f);

    /// compairing keys
    /// @param a flow a
    /// @param b flow b
    static bool flow_cmp_fcn(const FLOW * a, const FLOW * b);

    /// init a flow
    /// @param f the flow
    /// @param ctx the context
    static void flow_init_fcn(FLOW *f, void* ctx); 

    /// release a flow
    /// @param f the flow
    /// @param ctx the context
    static void flow_free_fcn(FLOW *f, void* ctx); 

    /// visit a flow
    /// @param f the flow
    /// @param ctx the context
    static void flow_travel_do_fcn(FLOW *f, void* ctx); 
    
    /// store flows of interest
    std::vector<FLOW> flows;

public:
    /// Constructor
    /// @param s flow table size
    /// @param t timeout
    StFlow(uint32_t s, double t);

    /// init flow table
    void create_table();

    /// update flow table
    /// @param p incoming packet
    /// @return the flow table entry
    FLOW* update_state(Packet& p);

    /// reset the flow table
    void flush(); 

    /// flow table traversal
    void travel_flows();

    /// get flows of interest
    /// @return the flows of interest
    std::vector<FLOW>* get_all_flows() {return &flows;}
};

#endif
