#include "stateless_operator.hpp"

namespace afs {

StatelessOperator::StatelessOperator() {
    daemon = StateManager::GetInstance();
    component_id = daemon->RegisterOperator(STATELESS);
}

StatelessOperator::~StatelessOperator() {
}

} // namespace afs
