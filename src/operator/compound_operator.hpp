// License here.

#ifndef COMPOUND_OPERATOR_HPP_
#define COMPOUND_OPERATOR_HPP_

#include <stdio.h>
#include <string>

#include "fault_tolerance/state_manager.hpp"

namespace afs {

class StateManager;

// StatefulOperator class is the basic class for all high-level operators provided.
// It abstracts the basic interfaces for stream processing algorithms.
class CompoundOperator {

private:
    /*
    uint32_t* component_ids;
    uint32_t num_component;
    uint32_t max_num_component;
    */

protected:
    // handler of the underlying ROLL daemon
    StateManager* daemon;

    // unique id of the operator within the scope of the StateManager
    uint32_t id;

public:
    CompoundOperator();
    virtual ~CompoundOperator() {}

    // process a single item from input stream
    // the processing includes:
    //      update the internal stateful structures
    //      or produce new items
    //      or both
    // the return value indicates the status of the processing:
    //      0: successful
    //      1: failed
    virtual long long Update(size_t key, long long value) = 0;    

    virtual void DumpStateToFile() = 0;

    virtual void Recovery() = 0;

    virtual void Reset() = 0;

    void AddOperatorID(uint32_t operator_id);
};

} // namespace afs

#endif // COMPOUND_OPERATOR_HPP_
