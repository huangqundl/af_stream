/*
 * SpoutDispatcher.hpp
 */

#ifndef __SpoutDispatcher_hpp__
#define __SpoutDispatcher_hpp__

#include <limits.h>
#include <string>

#include "ingress_base.hpp"
#include "Dispatcher/Router/Router.hpp"
#include "Packet.hpp"

/**
 * statistics of packets
 */
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

/**
 * Dispatcher to read streams from pcap file
 */
class SpoutDispatcher : public Dispatcher<Packet::BasicField> {
private:
    /**
     * statistics of trace packets
     */
    PacketStat packet_stat_;            // packet statistics

    /**
     * number of packets processed
     */
    uint64_t i_;

    /**
     * total number of packets
     */
    uint64_t pkt_cnt_;

    /**
     * instance of packet;
     */
    Packet p;

    // to generate timeout event
    /**
     * whether it is timeout
     */
    bool isTimeoutEvent;
    /**
     * timestamp of last timeout
     */
    double last_timeout;
    /**
     * length of time intervals
     */
    unsigned int timeoutEventInterval; // in second

    /**
     * timestamp of the first packet in the trace
     */
    double first_ts;

    /**
     * decode packets
     * @param pkt packet binary
     * @param len length of the binary
     * @param ts packet timestamp
     * @param p store the decoded packet
     * @return status of decoding result
     */
    PACKET_STATUS process_raw_packet(uint8_t* pkt, uint32_t len, double ts, Packet &p);

    Packet::BasicField* process(void*, uint32_t);

public:
    /**
     * Constructor
     * @param name name of the Dispatcher
     * @param filename input file name
     * @param size useless
     */
    SpoutDispatcher(std::string name, std::string filename, uint64_t size);
    void initialize();
    void finish();
};

#endif
