/*
 * FlowBoltComputeThread.hpp
 * - FlowBoltComputeThread thread - management on the flow level
 */

#ifndef __FlowBoltComputeThread_hpp__
#define __FlowBoltComputeThread_hpp__

#include <pthread.h>

#include "ComputeThread/ComputeThread.hpp"
#include "common.hpp"
#include "Packet.hpp"
#include "StFlow.hpp"
#include "HashFunc.hpp"

class FlowBoltComputeThread : public ComputeThread<Packet::TotalField, FLOW> {
private:
	Packet p;
    StFlow* st_flow_;                    // flow table
    uint32_t frag_table_size;

    uint64_t count;
    uint64_t expire_cnt;
    uint64_t expire_pkt;
    uint64_t export_pkt;

    // functions from class ComputeThread
    void process(Packet::TotalField&);
    void finish();

    double last_ts;
    unsigned int flowBoltTimeout;

#ifdef LOAD_BALANCE
    int get_exported_data(Packet::TotalField&, char*, uint32_t*, uint32_t*);
    int import_and_merge_data(char* data, uint32_t len);
    uint32_t get_state_key(Packet::TotalField& tf) {
        char key[7];
        unsigned int a = tf.bf.src_ip + tf.bf.dst_ip;
        unsigned short b = tf.bf.src_port + tf.bf.dst_port;
        memcpy(key, &a, 4);
        memcpy(key + 4, &b, 2);
        key[6] = (char)(tf.bf.proto);
        return HashFunc(key, 7);
    }
#endif

public:
    // public functions
    FlowBoltComputeThread(std::string name, int id);
    void initialize();
};

#endif
