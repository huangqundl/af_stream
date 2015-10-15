#include <sys/stat.h>
#include "util.hpp"
#include "data_manager.hpp"

namespace afs {

DataManager* DataManager::instance = NULL;

DataManager::DataManager(std::string name) : worker_name(name) {
    char filename[100];
    char seq_filename[100];

    Config* config = Config::getInstance();
    char* prefix = config->getstring("sys.fault_tolerance.data_backup_prefix", NULL);
    if (prefix==NULL) {
        LOG_ERR("Data backup path is not set\n");
        sprintf(filename, "bak_data_%s", worker_name.c_str());
        sprintf(seq_filename, "bak_seq_%s", worker_name.c_str());
    }
    else {
        sprintf(filename, "%sdata_%s", prefix, worker_name.c_str());
        sprintf(seq_filename, "%sseq_%s", prefix, worker_name.c_str());
    }

    data_file = fopen(filename,"wb");
    seq_file = fopen(seq_filename,"wb");

    socket = zmq_socket(zmq_ctx, ZMQ_PUSH);
    if (zmq_bind(socket, backup_addr.c_str()) == -1)
        LOG_ERR("zmq_bind(): %s\n", zmq_strerror(errno));
}

DataManager::~DataManager() {
    LOG_MSG("number of data bakcup %lu\n", data_cnt);
    printf("number of data bakcup %lu\n", data_cnt);
    fclose(data_file);
    fclose(seq_file);
    zmq_close(socket);
}

int DataManager::SaveData(uint64_t seq, void* data, uint32_t length) {
    data_cnt++;

    if (seq>1 && seq-last_seq>1) {
        //fclose(file);
        //file = fopen(filename,"wb");
    }

    memcpy(data_buffer, &length, sizeof(uint32_t));
    memcpy(data_buffer+sizeof(uint32_t), data, length);
    fwrite (data_buffer, sizeof(char), sizeof(uint32_t)+length, data_file);
    fwrite (&seq, sizeof(uint64_t), 1, seq_file);
    last_seq = seq;

    char tmp[100];
    memcpy(tmp, &seq, sizeof(uint64_t));
    memcpy(tmp+sizeof(uint64_t), &length, sizeof(uint32_t));
    memcpy(data_buffer+sizeof(uint64_t)+sizeof(uint32_t), data, length);
    if (zmq_send(socket, tmp, sizeof(uint64_t)+sizeof(uint32_t)+length, 0) != -1) {
        return 0;
    }
    else {
        return -1;
    }
}

// Currently, we load states from local file
// it is not thread safe
// So the stream processing system should guarantee the deterministics
//
AdapterRecovery* DataManager::RequestRecoveryData() {
    //result = fread (buffer, 1,lSize<Plug>PeepOpenFile);
    Config* config = Config::getInstance();
    char* prefix = config->getstring("sys.fault_tolerance.data_backup_prefix", NULL);
    char filename[100];
    char seq_filename[100];
    sprintf(filename, "%sdata_%s", prefix, worker_name.c_str());
    sprintf(seq_filename, "%sseq_%s", prefix, worker_name.c_str());

    FILE* data_file = fopen(filename, "rb");
    if (data_file == NULL) {
        LOG_MSG("No data backup file: %s\n", filename);
        return NULL;
    }
    FILE* seq_file = fopen(seq_filename, "rb");
    if (seq_file == NULL) {
        LOG_MSG("No seq backup file: %s\n", seq_filename);
        return NULL;
    }

    adapter = new AdapterRecovery(data_file, seq_file);
    adapter->Init();
    return adapter;
}

void DataManager::ClearRecoveryData() {
    delete adapter;
}

} // namespace afs
