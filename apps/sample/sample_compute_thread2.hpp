#include "thread/compute_thread.hpp"
#include "dummy_item.hpp"

class ComputeThreadThread2 : public afs::ComputeThread<DummyType, DummyType, afs::NullClass, afs::NullClass> {
public:
    ComputeThreadThread2(int num_out_queue, afs::RouterBase* r) :
        afs::ComputeThread<DummyType, DummyType, afs::NullClass, afs::NullClass>(num_out_queue, r) {}

private:
    void ComputeThreadInit() {}
    void ComputeThreadFinish() {}
    void ComputeThreadRecovery() {}

    void ProcessRecord(DummyType& tuple, uint64_t seq) {
        LOG_MSG("data number %d\n", tuple.number);
    }
    void ProcessTimeout() {}
};
