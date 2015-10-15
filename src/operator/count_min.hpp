// License here.

#ifndef COUNT_MIN_HPP_
#define COUNT_MIN_HPP_

#include <stddef.h>

#include "stateful_operator.hpp"
#include "compound_operator.hpp"
#include "counter_array.hpp"
#include "hash.hpp"

namespace afs {

// CountMin algorithm
// Elaborate
class CountMin : public CompoundOperator {

private:
    size_t row;     // number of rows 
    size_t width;   // number of buckets in a row

    CounterArray** arrays;
    Hash** hash;

public:
    // @param r: number of rows
    // @param w: number of buckets in a row
    // @param d: maximum allowed error in the sketch
    CountMin(size_t r, size_t w, double d);

    ~CountMin();

    // inherited from the basic class StatefulOperator
    long long Update(size_t key, long long value);    

    // inherited from the basic class StatefulOperator
    void Reset();

    // inherited from the basic class StatefulOperator
    void Recovery();

    void DumpStateToFile();

    void Error() {
        for (int i=0; i<row; i++) {
            arrays[i]->Error();
        }
    }
};

} // namespace afs

#endif // COUNT_MIN_
