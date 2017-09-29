#ifndef __AFS_PULLER_BASE_HPP_INCLUDED__
#define __AFS_PULLER_BASE_HPP_INCLUDED__

#include <vector>

#include "../util.hpp"
#include "io_callback.hpp"

namespace afs {

class OutCallbackBase //: public IOCallback
{

public:
    virtual int PullRecord(void** data, uint32_t* len) = 0;
    virtual void Ack(void* ACK) = 0;

    virtual void Init() {}
    virtual void Clean() {}
    virtual void Flush() {}

    //virtual void AddSource(int index, void* peer) = 0; 
    virtual void* GetReader() = 0;

    virtual ~OutCallbackBase() {}

    void set_worker_id(int id) {
        worker_id_ = id;
    }

protected:
    OutCallbackBase() : seq_(0), worker_id_(-1) {}

private:
    int seq_;
    int worker_id_;
};


}

#endif
