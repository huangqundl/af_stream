#ifndef PACKET_FIELD_ROUTER_HPP
#define PACKET_FIELD_ROUTER_HPP

#include <stdint.h>

#include "Router/Router.hpp"
#include "../Packet.hpp"
#include "HashFunc.hpp"

class PacketFieldBalancer : public Router<Packet::TotalField> {
public:
	inline int balance(Packet::TotalField* tf, int* flags=NULL) {
        uint64_t key = tf->bf.src_ip + tf->bf.dst_ip;
        return HashFunc((const char*)&key, 8) % attachedQueueCount;
    }
};

#endif
