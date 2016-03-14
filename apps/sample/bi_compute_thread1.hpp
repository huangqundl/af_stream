#include "thread/compute_thread.hpp"
#include "dummy_item.hpp"

class BiComputeThreadThread1 : public afs::ComputeThread<DummyType, DummyType, DummyType, afs::NullClass> {
public:
    BiComputeThreadThread1(int num_downstream) :
        afs::ComputeThread<DummyType, DummyType, DummyType, afs::NullClass>(0, num_downstream) {}

private:
    int cur = 0;

    void ComputeThreadInit() {}
    void ComputeThreadFinish() {}
    void ComputeThreadRecovery() {}

    void ProcessData(uint32_t worker, uint32_t thread, uint64_t seq, DummyType& tuple) {
        //LOG_MSG("ComputeThread %d, data number %u\n", get_tid(), tuple.number);
        EmitData(cur, tuple);
        cur = (cur+1) % GetNumDownstream();
    }

    void ProcessPunc() {}

    void ProcessFeedback(int src_worker, int src_thread, DummyType& tuple) {
        //LOG_MSG("feedback data number %d from worker %d thread %d\n", tuple.number, src_worker, src_thread);
    }
};
