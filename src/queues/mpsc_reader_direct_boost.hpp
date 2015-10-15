#ifndef __MPSC_READER_HPP_INCLUDED__
#define __MPSC_READER_HPP_INCLUDED__

#include "mpsc_object_direct_boost.hpp"

namespace afs {

template<class T>
class MPSCReader : public MPSCObject<T> {

public:
    // called by out_callback, so use void* instead of T*
    void* Read() {
        if (MPSCObject<T>::own->pop(data)) {
            return &data;
        }
        return NULL;
    }

    // called by out_callback, so use void* instead of T*
    void Ack(int writer, void* addr) {
    }

    MPSCReader<T>(int num_writers) :
        MPSCObject<T>(num_writers),
        num_writer(num_writers) {
    }

    void SetReaderId(int r) {
    }

    void Init() {
    }

    void Flush() {
    }

    void Clean() {
    }

private:
    int ack_cnt;
    int num_writer;

    T data;
};

}

#endif
