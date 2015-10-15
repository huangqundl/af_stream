#ifndef __ROLL_INTHREAD_I_BASE_HPP_INCLUDED__
#define __ROLL_INTHREAD_s_BASE_HPP_INCLUDED__

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

#include "queues/mpsc_channel.hpp"
#include "fault_tolerance/adapter_recovery.hpp"
#include "fault_tolerance/data_manager.hpp"

using namespace std;

namespace afs {

/**
 * Receive events outside the worker, executed by a single process
 * @tparam InT output class of dispatcher, used for input of analyzers in the same worker
 */
template <class InT>
class InThreadTraceMinibatch : public InThread<InT, NullClass> {

typedef WrapItem<InT> WInT;

public:
    InThreadTraceMinibatch(int num_analyzer, RouterBase* router);

    // derived from InThread
    void AddOutQueue(ZeroRingBuffer<WInT>* q);

    bool IsReverse() {
        return true;
    }

    int GetNumUpstream() {
        return 1;
    }

    MPSCReader<WNullClass>* GetReverseOutBufferReader(int i) {
        return reader_;
    }

protected:

    void Emit(void* data, uint32_t len) {
        int index = router_->GetDestination((InT*)data, sizeof(InT));
        WInT* slot = queues_[index]->GetSlot();
        slot->set_type(ITEM_NORMAL);
        slot->data() = *(InT*)data;
        queues_[index]->CompleteWrite();
    }

    void EmitTimeout() {
        for (int i=0; i<num_analyzer_; i++) {
            WInT* slot = queues_[i]->GetSlot();
            slot->set_type(ITEM_TIMEOUT);
            queues_[i]->CompleteWrite();
        }
        for (int i=0; i<num_analyzer_; i++) {
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

    DataManager* data_manager_;
    MPSCReader<WNullClass>* reader_;
    int num_analyzer_;
    InT* batch_data_;
    int batch_size;

    // derived from ThreadBase
    void ThreadInitHandler();
    void ThreadFinishHandler();
    void ThreadMainHandler();

    void DoRecovery() {
        uint64_t start_ts = now_us();
        AdapterRecovery* adapter_recovery =
            data_manager_->RequestRecoveryData();
        uint64_t end_ts = now_us();

        void* data;
        uint32_t len;
        while (1) {
            adapter_recovery->ReadRecord(&data, &len);
            if (data == NULL) {
                break;
            }

            //printf("%s", (char*)data);

            Emit(data, len);
        }
        LOG_MSG("Recovery time %lf\n", (end_ts-start_ts)/1000000.0);

    }
};

template<class InT>
InThreadTraceMinibatch<InT>::InThreadTraceMinibatch(
        int num_analyzer,
        RouterBase* router
        ) :
    InThread<InT, NullClass>(),
    router_(router),
    //parser_(parser),
    event_(0),
    num_analyzer_(num_analyzer) {
        reader_ = new MPSCReader<WNullClass>(num_analyzer);
    }

template<class T>
void InThreadTraceMinibatch<T>::AddOutQueue(ZeroRingBuffer<WInT>* q) {
    queues_.push_back(q);
}

template<class T>
void InThreadTraceMinibatch<T>::ThreadInitHandler() {

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

    // fault tolerance
    data_manager_ = DataManager::GetInstance();
    batch_size = config->getint("sys.fault_tolerance.mini_batch_size", 0);
    LOG_MSG("Minibatch size %d\n", batch_size);
    afs_assert(batch_size > 0, "sys.fault_tolerance.mini_batch_size should be larger than 0\n");
    batch_data_ = (T*)calloc(sizeof(T), batch_size);
    afs_assert(batch_data_, "allocate batch_data buffer failure\n");

    reader_->Init();
}

template<class T>
void InThreadTraceMinibatch<T>::ThreadMainHandler() {
    LOG_MSG("InThreadTraceMinibatch (%d) start to run\n", getpid());

    if (InThread<T, NullClass>::is_recovery) {
        DoRecovery();
    }

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

        if (event_ && event_%batch_size==0) {
            for (int i=0; i<batch_size; i++) {
                Emit(batch_data_+i, sizeof(T));
            }
            EmitTimeout();
            for (int i=0; i<num_analyzer_; i++) {
                void* data = NULL;
                while (data==NULL) {
                    data = reader_->Read();
                }
                WNullClass* item = (WNullClass*)(data);
                if (item->get_type()!=ITEM_REVERSE) {
                    LOG_WARN("Recv Non-reverse data\n");
                }
                reader_->Ack(item->get_thr_source(), data);
            }
            reader_->Flush();
            reader_->Clean();
        }
        
        batch_data_[event_%batch_size] = *((T*)data);
        data_manager_->SaveData(event_, data, len);
        event_++;
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

    for (int i=0; i<num_analyzer_; i++) {
        void* data = NULL;
        while (data==NULL) {
            data = reader_->Read();
        }
        WNullClass* item = (WNullClass*)(data);
        if (item->get_type()!=ITEM_FINISH) {
            LOG_WARN("Recv Non-finish data\n");
        }
        reader_->Ack(item->get_thr_source(), data);
    }
    reader_->Flush();
    reader_->Clean();
}

template<class T>
void InThreadTraceMinibatch<T>::ThreadFinishHandler() {
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
    delete adapter_;

    reader_->Clean();
    delete reader_;
}

} // namespace

#endif // INCLUDE
