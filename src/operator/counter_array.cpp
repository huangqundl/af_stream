#include <stdlib.h>
#include <string.h>
#include "counter_array.hpp"

namespace afs {

CounterArray::CounterArray(CompoundOperator* context, size_t l, double d): StatefulOperator(context, d), length(l) {
    array = (long long*)calloc(length, sizeof(long long));
    backup_array = (long long*)calloc(length, sizeof(long long));
}

CounterArray::~CounterArray() {
    free(array);
    free(backup_array);
}

int CounterArray::Update(size_t key, long long value) {
    array[key] += value;

    if (CalculateDiff(key) > delta) {
        backup_array[key] = array[key];
        daemon->SaveState(component_id, key, array[key]);
    }

    return 0;
}

double CounterArray::CalculateDiff(size_t key) {
    return array[key] - backup_array[key];
}

void CounterArray::Reset() {
    memset(array, 0, sizeof(long long)*length);
    memset(backup_array, 0, sizeof(long long)*length);
}

int CounterArray::ParseAndFillRecoveryState(unsigned char* data, size_t len) {
    char* space = strchr((char*)data, ' ');
    *space = 0;
    size_t key = strtoull((char*)data, NULL, 10); 
    long long value = strtoll((char*)space+1, NULL, 10);
    array[key] = value;
    backup_array[key] = value;

    return 0;
}

void CounterArray::DumpStateToFile() {
    char filename[100];
    sprintf(filename, "dump_%s_%u", daemon->get_daemon_name().c_str(), component_id);
    FILE* dump_file = fopen(filename, "w");
    for (size_t i=0; i<length; i++) {
        fprintf(dump_file, "%lu %lld %lld\n", i, array[i], backup_array[i]);
    }
    fclose(dump_file);
}

} // namespace afs
