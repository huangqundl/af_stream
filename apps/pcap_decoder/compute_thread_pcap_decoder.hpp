#ifndef ANALYZER_PCAP_DECODER_HPP_
#define ANALYZER_PCAP_DECODER_HPP_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <linux/udp.h>
#include <linux/if_ether.h>
#include <pcap.h>
#include <string>

#include "ComputeThread.hpp"

#include "Packet.hpp"
#include "RawPacket.hpp"

typedef struct {
    uint64_t tot_pkt_cnt;           // total # of packets observed
    uint64_t tot_byte_cnt;          // total # of bytes observed
    uint64_t valid_pkt_cnt;         // # of valid packets
    uint64_t valid_byte_cnt;        // # of valid bytes
    uint64_t non_ip_cnt;            // # of non-IP packets
    uint64_t ip_not_full_cnt;       // # of IP non-full packets
    uint64_t ip_ver_fail_cnt;       // # of IP version failed packets
    uint64_t ip_chksum_fail_cnt;    // # of IP checksum failed packets
    uint64_t ip_frag_cnt;           // # of IP fragments 
    uint64_t tcp_not_full_cnt;      // # of TCP-not-full packets
    uint64_t udp_not_full_cnt;      // # of UDP-not-full packets
    uint64_t icmp_not_full_cnt;     // # of ICMP-not-full packets
    uint64_t undefined_cnt;
    uint64_t non_gtp_cnt;
    uint64_t non_gprs_cnt;
    uint64_t used_time;
    double trace_start_ts;
    double trace_end_ts;
} PacketStat;

class PcapDecoder : public ComputeThread<RawPacket, Packet::BasicField> {
private:
    Packet p;
    uint64_t count;
    PacketStat packet_stat;     // packet statistics
    uint64_t start_time;

    void report_final_stat();

    // functions from basic class ComputeThread
    void finish();
    void process(RawPacket&);
    
public:
    // public functions
    PcapDecoder(std::string name, int id);

    void initialize();
};

#endif // ANALYZER_PCAP_DECODER
