#ifndef __FiledGrouping_hpp__
#define __FiledGrouping_hpp__

#include <stdint.h>

#include "../../../Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "../Packet.hpp"
#include "HashFunc.hpp"

class PacketFieldGrouping : public LoadBalancer <Packet::BasicField> {
    int cur;

public:
    PacketFieldGrouping(): cur(0) {}

	int balance(Packet::BasicField* bf, int* flags=NULL) {
        /*
        cur = cur+1 < attachedQueueCount ? cur+1 : 0; 
        return cur;
        */
        uint64_t key = bf->src_ip + bf->dst_ip;
        return HashFunc((const char*)&key, 8) % attachedQueueCount;
    }
};

#endif
