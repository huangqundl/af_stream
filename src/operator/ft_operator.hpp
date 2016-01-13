// License here.

#ifndef __AFS_FT_OPERATOR_HPP_INCLUDED_
#define __AFS_FT_OPERATOR_HPP_INCLUDED_

#include <string>

#include "ft_interface.hpp"
#include "fault_tolerance/operator_tracker.hpp"
//#include "compound_operator.hpp"

namespace afs {

class OperatorTracker;
//class CompoundOperator;

// FTOperator class is the basic class for all high-level operators provided.
// It abstracts the basic interfaces for stream processing algorithms.
class FTOperator : FTInterface {

protected:
    // unique id of the operator within the scope of the OperatorTracker
    uint32_t operator_id_;

public:
    FTOperator() {
        OperatorTracker* tracker = OperatorTracker::GetInstance();
        operator_id_ = tracker->RegisterFTOperator(this);
    }
    virtual ~FTOperator() {}

    uint32_t GetOperatorId() {
        return operator_id_;
    } 

    // configure divergence threshold
    void SetDivergenceThresh(double d) {
        theta_ = d;
        //LOG_MSG(" set theta %lf\n", theta_);
    }
};

} // namespace afs

#endif
