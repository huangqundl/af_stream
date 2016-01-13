/*
 * Packet.hpp
 * - packet header parsed from raw Ethernet packets
 */

#ifndef __Packet_h__
#define __Packet_h__

#include <stdint.h>
#include "PacketDecoder.hpp"

#define PAYLOAD_LEN 1500

class Packet {
public:
    // This class will be transferred on the wire between 32/64 bit machines,
    // so we add some dummy char array in the field.
    // This class include BasicField and payload of a packet.
	class BasicField {
    public:
		// 8 bytes
		double pkt_ts;				// timestamp of the packet
		// 8 bytes
		uint32_t src_ip;			// source IP address
		uint32_t dst_ip;			// dest IP address
		// 8 bytes
		uint16_t src_port;			// source port
		uint16_t dst_port;			// dest port
		uint16_t ip_dlen;			// inner IP datagram length (header + data)
		uint8_t proto;				// protocol
        // 8 bytes
		uint16_t data_len;			// true if no more incoming packets
		uint8_t is_tcpsyn:1;		// 1 if TCP SYN is set
		uint8_t is_tcpfin:1;		// 1 if TCP FIN is set
		uint8_t is_tcpack:1;		// 1 if TCP ACK is set
        char pad[5];

        uint32_t size() { return sizeof(BasicField); }
        uint32_t load() { return ip_dlen; }
	};

private:
    PacketDecoder* pd;

	// *** data members ***
	uint8_t start_;				// the start placeholder of all data members

	// *** START packet defintions - decoded from the raw packet
    BasicField bf;
    // *** END packet defintions - decoded from the raw packet

    void* flow_state_;				// pointer to the flow state

	uint8_t end_;				// the end placeholder of all data members

public:
    Packet();
    ~Packet();
	void reset();
    PACKET_STATUS decode(uint8_t* pkt, uint32_t len, double ts);

	inline double get_pkt_ts() { return bf.pkt_ts; }
	inline uint32_t get_src_ip() { return bf.src_ip; }
	inline uint32_t get_dst_ip() { return bf.dst_ip; }
	inline uint16_t get_src_port() { return bf.src_port; }
	inline uint16_t get_dst_port() { return bf.dst_port; }
	inline uint8_t get_proto() { return bf.proto; }
	inline uint16_t get_ip_dlen() { return bf.ip_dlen; }
    inline uint16_t get_data_len() {return bf.ip_dlen; }
	inline BasicField& get_bf() { return bf; }
	inline void set_flow_state(void* ptr) { flow_state_ = ptr; }
	inline void* get_flow_state() { return flow_state_; }
};
#endif
