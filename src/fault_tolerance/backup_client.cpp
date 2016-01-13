#include <sys/stat.h>
#include <zmq.h>
#include "config.hpp"
#include "util.hpp"
#include "backup_client.hpp"

namespace afs {

BackupClient::BackupClient(std::string worker_name, uint16_t id, std::string backup_server) : worker_name_(worker_name), worker_id_(id) {

    zmq_ctx_ = zmq_ctx_new();
    if (zmq_ctx_ == 0)
        LOG_ERR("zmq_ctx_new(): %s\n", zmq_strerror(errno));
    if (zmq_ctx_set(zmq_ctx_, ZMQ_IO_THREADS, 1) == -1)
        LOG_ERR("zmq_ctx_set(): %s\n", zmq_strerror(errno));

    socket_ = zmq_socket(zmq_ctx_, ZMQ_REQ);
    if (zmq_connect(socket_, backup_server.c_str()) == -1)
        LOG_ERR("zmq_connect(): %s\n", zmq_strerror(errno));
}

BackupClient::~BackupClient() {
    if (zmq_close(socket_) == -1)
        LOG_ERR("zmq_close(): %s\n", zmq_strerror(errno));

    //LOG_MSG("number of data bakcup %lu\n", data_cnt);
    if (zmq_ctx_destroy(zmq_ctx_) == -1)
        LOG_ERR("zmq_ctx_destroy(): %s\n", zmq_strerror(errno));
}

void BackupClient::MakeBackup(BackupItem& backup_item) {
    /*
                unsigned char* start = (unsigned char*)&backup_item;
                uint32_t wid = *(uint32_t*)start;
                uint32_t tid = *(uint32_t*)(start+4);
                uint32_t op = *(uint32_t*)(start+8);
                uint64_t seq = *(uint64_t*)(start+12);
                uint32_t len = *(uint32_t*)(start+20);
                LOG_MSG("receive backup request from worker %u, thread %u, op %u, seq %lu, len %u:\n    ", wid, tid, op, seq, len);
                for (unsigned int i=0; i<len; i++) {
                    LOG_MSG("%02x ", *(start+24+i));
                }
                LOG_MSG("\n");
    */
    char tmp;
    if (zmq_send(socket_, &backup_item, backup_item.size(), 0) == -1) {
        LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
    }
    if (zmq_recv(socket_, &tmp, 1, 0) == -1) {
        LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
    }
}

RecoveryItem* BackupClient::RetriveBackups() {

    BackupItem backup_item;
    backup_item.info.backup_op = 1;
    backup_item.info.worker_id = worker_id_;
    backup_item.data.meta.len = 0;
    if (zmq_send(socket_, &backup_item, backup_item.size(), 0) == -1) {
        LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
    }

    uint32_t count = 0;
    if (zmq_recv(socket_, &count, sizeof(uint32_t), 0) == -1) {
        LOG_ERR("ZeroQueue.read(): %s\n", zmq_strerror(errno));
    }
    LOG_MSG("Backup client count %u\n", count);

    backup_item.info.backup_op = 2;
    if (zmq_send(socket_, &backup_item, backup_item.size(), 0) == -1) {
        LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
    }

    RecoveryItem* recovery_item = new RecoveryItem(count);
    if (zmq_recv(socket_, recovery_item->data(), recovery_item->size(), 0) == -1) {
        LOG_ERR("ZeroQueue.read(): %s\n", zmq_strerror(errno));
    }

    return recovery_item;
    /*
    if (zmq_recv(socket_, &tmp, 1, 0) == -1) {
        LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
    }
    */
}

void BackupClient::EndRecovery(RecoveryItem* recovery_item) {
    delete recovery_item;
}

} // namespace afs
