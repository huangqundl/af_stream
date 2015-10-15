#ifndef __ROLL_OUTTHREAD_BASE_HPP_INCLUDED__
#define __ROLL_OUTTHREAD_BASE_HPP_INCLUDED__

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>

#include <string>

#include "thread.hpp"

#include "queues/zerocopy_ringbuffer.hpp"
#include "queues/mpsc_channel.hpp"

using namespace std;

namespace afs {

/// Thread to emit messages to outside destination (file system or next-hop workers)
/// maintains mulitple reader
/// each reader
///      (1) corresponds one external destination
///      (2) receives messages from mulitple writers (resides in analyzers)

template <class OutT, class RInT>
class OutThread : public ThreadBase {

typedef WrapItem<OutT> WOutT;
typedef WrapItem<RInT> WRInT;

public:

    OutThread() : ThreadBase(t_egress) {}

    virtual void AddDest(int dest_index, char* source) = 0;

    // get the buffer for i-th destination
    virtual MPSCReader<WOutT>* GetOutBufferReader(int i) = 0;

    // number of total destination
    virtual int GetDownstream() = 0;

    virtual bool IsReverse() {
        return false;
    }

    virtual void AddReverseInBuffer(ZeroRingBuffer<WRInT>* q) {
        LOG_ERR("Only OutThreadNet can reach this\n");
    }

    virtual ~OutThread() {}
};

} // namespace

#endif // INCLUDE
