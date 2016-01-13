#ifndef __FiledGrouping_hpp__
#define __FiledGrouping_hpp__

#include <stdint.h>

#include "Dispatcher/Router/Router.hpp"
#include "../Packet.hpp"
#include "HashFunc.hpp"

/**
 * specific LoadBalancer by simply hashing keys
 */
class PacketFieldGrouping : public Router <Packet::BasicField> {

public:
    /**
     * Constructor
     */
    PacketFieldGrouping() {}

	int balance(Packet::BasicField* bf, int* flags=NULL) {
        uint64_t key = bf->src_ip * 2;
        return HashFunc((const char*)&key, 8) % attachedQueueCount;
    }
};

#endif
