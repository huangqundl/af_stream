#ifndef __MPSC_WRITER_HPP_INCLUDED__
#define __MPSC_WRITER_HPP_INCLUDED__

#include "mpsc_object_indirect_mulwrb.hpp"
#include "util.hpp"

namespace afs {

template<class T>
class MPSCWriter : public MPSCObject<T> {

public:
    T* GetSlot() {
        T** ret = NULL;
        while (!ret) {
            ret = MPSCObject<T>::own->Extract();
        }
        MPSCObject<T>::own->Ack();
        return *ret;
    }

    void Write(int reader, T* addr) {
        T* tmp = addr;
        //LOG_MSG("Write addr %08x\n", tmp);
        MPSCObject<T>::peers[reader]->Insert(writer_id, &tmp);
    }

    MPSCWriter<T>(int num_readers) :
        MPSCObject<T>(num_readers),
        num_reader(num_readers),
        writer_id(-1)
    {
        slots = (T*) calloc(RB_SIZE_MULTI, sizeof(T));
        afs_assert(slots, "Slots allocate failure\n");
    }

    void SetWriterId(int w) {
        writer_id = w;
    }

    void Init() {
        afs_assert(writer_id != -1, "writer id is not set\n");
        T* tmp = NULL;
        for (auto d=0; d<RB_SIZE_MULTI-1; d++) {
            tmp = slots + d;
            /*
            if (d < 10) {
                LOG_MSG("Empty addr %08x\n", tmp);
            }
            */
            MPSCObject<T>::own->Insert(writer_id, &tmp);
        }
    }

    void Flush() {
        MPSCObject<T>::own->Flush(writer_id);
        for (int i=0; i<num_reader; i++) {
            MPSCObject<T>::peers[i]->Flush(i);
        }
    }

    void Clean() {
        for (auto d=0; d<RB_SIZE_MULTI-1; d++) {
            T** ret = NULL;
            while (!ret) {
                ret = MPSCObject<T>::own->Extract();
            };
            MPSCObject<T>::own->Ack();
        }
    }

private:
    T* slots;
    int num_reader;
    int writer_id;
};

}

#endif
