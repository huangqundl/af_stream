#include <string.h>

#include "util.hpp"
#include "stateful_operator.hpp"
#include "recovery_state.hpp"

namespace afs {

StatefulOperator::StatefulOperator(CompoundOperator* context, double d): delta(d), orig_delta(d) {
    daemon = StateManager::GetInstance();
    component_id = daemon->RegisterOperator(STATEFUL);
    if (context != NULL) {
        context->AddOperatorID(component_id);
    }
    //LOG_MSG("    max state error: %.02lf\n", delta);
}

StatefulOperator::~StatefulOperator() {
}

void StatefulOperator::Recovery() {
    RecoveryState* rs = daemon->RequestRecoveryState(component_id);

    size_t size = rs->get_num_record();
    unsigned char* data = rs->get_data();
    uint32_t len = 0;

    for (size_t i=0; i<size; i++) {
        memcpy(&len, data, sizeof(uint32_t));
        int status = ParseAndFillRecoveryState(data+sizeof(uint32_t), len);
        if (status != 0) {
            LOG_MSG("Recovery error\n");
        }
        data += sizeof(uint32_t) + len;
    }
    daemon->FreeRecoveryState(component_id);
}

} // namespace afs
