#ifndef __ROLL_ANALYZER_HPP__
#define __ROLL_ANALYZER_HPP__

#include <stdio.h>
#include <stdint.h>

#include <string>

#include "config.hpp"
#include "thread.hpp"
#include "in_thread.hpp"
#include "out_thread.hpp"

#include "wrap_item.hpp"
#include "router_base.hpp"
#include "queues/mpsc_channel.hpp"
#include "queues/zerocopy_ringbuffer.hpp"

namespace afs {

/**
 * Processing events
 * @param InT class of input event from Dispatcher
 * @param OutT class of output event to Collector
 */
//template <class InTf, class OutTf, class InTb, class OutTb>
template <class InT, class OutT, class RInT, class ROutT>
class Analyzer : public ThreadBase {

typedef WrapItem<InT> WInT;
typedef WrapItem<OutT> WOutT;
typedef WrapItem<RInT> WRInT;
typedef WrapItem<ROutT> WROutT;

public:

    /*
    void SetInQueue(ZeroRingBuffer<WInT>* q) {
        in_queue_ = q;
    }
    */

    /*
    void SetRInQueue(ZeroRingBuffer<WRInT>* q) {
        r_in_queue_ = q;
    }
    */

    /*
    void SetNumROutQueue(int n) {
        afs_assert(num_r_out_queue_==0, "num_r_out_queue has been set\n");

        num_r_out_queue_ = n;
        r_writer_ = new MPSCWriter<WROutT>(num_r_out_queue_);
    }
    */

    MPSCWriter<WOutT>* GetWriter() {
        return writer_;
    }

    MPSCWriter<WROutT>* GetRWriter() {
        return r_writer_;
    }

    void ConnectInThread(InThread<InT, ROutT>* in_thread) {
        in_queue_ = new ZeroRingBuffer<WInT>();
        in_thread->AddOutQueue(in_queue_);
        if (in_thread->IsReverse()) {
            // each upstream worker corresponds to an output buffer in in_thread
            // add this analyzer as a writer for all the output buffers
            int num_upstream = in_thread->GetNumUpstream();
            int writer_id = get_tid();
            for (int i=0; i<num_upstream; i++) {
                MPSCReader<WROutT>* r_reader = (MPSCReader<WROutT>*)in_thread->GetReverseOutBufferReader(i);
                afs_assert(r_writer_, "reverse writer %d is not available\n", writer_id);
                afs_assert(r_reader, "reverse reader %d is not available\n", i);
                r_writer_->SetWriterId(writer_id);
                r_writer_->ConnectPeer(i, r_reader);
                r_reader->SetReaderId(i);
                r_reader->ConnectPeer(writer_id, r_writer_);
            }

        }
    }

    void ConnectOutThread(OutThread<OutT, RInT>* out_thread) {
        int writer_id = get_tid();
        int num_out_thread_reader = out_thread->GetDownstream();
        for (int i=0; i<num_out_thread_reader; i++) {
            MPSCReader<WOutT>* reader = (MPSCReader<WOutT>*)out_thread->GetOutBufferReader(i);
            afs_assert(writer_, "writer %d is not available\n", writer_id);
            afs_assert(reader, "reader %d is not available\n", i);
            writer_->SetWriterId(writer_id);
            writer_->ConnectPeer(i, reader);
            reader->SetReaderId(i);
            reader->ConnectPeer(writer_id, writer_);
        }

        if (out_thread->IsReverse()) {
            r_in_queue_ = new ZeroRingBuffer<WRInT>();
            out_thread->AddReverseInBuffer(r_in_queue_);
        }
    }

    //Analyzer(int id);
    Analyzer(int num_out_queue, RouterBase* r, int num_r_out_queue=0, RouterBase* rr=NULL);

    virtual void AnalyzerRecovery() {};

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
        int dest = r_router_->GetDestination(&msg, sizeof(ROutT));
        afs_assert(r_writer_, "    reverse writer null\n");
        WROutT* slot = (WROutT*)r_writer_->GetSlot();
        slot->set_type(ITEM_REVERSE);
        slot->set_worker_source(get_wid());
        slot->set_thr_source(get_tid());
        slot->data() = msg;
        r_writer_->Write(dest, slot);
    }

    void FlushWriter() {
        writer_->Flush();
        writer_->Clean();
        writer_->Init();
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

    //  Unique id of the analyzer in the worker
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

    // derived from ThreadBase
    void ThreadInitHandler();
    void ThreadFinishHandler();
    void ThreadMainHandler();

    //  user-define interface to initialize and cleaning-up
    //  called by ThreadInitHandler and ThreadFinishHander respectively
    virtual void AnalyzerInit() = 0;
    virtual void AnalyzerFinish() = 0;

    //  user-define interface to process events of various types
    virtual void DoProcessRecord(InT& tuple) = 0;
    virtual void DoProcessRecord(InT& tuple, uint64_t seq=0) {
        afs_assert(-1, "not implemented\n");
    }
    virtual void DoProcessTimeout() = 0;
    virtual void DoProcessReverse(int src_worker, int src_thread, RInT& tuple) {
        LOG_MSG("Function DoProcessReverse does not be implemented\n");
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

template <class InT, class OutT, class RIntT, class ROutT>
Analyzer<InT, OutT, RIntT, ROutT>::Analyzer(int num_out_queue, RouterBase* r, int num_r_out_queue, RouterBase* rr) :
    ThreadBase(t_analyzer),
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
        LOG_MSG("analyzer out queue %d\n", num_out_queue);
        if (num_out_queue > 0) {
            writer_ = new MPSCWriter<WOutT>(num_out_queue);
        }
        if (num_r_out_queue > 0) {
            r_writer_ = new MPSCWriter<WROutT>(num_r_out_queue);
        }
    }

template <class InT, class OutT, class RIntT, class ROutT>
void Analyzer<InT, OutT, RIntT, ROutT>::ThreadInitHandler() {
    LOG_MSG(INDENT "initializing\n");
    if (writer_ != NULL) {
        writer_->Init();
    }
    if (r_writer_ != NULL) {
        r_writer_->Init();
    }
    AnalyzerInit();
}

template <class InT, class OutT, class RIntT, class ROutT>
void Analyzer<InT, OutT, RIntT, ROutT>::ThreadMainHandler() {
    LOG_MSG("%s start to run\n", get_thread_str());
    
    WInT* tuple = NULL;
    WRInT* r_tuple = NULL;
    bool stop = false;
    bool r_stop = false;
    if (!r_in_queue_) {
        r_stop = true;
    }

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
                    DoProcessReverse(r_tuple->get_worker_source(), r_tuple->get_thr_source(), r_tuple->data());
                    break;
                default:
                    LOG_ERR("Unidentified event type %d\n", (int)t);
                    break;
            }
            r_in_queue_->Ack();
        } // end of if

        //LOG_MSG("read forward queue\n");
        if (afs_unlikely(stop)) {
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
                        DoProcessTimeout();
                        break;
                    case ITEM_NORMAL:
                        event_++;
                        DoProcessRecord(tuple->data(), tuple->get_seq());
                        break;
                    default:
                        LOG_ERR("Unidentified event type %d\n", (int)t);
                        break;
                }
                in_queue_->Ack();
            } // end of if
        }
    } // end of while

    EmitReverseFinish();
    if (r_writer_ != NULL) {
        r_writer_->Clean();
    }
}

//  require to release resources
template <class InT, class OutT, class RIntT, class ROutT>
void Analyzer<InT, OutT, RIntT, ROutT>::ThreadFinishHandler() {
    LOG_MSG(INDENT "process %lu tuples, %lu tuples\n", event_, r_event_);

    AnalyzerFinish();
}

}

#endif // SAND_SUBANALYZER_SUBANALYZER_HPP_
