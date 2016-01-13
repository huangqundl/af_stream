#ifndef __AFS_OUTTHREAD_MPSC_HPP_INCLUDED__
#define __AFS_OUTTHREAD_MPSC_HPP_INCLUDED__

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>

#include <string>

#include "down_thread.hpp"
#include "wrap_item.hpp"

using namespace std;

namespace afs {

/**
 * Receive events outside the worker, executed by a single process
 * @tparam T output class of dispatcher, used for input of compute_threads in the same worker
 */
//template <class T>
class DownThreadMPSC : public DownThread {

public:
    void AddReaderSource(int index, void* peer) {
        for (int i=0; i<num_out_; i++) {
            readers_[i]->ConnectPeer(index, (MPSCWriter*)peer);
        }
    }

    void AddExternDest(int index, char* source) {}

    DownThreadMPSC(int num_in, int num_out) :
        DownThread(), num_in_(num_in), num_out_(num_out) {
        readers_ = (MPSCReader**)calloc(num_out, sizeof(MPSCReader*));
        for (int i=0; i<num_out_; i++) {
            readers_[i] = new afs::MPSCReader(num_in_);
        }
    }

    ~DownThreadMPSC() {
        for (int i=0; i<num_out_; i++) {
            delete readers_[i];
        }
        free(readers_);
    }

    int GetReaderNum() {
        return num_out_;
    }

    void* GetReader(int index) {
        return readers_[index];
    }

private:
    MPSCReader** readers_;
    int num_in_, num_out_;

    void ThreadInitHandler() {
    
    }

    void ThreadFinishHandler() {
        for (int i=0; i<num_out_; i++) {
            readers_[i]->Clean();
        }
    }

    void ThreadMainHandler() {
        int n = 0;
        WrapItem<NullClass>* msg = NULL;
        bool stop = false;

        while (!stop) {
            for (int i=0; i<num_out_; i++) {
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

        LOG_MSG("DownThread recv %d events\n", n);
    }
};

} // namespace

#endif // INCLUDE
