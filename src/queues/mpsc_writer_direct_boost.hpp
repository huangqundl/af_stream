#ifndef __MPSC_WRITER_HPP_INCLUDED__
#define __MPSC_WRITER_HPP_INCLUDED__

#include "mpsc_object_direct_boost.hpp"
#include "util.hpp"

namespace afs {

template<class T>
class MPSCWriter : public MPSCObject<T> {

public:
    T* GetSlot() {
        return &slot;
    }

    void Write(int reader, T* addr) {
        //LOG_MSG("Write addr %08x\n", tmp);
        MPSCObject<T>::peers[reader]->push(*addr);
    }

    MPSCWriter<T>(int num_readers) :
        MPSCObject<T>(num_readers),
        num_reader(num_readers)
    {
    }

    void SetWriterId(int w) {
    }

    void Init() {
    }

    void Flush() {
    }

    void Clean() {
    }

private:
    T slot;
    int num_reader;
};

}

#endif
