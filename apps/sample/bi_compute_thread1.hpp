#include "thread/compute_thread.hpp"
#include "dummy_item.hpp"

class BiComputeThreadThread1 : public afs::ComputeThread<DummyType, DummyType, DummyType, afs::NullClass> {
public:
    BiComputeThreadThread1(int num_out_queue, afs::RouterBase* r) :
        afs::ComputeThread<DummyType, DummyType, DummyType, afs::NullClass>(num_out_queue, r, 0, NULL) {}

private:
    void ComputeThreadInit() {}
    void ComputeThreadFinish() {}
    void ComputeThreadRecovery() {}

    void ProcessRecord(DummyType& tuple, uint64_t seq) {
        //printf("ComputeThread %d, data number %u\n", get_tid(), tuple.number);
        EmitData(tuple);
    }

    void ProcessTimeout() {}

    void ProcessReverse(int src_worker, int src_thread, DummyType& tuple) {
        //LOG_MSG("feedback data number %d from worker %d thread %d\n", tuple.number, src_worker, src_thread);
    }
};
