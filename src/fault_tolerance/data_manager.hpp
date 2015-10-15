#ifndef ROLL_worker_HPP_
#define ROLL_worker_HPP_

#include <stddef.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <zmq.h>

#include "config.hpp"
#include "operator/stateful_operator.hpp"
#include "recovery_state.hpp"
#include "adapter_recovery.hpp"

namespace afs {


// Existing implementation of DataManager only supports single Analyzer
// If multiple Analyzers are executed, each of them will create its own DataManager
// We need to modify SAND to multi-thread instead of multi-process implementation

class DataManager {

private:

    static DataManager* instance; // global static pointer to the unique DataManager instance

    std::string worker_name;  // id of the worker, the same as worker id

    FILE* data_file;
    FILE* seq_file;
    unsigned char data_buffer[sizeof(uint32_t) + MAX_RECORD_LENGTH];
    void* socket; // local files for state persistency, each for one registering operators

    // retrieve states in other servers for recovering the operator states
    // called when DataManager is created
    // @return: the status
    int RetrieveBackupState(); 

    uint64_t data_cnt;
    uint64_t last_seq;

    AdapterRecovery* adapter;

    void* zmq_ctx;
    std::string backup_addr;

public:

    DataManager(std::string name);
    ~DataManager();

    static void InitDataManagerContext(std::string name) {
        instance = new DataManager(name);
    }

    static void CleanDataManagerContext() {
        delete instance;
    }

    static DataManager* GetInstance() {
        if (instance == NULL) {
            // TODO: warning
            instance = new DataManager("");
        }
        return instance;
    }

    int SaveData(uint64_t seq, void* data, uint32_t length);
    AdapterRecovery* RequestRecoveryData();
    void ClearRecoveryData();

    std::string get_worker_name() {
        return worker_name;
    }
};

} // namespace afs

#endif // ROLL_worker_HPP_
