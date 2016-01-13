/*
 * TrueAnalyzer.hpp
 * - TrueAnalyzer thread - management on the flow level
 */

#ifndef __TrueAnalyzer_hpp__
#define __TrueAnalyzer_hpp__

#include <pthread.h>
#include "Analyzer.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "../Packet.hpp"
#include "../DetectCollector.hpp"
#include "Flow.hpp"
#include "StFlow.hpp"

#define MB (1024*1024)

/**
 * Count flows to build ground true
 */
class TrueAnalyzer : public Analyzer<Packet::BasicField, Dummy> {

private:
    /// Packet
	Packet p;
    /// Flow table
    StFlow* st_flow_;

    /// number of events
    uint64_t count;
    /// number of packets
    uint64_t total_pkt;
    /// number of bytes
    uint64_t total_byte;

    /// length of time interval
    unsigned int printInterval;

    /// timestamp of last time to print
    double last_ts;                         // metrics to print
    /// timestamp of the first packet
    double first_ts;
    /// number of time intervals
    int printCount;
    /// output file
    FILE* fp;
    /// print out flows
    /// @param f the flow
    void printFlow(FLOW& f);

    /// threshold for heavy key detection
    double thresh;
    
    /// detect heavy keys
    void detect();

    void timeoutHandler(Packet::BasicField& bf);

    // functions from class Analyzer
    void process(Packet::BasicField&);

    void finish();

#ifdef LOAD_BALANCE
    int get_exported_data(Packet::BasicField&, char*, uint32_t*, uint32_t*);
    int import_and_merge_data(char* data, uint32_t len);
    uint32_t get_state_key(Packet::BasicField& bf) {
        char key[7];
        unsigned int a = bf.src_ip + bf.dst_ip;
        unsigned short b = bf.src_port + bf.dst_port;
        memcpy(key, &a, 4);
        memcpy(key + 4, &b, 2);
        key[6] = (char)(bf.proto);
        return HashFunc(key, 7);
    }
#endif

public:
    // public functions
    /// Constructor
    /// @param name name of the Analyzer
    /// @param id id of the Analyzer
    TrueAnalyzer(std::string name, int id);

    void initialize();
};

#endif
