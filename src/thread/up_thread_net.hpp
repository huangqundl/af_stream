#ifndef __AFS_INTHREAD_NET_HPP_INCLUDED__
#define __AFS_INTHREAD_NET_HPP_INCLUDED__

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>

#include <string>

#include "util.hpp"
#include "up_thread.hpp"

#include "net/epoll.hpp"
#include "net/tcp_listener.hpp"
#include "net/options.hpp"

#include "in_callback_base.hpp"
#include "in_callback_simple.hpp"
#include "out_callback_base.hpp"
#include "out_callback_simple.hpp"


namespace afs {

template <class InT, class ROutT=NullClass>
class UpThreadNet : public UpThread<InT, ROutT> {

typedef WrapItem<InT> WInT;
typedef WrapItem<ROutT> WROutT;

public:

    UpThreadNet(int num_upstream,
        int num_compute_thread
            //InCallbackBase* in_callback,
            //std::vector<OutCallbackBase*>* out_callbacks
            );

    void AddSource(char* source);

    void AddOutQueue(ZeroRingBuffer<WInT>* q);

    MPSCReader<WROutT>* GetReverseOutBufferReader(int i) {
        return (MPSCReader<WROutT>*)out_callbacks_[i]->GetReader();
    }

    int GetNumUpstream() {
        return num_upstream_;
    }

    bool IsReverse() {
        return support_feedback_;
    }

private:

    int num_upstream_;
    bool support_feedback_;

    // listen on single port
    //int listen_port;
    std::string listen_addr_;

    // epoll
    afs_zmq::epoll_t* poller_;

    afs_zmq::options_t* options_;

    // route incoming events to compute_threads
    InCallbackBase* in_callback_;
    OutCallbackBase** out_callbacks_;

    //  monitor number of process events
    uint64_t event_;

    //struct timeval start_time, end_time;

    // copied from socket_base.hpp in 0MQ
    int parse_uri (const char *uri_,
            std::string &protocol_,
            std::string &address_);

    // copied from socket_base.hpp in 0MQ
    int check_protocol (const std::string &protocol_);

    // derived from ThreadBase
    void ThreadInitHandler();
    void ThreadFinishHandler();
    void ThreadMainHandler();
};

template <class InT, class ROutT>
UpThreadNet<InT, ROutT>::UpThreadNet(
        int num_upstream,
        int num_compute_thread
        //bool support_feedback
        //InCallbackBase* in_callback,
        //std::vector<OutCallbackBase*>* out_callbacks
            ) :
    UpThread<InT, ROutT>(),
    num_upstream_(num_upstream), support_feedback_(false),
    poller_(NULL), options_(NULL), 
    //in_callback_(in_callback),
    //out_callbacks_(NULL),
    event_(0) {
        
        in_callback_ = new afs::InCallbackSimple<InT>(num_upstream);

        support_feedback_
 = true;
        if (support_feedback_
    ) {
            out_callbacks_ = (OutCallbackBase**)calloc(num_upstream, sizeof(OutCallbackBase*));
            for (int i=0; i<num_upstream; i++) {
                out_callbacks_[i] = 
                    new afs::OutCallbackSimple<ROutT>(num_compute_thread);
            }
        }
        //if (out_callbacks) {
        //    num_upstream_ = out_callbacks->size();
        //    for (size_t i=0; i<out_callbacks->size(); i++) {
        //        out_callbacks_[i] = (*out_callbacks)[i];
        //    }
        //}
        //else {
        //    LOG_MSG("NULL out_callbacks_ in up_thread construct\n");
        //}
    }


template <class InT, class ROutT>
void UpThreadNet<InT, ROutT>::AddOutQueue(ZeroRingBuffer<WInT>* q) {
    in_callback_->AddOutQueue(q);
}


template <class InT, class ROutT>
void UpThreadNet<InT, ROutT>::AddSource(char* source) {
    //in_callback_->IncNumIn();
    listen_addr_ = source;
}

/*
template<class T>
void UpThreadNet<T>::set_queue(afs::ZeroRingBuffer_new* q) {
}
*/

template <class InT, class ROutT>
void UpThreadNet<InT, ROutT>::ThreadInitHandler() {

    int rc = 0;

    //  Parse addr_ string.
    std::string protocol;
    std::string address;
    rc = parse_uri (listen_addr_.c_str(), protocol, address);
    LOG_MSG("rc %d\n", rc);
    afs_assert(rc==0, "parse url error: %s\n", listen_addr_.c_str());

    rc = check_protocol (protocol);
    afs_assert(!rc, "check protocol error: %s\n", protocol.c_str());

    options_ = new (std::nothrow) afs_zmq::options_t;
    options_->type = ZMQ_PULL;
    afs_zmq::tcp_listener_t *listener =
        new (std::nothrow) afs_zmq::tcp_listener_t (
            in_callback_,
            out_callbacks_,
            *options_);
    alloc_assert (listener);
    rc = listener->set_address (address.c_str ());
    if (rc != 0) {
        delete listener;
        LOG_ERR("liseten error\n");
    }

    poller_ = new afs_zmq::epoll_t();
    afs_assert (poller_, "allocate poller_ error\n");
    listener->set_poller(poller_);
}

template <class InT, class ROutT>
void UpThreadNet<InT, ROutT>::ThreadMainHandler() {
    LOG_MSG("%s start to run\n", UpThread<InT, ROutT>::get_thread_str());
    //gettimeofday(&start_time, NULL);

    poller_->loop();

    /*
    if (support_feedback_) {
        for (int i=0; i<num_upstream_; i++) {
            out_callbacks_[i]->Flush();
        }
    }
    */
    //gettimeofday(&end_time, NULL);
}

template <class InT, class ROutT>
void UpThreadNet<InT, ROutT>::ThreadFinishHandler() {
    /*
    double used_time = ((end_time.tv_sec + end_time.tv_usec / 1000000.0) -
                (start_time.tv_sec + start_time.tv_usec / 1000000.0));
    LOG_MSG("\n=================================================\n"
            "Total packets: %" PRIu64 "\n"
            "run_time = %lf\n"
            "Average: %lf (msg/s)\n"
            "=================================================\n",
            event_,
            used_time,
            event_ / used_time);
    */

    in_callback_->Clean();
    if (support_feedback_
) {
        for (int i=0; i<num_upstream_; i++) {
            out_callbacks_[i]->Clean();
        }
    }
}

// Copied and modified from socket_base.cpp in 0MQ
template <class InT, class ROutT>
int UpThreadNet<InT, ROutT>::parse_uri (const char *uri_,
                        std::string &protocol_, std::string &address_)
{
    zmq_assert (uri_ != NULL);

    std::string uri (uri_);
    std::string::size_type pos = uri.find ("://");
    if (pos == std::string::npos) {
        errno = EINVAL;
        return -1;
    }
    protocol_ = uri.substr (0, pos);
    address_ = uri.substr (pos + 3);
    
    if (protocol_.empty () || address_.empty ()) {
        errno = EINVAL;
        return -1;
    }
    
    return 0;
}

// Copied and modified from socket_base.cpp in 0MQ
template <class InT, class ROutT>
int UpThreadNet<InT, ROutT>::check_protocol (const std::string &protocol_)
{
    //  First check out whether the protcol is something we are aware of.
    //  qhuang: we only allow tcp protocol
    if (protocol_ != "tcp") {
        errno = EPROTONOSUPPORT;
        return -1;
    }

    //  Protocol is available.
    return 0;
}

} // namespace

#endif // INCLUDE
