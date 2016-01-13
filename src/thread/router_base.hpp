#ifndef __AFS_ROUTER_BASE_HPP_INCLUDED__
#define __AFS_ROUTER_BASE_HPP_INCLUDED__

#include <vector>

#include "util.hpp"

namespace afs {

class RouterBase {

public:
    virtual int GetDestination(void* data, uint32_t len) = 0;
    void Init();
    void Clean();
};

}

#endif
