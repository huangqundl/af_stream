/*
 * ResultWriter.cpp
 * - ResultWriter thread - management on the global level
 */

#include <math.h>

#include "Config.hpp"
#include "common.hpp"
#include "collector_result_writer.hpp"

void ResultWriter::output_packet(Packet::BasicField &e) {
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
    fprintf(fout, "%lf %u %u\n",
            e.pkt_ts,
            e.src_ip,
            e.ip_dlen);
#endif
}

/****************************************************************************
 * Private functions
 ****************************************************************************/
void ResultWriter::process(Packet::BasicField &e) {
    LOG_DEBUG("MainComputeThread processing\n");
    tot_packet_count++;
    if (e.status == STATUS_VALID) {
        valid_packet_count++;
    }
    else {
        invalid_packet_count++;
    }
    output_packet(e);
    if (last_ts>0 && (uint64_t)e.pkt_ts/600 > (uint64_t)last_ts/600) {
        fprintf(fout, "timeout\n");
    }
    last_ts = e.pkt_ts;
}

void ResultWriter::finish() {
    printf("===================================================\n");
    printf("total packet count: %" PRIu64 "\n", tot_packet_count);
    printf("valid packet count: %" PRIu64 "\n", valid_packet_count);
    printf("invalid packet count: %" PRIu64 "\n", invalid_packet_count);
    printf("===================================================\n");
    Collector<Packet::BasicField, Dummy>::finish();
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
ResultWriter::ResultWriter(std::string name, int num): Collector<Packet::BasicField, Dummy>(name, num) {
    tot_packet_count = 0;
    valid_packet_count = 0;
    invalid_packet_count = 0;
    last_ts = 0;
}

void ResultWriter::initialize() {
    Config *config = Config::getInstance();
    const char *output_name = config->getstring("outputFile", NULL);
    if (output_name == NULL) {
        fout = NULL;
    } else {
        fout = fopen(output_name, "w");
        if (!fout)
            LOG_ERR("cannot open file %s\n", output_name);
    }

    Collector<Packet::BasicField, Dummy>::initialize();
}
