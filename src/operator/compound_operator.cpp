#include "compound_operator.hpp"

namespace afs {

CompoundOperator::CompoundOperator() {
    /*
    component_ids = (uint32_t*)calloc(max_num_component, sizeof(uint32_t));
    if (component_ids == NULL) {
        // error
    }
    */
    daemon = StateManager::GetInstance();
    this->id = daemon->RegisterOperator(COMPOUND);
}

void CompoundOperator::AddOperatorID(uint32_t operator_id) {
    daemon->RecordOperatorForCompound(this->id, operator_id);
}

}   // namespace afs
