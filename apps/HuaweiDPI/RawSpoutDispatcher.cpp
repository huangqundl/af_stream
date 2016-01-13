#include <stdio.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <inttypes.h>

#include "Config.hpp"
#include "Util.hpp"
#include "Dispatcher/Adapter/PcapMemoryAdapter.hpp"
#include "Dispatcher/Adapter/PcapFileAdapter.hpp"
#include "Dispatcher/Adapter/PcapDirAdapter.hpp"
#include "RawSpoutDispatcher.hpp"

RawPacket* RawSpoutDispatcher::process(void* data, uint32_t length)
{
    ++i_;

    // get the raw packet timestamp
    double pkt_ts = *(double *)data;
    last_ts = pkt_ts;

    if (i_ == 1) {
        start_time_ = now_us();
        start_ts = pkt_ts;
    }

    // update packet stats
    packet_stat_.tot_pkt_cnt++;
    packet_stat_.tot_byte_cnt += length;

    uint8_t *pkt = (uint8_t*) data + 12;

    if (p.decode(pkt, length, pkt_ts)) {
        packet_stat_.valid_pkt_cnt++;
        packet_stat_.valid_byte_cnt += length;
        return &p;
    } else {
        return NULL;
    }
}

RawSpoutDispatcher::RawSpoutDispatcher(std::string name, std::string filename,
        uint64_t size, LoadBalancer<RawPacket> *lb) :
    Dispatcher<RawPacket>(name) {
    Config *config = Config::getInstance();
    if (config->getboolean("readFromFile", false))
        adapter = new PcapFileAdapter(filename);
    else if (config->getboolean("readFromDir", false))
        adapter = new PcapDirAdapter(filename); 
    else
        adapter = new PcapMemoryAdapter(filename, size,
                config->getint("replayTimes", 1));

    loadBalancer = lb;
}

void RawSpoutDispatcher::initialize() {
    LOG_MSG("Dispatcher initializing\n");
    i_ = 0;
    last_byte = 0;
    memset(&packet_stat_, 0, sizeof(PacketStat));
    adapter->initiate(&pkt_cnt_);
    Dispatcher<RawPacket>::initialize();
}

void RawSpoutDispatcher::report_stat() {
    uint64_t cur_time = now_us();
    packet_stat_.used_time = cur_time - start_time_;
    double duration = (last_ts - start_ts);

    printf("\nDispatcher Stat\n"
            "===========================================\n"
            "\tTotal packets observed: %" PRIu64 "\n"
            "\tTotal bytes observed: %" PRIu64 "\n"
            "\tValid packet count: %" PRIu64 "\n"
            "\tValid byte count: %" PRIu64 "\n"
            "\tUsed time: %.2lf s\n"
            "\tSpeed: %.2lf packets/s (%.2lf Mb/s)\n"
            "\tDuration: %.2lf seconds (%.2lf mins)\n"
            "\tCapture rates: %.2lf packets/s, %.2lf Mb/s\n"
            "===========================================\n",
            packet_stat_.tot_pkt_cnt,
            packet_stat_.tot_byte_cnt,
            packet_stat_.valid_pkt_cnt,
            packet_stat_.valid_byte_cnt,
            packet_stat_.used_time / 1.0e6,
            packet_stat_.tot_pkt_cnt / (packet_stat_.used_time / 1.0e6),
            packet_stat_.tot_byte_cnt * 8.0 / packet_stat_.used_time,
            duration, duration / 60,
            packet_stat_.tot_pkt_cnt / duration,
            packet_stat_.tot_byte_cnt * 8.0 / duration / 1e6
            );
}

void RawSpoutDispatcher::finish() {
    report_stat();
    Dispatcher<RawPacket>::finish();
}
