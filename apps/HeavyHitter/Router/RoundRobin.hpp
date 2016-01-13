#ifndef __RoundRobinGrouping_hpp__
#define __RoundRobinGrouping_hpp__

#include <stdint.h>

#include "Dispatcher/LoadBalancer/LoadBalancer.hpp"
#include "HashFunc.hpp"
#include "../Flow.hpp"

/// Specific load balancer to allocate items in round robin
class RoundRobinGrouping : public LoadBalancer <FLOW> {
    /// current target
    int current;

public:

    RoundRobinGrouping(): current(-1) {
        LOG_MSG("LoadBalancer in ZMQMainAnalyzer: RoundRobinGrouping\n");
    }

	int balance(FLOW* f, int* old_index=NULL) {
		current = current+1 < attachedQueueCount ? current+1 : 0;
        return current;
    }
};

#endif
