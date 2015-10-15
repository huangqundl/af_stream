#ifndef __ROLL_INTHREAD_NET_HPP_INCLUDED__
#define __ROLL_INTHREAD_NET_HPP_INCLUDED__

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>

#include <string>

#include "util.hpp"
#include "in_thread.hpp"

#include "net/epoll.hpp"
#include "net/bench/tcp_listener_bench.hpp"
#include "net/options.hpp"
#include "in_callback_base.hpp"
#include "out_callback_base.hpp"


namespace afs {

template <class InT, class ROutT=NullClass>
class InThreadNetBench : public InThread<InT, ROutT> {

typedef WrapItem<InT> WInT;
typedef WrapItem<ROutT> WROutT;

public:

    InThreadNetBench(InCallbackBase* in_callback,
            std::vector<OutCallbackBase*>* out_callbacks);

    void AddSource(char* source);

    void AddOutQueue(ZeroRingBuffer<WInT>* q);

    MPSCReader<WROutT>* GetReverseOutBufferReader(int i) {
        return (MPSCReader<WROutT>*)out_callbacks_[i]->GetReader();
    }

    int GetNumUpstream() {
        return num_upstream_;
    }

    bool IsReverse() {
        return is_reverse_;
    }

private:

    int num_upstream_;
    bool is_reverse_;

    // listen on single port
    //int listen_port;
    std::string listen_addr_;

    // epoll
    zmq::epoll_t* poller;

    zmq::options_t* options;

    // route incoming events to analyzers
    InCallbackBase* in_callback_;
    OutCallbackBase** out_callbacks_;

    //  monitor number of process events
    uint64_t _event;

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
InThreadNetBench<InT, ROutT>::InThreadNetBench(InCallbackBase* in_callback,
            std::vector<OutCallbackBase*>* out_callbacks) :
    InThread<InT, ROutT>(),
    num_upstream_(0), is_reverse_(false),
    poller(NULL), options(NULL), 
    in_callback_(in_callback),
    out_callbacks_(NULL),
    _event(0) {

    LOG_MSG("Create InThread Benchmark\n");

        if (out_callbacks) {
            num_upstream_ = out_callbacks->size();
            is_reverse_ = true;
            out_callbacks_ = (OutCallbackBase**)calloc(out_callbacks->size(), sizeof(OutCallbackBase*));
            for (size_t i=0; i<out_callbacks->size(); i++) {
                out_callbacks_[i] = (*out_callbacks)[i];
            }
        }
        else {
            LOG_MSG("NULL out_callbacks_ in in_thread construct\n");
        }
    }

template <class InT, class ROutT>
void InThreadNetBench<InT, ROutT>::AddOutQueue(ZeroRingBuffer<WInT>* q) {
    in_callback_->AddOutQueue(q);
}

template <class InT, class ROutT>
void InThreadNetBench<InT, ROutT>::AddSource(char* source) {
    listen_addr_ = source;
}

/*
template<class T>
void InThreadNetBench<T>::set_queue(afs::ZeroRingBuffer_new* q) {
}
*/

template <class InT, class ROutT>
void InThreadNetBench<InT, ROutT>::ThreadInitHandler() {

    int rc = 0;

    //  Parse addr_ string.
    std::string protocol;
    std::string address;
    rc = parse_uri (listen_addr_.c_str(), protocol, address);
    LOG_MSG("rc %d\n", rc);
    afs_assert(rc==0, "parse url error: %s\n", listen_addr_.c_str());

    rc = check_protocol (protocol);
    afs_assert(!rc, "check protocol error: %s\n", protocol.c_str());

    options = new (std::nothrow) zmq::options_t;
    options->type = ZMQ_PULL;
    zmq::tcp_listener_bench_t *listener =
        new (std::nothrow) zmq::tcp_listener_bench_t (
            in_callback_,
            out_callbacks_,
            *options);
    alloc_assert (listener);
    rc = listener->set_address (address.c_str ());
    if (rc != 0) {
        delete listener;
        LOG_ERR("liseten error\n");
    }

    poller = new zmq::epoll_t();
    afs_assert (poller, "allocate poller error\n");
    listener->set_poller(poller);
}

template <class InT, class ROutT>
void InThreadNetBench<InT, ROutT>::ThreadMainHandler() {
    LOG_MSG("%s start to run\n", InThread<InT, ROutT>::get_thread_str());
    //gettimeofday(&start_time, NULL);

    poller->loop();

    /*
    if (is_reverse_) {
        for (int i=0; i<num_upstream_; i++) {
            out_callbacks_[i]->Flush();
        }
    }
    */
    //gettimeofday(&end_time, NULL);
}

template <class InT, class ROutT>
void InThreadNetBench<InT, ROutT>::ThreadFinishHandler() {
    /*
    double used_time = ((end_time.tv_sec + end_time.tv_usec / 1000000.0) -
                (start_time.tv_sec + start_time.tv_usec / 1000000.0));
    LOG_MSG("\n=================================================\n"
            "Total packets: %" PRIu64 "\n"
            "run_time = %lf\n"
            "Average: %lf (msg/s)\n"
            "=================================================\n",
            _event,
            used_time,
            _event / used_time);
    */

    in_callback_->Clean();
    if (is_reverse_) {
        for (int i=0; i<num_upstream_; i++) {
            out_callbacks_[i]->Clean();
        }
    }
}

// Copied and modified from socket_base.cpp in 0MQ
template <class InT, class ROutT>
int InThreadNetBench<InT, ROutT>::parse_uri (const char *uri_,
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
int InThreadNetBench<InT, ROutT>::check_protocol (const std::string &protocol_)
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
