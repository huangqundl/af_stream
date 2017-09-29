// License here.

#ifndef __AFS_COUNTER_ARRAY_MAX_HPP_INCLUDED__
#define __AFS_COUNTER_ARRAY_MAX_HPP_INCLUDED__

#include <stddef.h>
#include <string.h>
#include "../util.hpp"
#include "ft_operator.hpp"

namespace afs {

// Array of numeric values
template<class V>
class NumericArray_MAX : public FTOperator {

private:
    size_t length; // length of the array

    V *array; // address of the array
    V *backup_array;
    V max_diff;
    unsigned int max_diff_index;

public:
    NumericArray_MAX(size_t l, double d = 0);
    ~NumericArray_MAX();

    // interface for fault tolerance, derived from the base class FTInterface
    double CalculateDivergence();
    void SerializeState(BackupData &backup_data);
    void RecoveryState(BackupData &backup_data);

    // derived from the base class FTInterface
    void Clear();

    // for normal processing
    void UpdateValue(unsigned int index, V value);
    V GetValue(unsigned int index);

    void Print() {
        for (int i=0; i<length; i++) {
            if (array[i] > 0) {
                LOG_MSG(" index %d value %ld\n", i, array[i]);
            }
        }
    }
};

template<class V>
NumericArray_MAX<V>::NumericArray_MAX(size_t l, double d) : FTOperator(), length(l) {
    array = (V*)calloc(length, sizeof(V));
    backup_array = (V*)calloc(length, sizeof(V));
    memset(array, 0, sizeof(V)*length);
    memset(backup_array, 0, sizeof(V)*length);

    max_diff_index = 0;
    memset(&max_diff, 0, sizeof(V));
}

template<class V>
NumericArray_MAX<V>::~NumericArray_MAX() {
    free(array);
    free(backup_array);
}

template<class V>
void NumericArray_MAX<V>::UpdateValue(unsigned int index, V value) {
    array[index] = value;

    if (value-backup_array[index] > max_diff) {
        max_diff = value-backup_array[index];
        max_diff_index = index;
    }
}

template<class V>
V NumericArray_MAX<V>::GetValue(unsigned int index) {
    return array[index];
}

template<class V>
void NumericArray_MAX<V>::Clear() {
    memset(array, 0, sizeof(V)*length);
    memset(backup_array, 0, sizeof(V)*length);
    memset(&max_diff, 0, sizeof(V));
}

template<class V>
double NumericArray_MAX<V>::CalculateDivergence() {
    return max_diff;
}
    
template<class V>
void NumericArray_MAX<V>::SerializeState(BackupData &backup_data) {
    backup_data.meta.len = sizeof(unsigned int) + sizeof(V);
    backup_data.meta.key = max_diff_index;
    memcpy(backup_data.data, &max_diff_index, sizeof(unsigned int));
    memcpy(backup_data.data+sizeof(unsigned int), &array[max_diff_index], sizeof(V));

    backup_array[max_diff_index] = array[max_diff_index];
    max_diff_index = 0;
    memset(&max_diff, 0, sizeof(V));
}
    
template<class V>
void NumericArray_MAX<V>::RecoveryState(BackupData& backup_data) {
    //LOG_MSG("len %u key %lu\n", backup_data.meta.len, backup_data.meta.key);
    unsigned int index = backup_data.meta.key;
    array[index] = backup_array[index] = *(V*)(backup_data.data+sizeof(unsigned int));
    //LOG_MSG("    value %ld\n", array[index]);
}

} // namespace afs

#endif // __AFS_COUNTER_ARRAY_HPP_INCLUDED__
