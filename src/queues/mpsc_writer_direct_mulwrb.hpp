#ifndef __MPSC_WRITER_HPP_INCLUDED__
#define __MPSC_WRITER_HPP_INCLUDED__

#include "mpsc_object_direct_mulwrb.hpp"
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
        MPSCObject<T>::peers[reader]->Insert(writer_id, addr);
    }

    MPSCWriter<T>(int num_readers) :
        MPSCObject<T>(num_readers),
        num_reader(num_readers),
        writer_id(-1)
    {
        /*
        slots = (T*) calloc(RB_SIZE_MULTI, sizeof(T));
        afs_assert(slots, "Slots allocate failure\n");
        */
    }

    void SetWriterId(int w) {
        writer_id = w;
    }

    void Init() {
        afs_assert(writer_id != -1, "writer id is not set\n");
        /*
        T* tmp = NULL;
        for (auto d=0; d<RB_SIZE_MULTI-1; d++) {
            tmp = slots + d;
            MPSCObject<T>::own->Insert(&tmp);
        }
        */
    }

    void Flush() {
        //MPSCObject<T>::own->Flush(writer_id);
        for (int i=0; i<num_reader; i++) {
            MPSCObject<T>::peers[i]->Flush(writer_id);
        }
    }

    void Clean() {
        /*
        for (auto d=0; d<RB_SIZE_MULTI-1; d++) {
            T** ret = NULL;
            while (!ret) {
                ret = MPSCObject<T>::own->Extract();
            };
            MPSCObject<T>::own->Ack();
        }
        */
    }

private:
    T slot;
    int num_reader;
    int writer_id;
};

}

#endif
