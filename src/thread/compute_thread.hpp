#ifndef __AFS_COMPUTE_THREAD_HPP_INCLUDED__
#define __AFS_COMPUTE_THREAD_HPP_INCLUDED__

#include <stdio.h>
#include <stdint.h>

#include <string>

#include "../config.hpp"
#include "thread.hpp"
#include "up_thread.hpp"
#include "down_thread.hpp"

#include "../wrap_item.hpp"
#include "router_base.hpp"
#include "../queues/mpsc_channel.hpp"
#include "../queues/zerocopy_ringbuffer.hpp"

#include "../fault_tolerance/operator_tracker.hpp"
#include "../operator/ft_interface.hpp"

namespace afs {

/**
 * Processing events
 * @param InT class of input event from Dispatcher
 * @param OutT class of output event to Collector
 */
//template <class InTf, class OutTf, class InTb, class OutTb>
template <class InT, class OutT, class RInT, class ROutT>
class ComputeThread : public ThreadBase {

typedef WrapItem<InT> WInT;
typedef WrapItem<OutT> WOutT;
typedef WrapItem<RInT> WRInT;
typedef WrapItem<ROutT> WROutT;

public:
    ComputeThread(int num_upstream, int num_downstream);

    void ConnectUpThread(UpThread<InT, ROutT>* up_thread);
    void ConnectDownThread(DownThread<OutT, RInT>* down_thread);
    void ConnectComputeThread(ThreadBase* dst_thread);

    virtual void ComputeThreadRecovery() {};

protected:

    /**
     * Send data to next-hop worker via output_thread
     */
    /*
    void EmitData(OutT& msg) {
        int dest = router_->GetDestination(&msg, sizeof(OutT));
        WOutT* slot = (WOutT*)writer_->GetSlot();
        slot->set_type(ITEM_NORMAL);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        slot->data() = msg;
        down_writer_->Write(dest, slot);
    }
    */

    uint64_t GetNumDownstream() {
        return num_downstream_; 
    }

    void EmitData(int dest, OutT& msg) {
        //LOG_MSG("To emit %d\n", dest);
        WOutT* slot = (WOutT*)down_writer_->GetSlot();
        slot->set_type(ITEM_NORMAL);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        slot->data() = msg;
        down_writer_->Write(dest, slot);
        //LOG_MSG("   end emit\n");
    }

    /*
    WOutT* GetSlot() {
        return (WOutT*)writer_->GetSlot();
    }
    */

    /*
    void CompleteWriteSlot(int dest, WOutT* slot) {
        slot->set_type(ITEM_NORMAL);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        down_writer_->Write(dest, slot);
    }
    */

    /*
    int GetDest(OutT& msg) {
        return router_->GetDestination(&msg, sizeof(OutT));
    }
    */

    /**
     * Send data to next-hop worker via output_thread
     */
    void EmitReverseData(int dest, ROutT& msg) {
        //LOG_MSG("To emit reverse\n");
        afs_assert(up_writer_, "    reverse writer null\n");
        //int dest = r_router_->GetDestination(&msg, sizeof(ROutT));
        WROutT* slot = (WROutT*)up_writer_->GetSlot();
        slot->set_type(ITEM_REVERSE);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        slot->data() = msg;
        up_writer_->Write(dest, slot);
        //LOG_MSG("   end emit reverse\n");
    }

    void FlushReverseWriter() {
        up_writer_->Flush();
        up_writer_->Clean();
        up_writer_->Init();
    }
    
    void EmitReverseTimeout(int dest) {
        WROutT r_wrap_msg;
        r_wrap_msg.set_type(ITEM_REVERSE);
        r_wrap_msg.set_worker_source(get_wid());
        r_wrap_msg.set_thr_source(get_tid());
        EmitReverseWrapData(dest, r_wrap_msg);
    }

private:

    //  Unique id of the compute_thread in the worker
    //int id_;

    //  monitor number of process events
    uint64_t event_;
    uint64_t r_event_;

    int num_downstream_;
    int num_upstream_;

    // forward queues
    ZeroRingBuffer<WInT>* in_queue_;
    MPSCWriter<WOutT>* down_writer_;

    // backward queue
    ZeroRingBuffer<WRInT>* r_in_queue_;
    MPSCWriter<WROutT>* up_writer_;

    //RouterBase* router_;
    //RouterBase* r_router_;

    // fault-tolerant operators
    std::vector<FTInterface*> operators;

    // derived from ThreadBase
    void ThreadInitHandler();
    void ThreadFinishHandler();
    void ThreadMainHandler();

    //  user-define interface to initialize and cleaning-up
    //  called by ThreadInitHandler and ThreadFinishHander respectively
    virtual void ComputeThreadInit() = 0;
    virtual void ComputeThreadFinish() = 0;

    //  user-define interface to process events of various types
    //virtual void DoProcessRecord(InT& tuple) = 0;
    virtual void ProcessData(uint32_t src_worker, uint32_t src_thread, uint64_t seq, InT& tuple) = 0;
    virtual void ProcessPunc() = 0;
    virtual void ProcessFeedback(int src_worker, int src_thread, RInT& tuple) {
        LOG_MSG("Function ProcessFeedback does not be implemented\n");
    }

    void EmitWrapData(int dest, WOutT& msg) {
        WOutT* slot = (WOutT*)down_writer_->GetSlot();
        *slot = msg;
        down_writer_->Write(dest, slot);
    }

    void EmitReverseWrapData(int dest, WROutT& msg) {
        WROutT* slot = (WROutT*)up_writer_->GetSlot();
        *slot = msg;
        up_writer_->Write(dest, slot);
    }

    void EmitFinish() {
        WOutT wrap_msg;
        if (down_writer_ != NULL) {
            wrap_msg.set_type(ITEM_FINISH);
            wrap_msg.set_worker_source(get_wid());
            wrap_msg.set_thr_source(get_tid());
            for (int i = 0; i < num_downstream_; i++) {
                EmitWrapData(i, wrap_msg);
            }
            down_writer_->Flush();
            LOG_MSG("ComputeThread: emit finish\n");
        }
    }
    
    void EmitReverseFinish() {
        WROutT r_wrap_msg;
        if (up_writer_ != NULL) {
            r_wrap_msg.set_type(ITEM_FINISH);
            r_wrap_msg.set_worker_source(get_wid());
            r_wrap_msg.set_thr_source(get_tid());
            for (int i=0; i < num_upstream_; i++) {
                EmitReverseWrapData(i, r_wrap_msg);
            }
            up_writer_->Flush();
        }
    }

};

template <class InT, class OutT, class RInT, class ROutT>
ComputeThread<InT, OutT, RInT, ROutT>::ComputeThread(
        int num_upstream,
        //RouterBase* r,
        int num_downstream
        //, RouterBase* rr
        ) :
    ThreadBase(t_compute_thread),
    event_(0),
    r_event_(0),
    num_downstream_(num_downstream),
    num_upstream_(num_upstream),
    in_queue_(NULL),
    down_writer_(NULL),
    r_in_queue_(NULL),
    up_writer_(NULL)
    //router_(r),
    //r_router_(rr)
        {
        LOG_MSG("compute_thread downstream %d, upstream %d\n", num_downstream, num_upstream);
        if (num_downstream > 0) {
            down_writer_ = new MPSCWriter<WOutT>();
        }
        if (num_upstream > 0) {
            LOG_MSG("create reverse write_\n");
            up_writer_ = new MPSCWriter<WROutT>();
        }
    }

template <class InT, class OutT, class RInT, class ROutT>
void ComputeThread<InT, OutT, RInT, ROutT>::ThreadInitHandler() {
    LOG_MSG(INDENT "initializing\n");
    if (down_writer_ != NULL) {
        down_writer_->Init();
    }
    if (up_writer_ != NULL) {
        up_writer_->Init();
    }
    ComputeThreadInit();
    operators = *(OperatorTracker::GetInstance()->GetThreadOps(thr_id()));
}

template <class InT, class OutT, class RInT, class ROutT>
void ComputeThread<InT, OutT, RInT, ROutT>::ThreadMainHandler() {
    
    WInT* tuple = NULL;
    WRInT* r_tuple = NULL;
    int up_stop = 0;
    int down_stop = 0;

    // no upstream implies external source
    if (num_upstream_ == 0) {
        num_upstream_++;
    }

    unsigned int num_operator = operators.size();
    afs_zmq::command_t cmd;

    /*
    if (!r_in_queue_) {
        r_stop = true;
    }
    */

    //LOG_MSG("%s start to run\n", get_thread_str());
    //LOG_MSG("    %s operator number %u\n", get_thread_str(), num_operator);

    while (up_stop<num_upstream_ || down_stop<num_downstream_) {
        //LOG_MSG("while\n");
        while (down_stop<num_downstream_ && (r_tuple=r_in_queue_->Extract()) != NULL) {
            ITEM_TYPE t = r_tuple->get_type();
            switch (t) {
                case ITEM_FINISH:
                    down_stop++;
                    break;
                case ITEM_REVERSE:
                    r_event_++;
                    ProcessFeedback(r_tuple->get_worker_source(), r_tuple->get_thr_source(), r_tuple->data());
                    break;
                default:
                    LOG_ERR("Unidentified event type %d\n", (int)t);
                    break;
            }
            r_in_queue_->Ack();
        } // end of while reverse

        //LOG_MSG("read forward queue\n");
        if (afs_unlikely(up_stop==num_upstream_)) {
            //LOG_MSG("Attempt to clean\n");
            down_writer_->AttemptClean();
        }
        else {
            if ((tuple=in_queue_->Extract()) != NULL) {
                ITEM_TYPE t = tuple->get_type();
                uint32_t worker = tuple->get_worker_source();
                uint32_t thread = tuple->get_thr_source();
                uint64_t seq = tuple->get_seq();
                InT data = tuple->data();
                in_queue_->Ack();
                switch (t) {
                    case ITEM_FINISH:
                        up_stop++;
                        LOG_MSG("compute thread: up_stop %d\n", up_stop);
                        // send finish first
                        // but continue to wait for finish from downstream workers
                        if (up_stop == num_upstream_) {
                            EmitFinish();
                        }
                        break;
                    case ITEM_TIMEOUT:
                        ProcessPunc();
                        break;
                    case ITEM_NORMAL:
                        event_++;
                        ProcessData(worker, thread, seq, data);
                        break;
                    default:
                        LOG_ERR("Unidentified event type %d\n", (int)t);
                        break;
                }
            } // end of if
        }

        for (unsigned int i=0; i<num_operator; i++) {
            double d = operators[i]->CalculateDivergence();
            //LOG_MSG("thread %d, operator %d, divergence %lf, divergence thresh %lf\n", thr_id(), i, d, operators[i]->GetDivergenceThresh());
            if (d > operators[i]->GetDivergenceThresh()) {
                BackupItem& backup_item = cmd.args.backup_item;
                operators[i]->SerializeState(backup_item.data);
                if (backup_item.data.meta.len) {
                    cmd.type = afs_zmq::command_t::backup;
                    backup_item.info.backup_op = 0;
                    backup_item.info.worker_id = get_wid();
                    backup_item.info.thread_id = thr_id();
                    backup_item.info.op_index = i;
                    backup_item.info.seq = tuple->get_seq();
                    NotifyWorker(cmd);
                    WaitWorker(afs_zmq::command_t::backup, true);
                    backup_item.data.meta.len = 0;
                }
            }
        }
    } // end of while

    LOG_MSG("    compute thread (out-of-while): up_stop %d, upstream %d, down_stop %d, downstream %d\n", up_stop, num_upstream_, down_stop, num_downstream_);

    if (down_writer_) {
        down_writer_->AttemptClean();
    }
    LOG_MSG("    compute thread send reverse end\n");
    EmitReverseFinish();
    if (up_writer_ != NULL) {
        up_writer_->Clean();
    }

    LOG_MSG("    compute thread end\n");
}

template <class InT, class OutT, class RInT, class ROutT>
void ComputeThread<InT, OutT, RInT, ROutT>::ThreadFinishHandler() {
    LOG_MSG(INDENT "process %lu tuples, %lu tuples\n", event_, r_event_);

    ComputeThreadFinish();
}

template <class InT, class OutT, class RInT, class ROutT>
void ComputeThread<InT, OutT, RInT, ROutT>::ConnectUpThread(UpThread<InT, ROutT>* up_thread) {
    in_queue_ = new ZeroRingBuffer<WInT>();
    up_thread->AddOutQueue(in_queue_);
    if (up_thread->IsReverse()) {
        // each upstream worker corresponds to an output buffer in up_thread
        // add this compute_thread as a writer for all the output buffers
        int num_upstream = up_thread->GetNumUpstream();
        int writer_id = get_tid();
        for (int i=0; i<num_upstream; i++) {
            MPSCReader<WROutT>* r_reader = (MPSCReader<WROutT>*)up_thread->GetReverseOutBufferReader(i);
            afs_assert(up_writer_, "reverse writer %d is not available\n", writer_id);
            afs_assert(r_reader, "reverse reader %d is not available\n", i);
            up_writer_->SetWriterId(writer_id);
            up_writer_->ConnectPeer(i, r_reader);
            r_reader->SetReaderId(i);
            r_reader->ConnectPeer(writer_id, up_writer_);
        }
    }
}

template <class InT, class OutT, class RInT, class ROutT>
void ComputeThread<InT, OutT, RInT, ROutT>::ConnectDownThread(DownThread<OutT, RInT>* down_thread) {
    int writer_id = get_tid();
    int num_down_thread_reader = down_thread->GetDownstream();
    for (int i=0; i<num_down_thread_reader; i++) {
        MPSCReader<WOutT>* reader = (MPSCReader<WOutT>*)down_thread->GetOutBufferReader(i);
        afs_assert(down_writer_, "writer %d is not available\n", writer_id);
        afs_assert(reader, "reader %d is not available\n", i);
        down_writer_->SetWriterId(writer_id);
        down_writer_->ConnectPeer(i, reader);
        reader->SetReaderId(i);
        reader->ConnectPeer(writer_id, down_writer_);
    }
    
    if (down_thread->IsReverse()) {
        r_in_queue_ = new ZeroRingBuffer<WRInT>();
        down_thread->AddReverseInBuffer(r_in_queue_);
    }
}

} // namespace

#endif // SAND_SUBANALYZER_SUBANALYZER_HPP_
