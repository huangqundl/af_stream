#ifndef __MPMC_QUEUE_MESH_HPP_INCLUDED__
#define __MPMC_QUEUE_MESH_HPP_INCLUDED__

#include <vector>
#include "zerocopy_ringbuffer.hpp"

template<class T>
class LockFreeQueue {
public:

	void push(int writer_index, T x) {
		q_[writer_index]->Insert(&x);
	}

    bool pop(T& ret) {
        T* slot = NULL;
        for (int i=0; i<num_queue; i++) {
            if (cur >= num_queue) {
                cur = 0;
            }
            slot = q_[cur]->Extract();
            if (slot) {
                ret = *slot;
                q_[cur]->Ack();
                cur++;
                return true;
            }
            cur++;
        }
		return false;
	}

    void flush() {
        for (int i=0; i<num_queue; i++) {
            q_[i]->Flush();
        }
    }

    void AddQueue() {
        num_queue++;
        q_.push_back(new ZeroRingBuffer<T>());
    }

    LockFreeQueue() : num_queue(0), cur(0) {
        //q_ = (ZeroRingBuffer<T>**)calloc(num_writer, sizeof(ZeroRingBuffer<T>*));
        /*
        for (int i=0; i<num_writer; i++) {
            //q_[i] = new ZeroRingBuffer<T>();
            q_.push_back(new ZeroRingBuffer<T>());
        }
        */
    }

    ~LockFreeQueue() {
        for (int i=0; i<num_queue; i++) {
            delete q_[i];
        }
        //free(q_);
    }

private:
    //ZeroRingBuffer<T>** q_;
    std::vector<ZeroRingBuffer<T>*> q_;
    int num_queue;
    int cur;
};


#endif
