#ifndef __AFS_RECOVERY_ITEM_HPP_INCLUDED__
#define __AFS_RECOVERY_ITEM_HPP_INCLUDED__

#include <string.h>
#include "backup_item.hpp"

namespace afs {

class __attribute__ ((__packed__)) RecoveryItem {

private:
    unsigned char* raw;
    uint32_t count;

public:

    RecoveryItem(uint32_t c) : count(c) {
        LOG_MSG("RecoveryItem count %u size %u\n", count, size());
        raw = (unsigned char*)malloc(size());
        *(uint32_t*)raw = count;
    }

    ~RecoveryItem() {
        free(raw);
    }

    BackupItem* GetSlot(uint32_t i) {
        return (BackupItem*)(raw+sizeof(uint32_t)+sizeof(struct BackupItem)*i);
    }

    uint32_t GetCount() {
        return *(uint32_t*)raw;
    }

    void* data() {
        return raw;
    }

    uint32_t size() {
        return sizeof(uint32_t) + sizeof(struct BackupItem)*count;
    }
};

} // namespace afs

#endif
