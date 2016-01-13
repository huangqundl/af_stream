#ifndef __FiledGrouping_hpp__
#define __FiledGrouping_hpp__

#include <stdint.h>

#include "LoadBalancer/LoadBalancer.hpp"
#include "../Packet.hpp"
#include "HashFunc.hpp"

class PacketFieldBalancer : public LoadBalancer <Packet::TotalField> {
public:
	inline int balance(Packet::TotalField* tf, int* flags=NULL) {
        uint64_t key = tf->bf.src_ip + tf->bf.dst_ip;
        return HashFunc((const char*)&key, 8) % attachedQueueCount;
    }
};

#endif
