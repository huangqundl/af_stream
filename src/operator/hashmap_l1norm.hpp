// License here.

#ifndef __HASH_MAP_L1_INCLUDED_HPP__
#define __HASH_MAP_L1_INCLUDED_HPP__

#include <stddef.h>
#include <unordered_map>
#include "stateful_operator.hpp"

namespace afs {

// Array of intergers for counting
class HashMapL1 : public StatefulOperator {

private:
    std::unordered_map<size_t, double>* table;
    std::unordered_map<size_t, double>* bak_table;
    double total_diff;

    // derived from the basic class StatefulOperator
    int ParseAndFillRecoveryState(unsigned char* data, size_t len);

public:
    HashMapL1(CompoundOperator* context, double d);
    ~HashMapL1();

    // derived from the basic class StatefulOperator
    int Update(size_t key, long long value);    
    int UpdateF(size_t key, double value, bool is_backup);    

    // derived from the basic class StatefulOperator
    double CalculateDiff(size_t key);

    // derived from the basic class StatefulOperator
    void Reset();

    // derived from the basic class StatefulOperator
    void DumpStateToFile();

    std::unordered_map<size_t, double>& get_entire() {
        return *table;
    }

    bool HasKey(size_t key) {
        if (table->find(key) == table->end()) {
            return false;
        }
        return true;
    }

    double GetValue(size_t key) {
        return (*table)[key];
    }

    void Error() {
        table->clear();
        *table = *bak_table;
    }
};

} // namespace afs

#endif // COUNTER_ARRAY_HPP_
