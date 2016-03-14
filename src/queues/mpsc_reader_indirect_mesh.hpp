#ifndef __MPSC_READER_HPP_INCLUDED__
#define __MPSC_READER_HPP_INCLUDED__

#include "mpsc_object_indirect_mesh.hpp"

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
    void Ack(int writer, void* addr) {
        MPSCObject<T>::peers[writer]->push(reader_id, (T*)addr);
    }

    MPSCReader<T>() :
        MPSCObject<T>()
        //num_writer(num_writers) 
        {}

    void SetReaderId(int r) {
        reader_id = r; 
    }

    void Init() {
        afs_assert(reader_id != -1, "reader id is not set\n");
    }

    void Flush() {
        //for (auto d=0; d<RB_BATCH; d++) {
            for (uint64_t i=0; i<MPSCObject<T>::peers.size(); i++) {
                //MPSCObject<T>::peers[i]->push(reader_id, NULL);
                MPSCObject<T>::peers[i]->flush();
            }
        //}
    }

    void Clean() {}

private:
    int ack_cnt;
    //int num_writer;
    int reader_id;
};

}

#endif
