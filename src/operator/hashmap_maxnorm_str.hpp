// License here.

#ifndef __HASH_MAP_MAX_INCLUDED_HPP__
#define __HASH_MAP_MAX_INCLUDED_HPP__

#include <stddef.h>
#include <string>
#include <unordered_map>
#include "stateful_operator.hpp"

namespace afs {

// Array of intergers for counting
class HashMapMAXStr : public StatefulOperator {

private:
    std::unordered_map<std::string, long long>* table;
    std::unordered_map<std::string, long long>* bak_table;

    // derived from the basic class StatefulOperator
    int ParseAndFillRecoveryState(unsigned char* data, size_t len);

public:
    HashMapMAXStr(CompoundOperator* context, long long d);
    ~HashMapMAXStr();

    // derived from the basic class StatefulOperator
    int Update(size_t key, long long value);    

    int IncrementLL(std::string key, long long inc);

    // derived from the basic class StatefulOperator
    double CalculateDiff(size_t key);
    long long CalculateDiff(std::string key);

    // derived from the basic class StatefulOperator
    void Reset();

    // derived from the basic class StatefulOperator
    void DumpStateToFile();

    std::unordered_map<std::string, long long>& get_entire() {
        return *table;
    }

    /*
    long long v = (*table)[key];
    long long bak_v = (*bak_table)[key];

    v += inc;
    (*table)[key] = v;

    if (delta>=0 && abs(v-bak_v)>delta) {
        (*bak_table)[key] = v;
        daemon->SaveState(component_id, key, v);
    }
    */

};

} // namespace afs

#endif // __HASH_MAP_MAX_INCLUDED_HPP__
