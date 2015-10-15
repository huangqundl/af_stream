#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "util.hpp"
#include "hashmap_maxnorm_str.hpp"

namespace afs {

HashMapMAXStr::HashMapMAXStr(CompoundOperator* context, long long d): StatefulOperator(context, d) {
    table = new std::unordered_map<std::string, long long>;
    bak_table = new std::unordered_map<std::string, long long>;
}

HashMapMAXStr::~HashMapMAXStr() {
    delete table;
    delete bak_table;
}

int HashMapMAXStr::IncrementLL(std::string key, long long inc) {
    (*table)[key] += inc;

    if (delta>=0 && CalculateDiff(key)>delta) {
        (*bak_table)[key] = (*table)[key];
        daemon->SaveState(component_id, key, (*table)[key]);
    }

    return 0;
}

int HashMapMAXStr::Update(size_t key, long long value) {
    LOG_ERR("shoule nor here\n");
    return 0;
}

long long HashMapMAXStr::CalculateDiff(std::string key) {
    if (bak_table->find(key) == bak_table->end()) {
        return (*table)[key];
    }
    else {
        return abs((*table)[key] - (*bak_table)[key]);
    }
}

double HashMapMAXStr::CalculateDiff(size_t key) {
    return 0;
}

void HashMapMAXStr::Reset() {
    table->clear();
    bak_table->clear();
    StatefulOperator::Reset();
}

int HashMapMAXStr::ParseAndFillRecoveryState(unsigned char* data, size_t len) {
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

void HashMapMAXStr::DumpStateToFile() {
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
