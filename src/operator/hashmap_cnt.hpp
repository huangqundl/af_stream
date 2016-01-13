// License here.

#ifndef __AFS_HASHMAP_CNT_HPP_INCLUDED__
#define __AFS_HASHMAP_CNT_HPP_INCLUDED__

#include <stddef.h>
#include <string.h>
#include <unordered_map>
#include "util.hpp"
#include "ft_operator.hpp"

namespace afs {

// Array of numeric values
template<class K, class V>
class HashMap_CNT : public FTOperator {

private:
    std::unordered_map<K, V> map, backup_map;
    uint32_t backup_time;

public:
    HashMap_CNT(double d = 0);
    ~HashMap_CNT();

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
HashMap_CNT<K, V>::HashMap_CNT(double d) : FTOperator() {
    backup_time = 0;
}

template<class K, class V>
HashMap_CNT<K, V>::~HashMap_CNT() {
}

template<class K, class V>
void HashMap_CNT<K, V>::UpdateValue(K key, V value) {
    map[key] = value;
}

template<class K, class V>
V HashMap_CNT<K, V>::GetValue(K key) {
    return map[key];
}

template<class K, class V>
void HashMap_CNT<K, V>::Clear() {
    map.clear();
    backup_map.clear();
}

template<class K, class V>
double HashMap_CNT<K, V>::CalculateDivergence() {
    return map.size() - backup_map.size();
}
    
template<class K, class V>
void HashMap_CNT<K, V>::SerializeState(BackupData &backup_data) {
    uint32_t cap = (sizeof(struct BackupData)-sizeof(struct BackupMeta))/(sizeof(K)+sizeof(V));
    
    uint32_t cur = 0;
    unsigned char* start = backup_data.data;
    for (auto it : map) {
        if (cur == cap) {
            break;
        }
        if (backup_map.find(it->first)==backup_map.end) {
            backup_map[it->first] = it->second;
            memcpy(start, &(it->first), sizeof(K));
            memcpy(start+sizeof(K), &(it->second), sizeof(V));
            start += sizeof(K) + sizeof(V);
            cur++;
        }
    }

    backup_data.meta.len = (sizeof(K)+sizeof(V))*cur;
    backup_data.meta.key = backup_time++;
}
    
template<class K, class V>
void HashMap_CNT<K, V>::RecoveryState(BackupData& backup_data) {
    //LOG_MSG("len %u key %lu\n", backup_data.meta.len, backup_data.meta.key);
    K key;
    memcpy(&key, backup_data.data, sizeof(K));
    map[key] = backup_map[key] = *(V*)(backup_data.data+sizeof(K));
    //LOG_MSG("    value %ld\n", array[index]);
}

} // namespace afs

#endif
