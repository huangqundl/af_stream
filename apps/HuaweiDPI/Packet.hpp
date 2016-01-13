/*
 * Packet.hpp
 * - packet header parsed from raw Ethernet packets
 */

#ifndef __Packet_h__
#define __Packet_h__

#include <stdint.h>
#include "PacketDecoder.hpp"

#define PAYLOAD_LEN 1600

#ifdef GPRS_PARSER
#define IMSI_LEN 20
#endif

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
        uint64_t seq;
		// 8 bytes
		uint32_t src_ip;			// source IP address
		uint32_t dst_ip;			// dest IP address
		// 8 bytes
		uint16_t src_port;			// source port
		uint16_t dst_port;			// dest port
		uint16_t ip_dlen;			// inner IP datagram length (header + data)
		uint16_t data_len;			// data length
        // 8 bytes
        PACKET_STATUS status;
        int appId;
        // 8 bytes
        uint16_t raw_len;           // raw data length
		uint8_t proto;				// protocol
        char pad[5];
#ifdef GPRS_PARSER
        uint32_t bssgp_tlli_current;
        uint32_t bssgp_tlli_old;
        uint16_t bssgp_type;
        char bssgp_IMSI[IMSI_LEN];
        char pad2[2];
#endif

        uint32_t size() { return sizeof(BasicField); }
	};

    class TotalField {
    public:
        BasicField bf;
        unsigned char payload[PAYLOAD_LEN];
        unsigned char raw_data[PAYLOAD_LEN];
    
        TotalField& operator = (const TotalField& tf) {
            this->bf = tf.bf;
            memcpy(this->payload, tf.payload, tf.bf.data_len);
            memcpy(this->raw_data, tf.raw_data, tf.bf.raw_len);
            return *this;
        }

        uint32_t size() { return sizeof(BasicField) + (bf.raw_len == 0 ? bf.data_len : PAYLOAD_LEN + bf.raw_len); }
        uint32_t load() { return sizeof(BasicField) + bf.data_len; }
    };

private:
    static uint64_t seq_count;
    PacketDecoder* pd;
    TotalField tf_;
    void* flow_state_;				// pointer to the flow state
    bool copy_raw_;
	

public:
    Packet();
    ~Packet();
	PACKET_STATUS decode(uint8_t* pkt, uint32_t len, double ts); 

    void reset()
    {
        memset(&tf_, 0, sizeof(TotalField));
    }

	inline double get_pkt_ts() { return tf_.bf.pkt_ts; }
	inline uint32_t get_src_ip() { return tf_.bf.src_ip; }
	inline uint32_t get_dst_ip() { return tf_.bf.dst_ip; }
	inline uint16_t get_src_port() { return tf_.bf.src_port; }
	inline uint16_t get_dst_port() { return tf_.bf.dst_port; }
	inline uint8_t get_proto() { return tf_.bf.proto; }
	inline uint16_t get_ip_dlen() { return tf_.bf.ip_dlen; }
	inline uint16_t get_data_len() { return tf_.bf.data_len; }
	inline uint16_t get_raw_len() { return tf_.bf.raw_len; }
	inline BasicField& get_bf() { return tf_.bf; }
	inline TotalField& get_tf() { return tf_; }
    inline unsigned char* get_payload() {
        return (unsigned char*)(&tf_.payload);
    }
    inline unsigned char* get_raw_data() {
        return (unsigned char*)(&tf_.raw_data);
    }
	inline void set_flow_state(void* ptr) { flow_state_ = ptr; }
	inline void* get_flow_state() { return flow_state_; }
};
#endif
