#ifndef __AFS_INTHREAD_TRACE_ERR_BASE_HPP_INCLUDED__
#define __AFS_INTHREAD_TRACE_ERR_BASE_HPP_INCLUDED__

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>

#include <string>

#include "util.hpp"
#include "in_thread.hpp"

#include "adapter_base.hpp"
#include "adapter_disk.hpp"
#include "adapter_ram.hpp"
#include "adapter_dummy.hpp"

#include "router_base.hpp"

using namespace std;

namespace afs {

/**
 * Receive events outside the worker, executed by a single process
 * @tparam InT output class of dispatcher, used for input of analyzers in the same worker
 */
template <class InT>
class InThreadTraceError : public InThread<InT, NullClass> {

typedef WrapItem<InT> WInT;

public:
    InThreadTraceError(RouterBase* router);

    // derived from InThread
    void AddOutQueue(ZeroRingBuffer<WInT>* q);

protected:

    void Emit(void* data, uint32_t len) {
        int index = router_->GetDestination((InT*)data, sizeof(InT));
        WInT* slot = queues_[index]->GetSlot();
        slot->set_seq(event_);
        slot->set_type(ITEM_NORMAL);
        slot->data() = *(InT*)data;
        queues_[index]->CompleteWrite();
    }

    void EmitTimeout() {
        for (int i=0; i<queues_.size(); i++) {
            WInT* slot = queues_[i]->GetSlot();
            slot->set_type(ITEM_TIMEOUT);
            queues_[i]->CompleteWrite();
        }
        for (int i=0; i<queues_.size(); i++) {
            queues_[i]->Flush();
        }
    }
    
private:
    AdapterBase* adapter_;
    //TraceParserBase<T>* parser_;
    RouterBase* router_;

    std::vector<ZeroRingBuffer<WInT>*> queues_;

    //  monitor number of process events
    uint64_t event_;
    uint64_t emit_succ_;
    uint64_t emit_fail_;
    double used_time_;

    uint64_t error_freq_;
    int max_data_lost_;
    int cur_lost_;

    // derived from ThreadBase
    void ThreadInitHandler();
    void ThreadFinishHandler();
    void ThreadMainHandler();
};

template<class InT>
InThreadTraceError<InT>::InThreadTraceError(
        RouterBase* router
        ) :
    InThread<InT, NullClass>(),
    router_(router),
    //parser_(parser),
    event_(0) {}

template<class T>
void InThreadTraceError<T>::AddOutQueue(ZeroRingBuffer<WInT>* q) {
    queues_.push_back(q);
}

template<class T>
void InThreadTraceError<T>::ThreadInitHandler() {

    Config* config = Config::getInstance();

    /// set up adater
    char* adapter_type_str = config->getstring("adapter_type", NULL);
    afs_assert(adapter_type_str, "Adapter type is not specified\n");

    std::string adapter_type(adapter_type_str);
    if (adapter_type == "ram") {
        LOG_MSG("RAM adapter is used\n");
        adapter_ = new AdapterRAM();
    }
    else if (adapter_type == "disk") {
        LOG_MSG("Disk adapter is used\n");
        adapter_ = new AdapterDisk();
    }
    else if (adapter_type == "dummy") {
        LOG_MSG("Dummy adapter is used\n");
        adapter_ = new AdapterDummy();
    }
    else {
        PrintAdapterNotFound(adapter_type.c_str());
    }

    adapter_->Init();

    max_data_lost_ = config->getint("sys.fault_tolerance.max_data_lost", -1);
    afs_assert(max_data_lost_!=-1, "sys.fault_tolerance.max_data_lost not set\n");
    LOG_MSG("Max data lost %d\n", max_data_lost_);

    error_freq_ = config->getint("sys.fault_tolerance.error_freq", -1);
    afs_assert(max_data_lost_!=-1, "sys.fault_tolerance.error_freq not set\n");
    LOG_MSG("Error freq %d\n", error_freq_);

    cur_lost_ = 0;
}

template<class T>
void InThreadTraceError<T>::ThreadMainHandler() {
    LOG_MSG("InThreadTraceError (%d) start to run\n", getpid());
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    //T* data;
    void* data;
    uint32_t len;
    while (1) {
        adapter_->ReadRecord(&data, &len);
        if (data == NULL) {
            break;
        }
        event_++;
        if (event_ % error_freq_ == error_freq_/2) {
            EmitTimeout();
            cur_lost_ = max_data_lost_;
        }

        if (cur_lost_ == 0) {
            Emit(data, len);
        }
        else {
            cur_lost_--;
        }
        //parser_->ParseRecord(data, len);
        //ParseRecord(data, len);
    }

    gettimeofday(&end_time, NULL);
    used_time_ = ((end_time.tv_sec + end_time.tv_usec / 1000000.0) -
                (start_time.tv_sec + start_time.tv_usec / 1000000.0));

    WInT wrap_item;
    wrap_item.set_type(ITEM_FINISH);
    for (auto j=0; j<queues_.size(); j++) {
        queues_[j]->Insert(&wrap_item);
    }
    for (auto j=0; j<queues_.size(); j++) {
        queues_[j]->Flush();
    }
}

template<class T>
void InThreadTraceError<T>::ThreadFinishHandler() {
    LOG_MSG(INDENT_HLINE INDENT
            "Total msgs %" PRIu64 ", "
            "Time %lf (s), "
            "Througput: %lf (msg/s)\n"
            INDENT_HLINE,
            event_,
            used_time_,
            event_ / used_time_);

    Config* config = Config::getInstance();
    char* throughput_file = config->getstring("throughput_benchmark", NULL);
    if (throughput_file) {
        FILE* tf = fopen(throughput_file, "a");
        fprintf(tf,
            "Total msgs %" PRIu64 ", "
            "Time %lf (s), "
            "Througput: %lf (msg/s)\n",
            event_,
            used_time_,
            event_ / used_time_);
        fclose(tf);
    }

    adapter_->Clean();
}

} // namespace

#endif // INCLUDE
