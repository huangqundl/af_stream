#include <zmq.h>
#include "util.hpp"
#include "state_manager.hpp"
#include "config.hpp"

namespace afs {

StateManager* StateManager::instance = NULL;

StateManager::StateManager(std::string name) : daemon_name(name) {}

StateManager::~StateManager() {
    LOG_MSG("number of state bakcup %lu\n", state_cnt);
    printf("number of state bakcup %lu\n", state_cnt);

    for (size_t i=0; i<files.size(); i++) {
        fclose(files[i]);
    }
    for (size_t i=0; i<sockets.size(); i++) {
        zmq_close(sockets[i]);
    }
}

// Currently, it is not thread safe
// So the stream processing system should guarantee
// to register operators deterministically
uint32_t StateManager::RegisterOperator(OPERATOR_TYPE t) {
    uint32_t ret = 0;
    if (t == STATEFUL) {
        ret = files.size();
        char filename[100];
        Config* config = Config::getInstance();
        char* prefix = config->getstring("sys.fault_tolerance.state_backup_prefix", NULL);
        if (prefix==NULL) {
            LOG_ERR("State backup path is not set\n");
        }
        sprintf(filename, "%sbak_state_%s_%u", prefix, daemon_name.c_str(), ret);
        FILE* tmpf = fopen(filename,"w");
        files.push_back(tmpf);
        if (tmpf == NULL) {
            // sand warning
        }

        void* socket = zmq_socket(zmq_ctx, ZMQ_PUSH);
        if (zmq_bind(socket, backup_addr.c_str()) == -1)
            LOG_ERR("zmq_bind(): %s\n", zmq_strerror(errno));
        sockets.push_back(socket);
        //LOG_MSG("    create state backup file %s\n", filename);
    }
    else if (t == STATELESS) {
        ret = stateless_id.size();
        stateless_id.push_back(ret);
    }
    else if (t == COMPOUND) {
    
    }
    else {
        // error
    }
    return ret;
}

int StateManager::SaveState(size_t component_id, size_t key, long long value) {

    state_cnt++;
    char tmp[50];
    sprintf(tmp, "%lu %lld", key, value);

    FILE* out_file = files[component_id];
    fprintf(out_file, "%s\n", tmp);

    void* socket = sockets[component_id];
    if (zmq_send(socket, tmp, sizeof(tmp), 0) != -1) {
        return 0;
    }
    else {
        return -1;
    }
}

int StateManager::SaveState(size_t component_id, std::string key, long long value) {
    state_cnt++;

    FILE* out_file = files[component_id];
    fprintf(out_file, "%s %lld\n", key.c_str(), value);
    void* socket = sockets[component_id];

    if (zmq_send(socket, key.c_str(), key.size(), 0) != -1) {
    }
    else {
        return -1;
    }
    char tmp[50];
    sprintf(tmp, "%ld", value);
    if (zmq_send(socket, tmp, sizeof(tmp), 0) != -1) {
    }
    else {
        return -1;
    }
}

int StateManager::SaveState(size_t component_id, size_t key, double value) {
    state_cnt++;
    char tmp[50];
    sprintf(tmp, "%lu %lf", key, value);

    FILE* out_file = files[component_id];
    fprintf(out_file, "%s\n", tmp);

    void* socket = sockets[component_id];
    if (zmq_send(socket, tmp, sizeof(tmp), 0) != -1) {
        return 0;
    }
    else {
        return -1;
    }
}

int StateManager::SaveState(size_t component_id, std::string key, double value) {
    state_cnt++;

    FILE* out_file = files[component_id];
    fprintf(out_file, "%s %lf\n", key.c_str(), value);

    void* socket = sockets[component_id];
    if (zmq_send(socket, key.c_str(), key.size(), 0) != -1) {
    }
    else {
        return -1;
    }
    char tmp[50];
    sprintf(tmp, "%lf", value);
    if (zmq_send(socket, tmp, sizeof(tmp), 0) != -1) {
    }
    else {
        return -1;
    }
}

void StateManager::RecordOperatorForCompound(uint32_t compound_id, uint32_t op_id) {}

// Currently, we load states from local file
// it is not thread safe
// So the stream processing system should guarantee the deterministics
RecoveryState* StateManager::RequestRecoveryState(uint32_t component_id) {
    char filename[50];

    RecoveryState* ret = new RecoveryState();
    sprintf(filename, "backup/%s_%u", daemon_name.c_str(), component_id);
    ret->LoadFromFile(filename);
    recovery_states[component_id] = ret;
    return ret;
}

void StateManager::FreeRecoveryState(uint32_t component_id) {
    delete recovery_states[component_id];
}

} // namespace afs
