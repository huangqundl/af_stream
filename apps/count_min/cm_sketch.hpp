// License here.

#include <stddef.h>

#include "operator/array_max.hpp"
#include "operator/hash.hpp"

// CountMin algorithm

class CountMin {

private:
    size_t row;     // number of rows 
    size_t width;   // number of buckets in a row

    afs::NumericArray_MAX<long long>** arrays;
    afs::Hash** hash;

public:
    // @param r: number of rows
    // @param w: number of buckets in a row
    // @param d: maximum allowed error in the sketch
    CountMin(size_t r, size_t w, double d);

    ~CountMin();

    // inherited from the basic class StatefulOperator
    long long UpdateSketch(size_t key, long long value);    

    long long GetValue(int r, int w);
    void SetValue(int r, int w, long long v);

    long long GetEstimate(size_t key);

    // inherited from the basic class StatefulOperator
    void Clear();

    /*
    void Recovery();

    void DumpStateToFile();
    */

    /*
    void Error() {
        for (int i=0; i<row; i++) {
            arrays[i]->Error();
        }
    }
    */
};
