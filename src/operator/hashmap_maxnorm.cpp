#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "util.hpp"
#include "hashmap_maxnorm.hpp"

namespace afs {

HashMapMAX::HashMapMAX(CompoundOperator* context, double d): StatefulOperator(context, d) {
    table = new std::unordered_map<size_t, double>;
    bak_table = new std::unordered_map<size_t, double>;
}

HashMapMAX::~HashMapMAX() {
    delete table;
    delete bak_table;
}


int HashMapMAX::UpdateF(size_t key, double value) {
    (*table)[key] = value;

    if (CalculateDiff(key) > delta) {
        (*bak_table)[key] = (*table)[key];
        daemon->SaveState(component_id, key, (*table)[key]);
    }

    return 0;
}

int HashMapMAX::UpdateLL(size_t key, long long value) {
    (*table)[key] = value;

    if (CalculateDiff(key) > delta) {
        (*bak_table)[key] = (*table)[key];
        daemon->SaveState(component_id, key, (*table)[key]);
    }

    return 0;
}

int HashMapMAX::IncrementLL(size_t key, long long inc) {
    (*table)[key] += inc;

    if (CalculateDiff(key) > delta) {
        (*bak_table)[key] = (*table)[key];
        daemon->SaveState(component_id, key, (*table)[key]);
    }

    return 0;
}

int HashMapMAX::Update(size_t key, long long value) {
    LOG_ERR("shoule nor here\n");
    return 0;
}

double HashMapMAX::CalculateDiff(size_t key) {
    if (bak_table->find(key) == bak_table->end()) {
        return (*table)[key];
    }
    else {
        return abs((*table)[key] - (*bak_table)[key]);
    }
}

void HashMapMAX::Reset() {
    table->clear();
    bak_table->clear();
}

int HashMapMAX::ParseAndFillRecoveryState(unsigned char* data, size_t len) {
    /*
    char* space = strchr((char*)data, ' ');
    *space = 0;
    size_t key = strtoull((char*)data, NULL, 10); 
    long long value = strtoll((char*)space+1, NULL, 10);
    array[key] = value;
    backup_array[key] = value;
    */

    return 0;
}

void HashMapMAX::DumpStateToFile() {
    /*
    char filename[100];
    sprintf(filename, "dump_%s_%u", daemon->get_daemon_name().c_str(), component_id);
    FILE* dump_file = fopen(filename, "w");
    for (size_t i=0; i<length; i++) {
        fprintf(dump_file, "%lu %lld %lld\n", i, array[i], backup_array[i]);
    }
    fclose(dump_file);
    */
}

} // namespace afs
