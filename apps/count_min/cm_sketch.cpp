#include "cm_sketch.hpp"

CountMin::CountMin(size_t r, size_t w, double d) : row(r), width(w) {
    arrays = (afs::NumericArray_MAX<long long>**) calloc(r, sizeof(afs::NumericArray_MAX<long long>*));
    hash = (afs::Hash**) calloc(r, sizeof(afs::Hash*));

    for (size_t i=0; i<r; i++) {
        arrays[i] = new afs::NumericArray_MAX<long long>(w);
        arrays[i]->SetDivergenceThresh(d);
        hash[i] = new afs::Hash(w);
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

long long CountMin::UpdateSketch(size_t key, long long value) {

        /*
    for (size_t i=0; i<row; i++) {
        LOG_MSG("array %u\n", i);
        arrays[i]->Print();
        for (size_t j=0; j<width; j++) {
            if (arrays[i]->GetValue(j)) {
                LOG_MSG("before update i %d j %d v %ld\n", i, j, arrays[i]->GetValue(j));
            }
        }
    }
        */

    //fprintf(stderr, "key %lu, value %lld:\n", key, value);
    long long ret = 0;
    for (size_t i=0; i<row; i++) {
        auto index = hash[i]->GetHashValue((unsigned char*)(&key), sizeof(size_t));
        //LOG_MSG("index %u old value %u\n", index, arrays[i]->GetValue(index));
        long long new_v = arrays[i]->GetValue(index) + value;
        arrays[i]->UpdateValue(index, new_v);
        //LOG_MSG("    index %u new value %u\n", index, arrays[i]->GetValue(index));
        if (i==0 || new_v<ret) {
            ret = new_v;
        }
    }

    /*
    for (size_t i=0; i<row; i++) {
        LOG_MSG("after array %u\n", i);
        arrays[i]->Print();
    }

    for (size_t i=0; i<row; i++) {
        for (size_t j=0; j<width; j++) {
            if (arrays[i]->GetValue(j)) {
                LOG_MSG("update i %d j %d v %ld\n", i, j, arrays[i]->GetValue(j));
            }
        }
    }
    */

    return ret;
}

long long CountMin::GetEstimate(size_t key) {
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

long long CountMin::GetValue(int i, int j) {
    //LOG_MSG("r %d w %d v %ld\n", r, w, arrays[r]->GetValue(w));
    long long v = arrays[i]->GetValue(j);
    if (v > 0) {
        //LOG_MSG("cm: r %d w %d v %ld\n", i, j, v);
    }
    return v;
    //return 1;
}

void CountMin::SetValue(int i, int j, long long v) {
    arrays[i]->UpdateValue(j, v);
}

void CountMin::Clear() {
    for (size_t i=0; i<row; i++) {
        arrays[i]->Clear();
    }
}

/*
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
*/
