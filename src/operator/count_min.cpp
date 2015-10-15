#include "count_min.hpp"

namespace afs {

CountMin::CountMin(size_t r, size_t w, double d) : CompoundOperator(), row(r), width(w) {
    arrays = (CounterArray**) calloc(r, sizeof(CounterArray*));
    hash = (Hash**) calloc(r, sizeof(Hash*));

    for (size_t i=0; i<r; i++) {
        arrays[i] = new CounterArray(this, w, d);
        hash[i] = new Hash(w);
    }
}

CountMin::~CountMin() {
    for (size_t i=0; i<row; i++) {
        delete arrays[i];
        delete hash[i];
    }
    free(arrays);
    free(hash);
}

long long CountMin::Update(size_t key, long long value) {
    //fprintf(stderr, "key %lu, value %lld:\n", key, value);
    long long ret = 0;
    for (size_t i=0; i<row; i++) {
        auto index = hash[i]->Process((unsigned char*)(&key), sizeof(size_t));
        //fprintf(stderr, "    i %lu, index %lu\n", i, index);
        arrays[i]->Update(index, value);
        if (i==0 || arrays[i]->GetValue(index)<ret) {
            ret = arrays[i]->GetValue(index);
        }
    }

    return ret;
}

void CountMin::Reset() {
    for (size_t i=0; i<row; i++) {
        arrays[i]->Reset();
    }
}

void CountMin::Recovery() {
    for (size_t i=0; i<row; i++) {
        arrays[i]->Recovery();
    }
}

void CountMin::DumpStateToFile() {
    for (size_t i=0; i<row; i++) {
        arrays[i]->DumpStateToFile();
    }
}

} // namespace afs
