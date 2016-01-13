#ifndef __MPMC_QUEUE_MESH_HPP_INCLUDED__
#define __MPMC_QUEUE_MESH_HPP_INCLUDED__

#include "zerocopy_ringbuffer.hpp"

template<class T>
class LockFreeQueue {
public:
    T* GetSlot(int writer_index) {
		return q_[writer_index]->GetSlot();
    }

    void CompleteWrite(int writer_index) {
		return q_[writer_index]->CompleteWrite();
    }

	void Insert(int writer_index, T* x) {
		q_[writer_index]->Insert(x);
	}

    T* Extract() {
        T* slot = NULL;
        //for (int i=0; i<num_queue; i++) {
            if (cur >= num_queue) {
                cur = 0;
            }
            slot = q_[cur]->Extract();
            if (slot) {
                //q_[cur]->Ack();
                last_read = cur;
                cur++;
            }
            //cur++;
        //}
        return slot;
	}

    void Ack() {
        q_[last_read]->Ack();
    }

    void Flush(int writer_id) {
        q_[writer_id]->Flush();
    }

    LockFreeQueue(int num_writer) : num_queue(num_writer), cur(0), last_read(0) {
        q_ = (ZeroRingBuffer<T>**)calloc(num_writer, sizeof(ZeroRingBuffer<T>*));
        for (int i=0; i<num_writer; i++) {
            q_[i] = new ZeroRingBuffer<T>();
        }
    }

    ~LockFreeQueue() {
        for (int i=0; i<num_queue; i++) {
            delete q_[i];
        }
        free(q_);
    }

private:
    ZeroRingBuffer<T>** q_;
    int num_queue;
    int cur;
    int last_read;
};


#endif
