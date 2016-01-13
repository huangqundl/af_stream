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
#include "SpoutDispatcher.hpp"
#include "PacketFieldGrouping.hpp"

/************************************************************************
 * functions from class Dispatcher
 * **********************************************************************/

Packet::TotalField* SpoutDispatcher::process(void* data, uint32_t length) {
    double pkt_ts;                                  // packet timestamp

    ++i_;
    if (i_ == 1) {
        starttime_ = now_us();
        last_time = starttime_;
    }
    if (i_ % 1 == 0) {
        LOG_DEBUG("nextTuple: number %" PRIu64 "\n", i_);
        if (is_report_stat) {
            report_stat();
        }
    }
    if (i_ > pkt_cnt_) {
        LOG_DEBUG("nextTuple: no packet\n");
    }

    // get the raw packet data and timestamp
    memcpy(&pkt_ts, data, 8);

    // process the raw packet 
    p.reset();
    PACKET_STATUS status = process_raw_packet((uint8_t*) data + 12, length, pkt_ts, p);

    if (status != STATUS_VALID) {
        LOG_DEBUG("nextTuple: not a valid packet\n");
        return 0;
    }
    return &(p.get_tf());
}

/************************************************************************
 * private functions
 * **********************************************************************/

PACKET_STATUS SpoutDispatcher::process_raw_packet(uint8_t* pkt, uint32_t len, double ts, Packet &p) {
    PACKET_STATUS status;  // status

    // decode
    status = p.decode(pkt, len, ts);

    // update packet stats
    packet_stat_.tot_pkt_cnt++;
    packet_stat_.tot_byte_cnt += len;
    switch (status) {
        case STATUS_VALID:
            packet_stat_.valid_pkt_cnt++;
            packet_stat_.valid_byte_cnt += p.get_ip_dlen();
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

    return status;
}

/************************************************************************
 *
 * Public Functions:
 *
 * **********************************************************************/

SpoutDispatcher::SpoutDispatcher(std::string name, std::string filename, uint64_t size) : Dispatcher<Packet::TotalField>(name) {
    Config *config = Config::getInstance();

    if (config->getboolean("readFromFile", false))
        adapter = new PcapFileAdapter(filename);
    else
        adapter = new PcapMemoryAdapter(filename, size, config->getint("replayTimes", 1));

    is_report_stat = config->getboolean("isReportStat", false);
    loadBalancer = new PacketFieldGrouping();
}

void SpoutDispatcher::initialize() {
    LOG_MSG("Dispatcher initializing\n");
    i_ = 0;
    last_byte = 0;
    memset(&packet_stat_, 0, sizeof(PacketStat));
    memset(&tmp_packet_stat, 0, sizeof(PacketStat));
    adapter->initiate(&pkt_cnt_);
    Dispatcher<Packet::TotalField>::initialize();
}

void SpoutDispatcher::report_stat() {
    uint64_t cur_ts = now_us();
    tmp_packet_stat.used_time = cur_ts - last_time;
    last_time = cur_ts;
    tmp_packet_stat.tot_byte_cnt = packet_stat_.tot_byte_cnt - last_byte;
    last_byte = packet_stat_.tot_byte_cnt;
    if (sizeof(tmp_packet_stat) <= CONTROL_MSG_MAX_SIZE) {
        ControlMsg msg;
        msg.type = STAT_DATA;
        msg.length = sizeof(tmp_packet_stat);
        memcpy(msg.data, &tmp_packet_stat, sizeof(tmp_packet_stat));
        while (this->controlQueue->write(&msg) == -1);
    }
    else
        LOG_WARN("stat data is too large.\n");
}

void SpoutDispatcher::report_final_stat() {
    uint64_t cur_ts = now_us();
    packet_stat_.used_time = cur_ts - starttime_;
    //starttime_ = cur_ts;
    if (sizeof(packet_stat_) <= CONTROL_MSG_MAX_SIZE) {
        ControlMsg msg;
        msg.type = STAT_DATA;
        msg.length = sizeof(packet_stat_);
        memcpy(msg.data, &packet_stat_, sizeof(packet_stat_));
        while (this->controlQueue->write(&msg) == -1);
    }
    else
        LOG_WARN("stat data is too large.\n");
}

void SpoutDispatcher::finish() {
    report_final_stat();
    Dispatcher<Packet::TotalField>::finish();
}
