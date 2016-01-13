// License here.

#ifndef __AFS_SET_CNT_HPP_INCLUDED__
#define __AFS_SET_CNT_HPP_INCLUDED__

#include <stddef.h>
#include <string.h>
#include <unordered_set>
#include "util.hpp"
#include "ft_operator.hpp"

namespace afs {

// Array of numeric values
template<class K>
class Set_CNT : public FTOperator {

private:
    std::unordered_set<K> set, backup_set;
    uint32_t backup_time;

public:
    Set_CNT(double d = 0);
    ~Set_CNT();

    // interface for fault tolerance, derived from the base class FTInterface
    double CalculateDivergence();
    void SerializeState(BackupData &backup_data);
    void RecoveryState(BackupData &backup_data);

    // derived from the base class FTInterface
    void Clear();

    // for normal processing
    void Insert(K key);
    bool Query(K key);
};

template<class K>
Set_CNT<K>::Set_CNT(double d) : FTOperator() {
    backup_time = 0;
}

template<class K>
Set_CNT<K>::~Set_CNT() {
}

template<class K>
void Set_CNT<K>::Insert(K key) {
    set.insert(key);
}

template<class K>
bool Set_CNT<K>::Query(K key) {
    if (set.find(key==set.end())) {
        return false;
    }
    return true;
}

template<class K>
void Set_CNT<K>::Clear() {
    set.clear();
    backup_set.clear();
}

template<class K>
double Set_CNT<K>::CalculateDivergence() {
    return set.size() - backup_set.size();
}
    
template<class K>
void Set_CNT<K>::SerializeState(BackupData &backup_data) {
    uint32_t cap = (sizeof(struct BackupData)-sizeof(struct BackupMeta))/(sizeof(K));
    
    uint32_t cur = 0;
    unsigned char* start = backup_data.data;
    for (auto it : set) {
        if (cur == cap) {
            break;
        }
        if (backup_set.find(it)==backup_set.end) {
            backup_set.insert(*it);
            memcpy(start, &(*it), sizeof(K));
            start += sizeof(K);
            cur++;
        }
    }

    backup_data.meta.len = sizeof(K)*cur;
    backup_data.meta.key = backup_time++;
}
    
template<class K>
void Set_CNT<K>::RecoveryState(BackupData& backup_data) {
    //LOG_MSG("len %u key %lu\n", backup_data.meta.len, backup_data.meta.key);
    K key;
    memcpy(&key, backup_data.data, sizeof(K));
    set.insert(key);
    backup_set.insert(key);
    //LOG_MSG("    value %ld\n", array[index]);
}

} // namespace afs

#endif
