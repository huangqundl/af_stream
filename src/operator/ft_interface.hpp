// License here.

#ifndef __AFS_FT_INTERFACE_HPP_INCLUDED_
#define __AFS_FT_INTERFACE_HPP_INCLUDED_

#include "../fault_tolerance/backup_item.hpp"

namespace afs {

/************
 *
 * FTInterface abstracts the interfaces of fault tolerant operators 
 * These interfaces are only visible for StateManager and ComputeThread
 *
 ***********/

class FTInterface {

protected:
    double theta_;

public:
    virtual double CalculateDivergence() = 0;
    virtual void SerializeState(BackupData &backup_data) = 0;
    virtual void RecoveryState(BackupData &backup_data) = 0;
    virtual void Clear() = 0;

    double GetDivergenceThresh() {
        return theta_;
    }
};

} // namespace afs

#endif
