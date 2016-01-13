#include "thread/compute_thread.hpp"
#include "dummy_item.hpp"

class ComputeThreadThread2 : public afs::ComputeThread<DummyType, DummyType, afs::NullClass, DummyType> {
public:
    ComputeThreadThread2(int num_in_queue, afs::RouterBase* r) :
        afs::ComputeThread<DummyType, DummyType, afs::NullClass, DummyType>(0, NULL, num_in_queue, r) {
            LOG_MSG("in queue %d\n", num_in_queue)
        }

private:
    void ComputeThreadInit() {}
    void ComputeThreadFinish() {}
    void ComputeThreadRecovery() {}

    void ProcessRecord(DummyType& tuple, uint64_t seq) {
        //LOG_MSG("data number %d\n", tuple.number);
        tuple.number += 10000;
        EmitReverseData(tuple);
        //LOG_MSG("    feedback ok\n");
    }

    void ProcessTimeout() {}
};
