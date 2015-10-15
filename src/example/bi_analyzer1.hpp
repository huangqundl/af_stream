#include "analyzer.hpp"
#include "dummy_item.hpp"

class BiAnalyzerThread1 : public afs::Analyzer<DummyType, DummyType, DummyType, afs::NullClass> {
public:
    BiAnalyzerThread1(int num_out_queue, afs::RouterBase* r) :
        afs::Analyzer<DummyType, DummyType, DummyType, afs::NullClass>(num_out_queue, r, 0, NULL) {}

private:
    void AnalyzerInit() {}
    void AnalyzerFinish() {}
    void AnalyzerRecovery() {}

    void DoProcessRecord(DummyType& tuple) {
        //printf("Analyzer %d, data number %u\n", get_tid(), tuple.number);
        EmitData(tuple);
    }

    void DoProcessTimeout() {}
    void DoProcessReverse(int src_worker, int src_thread, DummyType& tuple) {
        //LOG_MSG("feedback data number %d from worker %d thread %d\n", tuple.number, src_worker, src_thread);
    }
};
