#ifndef __MPSC_READER_HPP_INCLUDED__
#define __MPSC_READER_HPP_INCLUDED__

#include "mpsc_object_indirect_natsys.hpp"

namespace afs {

template<class T>
class MPSCReader : public MPSCObject<T> {

public:

    // called by out_callback, so use void* instead of T*
    void* Read() {
        T* ret = NULL;
        if (MPSCObject<T>::own->pop(ret)) {
            return ret;
        }
        return NULL;
    }

    // called by out_callback, so use void* instead of T*
    void Ack(int source, void* addr) {
        MPSCObject<T>::peers[source]->push((T*)addr);
    }

    MPSCReader<T>(int num_writers) : MPSCObject<T>(num_writers) {
    }

    void SetReaderId(int r) {}

    void Init() {
    }

    void Clean() {
    }

private:
    int ack_cnt;
};

}

#endif
