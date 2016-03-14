#ifndef __AFS_OUTTHREAD_NET_HPP_INCLUDED__
#define __AFS_OUTTHREAD_NET_HPP_INCLUDED__

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>

#include <vector>
#include <string>

#include "util.hpp"
#include "down_thread.hpp"

#include "in_callback_base.hpp"
#include "out_callback_base.hpp"
#include "in_callback_simple.hpp"
#include "out_callback_simple.hpp"

#include "net/epoll.hpp"
#include "net/options.hpp"

#include "net/address.hpp"
#include "net/tcp_connecter.hpp"

namespace afs {

/// A specific DownThread, whose destinations are next-hop workers
/// each reader resides in one out_callback

template <class OutT, class RInT=NullClass>
class DownThreadNet : public DownThread<OutT, RInT> {

typedef WrapItem<OutT> WOutT;
typedef WrapItem<RInT> WRInT;

public:

    DownThreadNet(uint64_t num_compute_thread
            //InCallbackBase* in_callback,
            //std::vector<OutCallbackBase*> &out_callbacks
            );

    ~DownThreadNet() {
        delete poller_;
        delete options_;
        //free(out_callbacks_);
    }

    //  each destination is handled by a stream_engine
    //  each stream_engine maintains one in_queue
    void AddDest(int dest_index, char* dest);

    MPSCReader<WOutT>* GetOutBufferReader(int i) {
        return (MPSCReader<WOutT>*)out_callbacks_[i]->GetReader();
    }

    int GetDownstream() {
        return num_downstream_;
    }

    bool IsReverse() {
        return support_feedback_;
    }

    void AddReverseInBuffer(ZeroRingBuffer<WRInT>* q) {
        in_callback_->AddOutQueue(q);
    }

private:

    bool support_feedback_;

    size_t num_compute_thread_, num_downstream_;

    InCallbackBase* in_callback_;
    //OutCallbackBase** out_callbacks_;

    // listen on single port
    //int listen_port;
    std::vector<std::string> connect_addrs_;
    std::vector<OutCallbackBase*> out_callbacks_;

    // epoll
    afs_zmq::epoll_t* poller_;

    afs_zmq::options_t* options_;

    //  monitor number of process events
    uint64_t event_;

    // copied from socket_base.hpp in 0MQ
    int parse_uri (const char *uri_,
            std::string &protocol_,
            std::string &address_);

    void Connect(int i, const char* addr);

    // copied from socket_base.hpp in 0MQ
    int check_protocol (const std::string &protocol_);

    // derived from ThreadBase
    void ThreadInitHandler();
    void ThreadFinishHandler();
    void ThreadMainHandler();
};

template <class OutT, class RInT>
DownThreadNet<OutT, RInT>::DownThreadNet(uint64_t num_compute_thread
        //,uint64_t num_downstream
        //InCallbackBase* in_callback,
        //std::vector<OutCallbackBase*> &callbacks
        ) :
    DownThread<OutT, RInT>(),
    num_compute_thread_(num_compute_thread),
    num_downstream_(0),
    //in_callback_(in_callback),
    //out_callbacks_(NULL),
    poller_(NULL), options_(NULL), event_(0) {
    
    //afs_assert(num_outdownstreamallbacks.size(), "num_outdownstreamu num of callbacks %lu\n",
    //        num_outdownstreamallbacks.size());

    support_feedback_ = true;
    if (support_feedback_) {
        in_callback_ = new afs::InCallbackSimple<RInT>();
    }

    //out_callbacks_ = (OutCallbackBase**)calloc(num_outdownstreamizeof(OutCallbackBase*));

    options_ = new (std::nothrow) afs_zmq::options_t;
    options_->type = ZMQ_PUSH;

    poller_ = new afs_zmq::epoll_t();
    afs_assert (poller_, "allocate poller_ error\n");
}

template <class OutT, class RInT>
void DownThreadNet<OutT, RInT>::AddDest(int dest_index, char* dest) {
    
    num_downstream_++;

    in_callback_->IncNumIn();
    out_callbacks_.push_back(new afs::OutCallbackSimple<OutT>(num_compute_thread_));
    connect_addrs_.push_back(dest);
}

template <class OutT, class RInT>
void DownThreadNet<OutT, RInT>::Connect(int i, const char* addr_) {
    int rc = 0;

    //  Parse addr_ string.
    std::string protocol;
    std::string address;

    rc = parse_uri (addr_, protocol, address);
    afs_assert(rc==0, "parse url error: %s\n", addr_);

    rc = check_protocol (protocol);
    afs_assert(rc==0, "check protocol error: %s\n", protocol.c_str());

    afs_zmq::address_t *paddr = new (std::nothrow) afs_zmq::address_t (protocol, address);
    paddr->resolved.tcp_addr = new (std::nothrow) afs_zmq::tcp_address_t ();
    alloc_assert (paddr->resolved.tcp_addr);

    rc = paddr->resolved.tcp_addr->resolve (
            address.c_str (), false, options_->ipv6);
    afs_assert(rc==0, "tcp resolve\n");

    afs_zmq::tcp_connecter_t *connecter = 
        new (std::nothrow) afs_zmq::tcp_connecter_t (
            DownThread<OutT, RInT>::get_wid(),
            in_callback_, out_callbacks_[i],
            *options_, paddr, false);
    alloc_assert (connecter);

    connecter->set_poller(poller_);
    connecter->start_connecting();
}

template <class OutT, class RInT>
void DownThreadNet<OutT, RInT>::ThreadInitHandler() {

    afs_assert(num_downstream_== connect_addrs_.size(),
            "num_outdownstream %lu, connect_addr_ %lu\n", num_downstream_, connect_addrs_.size());
    for (size_t i=0; i<num_downstream_; i++) {
        Connect(i, connect_addrs_[i].c_str());
    }

    LOG_MSG("connect ok\n");
}

template <class OutT, class RInT>
void DownThreadNet<OutT, RInT>::ThreadMainHandler() {
    LOG_MSG("DownThreadNet (%d) start to run\n", getpid());

    poller_->loop();

    /*
    for (size_t i=0; i<num_downstream_; i++) {
        out_callbacks_[i]->Flush();
    }
    */
}

template <class OutT, class RInT>
void DownThreadNet<OutT, RInT>::ThreadFinishHandler() {
    if (in_callback_) {
        in_callback_->Clean();
    }
    for (size_t i=0; i<num_downstream_; i++) {
        out_callbacks_[i]->Clean();
    }
}

// Copied and modified from socket_base.cpp in 0MQ
template <class OutT, class RInT>
int DownThreadNet<OutT, RInT>::parse_uri (const char *uri_,
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
template <class OutT, class RInT>
int DownThreadNet<OutT, RInT>::check_protocol (const std::string &protocol_)
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
