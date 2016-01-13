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
    Config *config = Config::getInstance();
    copy_raw_ = config->getboolean("copyRaw", true);
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

    tf_.bf.data_len = 0;
    if (copy_raw_) {
        tf_.bf.raw_len = len < PAYLOAD_LEN ? len : PAYLOAD_LEN;
        memcpy(&tf_.raw_data, pkt, tf_.bf.raw_len);
    } else
        tf_.bf.raw_len = 0;

#ifdef GPRS_PARSER
    bssgp_msg bssgpMsg;
    PACKET_STATUS ret = pd->decode(pkt, len, ts, &ip_hdr, &ip_len, &bssgpMsg);
    tf_.bf.bssgp_type = bssgpMsg.bssgp_type;
    strncpy(tf_.bf.bssgp_IMSI, bssgpMsg.bssgp_IMSI, IMSI_LEN);
    tf_.bf.bssgp_IMSI[IMSI_LEN - 1] = '\0';
    tf_.bf.bssgp_tlli_current = bssgpMsg.bssgp_tlli_current;
    tf_.bf.bssgp_tlli_old = bssgpMsg.bssgp_tlli_old;
#else
    PACKET_STATUS ret = pd->decode(pkt, len, ts, &ip_hdr, &ip_len);
#endif
    tf_.bf.seq = ++seq_count;
    tf_.bf.status = ret;
    if (ret != STATUS_VALID)
        return ret;

    // assign the fields
    tf_.bf.pkt_ts = ts;
    tf_.bf.src_ip = ip_hdr->ip_src.s_addr;
    tf_.bf.dst_ip = ip_hdr->ip_dst.s_addr;
    tf_.bf.ip_dlen = ntohs(ip_hdr->ip_len);
    tf_.bf.proto = ip_hdr->ip_p;

    if (ip_hdr->ip_p == IPPROTO_TCP) {
        // TCP
        tcp_hdr = (struct tcphdr*)((uint8_t*)ip_hdr + (ip_hdr->ip_hl << 2));
        tf_.bf.src_port = ntohs(tcp_hdr->source);
        tf_.bf.dst_port = ntohs(tcp_hdr->dest);
    }
    else if (ip_hdr->ip_p == IPPROTO_UDP) {
        // UDP
        udp_hdr = (struct udphdr*)((uint8_t*)ip_hdr + (ip_hdr->ip_hl << 2));
        tf_.bf.src_port = ntohs(udp_hdr->source);
        tf_.bf.dst_port = ntohs(udp_hdr->dest);
    } else {
        // Other L4
        tf_.bf.src_port = 0;
        tf_.bf.dst_port = 0;
    }

    ip_len = ip_len > PAYLOAD_LEN ? PAYLOAD_LEN : ip_len;
    tf_.bf.data_len = ip_len;
    memcpy(&tf_.payload, ip_hdr, ip_len);

    pd->clean_up();

    return STATUS_VALID;
}

