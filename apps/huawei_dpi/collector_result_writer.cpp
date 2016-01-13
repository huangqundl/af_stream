/*
 * DPIBoltMainComputeThread.cpp
 * - DPIBoltMainComputeThread thread - management on the global level
 */

#include <math.h>

#include "Config.hpp"
#include "common.hpp"
#include "collector_result_writer.hpp"
#include "base64.h"

void DPIBoltMainComputeThread::output_packet(Packet::TotalField &e) {
    if (!fout)
        return;
#ifdef GPRS_PARSER
    char addr1[30];
    char addr2[30];
    fprintf(fout, "%" PRIu64 "|%lf|%d|%u|%s|%s|%u|%s|%u|%u|\n", e.bf.seq,
            e.bf.pkt_ts, e.bf.appId, e.bf.raw_len, ip2a(e.bf.src_ip, addr1),
            ip2a(e.bf.dst_ip, addr2), e.bf.bssgp_type, e.bf.bssgp_IMSI, e.bf.bssgp_tlli_current,
            e.bf.bssgp_tlli_old);
#else
    fprintf(fout, "%" PRIu64 "|%lf|%d|%u|%s|\n", e.bf.seq, e.bf.pkt_ts,
            e.bf.appId, e.bf.raw_len, base64_encode(e.raw_data,
                e.bf.raw_len).c_str());
#endif
}

/****************************************************************************
 * Private functions
 ****************************************************************************/
void DPIBoltMainComputeThread::process(Packet::TotalField &e) {
    LOG_DEBUG("MainComputeThread processing\n");
    tot_packet_count++;
    if (e.bf.status == STATUS_VALID) {
        valid_packet_count++;
        if (e.bf.appId != 0)
            known_packet_count++;
    }
    else
        unknown_packet_count++;
    output_packet(e);

    if (dumper && e.bf.status == STATUS_VALID) {
        pcap_pkthdr hdr;
        hdr.ts.tv_sec = floor(e.bf.pkt_ts);
        hdr.ts.tv_usec = (e.bf.pkt_ts - floor(e.bf.pkt_ts)) * 1e6;
        hdr.caplen = hdr.len = e.bf.data_len;
        pcap_dump((u_char *) dumper, &hdr, e.payload);
    }
}

void DPIBoltMainComputeThread::finish() {
    if (dumper)
        pcap_dump_close(dumper);
    printf("===================================================\n");
    printf("total packet count: %" PRIu64 "\n", tot_packet_count);
    printf("known packet count: %" PRIu64 "\n", known_packet_count);
    printf("valid packet count: %" PRIu64 "\n", valid_packet_count);
    printf("detection rate: %.2lf %%\n", (double) known_packet_count / valid_packet_count * 100);
    printf("===================================================\n");
    Collector<Packet::TotalField, Dummy>::finish();
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
DPIBoltMainComputeThread::DPIBoltMainComputeThread(std::string name, int num): Collector<Packet::TotalField, Dummy>(name, num) {
    tot_packet_count = 0;
    valid_packet_count = 0;
    known_packet_count = 0;
    unknown_packet_count = 0;
}

void DPIBoltMainComputeThread::initialize() {
    Config *config = Config::getInstance();
    const char *output_name = config->getstring("outputFile", NULL);
    if (output_name == NULL) {
        fout = NULL;
    } else {
        fout = fopen(output_name, "w");
        if (!fout)
            LOG_ERR("cannot open file %s\n", output_name);
    }
    pcap_t *pcap = pcap_open_dead(DLT_RAW, 1600);
    char *filename = config->getstring("dumpFile", NULL);
    if (filename)
        dumper = pcap_dump_open(pcap, filename);
    else
        dumper = NULL;

    Collector<Packet::TotalField, Dummy>::initialize();
}
