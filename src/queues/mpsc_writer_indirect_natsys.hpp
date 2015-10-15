#ifndef __MPSC_WRITER_HPP_INCLUDED__
#define __MPSC_WRITER_HPP_INCLUDED__

#include "mpsc_object_indirect_natsys.hpp"
#include "util.hpp"

namespace afs {

template<class T>
class MPSCWriter : public MPSCObject<T> {

public:
    T* GetSlot() {
        T* ret = NULL;
        while (!MPSCObject<T>::own->pop(ret)) {}
        return ret;
    }

    void Write(int index, T* addr) {
        MPSCObject<T>::peers[index]->push(addr);
    }

    MPSCWriter<T>(int num_readers) :
        MPSCObject<T>(num_readers) {
        slots = (T*) calloc(QUEUE_SIZE, sizeof(T));
        afs_assert(slots, "Slots allocate failure\n");
    }

    void SetWriterId(int w) {}

    void Init() {
        for (auto d=0; d<QUEUE_SIZE; d++) {
            MPSCObject<T>::own->push(slots + d);
        }
    }

    void Clean() {
        for (auto d=0; d<QUEUE_SIZE; d++) {
            T* ret = NULL;
            while (!MPSCObject<T>::own->pop(ret)) {};
        }
    }

private:
    T* slots;
};

}

#endif
