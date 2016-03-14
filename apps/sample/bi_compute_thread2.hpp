#include "thread/compute_thread.hpp"
#include "dummy_item.hpp"

class ComputeThreadThread2 : public afs::ComputeThread<DummyType, DummyType, afs::NullClass, DummyType> {
public:
    ComputeThreadThread2(int num_upstream) :
        afs::ComputeThread<DummyType, DummyType, afs::NullClass, DummyType>(num_upstream, 0) {}

private:
    void ComputeThreadInit() {}
    void ComputeThreadFinish() {}
    void ComputeThreadRecovery() {}

    void ProcessData(uint32_t worker, uint32_t thread, uint64_t seq, DummyType& tuple) {
        //LOG_MSG("data number %d\n", tuple.number);
        tuple.number += 10000;
        EmitReverseData(worker, tuple);
        //LOG_MSG("    feedback ok\n");
    }

    void ProcessPunc() {}
};
