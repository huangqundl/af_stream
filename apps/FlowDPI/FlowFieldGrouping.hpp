#ifndef __FlowFieldGrouping_hpp__
#define __FlowFieldGrouping_hpp__

#include <stdint.h>

#include "../../Dispatcher/Router/Router.hpp"
#include "HashFunc.hpp"
#include "Flow.hpp"

class FlowFieldGrouping : public LoadBalancer <FLOW> {

public:
    FlowFieldGrouping() {
        LOG_MSG("LoadBalancer in ZMQMainComputeThread: FlowFieldGrouping\n");
    }

	int balance(FLOW* f, int* old_index=NULL) {
        uint64_t key = f->field.orig_ip + f->field.resp_ip;
        return HashFunc((const char*)&key, 8) % attachedQueueCount;
    }
};

#endif
