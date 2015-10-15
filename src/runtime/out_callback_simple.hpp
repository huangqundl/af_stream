#ifndef __ROLL_PULLER_SIMPLE_HPP_INCLUDED__
#define __ROLL_PULLER_SIMPLE_HPP_INCLUDED__

#include <vector>

#include "util.hpp"
#include "out_callback_base.hpp"
#include "queues/mpsc_channel.hpp"

namespace afs {

template<class T>
class OutCallbackSimple : public OutCallbackBase {

typedef WrapItem<T> WrapT;

public:
    int PullRecord(void** data, uint32_t* len) {
        *data = reader_->Read();
        *len = sizeof(WrapT);

        if (*data) {
            event_++;
            //LOG_MSG("outcallback event %lu\n", event_);
            if (afs_unlikely(((WrapT*)(*data))->get_type()==ITEM_FINISH)) {
                num_stop_++;
                if (num_stop_ == num_in_) {
                    WrapT* item = (WrapT*)(*data);
                    reader_->Ack(item->get_thr_source(), *data);
                    return 1;
                }
                else {
                    WrapT* item = (WrapT*)(*data);
                    reader_->Ack(item->get_thr_source(), *data);
                    *data = NULL;
                    *len = 0;
                }
            }
        }

        return 0;
    }

    OutCallbackSimple(int num_in) : OutCallbackBase(), num_in_(num_in), num_stop_(0), event_(0) {
        reader_ = new MPSCReader<WrapT>(num_in_);
    }

    ~OutCallbackSimple() {
        delete reader_;
    }

    void Ack(void* data) {
        int source = ((WrapT*)data)->get_thr_source();
        reader_->Ack(source, data);
    }

    void Init() {
        reader_->Init();
    }

    void Clean() {
        reader_->Clean();
    }

    void Flush() {
        reader_->Flush();
    }

    void* GetReader() {
        return reader_;
    }

    /*
    void AddSource(int index, void* peer) {
        reader_->ConnectPeer(index, (MPSCWriter<WrapT>*)peer);
    }
    */

private:
    MPSCReader<WrapT>* reader_;
    int num_in_;
    int num_stop_;
    size_t event_;
};


}

#endif
