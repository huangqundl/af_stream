// License here.

#ifndef __AFS_HASHMAP_MAX_HPP_INCLUDED__
#define __AFS_HASHMAP_MAX_HPP_INCLUDED__

#include <stddef.h>
#include <string.h>
#include <unordered_map>
#include "util.hpp"
#include "ft_operator.hpp"

namespace afs {

// Array of numeric values
template<class K, class V>
class HashMap_MAX : public FTOperator {

private:
    std::unordered_map<K, V> map, backup_map;
    V max_diff;
    K max_diff_key;

public:
    HashMap_MAX(double d = 0);
    ~HashMap_MAX();

    // interface for fault tolerance, derived from the base class FTInterface
    double CalculateDivergence();
    void SerializeState(BackupData &backup_data);
    void RecoveryState(BackupData &backup_data);

    // derived from the base class FTInterface
    void Clear();

    // for normal processing
    void UpdateValue(K key, V value);
    V GetValue(K key);
};

template<class K, class V>
HashMap_MAX<K, V>::HashMap_MAX(double d) : FTOperator() {
    memset(&max_diff_key, 0, sizeof(K));
    memset(&max_diff, 0, sizeof(V));
}

template<class K, class V>
HashMap_MAX<K, V>::~HashMap_MAX() {
}

template<class K, class V>
void HashMap_MAX<K, V>::UpdateValue(K key, V value) {
    map[key] = value;

    if (value-backup_map[key] > max_diff) {
        max_diff = value-backup_map[index];
        max_diff_key = key;
    }
}

template<class K, class V>
V HashMap_MAX<K, V>::GetValue(K key) {
    return map[key];
}

template<class K, class V>
void HashMap_MAX<K, V>::Clear() {
    map.clear();
    backup_map.clear();
    memset(&max_diff_key, 0, sizeof(K));
    memset(&max_diff, 0, sizeof(V));
}

template<class K, class V>
double HashMap_MAX<K, V>::CalculateDivergence() {
    return max_diff;
}
    
template<class K, class V>
void HashMap_MAX<K, V>::SerializeState(BackupData &backup_data) {
    backup_data.meta.len = sizeof(K) + sizeof(V);
    backup_data.meta.key = (uint64_t)max_diff_key;
    memcpy(backup_data.data, &max_diff_key, sizeof(K));
    memcpy(backup_data.data+sizeof(K), &map[max_diff_key], sizeof(V));

    backup_map[max_diff_key] = map[max_diff_key];
    memset(&max_diff_key, 0, sizeof(K));
    memset(&max_diff, 0, sizeof(V));
}
    
template<class K, class V>
void HashMap_MAX<K, V>::RecoveryState(BackupData& backup_data) {
    //LOG_MSG("len %u key %lu\n", backup_data.meta.len, backup_data.meta.key);
    K key;
    memcpy(&key, backup_data.data, sizeof(K));
    map[key] = backup_map[key] = *(V*)(backup_data.data+sizeof(K));
    //LOG_MSG("    value %ld\n", array[index]);
}

} // namespace afs

#endif // __AFS_COUNTER_ARRAY_HPP_INCLUDED__
