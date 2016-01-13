/*
 * SpoutDispatcher.hpp
 */

#ifndef __SpoutDispatcher_hpp__
#define __SpoutDispatcher_hpp__

#include <limits.h>
#include <string>

#include "../../Dispatcher/Dispatcher.hpp"
#include "../../Dispatcher/Router/Router.hpp"
#include "Packet.hpp"

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
    uint64_t used_time;
} PacketStat;

class SpoutDispatcher : public Dispatcher<Packet::TotalField> {
private:
    PacketStat packet_stat_;            // packet statistics
    PacketStat tmp_packet_stat;            // packet statistics
    uint64_t i_;
    uint64_t pkt_cnt_;
    uint64_t starttime_;
    uint64_t last_time;
    uint64_t last_byte;
    
    Packet p;

    PACKET_STATUS process_raw_packet(uint8_t* pkt, uint32_t len, double ts, Packet &p);
    Packet::TotalField* process(void*, uint32_t);

    // report throughput every some packets
    bool is_report_stat;
    void report_stat();
    void report_final_stat();

public:
    SpoutDispatcher(std::string, std::string, uint64_t);
    void initialize();
    void finish();
};

#endif
