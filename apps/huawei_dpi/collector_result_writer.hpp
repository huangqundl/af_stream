/*
 * DPIBoltMainComputeThread.hpp
 * - DPIBoltMainComputeThread thread - management on the global level
 */

#ifndef COLLECTOR_RESULT_WRITER_HPP_
#define COLLECTOR_RESULT_WRITER_HPP_

#include <stdio.h>

#include "Collector.hpp"
#include "common.hpp"
#include "Packet.hpp"
#include "Flow.hpp"

class Dummy {
public:
    uint32_t size() { return 0; }
    uint32_t get_size() { return 0; }
};

class DPIBoltMainComputeThread : public Collector<Packet::TotalField, Dummy> {
private:
    FILE *fout;
    uint64_t tot_packet_count;
    uint64_t valid_packet_count;
    uint64_t known_packet_count;
    uint64_t unknown_packet_count;
    pcap_dumper_t *dumper;

    void process(Packet::TotalField& e);
    void finish();
    void output_packet(Packet::TotalField &e);

public:
    DPIBoltMainComputeThread(std::string name, int num);
    void initialize();
};

#endif // COLLECTOR_RESULT_WRITER_HPP_
