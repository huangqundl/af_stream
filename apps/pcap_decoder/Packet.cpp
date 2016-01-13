/*
 * Packet.cpp
 * - packet header parsed from raw packets
 */

#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include "Packet.hpp"
#include "Config.hpp"

#ifdef GPRS_PARSER
extern "C" {
#include "proto_bssgp_parse.h"
}
#endif

uint64_t Packet::seq_count = 0;

Packet::Packet() {
    pd = new PacketDecoder();
}

Packet::~Packet() {
    delete pd;
}

/*
 * decode raw packets and retrieve the useful fields
 *
 * @param pkt - pointer to the raw Ethernet packet
 * @param len - length of the raw Ethernet packet
 * @param ts - timestamp of the received packet
 * @return - the decoding status
 */
PACKET_STATUS Packet::decode(uint8_t* pkt, uint32_t len, double ts) {
    struct ip* ip_hdr;
    struct tcphdr* tcp_hdr;
    struct udphdr* udp_hdr;
    uint32_t ip_len;

    //bf.data_len = 0;

#ifdef GPRS_PARSER
    bssgp_msg bssgpMsg;
    PACKET_STATUS ret = pd->decode(pkt, len, ts, &ip_hdr, &ip_len, &bssgpMsg);
    bf.bssgp_type = bssgpMsg.bssgp_type;
    strncpy(bf.bssgp_IMSI, bssgpMsg.bssgp_IMSI, IMSI_LEN);
    bf.bssgp_IMSI[IMSI_LEN - 1] = '\0';
    bf.bssgp_tlli_current = bssgpMsg.bssgp_tlli_current;
    bf.bssgp_tlli_old = bssgpMsg.bssgp_tlli_old;
#else
    PACKET_STATUS ret = pd->decode(pkt, len, ts, &ip_hdr, &ip_len);
#endif
    bf.seq = ++seq_count;
    bf.status = ret;
    if (ret != STATUS_VALID)
        return ret;

    // assign the fields
    bf.pkt_ts = ts;
    bf.src_ip = ip_hdr->ip_src.s_addr;
    bf.dst_ip = ip_hdr->ip_dst.s_addr;
    bf.ip_dlen = ntohs(ip_hdr->ip_len);
    bf.proto = ip_hdr->ip_p;

    if (ip_hdr->ip_p == IPPROTO_TCP) {
        // TCP
        tcp_hdr = (struct tcphdr*)((uint8_t*)ip_hdr + (ip_hdr->ip_hl << 2));
        bf.src_port = ntohs(tcp_hdr->source);
        bf.dst_port = ntohs(tcp_hdr->dest);
    }
    else if (ip_hdr->ip_p == IPPROTO_UDP) {
        // UDP
        udp_hdr = (struct udphdr*)((uint8_t*)ip_hdr + (ip_hdr->ip_hl << 2));
        bf.src_port = ntohs(udp_hdr->source);
        bf.dst_port = ntohs(udp_hdr->dest);
    } else {
        // Other L4
        bf.src_port = 0;
        bf.dst_port = 0;
    }

    //ip_len = ip_len > PAYLOAD_LEN ? PAYLOAD_LEN : ip_len;
    //bf.data_len = ip_len;

    pd->clean_up();

    return STATUS_VALID;
}

