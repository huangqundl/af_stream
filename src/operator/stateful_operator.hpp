// License here.

#ifndef STATEFUL_OPERATOR_HPP_
#define STATEFUL_OPERATOR_HPP_

#include <string>

#include "fault_tolerance/state_manager.hpp"
#include "compound_operator.hpp"

namespace afs {

class StateManager;
class CompoundOperator;

// StatefulOperator class is the basic class for all high-level operators provided.
// It abstracts the basic interfaces for stream processing algorithms.
class StatefulOperator {

private:
    virtual int ParseAndFillRecoveryState(unsigned char* data, size_t len) = 0;

protected:
    // handler of the underlying ROLL daemon
    StateManager* daemon;

    // unique id of the operator within the scope of the StateManager
    uint32_t component_id;

    // the maximum difference allowed for any key in the operator
    double delta;
    double orig_delta;

public:
    StatefulOperator(CompoundOperator* context, double d);
    virtual ~StatefulOperator();

    // process a single item from input stream
    // the processing includes:
    //      update the internal stateful structures
    //      or produce new items
    //      or both
    // the return value indicates the status of the processing:
    //      0: successful
    //      1: failed
    //virtual int Update(void* key, long long value) = 0;    
    virtual int Update(size_t key, long long value) = 0;    
    //virtual int Update(std::string key, long long value) = 0;    

    // estimate the difference for key
    //virtual double CalculateDiff(void* key) = 0;
    virtual double CalculateDiff(size_t key) = 0;
    //virtual double CalculateDiff(std::string key) = 0;

    virtual void Reset() {
        delta = orig_delta;
    }

    virtual void DumpStateToFile() = 0;

    void Recovery();
};

} // namespace afs

#endif // STATEFUL_OPERATOR_HPP_
