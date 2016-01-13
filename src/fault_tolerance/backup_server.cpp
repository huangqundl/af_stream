#include <sys/stat.h>
#include <zmq.h>
#include <unordered_map>
#include "util.hpp"
#include "backup_item.hpp"
#include "recovery_item.hpp"

afs::BackupItem backup_item;

void* z_ctx = NULL;
void* z_socket = NULL;

void ZRead(void* dst, int len) {
    if (zmq_recv(z_socket, dst, len, 0) == -1) {
        LOG_ERR("ZeroMQ Receive: %s\n", zmq_strerror(errno));
    }
}

void ZWrite(void* data, int len) {
    if (zmq_send(z_socket, data, len, 0) == -1) {
        LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
    }
}

inline uint64_t MAKE_OP_KEY(uint16_t worker, uint16_t thread, uint32_t op) {
    return (uint64_t)worker << 32 | (uint64_t)thread << 16 | op;
}

void CopyBackupItem(afs::BackupItem &dst, afs::BackupItem &src) {
    dst.info = src.info;
    dst.data.meta = src.data.meta;
    memcpy(dst.data.data, src.data.data, src.data.meta.len);
}

// for each (worker, thread, op) tuple
class BackupContent {
    std::unordered_map<uint64_t, uint32_t>* map;
    afs::BackupItem* slots;
    uint32_t used_slots, max_slots;

private:
    void Expand() {
        max_slots *= 2;
        slots = (afs::BackupItem*)realloc(slots, max_slots*sizeof(afs::BackupItem));
    }

    uint32_t SelectSlot(afs::BackupItem &item) {
        uint64_t bak_key = item.data.meta.key;
        auto it = map->find(bak_key);
        uint32_t ret = 0;
        if (it == map->end()) {
            ret = used_slots++;
            map->insert(std::make_pair(bak_key, ret));
            if (ret == max_slots) {
                Expand();
            }
        }
        else {
            ret = it->second; 
        }
        //LOG_MSG("key %lu, select slot %u, max slot %u\n", bak_key, ret, max_slots);
        return ret;
    }

public:
    BackupContent() {
        map = new std::unordered_map<uint64_t, uint32_t>;
        max_slots = 1;
        used_slots = 0;
        slots = (afs::BackupItem*) calloc(max_slots, sizeof(afs::BackupItem));
    }

    ~BackupContent() {
        free(slots);
        delete map;
    }

    void SaveBackup(afs::BackupItem &item) {
        uint32_t index = SelectSlot(item);
        CopyBackupItem(slots[index], item);
    }

    void AppendRecoveryItem(afs::RecoveryItem* ret, uint32_t cur_count) {
        //LOG_MSG("Start append\n");
        if (used_slots != map->size()) {
            LOG_WARN("Used slots %u, map size: %lu\n", used_slots, map->size());
        }

        //afs::RecoveryItem* ret = new afs::RecoveryItem(used_slots);
        for (uint32_t i=0; i<used_slots; i++) {
            CopyBackupItem(*(ret->GetSlot(cur_count+i)), slots[i]);
        }
        //return ret;
    }

    uint32_t GetCount() {
        return used_slots;
    }
};


std::unordered_map<uint64_t, struct BackupContent*> ops;
//std::unordered_map<uint16_t, struct BackupContent*> worker_ops;
std::unordered_map<uint16_t, std::vector<struct BackupContent*>> worker_ops_new;

int main (int argc, char *argv [])
{
    if (argc != 3) {
        printf ("usage: %s <address> <num_of_sender>\n", argv[0]);
        return 1;
    }

    z_ctx = zmq_ctx_new();
    if (z_ctx == 0)
        LOG_ERR("zmq_ctx_new(): %s\n", zmq_strerror(errno));
    if (zmq_ctx_set(z_ctx, ZMQ_IO_THREADS, 1) == -1)
        LOG_ERR("zmq_ctx_set(): %s\n", zmq_strerror(errno));

    z_socket = zmq_socket(z_ctx, ZMQ_REP);
    if (zmq_bind(z_socket, argv[1]) == -1)
        LOG_ERR("zmq_bind(): %s\n", zmq_strerror(errno));

    int num_sender = atoi(argv[2]);
    int cnt = 0;
    int cur_stop = 0;

    while (1) {
        ZRead(&backup_item, sizeof(afs::BackupItem));
        if (backup_item.info.backup_op == 0) {
            // save backup
            if (backup_item.data.meta.len == 0) {
                cur_stop++;
                ZWrite(NULL, 0);
                if (cur_stop == num_sender) {
                    break;
                }
            }
            else 
            {
                cnt++;
                uint64_t op_key = MAKE_OP_KEY(
                        backup_item.info.worker_id, 
                        backup_item.info.thread_id, 
                        backup_item.info.op_index); 

                struct BackupContent* content = ops[op_key];
                if (content == NULL) {
                    //LOG_MSG("Create new key for %lu\n", op_key);
                    content = new struct BackupContent;
                    ops[op_key] = content;
                    //if (worker_ops.find(backup_item.info.worker_id)==worker_ops.end()) {
                    //    LOG_MSG("    worker id: %u\n", backup_item.info.worker_id);
                    //    worker_ops[backup_item.info.worker_id] = content;
                    //}
                    worker_ops_new[backup_item.info.worker_id].push_back(content);
                }
                content->SaveBackup(backup_item);
                ZWrite(NULL, 0);
                
                //LOG_MSG("receive backup request from worker %u, thread %u, op %u, seq %lu, len %u, key %lu:\n    ",
                //        backup_item.info.worker_id, 
                //        backup_item.info.thread_id, 
                //        backup_item.info.op_index, 
                //        backup_item.info.seq, 
                //        backup_item.data.meta.len,
                //        backup_item.data.meta.key
                //        );
                //unsigned char* start = backup_item.data.data;
                //for (unsigned int i=0; i<backup_item.data.meta.len; i++) {
                //    LOG_MSG("%02x ", *(start+i));
                //}
                //LOG_MSG("\n");
            }
        }
        else if (backup_item.info.backup_op == 1) {
            LOG_MSG("op: get recovery size\n");
            uint32_t tot_count = 0;
            for (unsigned int i=0; i<worker_ops_new[backup_item.info.worker_id].size(); i++) {
                tot_count += worker_ops_new[backup_item.info.worker_id][i]->GetCount();
            }
            LOG_MSG("tot count %u\n", tot_count);
            //uint32_t count = content->GetCount();
            ZWrite(&tot_count, sizeof(uint32_t));
        }
        else if (backup_item.info.backup_op == 2) {
            LOG_MSG("op: get recovery data\n");
            uint32_t tot_count = 0;
            for (unsigned int i=0; i<worker_ops_new[backup_item.info.worker_id].size(); i++) {
                tot_count += worker_ops_new[backup_item.info.worker_id][i]->GetCount();
            }

            afs::RecoveryItem* recovery_item = new afs::RecoveryItem(tot_count);
            uint32_t cur_count = 0;
            for (unsigned int i=0; i<worker_ops_new[backup_item.info.worker_id].size(); i++) {
                worker_ops_new[backup_item.info.worker_id][i]->AppendRecoveryItem(recovery_item, cur_count);
                cur_count += worker_ops_new[backup_item.info.worker_id][i]->GetCount();
            }

            LOG_MSG("reply data with count %u size %u\n", recovery_item->GetCount(), recovery_item->size());
            ZWrite(recovery_item->data(), recovery_item->size());
            delete recovery_item;
        }
    }

    if (zmq_close(z_socket) == -1)
        LOG_ERR("zmq_close(): %s\n", zmq_strerror(errno));
    if (zmq_ctx_destroy(z_ctx) == -1)
        LOG_ERR("zmq_ctx_destroy(): %s\n", zmq_strerror(errno));

    LOG_MSG("Receive %d state backups\n", cnt);

    return 0;
}

