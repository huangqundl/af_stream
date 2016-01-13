#ifndef __AFS_BACKUP_ITEM_HPP_INCLUDED__
#define __AFS_BACKUP_ITEM_HPP_INCLUDED__

#include <string.h>
#include "params.hpp"

namespace afs {

struct __attribute__ ((__packed__)) BackupInfo {
    uint32_t backup_op;     // 0: write, 1: read len, 2: read data
    uint32_t worker_id;
    uint16_t thread_id;
    uint16_t op_index;
    uint64_t seq;
};

struct __attribute__ ((__packed__)) BackupMeta {
    uint32_t len;
    uint64_t key;
};

struct __attribute__ ((__packed__)) BackupData {
    struct BackupMeta meta;
    unsigned char data[MAX_BACKUP_ITEM_DATA_LEN];
};

class __attribute__ ((__packed__)) BackupItem {

public:
    struct BackupInfo info;
    struct BackupData data;

    inline uint32_t size() {
        return sizeof(struct BackupInfo) + sizeof(struct BackupMeta) + data.meta.len;
    }

    /*
    BackupItem& operator=(const BackupItem& other) {
        //this->info.backup_op = other.info.backup_op;
        //this->info.worker_id = other.info.worker_id;
        //this->info.thread_id = other.info.thread_id;
        //this->info.op_index = other.info.op_index;
        //this->info.seq = other.info.seq;
        //this->info = other.info;
        //this->data.meta = other.data.meta;
        memcpy(this->data.data, other.data.data, other.data.meta.len);
        return *this;
    }
    */
};

} // namespace afs

#endif
