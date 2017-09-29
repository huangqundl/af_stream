#ifndef __MPSC_OBJECT_HPP_INCLUDED__
#define __MPSC_OBJECT_HPP_INCLUDED__

#include <vector>

#include "mpsc_queue_mesh_zerocopy.hpp"
#include "../util.hpp"

template<class T>
class MPSCObject {

public:
    MPSCObject<T>() {
        //peers = (LockFreeQueue<T*>**)calloc(num_peers, sizeof(LockFreeQueue<T*>*));
        //afs_assert(peers, "Peers queue array allocate failure\n");

        own = new LockFreeQueue<T*>();
        afs_assert(own, "Full queue allocate failure\n");
    }

    ~MPSCObject() {
        //delete peers;
        free(own);
    }

    LockFreeQueue<T*>* GetOwnQueue() {
        return own;
    }

    void ConnectPeer(int index, MPSCObject<T>* peer) {
        own->AddQueue();
        peers.push_back(peer->GetOwnQueue());
    }

protected:
    LockFreeQueue<T*>* own;
    //LockFreeQueue<T*>** peers;
    std::vector<LockFreeQueue<T*>*> peers;
};

#endif
