#ifndef __FlowFieldGrouping_hpp__
#define __FlowFieldGrouping_hpp__

#include <stdint.h>

#include "Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "HashFunc.hpp"
#include "../Flow.hpp"

class FlowFieldGrouping : public LoadBalancer <FLOW> {

public:
    FlowFieldGrouping() {
        LOG_MSG("LoadBalancer in ZMQMainAnalyzer: FlowFieldGrouping\n");
    }

	int balance(FLOW* f, int* old_index=NULL) {
        uint64_t key = f->field.orig_ip + f->field.resp_ip;
        return HashFunc((const char*)&key, 8) % attachedQueueCount;
    }
};

#endif
