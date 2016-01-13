#ifndef RAW_PACKET_BALANCER_HPP
#define RAW_PACKET_BALANCER_HPP

#include <stdint.h>

#include "LoadBalancer/LoadBalancer.hpp"
#include "../RawPacket.hpp"
#include "HashFunc.hpp"

class RawPacketBalancer : public LoadBalancer <RawPacket> {
public:
	inline int balance(RawPacket *p, int* flags=NULL) {
        return HashFunc((const char *) &p->key, 4) % attachedQueueCount;
    }
};

#endif // RAW_PACKET_BALANCER_HPP
