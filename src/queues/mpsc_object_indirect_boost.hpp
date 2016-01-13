#ifndef __MPSC_OBJECT_HPP_INCLUDED__
#define __MPSC_OBJECT_HPP_INCLUDED__

#include "mpmc_queue_boost.hpp"
#include "util.hpp"

template<class T>
class MPSCObject {

public:
    MPSCObject<T>(int num_peers) {
        peers = (LockFreeQueue<T*>**) calloc(num_peers, sizeof(LockFreeQueue<T*>*));
        afs_assert(peers, "Peers queue array allocate failure\n");

        own = new LockFreeQueue<T*>(num_peers, 1);
        afs_assert(own, "Full queue allocate failure\n");
    }

    ~MPSCObject() {
        delete peers;
        free(own);
    }

    LockFreeQueue<T*>* GetOwnQueue() {
        return own;
    }

    void ConnectPeer(int index, MPSCObject<T>* peer) {
        peers[index] = peer->GetOwnQueue();
    }

protected:
    LockFreeQueue<T*>* own;
    LockFreeQueue<T*>** peers;
};

#endif
