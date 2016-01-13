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
    
uint64_t Packet::seq_count = 0;

Packet::Packet() {
    pd = new PacketDecoder();
}

Packet::~Packet() {
    delete pd;
}

/*
 * reset packet
 */
void Packet::reset() {
    memset(&bf, 0, sizeof(bf));
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

    // reset the packet
    reset();

    // decode packet and remove outer ip header
    // inner ip header is stored in ip_hdr
    PACKET_STATUS ret = pd->decode(pkt, len, ts, &ip_hdr, &ip_len);
    if (ret != STATUS_VALID)
        return ret;

    // assign the fields
    //bf.pkt_ts = ts;
    bf.src_ip = ip_hdr->ip_src.s_addr;
    bf.dst_ip = ip_hdr->ip_dst.s_addr;
    //bf.ip_dlen = ntohs(ip_hdr->ip_len);
    bf.accumulated_pkt = 1;
    bf.accumulated_byte = ntohs(ip_hdr->ip_len);
    bf.seq = ++seq_count;
    bf.pre_assign = -1;
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

    pd->clean_up();

    return STATUS_VALID;
}

