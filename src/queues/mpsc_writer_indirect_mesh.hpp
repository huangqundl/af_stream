#ifndef __MPSC_WRITER_HPP_INCLUDED__
#define __MPSC_WRITER_HPP_INCLUDED__

#include "mpsc_object_indirect_mesh.hpp"
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

    void Write(int reader, T* addr) {
        MPSCObject<T>::peers[reader]->push(writer_id, addr);
    }

    MPSCWriter<T>() :
        MPSCObject<T>(),
        //num_reader(num_readers),
        writer_id(-1)
    {
        slots = (T*) calloc(RB_SIZE, sizeof(T));
        afs_assert(slots, "Slots allocate failure\n");
    }

    void SetWriterId(int w) {
        writer_id = w;
    }

    void Init() {
        afs_assert(writer_id != -1, "writer id is not set\n");
        for (auto d=0; d<RB_SIZE-1; d++) {
            MPSCObject<T>::own->push(0, slots + d);
        }
        clean_cnt++;
    }

    void Flush() {
        for (uint64_t i=0; i<MPSCObject<T>::peers.size(); i++) {
            MPSCObject<T>::peers[i]->flush();
        }
    }

    void AttemptClean() {
        T* ret = NULL;
        while (MPSCObject<T>::own->pop(ret)) {
            clean_cnt++;
        }
    }

    void Clean() {
        for (auto d=0; d<RB_SIZE-1-clean_cnt; d++) {
            T* ret = NULL;
            while (!MPSCObject<T>::own->pop(ret)) {};
        }
    }

private:
    T* slots;
    //int num_reader;
    int writer_id;
    int clean_cnt;
};

}

#endif
