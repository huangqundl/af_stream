// License here.

#include <stddef.h>

#include <afstream.h>

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
    CountMin(size_t r, size_t w, double d) {
        arrays = (afs::NumericArray_MAX<long long>**) calloc(r, sizeof(afs::NumericArray_MAX<long long>*));
        hash = (afs::Hash**) calloc(r, sizeof(afs::Hash*));
        row = r;
        width = w;

        for (size_t i=0; i<r; i++) {
            arrays[i] = new afs::NumericArray_MAX<long long>(w);
            arrays[i]->SetDivergenceThresh(d);
            hash[i] = new afs::Hash(w);
        }
    }

    ~CountMin() {
        for (size_t i=0; i<row; i++) {
            delete arrays[i];
            delete hash[i];
        }
        free(arrays);
        free(hash);
    }

    // inherited from the basic class StatefulOperator
    long long UpdateSketch(size_t key, long long value) {

        long long ret = 0;
        for (size_t i=0; i<row; i++) {
            auto index = hash[i]->GetHashValue((unsigned char*)(&key), sizeof(size_t));
            long long new_v = arrays[i]->GetValue(index) + value;

            arrays[i]->UpdateValue(index, new_v);
            if (i==0 || new_v<ret) {
                ret = new_v;
            }
        }

        return ret;
    }    

    long long GetValue(int r, int w) {
        long long v = arrays[r]->GetValue(w);
        return v;
    }

    void SetValue(int r, int w, long long v) {
        arrays[r]->UpdateValue(w, v);
    }

    long long GetEstimate(size_t key) {

        long long ret = 0;
        for (size_t i=0; i<row; i++) {
            auto index = hash[i]->GetHashValue((unsigned char*)(&key), sizeof(size_t));
            long long v = arrays[i]->GetValue(index);
            if (i==0 || v < ret) {
                ret = v;
            }
        }

        return ret;
    }

    // inherited from the basic class StatefulOperator
    void Clear() {
        for (size_t i=0; i<row; i++) {
            arrays[i]->Clear();
        }
    }

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
