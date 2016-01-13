#ifndef __ROLL_OUTTHREAD_MPSC_HPP_INCLUDED__
#define __ROLL_OUTTHREAD_MPSC_HPP_INCLUDED__

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>

#include <string>

#include "down_thread.hpp"
#include "raw_item.hpp"

using namespace std;

namespace afs {

/**
 * Receive events outside the worker, executed by a single process
 * @tparam T output class of dispatcher, used for input of compute_threads in the same worker
 */
//template <class T>
class OutThreadMPSC : public OutThread<RawItem, RawItem> {

public:
    void AddDest(int index, char* source) {}

    MPSCReader<WRawItem>* GetOutBufferReader(int i) {
        return readers_[i];
    }

    OutThreadMPSC(int num_in) :
        OutThread(), num_in_(num_in) {
        readers_ = (MPSCReader<WRawItem>**)calloc(num_in_, sizeof(MPSCReader<WRawItem>*));
        for (int i=0; i<num_in_; i++) {
            readers_[i] = new afs::MPSCReader<WRawItem>(num_in_);
        }
    }

    ~OutThreadMPSC() {
        for (int i=0; i<num_in_; i++) {
            delete readers_[i];
        }
        free(readers_);
    }

    int GetDownstream() {
        return num_in_;
    }

private:
    MPSCReader<WRawItem>** readers_;
    int num_in_;

    void ThreadInitHandler() {
    
    }

    void ThreadFinishHandler() {
        for (int i=0; i<num_in_; i++) {
            readers_[i]->Clean();
        }
    }

    void ThreadMainHandler() {
        int n = 0;
        WrapItem<NullClass>* msg = NULL;
        bool stop = false;

        while (!stop) {
            for (int i=0; i<num_in_; i++) {
                msg = (WrapItem<NullClass>*)readers_[i]->Read();
                if (msg) {
                    if (msg->get_type() == ITEM_FINISH) {
                        stop = true;
                    }
                    else {
                        n++;
                    }
                    readers_[i]->Ack(0, msg);
                }
            }
        }

        LOG_MSG("OutThread recv %d events\n", n);
    }
};

} // namespace

#endif // INCLUDE
