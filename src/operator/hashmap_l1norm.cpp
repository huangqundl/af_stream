#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashmap_l1norm.hpp"
#include "util.hpp"

namespace afs {

HashMapL1::HashMapL1(CompoundOperator* context, double d): StatefulOperator(context, d), total_diff(0) {
    table = new std::unordered_map<size_t, double>;
    bak_table = new std::unordered_map<size_t, double>;
}

HashMapL1::~HashMapL1() {
    delete table;
    delete bak_table;
}

int HashMapL1::UpdateF(size_t key, double value, bool is_backup) {
    double key_diff_old = 0, key_diff_new = 0;
    if (bak_table->find(key) == bak_table->end()) {
        key_diff_old = abs((*table)[key]);
        key_diff_new = value;
    }
    else {
        key_diff_old = abs((*table)[key] - (*bak_table)[key]);
        key_diff_new = abs(value - (*bak_table)[key]);
    }

    (*table)[key] = value;
    total_diff = total_diff - key_diff_old + key_diff_new;

    if (is_backup && delta>=0 && CalculateDiff(key)>delta) {
        (*bak_table)[key] = (*table)[key];
        total_diff -= key_diff_new;
        daemon->SaveState(component_id, key, (*table)[key]);
    }

    return 0;
}

int HashMapL1::Update(size_t key, long long value) {
    afs_assert(0, "Currently, should not here\n");
    return 0;
}

double HashMapL1::CalculateDiff(size_t key) {
    return total_diff;
}

void HashMapL1::Reset() {
    table->clear();
    bak_table->clear();
}

int HashMapL1::ParseAndFillRecoveryState(unsigned char* data, size_t len) {
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

void HashMapL1::DumpStateToFile() {
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
