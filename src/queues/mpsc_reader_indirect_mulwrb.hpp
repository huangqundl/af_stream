#ifndef __MPSC_READER_HPP_INCLUDED__
#define __MPSC_READER_HPP_INCLUDED__

#include "mpsc_object_indirect_mulwrb.hpp"

namespace afs {

template<class T>
class MPSCReader : public MPSCObject<T> {

public:

    // called by out_callback, so use void* instead of T*
    void* Read() {
        T** ret = MPSCObject<T>::own->Extract();
        if (ret == NULL) {
            return NULL;
        }
        MPSCObject<T>::own->Ack();
        return *ret;
    }

    // called by out_callback, so use void* instead of T*
    void Ack(int writer, void* addr) {
        T* tmp = (T*)addr;
        MPSCObject<T>::peers[writer]->Insert(reader_id, &tmp);
    }

    MPSCReader<T>(int num_writers) :
        MPSCObject<T>(num_writers),
        num_writer(num_writers) {
    }

    void SetReaderId(int r) {
        reader_id = r; 
    }

    void Init() {
        afs_assert(reader_id != -1, "reader id is not set\n");
    }

    void Flush() {
        MPSCObject<T>::own->Flush(reader_id);
        for (int i=0; i<num_writer; i++) {
            MPSCObject<T>::peers[i]->Flush(i);
        }
    }

    void Clean() {
    }

private:
    int ack_cnt;
    int num_writer;
    int reader_id;
};

}

#endif
