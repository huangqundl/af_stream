/*
 * Packet.hpp
 * - packet header parsed from raw Ethernet packets
 */

#ifndef __Packet_h__
#define __Packet_h__

#include <stdint.h>
#include "PacketDecoder.hpp"

#define PAYLOAD_LEN 1500

/**
 * Structure of packet
 */
class Packet {
public:
    // This class will be transferred on the wire between 32/64 bit machines,
    // so we add some dummy char array in the field.
    // This class include BasicField and payload of a packet.
    /**
     * Metadata of a packet
     */
	class BasicField {
    public:
		// 8 bytes
        /**
         * Source IP
         */
		uint32_t src_ip;			
        /**
         * Destination IP
         */
		uint32_t dst_ip;
		// 8 bytes
        /**
         * Source port
         */
		uint16_t src_port;
        /**
         * Destination port
         */
		uint16_t dst_port;
		uint8_t proto;				// protocol
        /// padding
        uint8_t dummy[3];
        // 8 bytes
        /**
         * aggregated number of packets
         */
        uint64_t accumulated_pkt;
        /**
         * aggregated number of bytes
         */
        uint64_t accumulated_byte;
        /**
         * sequence number
         */
        uint64_t seq;
        // 8 bytes
        /**
         * which worker the packet is assigned to
         */
        int pre_assign;
        /// padding
        uint8_t dummy1[4];

        /// get number of bytes of metadata
        uint32_t size() { return sizeof(BasicField); }

        /// get workload
        uint32_t load() { return 1; }
	};

private:
    /**
     * sequence number
     */
    static uint64_t seq_count;

    /**
     * Packet decoder
     */
    PacketDecoder* pd;

    /**
     * Metadata of packet
     */
    BasicField bf; 

public:
    /**
     * Constructor
     */
    Packet();

    /**
     * Deconstructor
     */
    ~Packet();

    /**
     * Clear the Structure
     */
	void reset();

    /**
     * Decode a binary array
     * @param pkt the binary
     * @param len length of the binary
     * @param ts timestamp
     * @return the status of packet
     */
    PACKET_STATUS decode(uint8_t* pkt, uint32_t len, double ts);

    /**
     * get source IP
     */
	inline uint32_t get_src_ip() { return bf.src_ip; }

    /**
     * get destination IP
     */
	inline uint32_t get_dst_ip() { return bf.dst_ip; }

    /**
     * get number of bytes
     */
	inline uint16_t get_ip_dlen() { return bf.accumulated_byte; }

    /**
     * get metadata
     */
	inline BasicField& get_bf() { return bf; }
};
#endif
