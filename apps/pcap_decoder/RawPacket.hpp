#ifndef RAW_PACKET_HPP
#define RAW_PACKET_HPP

#include <stdint.h>
#include <string.h>
#include <netinet/ether.h>
#include <netinet/ip.h>

#define PAYLOAD_LEN 1600
#define HAVE_ETHERNET_HEADER 0

class RawPacket {
public:
    uint32_t key;
    uint32_t len;
    double ts;
    char payload[PAYLOAD_LEN];

    RawPacket& operator = (const RawPacket& p) {
        this->key = p.key;
        this->len = p.len;
        this->ts = p.ts;
        memcpy(this->payload, p.payload, p.len);
        return *this;
    }

    uint32_t size() { return sizeof(uint32_t) * 2 + sizeof(double) + len; }
    uint32_t load() { return sizeof(uint32_t) * 2 + sizeof(double) + len; }

	bool decode(uint8_t* pkt, uint32_t len, double ts) {
        int eth_len = 0;
        struct ip* ip_hdr = NULL;
        if (HAVE_ETHERNET_HEADER) {
            struct ether_header* eth_hdr;
            eth_hdr = (struct ether_header*)pkt;
            eth_len = 14;
            if (ntohs(eth_hdr->ether_type) == ETHERTYPE_VLAN) {
                eth_len = 18;
            }
            else if (ntohs(eth_hdr->ether_type) != ETH_P_IP) {
                return false;
            }
        }

        // error checking (IP level)
        ip_hdr = (struct ip*)(pkt + eth_len);
        // i) IP header length check
        if ((int)len < eth_len + (ip_hdr->ip_hl << 2)) {
            return false;
        }
        // ii) IP version check
        if (ip_hdr->ip_v != 4) {
            return false;
        }

        this->ts = ts;
        this->len = len;
        this->key = ip_hdr->ip_src.s_addr + ip_hdr->ip_dst.s_addr;
        if (this->len > PAYLOAD_LEN)
            return false;
        memcpy(this->payload, pkt, this->len);
        return true;
    }
};

#endif // RAW_PACKET_HPP
