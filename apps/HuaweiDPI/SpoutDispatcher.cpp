/*
 * SpoutDispatcher.cpp
 */

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
#include "SpoutDispatcher.hpp"
#include "LB/PacketFieldBalancer.hpp"

/************************************************************************
 * functions from class Dispatcher
 * **********************************************************************/

Packet::TotalField* SpoutDispatcher::process(void* data, uint32_t length)
{
    ++i_;

    // get the raw packet timestamp
    double pkt_ts = *(double *)data;
    last_ts = pkt_ts;

    if (i_ == 1) {
        start_time = now_us();
        last_time = start_time;
        start_ts = pkt_ts;
    }


    uint8_t *pkt = (uint8_t*) data + 12;
    PACKET_STATUS status = p.decode(pkt, length, pkt_ts);

    // update packet stats
    packet_stat_.tot_pkt_cnt++;
    packet_stat_.tot_byte_cnt += length;
    switch (status) {
        case STATUS_VALID:
            packet_stat_.valid_pkt_cnt++;
            packet_stat_.valid_byte_cnt += length;
            break;
        case STATUS_NON_IP:
            packet_stat_.non_ip_cnt++;
            LOG_DEBUG("non valid status: non ip\n");
            break;
        case STATUS_IP_NOT_FULL:
            packet_stat_.ip_not_full_cnt++;
            LOG_DEBUG("non valid status: ip not full\n");
            break;
        case STATUS_IP_VER_FAIL:
            packet_stat_.ip_ver_fail_cnt++;
            LOG_DEBUG("non valid status: ip ver fail\n");
            break;
        case STATUS_IP_CHKSUM_FAIL:
            packet_stat_.ip_chksum_fail_cnt++;
            LOG_DEBUG("non valid status: ip chksum fail\n");
            break;
            break;
        case STATUS_IP_FRAG:
            packet_stat_.ip_frag_cnt++;
            LOG_DEBUG("non valid status: ip frag\n");
            break;
        case STATUS_TCP_NOT_FULL:
            packet_stat_.tcp_not_full_cnt++;
            LOG_DEBUG("non valid status: tcp not full\n");
            break;
        case STATUS_UDP_NOT_FULL:
            packet_stat_.udp_not_full_cnt++;
            LOG_DEBUG("non valid status: udp not full\n");
            break;
        case STATUS_ICMP_NOT_FULL:
            packet_stat_.icmp_not_full_cnt++;
            LOG_DEBUG("non valid status: icmp not full\n");
            break;
        default:
            break;
    }

    if (status != STATUS_VALID) {
        LOG_DEBUG("nextTuple: not a valid packet\n");
        //return 0;
    }

    return &(p.get_tf());
}

/************************************************************************
 *
 * Public Functions:
 *
 * **********************************************************************/

SpoutDispatcher::SpoutDispatcher(std::string name, std::string filename,
        uint64_t size, LoadBalancer<Packet::TotalField> *lb) :
    Dispatcher<Packet::TotalField>(name) {
    Config *config = Config::getInstance();
    if (config->getboolean("readFromFile", false))
        adapter = new PcapFileAdapter(filename);
    else if (config->getboolean("readFromDir", false))
        adapter = new PcapDirAdapter(filename); 
    else
        adapter = new PcapMemoryAdapter(filename, size, config->getint("replayTimes", 1));

    loadBalancer = lb;
}

void SpoutDispatcher::initialize() {
    LOG_MSG("Dispatcher initializing\n");
    i_ = 0;
    last_byte = 0;
    memset(&packet_stat_, 0, sizeof(PacketStat));
    adapter->initiate(&pkt_cnt_);
    Dispatcher<Packet::TotalField>::initialize();
}

void SpoutDispatcher::report_final_stat() {
    uint64_t cur_ts = now_us();
    packet_stat_.used_time = cur_ts - start_time;
    if (sizeof(packet_stat_) <= CONTROL_MSG_MAX_SIZE) {
        ControlMsg msg;
        msg.type = STAT_DATA;
        msg.length = sizeof(packet_stat_);
        memcpy(msg.data, &packet_stat_, sizeof(packet_stat_));
        while (this->controlQueue->write(&msg) == -1);
    }
    else
        LOG_WARN("stat data is too large.\n");

    double duration = (last_ts - start_ts);
    printf("\nDispatcher Stat\n"
            "===========================================\n"
            "\tTotal packets observed: %" PRIu64 "\n"
            "\tTotal bytes observed: %" PRIu64 "\n"
            "\tValid packet count: %" PRIu64 "\n"
            "\tValid byte count: %" PRIu64 "\n"
            "\tNon-IP packet count: %" PRIu64 "\n"
            "\tIP-not-full packet count: %" PRIu64 "\n"
            "\tIP-version-failed packet count: %" PRIu64 "\n"
            "\tTCP-not-full packet count: %" PRIu64 "\n"
            "\tIP Fragment packet count: %" PRIu64 "\n"
            "\tUsed time: %.2lf s\n"
            "\tSpeed: %.2lf packets/s (%.2lf Mb/s)\n"
            "\tDuration: %.2lf seconds (%.2lf mins)\n"
            "\tCapture rates: %.2lf packets/s, %.2lf Mb/s\n"
            "===========================================\n",
            packet_stat_.tot_pkt_cnt,
            packet_stat_.tot_byte_cnt,
            packet_stat_.valid_pkt_cnt,
            packet_stat_.valid_byte_cnt,
            packet_stat_.non_ip_cnt,
            packet_stat_.ip_not_full_cnt,
            packet_stat_.ip_ver_fail_cnt,
            packet_stat_.tcp_not_full_cnt,
            packet_stat_.ip_frag_cnt,
            packet_stat_.used_time / 1.0e6,
            packet_stat_.tot_pkt_cnt / (packet_stat_.used_time / 1.0e6),
            packet_stat_.tot_byte_cnt * 8.0 / packet_stat_.used_time,
            duration, duration / 60,
            packet_stat_.tot_pkt_cnt / duration,
            packet_stat_.tot_byte_cnt * 8.0 / duration / 1e6
            );
}

void SpoutDispatcher::finish() {
    report_final_stat();
    Dispatcher<Packet::TotalField>::finish();
}
