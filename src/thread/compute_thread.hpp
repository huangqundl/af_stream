#ifndef __AFS_COMPUTE_THREAD_HPP_INCLUDED__
#define __AFS_COMPUTE_THREAD_HPP_INCLUDED__

#include <stdio.h>
#include <stdint.h>

#include <string>

#include "config.hpp"
#include "thread.hpp"
#include "up_thread.hpp"
#include "down_thread.hpp"

#include "wrap_item.hpp"
#include "router_base.hpp"
#include "queues/mpsc_channel.hpp"
#include "queues/zerocopy_ringbuffer.hpp"

#include "fault_tolerance/operator_tracker.hpp"
#include "operator/ft_interface.hpp"

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
    //ComputeThread(int id);
    ComputeThread(int num_out_queue, RouterBase* r, int num_r_out_queue=0, RouterBase* rr=NULL);

    void ConnectUpThread(UpThread<InT, ROutT>* up_thread);
    void ConnectDownThread(DownThread<OutT, RInT>* down_thread);
    void ConnectComputeThread(ThreadBase* dst_thread);

    virtual void ComputeThreadRecovery() {};

    MPSCWriter<WOutT>* GetWriter() {
        return writer_;
    }

    MPSCWriter<WROutT>* GetRWriter() {
        return r_writer_;
    }

protected:

    /**
     * Send data to next-hop worker via output_thread
     */
    void EmitData(OutT& msg) {
        int dest = router_->GetDestination(&msg, sizeof(OutT));
        WOutT* slot = (WOutT*)writer_->GetSlot();
        slot->set_type(ITEM_NORMAL);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        slot->data() = msg;
        writer_->Write(dest, slot);
    }

    void EmitData(int dest, OutT& msg) {
        WOutT* slot = (WOutT*)writer_->GetSlot();
        slot->set_type(ITEM_NORMAL);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        slot->data() = msg;
        writer_->Write(dest, slot);
    }

    WOutT* GetSlot() {
        return (WOutT*)writer_->GetSlot();
    }

    void CompleteWriteSlot(int dest, WOutT* slot) {
        slot->set_type(ITEM_NORMAL);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        writer_->Write(dest, slot);
    }

    int GetDest(OutT& msg) {
        return router_->GetDestination(&msg, sizeof(OutT));
    }

    /**
     * Send data to next-hop worker via output_thread
     */
    void EmitReverseData(ROutT& msg) {
        afs_assert(r_writer_, "    reverse writer null\n");
        int dest = r_router_->GetDestination(&msg, sizeof(ROutT));
        WROutT* slot = (WROutT*)r_writer_->GetSlot();
        slot->set_type(ITEM_REVERSE);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        slot->data() = msg;
        r_writer_->Write(dest, slot);
    }

    void FlushReverseWriter() {
        r_writer_->Flush();
        r_writer_->Clean();
        r_writer_->Init();
    }
    
    void EmitReverseTimeout() {
        WROutT r_wrap_msg;
        r_wrap_msg.set_type(ITEM_REVERSE);
        r_wrap_msg.set_worker_source(get_wid());
        r_wrap_msg.set_thr_source(get_tid());
        EmitReverseWrapData(0, r_wrap_msg);
    }

private:

    //  Unique id of the compute_thread in the worker
    //int id_;

    //  monitor number of process events
    uint64_t event_;
    uint64_t r_event_;

    int num_out_queue_;
    int num_r_out_queue_;

    // forward queues
    ZeroRingBuffer<WInT>* in_queue_;
    MPSCWriter<WOutT>* writer_;

    // backward queue
    ZeroRingBuffer<WRInT>* r_in_queue_;
    MPSCWriter<WROutT>* r_writer_;

    RouterBase* router_;
    RouterBase* r_router_;

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
    virtual void ProcessRecord(InT& tuple, uint64_t seq) = 0;
    virtual void ProcessTimeout() = 0;
    virtual void ProcessReverse(int src_worker, int src_thread, RInT& tuple) {
        LOG_MSG("Function ProcessReverse does not be implemented\n");
    }

    void EmitWrapData(int dest, WOutT& msg) {
        WOutT* slot = (WOutT*)writer_->GetSlot();
        *slot = msg;
        writer_->Write(dest, slot);
    }

    void EmitReverseWrapData(int dest, WROutT& msg) {
        WROutT* slot = (WROutT*)r_writer_->GetSlot();
        *slot = msg;
        r_writer_->Write(dest, slot);
    }

    void EmitFinish() {
        WOutT wrap_msg;
        if (writer_ != NULL) {
            wrap_msg.set_type(ITEM_FINISH);
            wrap_msg.set_worker_source(get_wid());
            wrap_msg.set_thr_source(get_tid());
            for (int i = 0; i < num_out_queue_; i++) {
                EmitWrapData(i, wrap_msg);
            }
            writer_->Flush();
            LOG_MSG("ComputeThread: emit finish\n");
        }
    }
    
    void EmitReverseFinish() {
        WROutT r_wrap_msg;
        if (r_writer_ != NULL) {
            r_wrap_msg.set_type(ITEM_FINISH);
            r_wrap_msg.set_worker_source(get_wid());
            r_wrap_msg.set_thr_source(get_tid());
            for (int i = 0; i < num_r_out_queue_; i++) {
                EmitReverseWrapData(i, r_wrap_msg);
            }
            r_writer_->Flush();
        }
    }

};

template <class InT, class OutT, class RInT, class ROutT>
ComputeThread<InT, OutT, RInT, ROutT>::ComputeThread(int num_out_queue, RouterBase* r, int num_r_out_queue, RouterBase* rr) :
    ThreadBase(t_compute_thread),
    event_(0),
    r_event_(0),
    num_out_queue_(num_out_queue),
    num_r_out_queue_(num_r_out_queue),
    in_queue_(NULL),
    writer_(NULL),
    r_in_queue_(NULL),
    r_writer_(NULL),
    router_(r),
    r_router_(rr)
        {
        LOG_MSG("compute_thread out queue %d, reverse queue %d\n", num_out_queue, num_r_out_queue);
        if (num_out_queue > 0) {
            writer_ = new MPSCWriter<WOutT>(num_out_queue);
        }
        if (num_r_out_queue > 0) {
            LOG_MSG("create reverse write_\n");
            r_writer_ = new MPSCWriter<WROutT>(num_r_out_queue);
        }
    }

template <class InT, class OutT, class RInT, class ROutT>
void ComputeThread<InT, OutT, RInT, ROutT>::ThreadInitHandler() {
    LOG_MSG(INDENT "initializing\n");
    if (writer_ != NULL) {
        writer_->Init();
    }
    if (r_writer_ != NULL) {
        r_writer_->Init();
    }
    ComputeThreadInit();
    operators = *(OperatorTracker::GetInstance()->GetThreadOps(thr_id()));
}

template <class InT, class OutT, class RInT, class ROutT>
void ComputeThread<InT, OutT, RInT, ROutT>::ThreadMainHandler() {
    
    WInT* tuple = NULL;
    WRInT* r_tuple = NULL;
    bool stop = false;
    bool r_stop = false;

    unsigned int num_operator = operators.size();
    afs_zmq::command_t cmd;

    if (!r_in_queue_) {
        r_stop = true;
    }

    LOG_MSG("%s start to run\n", get_thread_str());
    LOG_MSG("    %s operator number %u\n", get_thread_str(), num_operator);

    while (!stop || !r_stop) {
        //LOG_MSG("while\n");
        if (!r_stop && (r_tuple=r_in_queue_->Extract()) != NULL) {
            ITEM_TYPE t = r_tuple->get_type();
            switch (t) {
                case ITEM_FINISH:
                    r_stop = true;
                    break;
                case ITEM_REVERSE:
                    r_event_++;
                    ProcessReverse(r_tuple->get_worker_source(), r_tuple->get_thr_source(), r_tuple->data());
                    break;
                default:
                    LOG_ERR("Unidentified event type %d\n", (int)t);
                    break;
            }
            r_in_queue_->Ack();
        } // end of if

        //LOG_MSG("read forward queue\n");
        if (afs_unlikely(stop)) {
            //LOG_MSG("Attempt to clean\n");
            writer_->AttemptClean();
        }
        else {
            if ((tuple=in_queue_->Extract()) != NULL) {
                ITEM_TYPE t = tuple->get_type();
                switch (t) {
                    case ITEM_FINISH:
                        stop = true;
                        // send finish first
                        // but continue to wait for finish from downstream workers
                        EmitFinish();
                        break;
                    case ITEM_TIMEOUT:
                        ProcessTimeout();
                        break;
                    case ITEM_NORMAL:
                        event_++;
                        ProcessRecord(tuple->data(), tuple->get_seq());
                        break;
                    default:
                        LOG_ERR("Unidentified event type %d\n", (int)t);
                        break;
                }
                in_queue_->Ack();
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
                    SendWorker(cmd);
                    //SendBackup(backup_item);
                    WaitWorker(afs_zmq::command_t::backup, true);
                    backup_item.data.meta.len = 0;
                }
            }
        }
    } // end of while

    if (writer_) {
        writer_->AttemptClean();
    }
    EmitReverseFinish();
    if (r_writer_ != NULL) {
        r_writer_->Clean();
    }
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
            afs_assert(r_writer_, "reverse writer %d is not available\n", writer_id);
            afs_assert(r_reader, "reverse reader %d is not available\n", i);
            r_writer_->SetWriterId(writer_id);
            r_writer_->ConnectPeer(i, r_reader);
            r_reader->SetReaderId(i);
            r_reader->ConnectPeer(writer_id, r_writer_);
        }
    }
}

template <class InT, class OutT, class RInT, class ROutT>
void ComputeThread<InT, OutT, RInT, ROutT>::ConnectDownThread(DownThread<OutT, RInT>* down_thread) {
    int writer_id = get_tid();
    int num_down_thread_reader = down_thread->GetDownstream();
    for (int i=0; i<num_down_thread_reader; i++) {
        MPSCReader<WOutT>* reader = (MPSCReader<WOutT>*)down_thread->GetOutBufferReader(i);
        afs_assert(writer_, "writer %d is not available\n", writer_id);
        afs_assert(reader, "reader %d is not available\n", i);
        writer_->SetWriterId(writer_id);
        writer_->ConnectPeer(i, reader);
        reader->SetReaderId(i);
        reader->ConnectPeer(writer_id, writer_);
    }
    
    if (down_thread->IsReverse()) {
        r_in_queue_ = new ZeroRingBuffer<WRInT>();
        down_thread->AddReverseInBuffer(r_in_queue_);
    }
}

} // namespace

#endif // SAND_SUBANALYZER_SUBANALYZER_HPP_
