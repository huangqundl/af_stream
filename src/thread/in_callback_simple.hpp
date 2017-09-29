#ifndef __AFS_IN_CALLBACK_SIMPLE_HPP_INCLUDED__
#define __AFS_IN_CALLBACK_SIMPLE_HPP_INCLUDED__

#include <vector>

#include "in_callback_base.hpp"
#include "../queues/zerocopy_ringbuffer.hpp"
#include "../wrap_item.hpp"
#include "../util.hpp"

namespace afs {

template<class T>
class InCallbackSimple : public InCallbackBase {

typedef WrapItem<T> WrapT;

public:
    int PushRecord(void* data, uint32_t len);

    //void Init();
    void Clean() {
        double used_time = (stop_ts - start_ts)/1000000.0;
        LOG_MSG(INDENT_HLINE INDENT
                "Recv %d msgs, Time %.02lf (s), "
                "Throughput %.02lf (msg/s), %.02lf (Gb/s)\n"
                INDENT_HLINE,
                event_,
                used_time,
                event_/used_time,
                8*event_*sizeof(WrapT)/used_time/1024/1024/1024
                );
    }

    void AddOutQueue(void* q) {
        queues_.push_back((ZeroRingBuffer<WrapT>*)q);
        stopped_.push_back(false);
    }


    InCallbackSimple(int num_in) : 
        InCallbackBase(num_in),
        num_stop_(0),
        event_(0),
        start_ts(0), stop_ts(0)
    {}

private:
    std::vector<ZeroRingBuffer<WrapT>*> queues_;
    std::vector<bool> stopped_;
    
    int num_stop_;
    int event_;
    uint64_t start_ts, stop_ts;

    //virtual int GetDestination(void* data, uint32_t len) = 0;
    int GetDestination(void* data, uint32_t len) {
        return 0;
    }
};

template<class T>
int InCallbackSimple<T>::PushRecord(void* data, uint32_t len) {
    if (afs_unlikely(((WrapT*)data)->get_type()==ITEM_FINISH)) {
        WrapT* wrap_item = (WrapT*)data;
        int source = wrap_item->get_worker_source();
        LOG_MSG("in callback: get finish from %d, num_in %d\n", source, num_in_);
        if (!stopped_[source]) {
            stopped_[source] = true;
            num_stop_++;
            for (auto x : queues_) {
                x->Insert(data);
                x->Flush();
            }
            if (num_stop_ == num_in_) {
                stop_ts = now_us();
                //LOG_MSG("in_callback recv %d\n", event_);
                //for (int i=0; i<RB_BATCH; i++) {
                //}
                LOG_MSG("    incallback end\n");
                return 1;
            }
        }
    }
    else {
        ++event_;
        if (event_ == 1) {
            start_ts = now_us();
        }
        //LOG_MSG("incallback %d\n", event_);
        int d = GetDestination(data, len);
        //LOG_MSG("    to insert in destination %d\n", d);
        queues_[d]->Insert(data);
        //LOG_MSG("    incallback inserted %d\n", event_);
    }
    return 0;
}

}

#endif
