#include <zmq.h>
#include "util.hpp"
#include "config.hpp"
#include "thread/thread_id.hpp"

#include "operator_tracker.hpp"

namespace afs {

OperatorTracker* OperatorTracker::instance = NULL;

OperatorTracker::OperatorTracker(std::string name, uint16_t worker_id) : daemon_name(name), worker_id_(worker_id), seed_(0) {
    Config* config = Config::getInstance();
    num_analyzer_ = config->getint("num_compute_threads", 0);
    operators_ = (std::vector<FTInterface*>**)calloc(num_analyzer_, sizeof(std::vector<FTInterface*>**));
    for (auto i=0; i<num_analyzer_; i++) {
        operators_[i] = new std::vector<FTInterface*>;
    }
}

OperatorTracker::~OperatorTracker() {
    for (auto i=0; i<num_analyzer_; i++) {
        delete operators_[i];
    }
    free(operators_);
}

uint32_t OperatorTracker::RegisterFTOperator(FTInterface* op) {
    uint32_t ret = 0;
    //LOG_MSG("Register operator for ComputeThread %d\n", thr_id());
    operators_[thr_id()]->push_back(op);
    return ret;
}

std::vector<FTInterface*>* OperatorTracker::GetThreadOps(int thr_id) {
    return operators_[thr_id];
}

void OperatorTracker::RecoverOps(RecoveryItem* recovery_item) {
    uint32_t count = recovery_item->GetCount();
    LOG_MSG("count %u\n", count);
    for (uint32_t i=0; i<count; i++) {
        BackupItem* bak_item = recovery_item->GetSlot(i);
        if (bak_item->info.worker_id != worker_id_) {
            LOG_ERR("worker id unmatch: %u (recovery) %u (local)\n", bak_item->info.worker_id, worker_id_);
        }
        LOG_MSG("thread %u, op_index %u\n", bak_item->info.thread_id, bak_item->info.op_index);
        operators_[bak_item->info.thread_id]->at(bak_item->info.op_index)->RecoveryState(bak_item->data);
    }
}

/*
RecoveryState* OperatorTracker::RequestRecoveryState(uint32_t component_id) {
    char filename[50];

    RecoveryState* ret = new RecoveryState();
    sprintf(filename, "backup/%s_%u", daemon_name.c_str(), component_id);
    ret->LoadFromFile(filename);
    recovery_states[component_id] = ret;
    return ret;
}

void OperatorTracker::FreeRecoveryState(uint32_t component_id) {
    delete recovery_states[component_id];
}
*/

} // namespace afs
