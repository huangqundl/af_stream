/*
 * collector_result_writer.hpp
 * - define and implement ResultWriter (basic class Collector)
 * - 1: collect packets decoded by ComputeThread (compute_thread_pcap_decoder)
 * - 2: write the packets into files as key/value pairs
 */

#ifndef COLLECTOR_RESULT_WRITER_HPP_
#define COLLECTOR_RESULT_WRITER_HPP_

#include <stdio.h>

#include "Collector.hpp"
#include "common.hpp"
#include "Packet.hpp"

class Dummy {
public:
    uint32_t size() { return 0; }
    uint32_t get_size() { return 0; }
};

class ResultWriter : public Collector<Packet::BasicField, Dummy> {
private:
    FILE *fout;
    uint64_t tot_packet_count;
    uint64_t valid_packet_count;
    uint64_t invalid_packet_count;
    double last_ts;

    void process(Packet::BasicField& e);
    void finish();
    void output_packet(Packet::BasicField &e);

public:
    ResultWriter(std::string name, int num);
    void initialize();
};

#endif // COLLECTOR_RESULT_WRITER_HPP_
