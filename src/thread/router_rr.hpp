#ifndef __AFS_ROUTER_RR_HPP_INCLUDED__
#define __AFS_ROUTER_RR_HPP_INCLUDED__

#include <vector>

#include "util.hpp"

namespace afs {

class RouterRR : public RouterBase {

private:
    int cur_, max_;

public:

    int GetDestination(void* data, uint32_t len) {
        cur_ = (cur_ + 1) % max_;
        return cur_;
    }

    void Init();
    void Clean();

    RouterRR(int max) : cur_(-1), max_(max) {}
};

}

#endif
