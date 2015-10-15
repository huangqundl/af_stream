#ifndef __MPSC_WRITER_HPP_INCLUDED__
#define __MPSC_WRITER_HPP_INCLUDED__

#include "mpsc_object_direct_mesh.hpp"
#include "util.hpp"

namespace afs {

template<class T>
class MPSCWriter : public MPSCObject<T> {

public:
    /*
    T* GetSlot(int reader) {
        //return MPSCObject<T>::peers[reader]->GetSlot(writer_id);
        return &slot;
    }
    */

    void Write(int reader, T* addr) {
        //LOG_MSG("Write addr %08x\n", tmp);
        //MPSCObject<T>::peers[reader]->CompleteWrite(writer_id);
        MPSCObject<T>::peers[reader]->Insert(writer_id, addr);
    }

    MPSCWriter<T>(int num_readers) :
        MPSCObject<T>(num_readers),
        num_reader(num_readers),
        writer_id(-1)
    {
    }

    void SetWriterId(int w) {
        writer_id = w;
    }

    void Init() {
        afs_assert(writer_id != -1, "writer id is not set\n");
    }

    void Flush() {
        //MPSCObject<T>::own->Flush(writer_id);
        for (int i=0; i<num_reader; i++) {
            MPSCObject<T>::peers[i]->Flush(writer_id);
        }
    }

    void Clean() {
    }

private:
    T slot;
    int num_reader;
    int writer_id;
};

}

#endif
