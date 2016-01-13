#ifndef __AFS_IN_CALLBACK_BASE_HPP_INCLUDED__
#define __AFS_IN_CALLBACK_BASE_HPP_INCLUDED__

#include <vector>

//#include "queues/zerocopy_ringbuffer_new.hpp"
#include "util.hpp"
#include "io_callback.hpp"

namespace afs {

class InCallbackBase : public IOCallback {

public:
    //virtual int PushRecord(void* data, uint32_t len, afs_zmq::i_engine* engine=NULL) = 0;
    virtual int PushRecord(void* data, uint32_t len) = 0;
    virtual void Init() {}
    virtual void Clean() {}

    virtual void AddOutQueue(void* q) = 0;

private:
    virtual int GetDestination(void* data, uint32_t len) = 0;
};


}

#endif
