#ifndef ROLL_DAEMON_HPP_
#define ROLL_DAEMON_HPP_

#include <stddef.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "operator/stateful_operator.hpp"
#include "recovery_state.hpp"

namespace afs {

enum OPERATOR_TYPE {STATEFUL, STATELESS, COMPOUND};

class StatefulOperator;

// Existing implementation of StateManager only supports single Analyzer
// If multiple Analyzers are executed, each of them will create its own StateManager
// We need to modify SAND to multi-thread instead of multi-process implementation

class StateManager {

private:

    static StateManager* instance; // global static pointer to the unique StateManager instance

    std::string daemon_name;  // id of the daemon, the same as worker id

    std::vector<FILE*> files; // local files for state persistency, each for one registering operators
    std::vector<void*> sockets; // local files for state persistency, each for one registering operators
    std::vector<uint32_t> stateless_id;
    std::vector<uint32_t> stateful_id;

    std::unordered_map<uint32_t, RecoveryState*> recovery_states;

    // retrieve states in other servers for recovering the operator states
    // called when StateManager is created
    // @return: the status
    int RetrieveBackupState(); 

    uint64_t state_cnt;

    void* zmq_ctx;
    std::string backup_addr;

public:

    StateManager(std::string name);
    ~StateManager();

    static void InitStateManagerContext(std::string name) {
        instance = new StateManager(name);
    }

    static void CleanStateManagerContext() {
        delete instance;
    }

    static StateManager* GetInstance() {
        if (instance == NULL) {
            // TODO: warning
            instance = new StateManager("");
        }
        return instance;
    }

    // an operator call this function to request a component id in the scope of this daemon
    // the return value is the allocated id for the component
    uint32_t RegisterOperator(OPERATOR_TYPE t);

    int SaveState(size_t component_id, size_t key, long long value);
    int SaveState(size_t component_id, std::string key, long long value);
    int SaveState(size_t component_id, size_t key, double value);
    int SaveState(size_t component_id, std::string key, double value);

    int RestoreState(StatefulOperator* op);

    RecoveryState* RequestRecoveryState(uint32_t component_id);
    void FreeRecoveryState(uint32_t component_id);

    void RecordOperatorForCompound(uint32_t compound_id, uint32_t op_id); 

    std::string get_daemon_name() {
        return daemon_name;
    }
};

} // namespace afs

#endif // ROLL_DAEMON_HPP_
