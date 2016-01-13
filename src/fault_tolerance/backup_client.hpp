#ifndef __AFS_BACKUP_CLIENT_HPP_INCLUDED__
#define __AFS_BACKUP_CLIENT_HPP_INCLUDED__

#include <stddef.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <zmq.h>
#include "backup_item.hpp"
#include "recovery_item.hpp"

//#include "operator/stateful_operator.hpp"
//#include "recovery_state.hpp"
//#include "adapter_recovery.hpp"

namespace afs {


// Existing implementation of BackupClient only supports single Analyzer
// If multiple Analyzers are executed, each of them will create its own BackupClient
// We need to modify SAND to multi-thread instead of multi-process implementation

class BackupClient {

private:
    void* zmq_ctx_;
    void* socket_;

    std::string backup_server_;
    std::string worker_name_;
    uint16_t worker_id_;

public:

    BackupClient(std::string name, uint16_t id, std::string backup_server);
    ~BackupClient();

    void MakeBackup(BackupItem& backup_item);
    RecoveryItem* RetriveBackups();
    void EndRecovery(RecoveryItem* recovery_item);
};

} // namespace afs

#endif
