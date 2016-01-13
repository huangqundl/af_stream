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
#include "Flow.hpp"

class FlowBoltComputeThread : public ComputeThread<Packet::BasicField, FLOW> {

private:
    uint64_t count;
    unsigned int scale;

    // functions from class ComputeThread
    void process(Packet::BasicField&);
    void finish();

#ifdef LOAD_BALANCE
    int get_exported_data(Packet::BasicField&, char*, uint32_t*, uint32_t*);
    int import_and_merge_data(char* data, uint32_t len);
    /*
    uint32_t get_state_key(Packet::BasicField& bf) {
        char key[7];
        unsigned int a = bf.src_ip + bf.dst_ip;
        unsigned short b = bf.src_port + bf.dst_port;
        memcpy(key, &a, 4);
        memcpy(key + 4, &b, 2);
        key[6] = (char)(bf.proto);
        return HashFunc(key, 7);
    }
    */
#endif

public:
    // public functions
    FlowBoltComputeThread(std::string name, int id);
    void initialize();
};

#endif
