#ifndef __AFS_STATE_MANAGER_HPP_INCLUDED__
#define __AFS_STATE_MANAGER_HPP_INCLUDED__

#include <stddef.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "../util.hpp"
#include "../operator/ft_interface.hpp"
#include "recovery_item.hpp"

namespace afs {

// Currently, OperatorTracker is not thread safe
// It requires all ft-operators are created in ThreadInitHandler
// By this, AFS guarantees to register operators deterministically

class OperatorTracker {

private:

    static OperatorTracker* instance; // global static pointer to the unique OperatorTracker instance
    std::string daemon_name;  // id of the daemon, the same as worker id
    uint32_t worker_id_;

    // for state backups: track operators of each analyzer
    int num_analyzer_;
    std::vector<FTInterface*> **operators_;

    /*
    std::vector<FILE*> files; // local files for state persistency, each for one registering operators
    std::vector<void*> sockets; // local files for state persistency, each for one registering operators
    */

    //std::unordered_map<uint32_t, RecoveryState*> recovery_states;

    // retrieve states in other servers for recovering the operator states
    // called when OperatorTracker is created
    // @return: the status
    int RetrieveBackupState(); 

    uint32_t seed_;

    void* zmq_ctx;
    std::string backup_addr;

public:

    OperatorTracker(std::string name, uint16_t worker_id);
    ~OperatorTracker();

    static void InitOperatorTracker(std::string name, uint16_t worker_id) {
        instance = new OperatorTracker(name, worker_id);
    }

    static void CleanOperatorTracker() {
        delete instance;
    }

    static OperatorTracker* GetInstance() {
        if (instance == NULL) {
            LOG_ERR("NULL OperatorTracker instance\n");
        }
        return instance;
    }

    //  an operator call this function to register itself, the return value is a unique id for the operator
    uint32_t RegisterFTOperator(FTInterface* op);

    std::vector<FTInterface*>* GetThreadOps(int thr_id);

    void RecoverOps(RecoveryItem* recovery_item);

    //RecoveryState* RequestRecoveryState(uint32_t component_id);
    //void FreeRecoveryState(uint32_t component_id);

    std::string get_daemon_name() {
        return daemon_name;
    }

    uint64_t GetSeed() {
        //return ((uint64_t)worker_id_ << 32) + seed_++;
        return seed_++;
    }
};

} // namespace afs

#endif
