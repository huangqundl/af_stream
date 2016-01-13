#ifndef __FiledGrouping_hpp__
#define __FiledGrouping_hpp__

#include <stdint.h>

#include "../../Dispatcher/Router/Router.hpp"
#include "Packet.hpp"
#include "HashFunc.hpp"

class PacketFieldGrouping : public LoadBalancer <Packet::TotalField> {
    int cur;

public:
    PacketFieldGrouping(): cur(0) {}

	int balance(Packet::TotalField* tf, int* flags=NULL) {
        /*
        cur = cur+1 < attachedQueueCount ? cur+1 : 0; 
        return cur;
        */
        uint64_t key = tf->bf.src_ip + tf->bf.dst_ip;
        return HashFunc((const char*)&key, 8) % attachedQueueCount;
    }
};

#endif
