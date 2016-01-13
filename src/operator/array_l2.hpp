// License here.

#ifndef __AFS_COUNTER_ARRAY_L2_HPP_INCLUDED__
#define __AFS_COUNTER_ARRAY_L2_HPP_INCLUDED__

#include <stddef.h>
#include <string.h>
#include "util.hpp"
#include "ft_operator.hpp"

namespace afs {

// Array of numeric values
template<class V>
class NumericArray_L2 : public FTOperator {

private:
    size_t length; // length of the array

    V *array; // address of the array
    V *backup_array;
    V tot_diff;

    unsigned char backup_str[sizeof(unsigned int)+sizeof(V)];

    inline V sqr(V a) {
        return a*a;
    }

public:
    NumericArray_L2(size_t l, double d = 0);
    ~NumericArray_L2();

    // interface for fault tolerance, derived from the base class FTInterface
    double CalculateDivergence();
    void SerializeState(BackupData &backup_data);
    void RecoveryState(BackupData &backup_data);

    // derived from the base class FTInterface
    void Clear();

    // for normal processing
    void UpdateValue(unsigned int index, V value);
    V GetValue(unsigned int index);
};

template<class V>
NumericArray_L2<V>::NumericArray_L2(size_t l, double d) : FTOperator() {
    array = (V*)calloc(length, sizeof(V));
    backup_array = (V*)calloc(length, sizeof(V));

    memset(&tot_diff, 0, sizeof(V));
}

template<class V>
NumericArray_L2<V>::~NumericArray_L2() {
    free(array);
    free(backup_array);
}

template<class V>
void NumericArray_L2<V>::UpdateValue(unsigned int index, V value) {
    V old_diff = sqr(array[index] - backup_array[index]);
    V new_diff = sqr(value -  backup_array[index]);
    tot_diff = tot_diff - old_diff + new_diff;
    array[index] = value;
}

template<class V>
V NumericArray_L2<V>::GetValue(unsigned int index) {
    return array[index];
}

template<class V>
void NumericArray_L2<V>::Clear() {
    memset(array, 0, sizeof(V)*length);
    memset(backup_array, 0, sizeof(V)*length);
    memset(&tot_diff, 0, sizeof(V));
}

template<class V>
double NumericArray_L2<V>::CalculateDivergence() {
    return tot_diff;
}
    
template<class V>
void NumericArray_L2<V>::SerializeState(BackupData &backup_data) {
    unsigned int max_diff_index = 0;
    for (unsigned int i=0; i<length; i++) {
        if (sqr(array[i]-backup_array[i])>sqr(array[max_diff_index]-backup_array[max_diff_index])) {
            max_diff_index = i;
        }
    }

    backup_data.meta.len = sizeof(unsigned int) + sizeof(V);
    backup_data.meta.key = max_diff_index;
    memcpy(backup_data.data, &max_diff_index, sizeof(unsigned int));
    memcpy(backup_data.data+sizeof(unsigned int), &array[max_diff_index], sizeof(V));

    tot_diff -= sqr(array[max_diff_index]-backup_array[max_diff_index]);
    backup_array[max_diff_index] = array[max_diff_index];
}
    
template<class V>
void NumericArray_L2<V>::RecoveryState(BackupData& backup_data) {
    //LOG_MSG("len %u key %lu\n", backup_data.meta.len, backup_data.meta.key);
    unsigned int index = backup_data.meta.key;
    array[index] = backup_array[index] = *(V*)(backup_data.data+sizeof(unsigned int));
    //LOG_MSG("    value %ld\n", array[index]);
}

} // namespace afs

#endif // __AFS_COUNTER_ARRAY_HPP_INCLUDED__
