#ifndef __MPSC_OBJECT_HPP_INCLUDED__
#define __MPSC_OBJECT_HPP_INCLUDED__

#include "zerocopy_ringbuffer_multi_writer.hpp"
#include "util.hpp"

namespace afs {

template<class T>
class MPSCObject {

public:
    MPSCObject<T>(int num_peers) {
        peers = (ZeroRingBufferMulWriter<T*>**) calloc(num_peers, sizeof(ZeroRingBufferMulWriter<T*>*));
        afs_assert(peers, "Peers queue array allocate failure\n");

        own = new ZeroRingBufferMulWriter<T*>(num_peers);
        afs_assert(own, "Full queue allocate failure\n");
    }

    ~MPSCObject() {
        delete peers;
        free(own);
    }

    ZeroRingBufferMulWriter<T*>* GetOwnQueue() {
        return own;
    }

    void ConnectPeer(int index, MPSCObject<T>* peer) {
        peers[index] = peer->GetOwnQueue();
    }

protected:
    ZeroRingBufferMulWriter<T*>* own;
    ZeroRingBufferMulWriter<T*>** peers;
};

}

#endif
