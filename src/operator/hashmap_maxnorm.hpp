// License here.

#ifndef __HASH_MAP_MAX_INCLUDED_HPP__
#define __HASH_MAP_MAX_INCLUDED_HPP__

#include <stddef.h>
#include <unordered_map>
#include "stateful_operator.hpp"

namespace afs {

// Array of intergers for counting
class HashMapMAX : public StatefulOperator {

private:
    std::unordered_map<size_t, double>* table;
    std::unordered_map<size_t, double>* bak_table;

    // derived from the basic class StatefulOperator
    int ParseAndFillRecoveryState(unsigned char* data, size_t len);

public:
    HashMapMAX(CompoundOperator* context, double d);
    ~HashMapMAX();

    // derived from the basic class StatefulOperator
    int Update(size_t key, long long value);    
    int UpdateLL(size_t key, long long value);    
    int UpdateF(size_t key, double value);    

    int IncrementLL(size_t key, long long inc);

    // derived from the basic class StatefulOperator
    double CalculateDiff(size_t key);

    // derived from the basic class StatefulOperator
    void Reset();

    // derived from the basic class StatefulOperator
    void DumpStateToFile();

    std::unordered_map<size_t, double>& get_entire() {
        return *table;
    }

};

} // namespace afs

#endif // COUNTER_ARRAY_HPP_
