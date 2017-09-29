#ifndef __AFS_INTHREAD_BASE_HPP_INCLUDED__
#define __AFS_INTHREAD_BASE_HPP_INCLUDED__

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>

#include <string>

#include "thread.hpp"
//#include "compute_thread.hpp"

#include "../wrap_item.hpp"
#include "../queues/zerocopy_ringbuffer.hpp"
#include "../queues/mpsc_channel.hpp"

using namespace std;

namespace afs {

/**
 * Receive events outside the worker, executed by a single process
 * @tparam InT 
 */
template <class InT, class ROutT>
class UpThread : public ThreadBase {

typedef WrapItem<InT> WInT;
typedef WrapItem<ROutT> WROutT;

public:

    UpThread() : ThreadBase(t_ingress), is_recovery(false) {}

    virtual int GetNumUpstream() {
        return 0;
    }

    virtual bool IsReverse() {
        return false;
    }

    virtual void AddOutQueue(ZeroRingBuffer<WInT>* q) = 0;

    virtual MPSCReader<WROutT>* GetReverseOutBufferReader(int i) {
        LOG_ERR("Only UpThreadNet, UpThreadNet, UpThreadUpbackup can reach this\n");
    }

    void EnableRecovery() {
        is_recovery = true;
    }

private:
    virtual void DoRecovery() {
        LOG_MSG("Do nothing for recovery\n");
    }

protected:
    bool is_recovery;
};

} // namespace

#endif // INCLUDE
