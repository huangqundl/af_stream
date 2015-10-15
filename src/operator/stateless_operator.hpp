// License here.

#ifndef STATELESS_OPERATOR_HPP_
#define STATELESS_OPERATOR_HPP_

#include <string>

#include "fault_tolerance/state_manager.hpp"

namespace afs {

class StateManager;

// StatelessOperator class is the basic class for all high-level stateless operators provided.
// This family of operators does not maintain internal state structures, thus requires no storage.
// It abstracts the basic interfaces for stream processing algorithms.
class StatelessOperator {

protected:
    // handler of the underlying ROLL daemon
    StateManager* daemon;

    // unique id of the operator within the scope of the StateManager
    uint32_t component_id;

public:
    StatelessOperator();
    virtual ~StatelessOperator();

    // process a single item from input stream
    // the processing may produce new items
    // the return value indicates the status of the processing:
    //      0: successful
    //      1: failed
    //virtual void* process(void* key, long long value) = 0;    
    //virtual void* process(size_t key, long long value) = 0;    
    //virtual void* process(std::string key, long long value) = 0;    
};

} // namespace afs

#endif // STATELESS_OPERATOR_HPP_
