// License here.

#ifndef COUNTER_ARRAY_HPP_
#define COUNTER_ARRAY_HPP_

#include <stddef.h>
#include <string.h>
#include "stateful_operator.hpp"

namespace afs {

// Array of intergers for counting
class CounterArray : public StatefulOperator {

private:
    size_t length; // length of the array
    long long* array; // address of the array

    // the array value has been backed up, not an efficient implementation and will be refined later on
    long long* backup_array;

    // derived from the basic class StatefulOperator
    int ParseAndFillRecoveryState(unsigned char* data, size_t len);

public:
    CounterArray(CompoundOperator* context, size_t l, double d);
    ~CounterArray();

    // derived from the basic class StatefulOperator
    int Update(size_t key, long long value);    

    // derived from the basic class StatefulOperator
    double CalculateDiff(size_t key);

    long long GetValue(int index) {
        return array[index];
    }

    // derived from the basic class StatefulOperator
    void Reset();

    // derived from the basic class StatefulOperator
    void DumpStateToFile();

    void Error() {
        memcpy(array, backup_array, length*sizeof(long long));
    }

};

} // namespace afs

#endif // COUNTER_ARRAY_HPP_
