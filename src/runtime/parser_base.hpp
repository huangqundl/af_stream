#ifndef __ROLL_PARSER_BASE_HPP_INCLUDED__
#define __ROLL_PARSER_BASE_HPP_INCLUDED__

#include <stdint.h>
#include <vector>

#include "queues/zerocopy_ringbuffer.hpp"
#include "util.hpp"
#include "wrap_item.hpp"

namespace afs {

template <class T>
class TraceParserBase {

public:
    virtual void ParseRecord(void* data, uint32_t len) = 0;
    virtual void Init() = 0;
    virtual void Clean() = 0;

    void AddOutQueue(ZeroRingBuffer<T>* q);
    void PrintStat();

protected:

    TraceParserBase() : emit_succ_(0), emit_fail_(0) {}

    void Emit(T &event, int index);

private:
    std::vector<ZeroRingBuffer<T>*> queues_;

    //  monitor number of emitted events
    uint64_t emit_succ_;

    //  monitor number of fail-emitted events
    uint64_t emit_fail_;

    /**
     * Assign event to the specific analyzer
     * @param event the event to be emitted
     * @param index index of target analyzer
     * @param type type of event
     */

};

template<class T>
void TraceParserBase<T>::AddOutQueue(ZeroRingBuffer<T>* q) {
    queues_.push_back(q);
}

template<class T>
void TraceParserBase<T>::Emit(T &msg, int queue_index) {
    if (queues_[queue_index]->Insert(msg)==0) {
        emit_succ_++;
    }
    else {
        emit_fail_++;
    }
}

template<class T>
void TraceParserBase<T>::PrintStat() {
    LOG_MSG("\n=================================================\n"
            "Emit successfully: %" PRIu64 "\n"
            "Emit fail: %" PRIu64 "\n"
            "\n=================================================\n",
            emit_succ_,
            emit_fail_);
}

template<class T>
void TraceParserBase<T>::Clean() {
    // emit RB_BATCH dummy events to pop pending events in the ring buffer
    WrapItem<T> finish;
    finish.set_type(afs::ITEM_FINISH);
    for (int i = 0; i < queues_.size() ; i++) {
        for (int j = 0; j < RB_BATCH; j++) {
            //Emit(finish, i);
        }
    }
}

}

#endif
