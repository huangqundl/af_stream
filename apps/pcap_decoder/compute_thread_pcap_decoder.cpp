#include <inttypes.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util.hpp"
#include "Config.hpp"
#include "common.hpp"
#include "compute_thread_pcap_decoder.hpp"

extern "C" {
#include "nids.h"
#include "ip_fragment.h"
}

void PcapDecoder::process(RawPacket &rp)
{
    count++;
    if (count % 10000 == 0)
        LOG_MSG("\rProcessing %8" PRIu64 "th packet", count);

    if (count == 1) {
        start_time = now_us();
        packet_stat.trace_start_ts = rp.ts;
    }
    packet_stat.trace_end_ts = rp.ts;

    PACKET_STATUS status = p.decode((uint8_t *) rp.payload, rp.len, rp.ts);
    // update packet stats
    packet_stat.tot_pkt_cnt++;
    packet_stat.tot_byte_cnt += rp.len;
    switch (status) {
        case STATUS_VALID:
            packet_stat.valid_pkt_cnt++;
            packet_stat.valid_byte_cnt += rp.len;
            break;
        case STATUS_NON_IP:
            packet_stat.non_ip_cnt++;
            LOG_DEBUG("non valid status: non ip\n");
            break;
        case STATUS_IP_NOT_FULL:
            packet_stat.ip_not_full_cnt++;
            LOG_DEBUG("non valid status: ip not full\n");
            break;
        case STATUS_IP_VER_FAIL:
            packet_stat.ip_ver_fail_cnt++;
            LOG_DEBUG("non valid status: ip ver fail\n");
            break;
        case STATUS_IP_CHKSUM_FAIL:
            packet_stat.ip_chksum_fail_cnt++;
            LOG_DEBUG("non valid status: ip chksum fail\n");
            break;
        case STATUS_IP_FRAG:
            packet_stat.ip_frag_cnt++;
            LOG_DEBUG("non valid status: ip frag\n");
            break;
        case STATUS_TCP_NOT_FULL:
            packet_stat.tcp_not_full_cnt++;
            LOG_DEBUG("non valid status: tcp not full\n");
            break;
        case STATUS_UDP_NOT_FULL:
            packet_stat.udp_not_full_cnt++;
            LOG_DEBUG("non valid status: udp not full\n");
            break;
        case STATUS_ICMP_NOT_FULL:
            packet_stat.icmp_not_full_cnt++;
            LOG_DEBUG("non valid status: icmp not full\n");
            break;
        case STATUS_UNDEFINED:
            packet_stat.undefined_cnt++;
            LOG_DEBUG("non valid status: packet not defined\n");
            break;
        case STATUS_NON_GTP:
            packet_stat.non_gtp_cnt++;
            LOG_DEBUG("non valid status: non gtp\n");
            break;
        case STATUS_NON_GPRS:
            packet_stat.non_gprs_cnt++;
            LOG_DEBUG("non valid status: non gprs\n");
            break;
        default:
            break;
    }

    emit(p.get_bf());
}

void PcapDecoder::finish()
{
    LOG_MSG("ComputeThread (PcapDecoder) %d (%d) processed %" PRIu64 "\n", subId,
            getpid(), count);

    report_final_stat();
    ComputeThread<RawPacket, Packet::BasicField>::finish();
}

PcapDecoder::PcapDecoder(std::string name, int id) :
    ComputeThread<RawPacket, Packet::BasicField>(name, id)
{
    count = 0;
}

void PcapDecoder::initialize()
{
    memset(&packet_stat, 0, sizeof(PacketStat));
    ComputeThread<RawPacket, Packet::BasicField >::initialize();
}

void PcapDecoder::report_final_stat() {
    uint64_t cur_ts = now_us();
    packet_stat.used_time = cur_ts - start_time;

    LOG_MSG("\nPcapDecoder (ComputeThread) Stat\n"
            "===========================================\n"
            "\tTotal packets observed: %" PRIu64 "\n"
            "\tTotal bytes observed: %" PRIu64 "\n"
            "\tValid packet count: %" PRIu64 "\n"
            "\tValid byte count: %" PRIu64 "\n"
            "\tNon-IP packet count: %" PRIu64 "\n"
            "\tIP-not-full packet count: %" PRIu64 "\n"
            "\tIP-version-failed packet count: %" PRIu64 "\n"
            "\tIP-checksum-failed packet count: %" PRIu64 "\n"
            "\tIP-fragment packet count: %" PRIu64 "\n"
            "\tTCP-not-full packet count: %" PRIu64 "\n"
            "\tUDP-not-full packet count: %" PRIu64 "\n"
            "\tICMP-not-full packet count: %" PRIu64 "\n"
            "\tUndefined packet count: %" PRIu64 "\n"
            "\tNon-GTP packet count: %" PRIu64 "\n"
            "\tNon-GPRS packet count: %" PRIu64 "\n"
            "\tUsed time: %.2lf s\n"
            "\tSpeed: %.2lf packets/s (%.2lf Mb/s)\n"
            "\tTrace duration: %.2lf s\n"
            "===========================================\n",
            packet_stat.tot_pkt_cnt,
            packet_stat.tot_byte_cnt,
            packet_stat.valid_pkt_cnt,
            packet_stat.valid_byte_cnt,
            packet_stat.non_ip_cnt,
            packet_stat.ip_not_full_cnt,
            packet_stat.ip_ver_fail_cnt,
            packet_stat.ip_chksum_fail_cnt++,
            packet_stat.ip_frag_cnt++,
            packet_stat.tcp_not_full_cnt,
            packet_stat.udp_not_full_cnt,
            packet_stat.icmp_not_full_cnt,
            packet_stat.undefined_cnt,
            packet_stat.non_gtp_cnt,
            packet_stat.non_gprs_cnt,
            packet_stat.used_time / 1.0e6,
            packet_stat.tot_pkt_cnt / (packet_stat.used_time / 1.0e6),
            packet_stat.tot_byte_cnt * 8.0 / packet_stat.used_time,
            packet_stat.trace_end_ts-packet_stat.trace_start_ts);
}
