#include "analyzer.hpp"
#include "dummy_item.hpp"

class AnalyzerThread2 : public afs::Analyzer<DummyType, DummyType, afs::NullClass, DummyType> {
public:
    AnalyzerThread2(int num_in_queue, afs::RouterBase* r) :
        afs::Analyzer<DummyType, DummyType, afs::NullClass, DummyType>(0, NULL, num_in_queue, r) {}

private:
    void AnalyzerInit() {}
    void AnalyzerFinish() {}
    void AnalyzerRecovery() {}

    void DoProcessRecord(DummyType& tuple) {
        //LOG_MSG("data number %d\n", tuple.number);
        tuple.number += 10000;
        EmitReverseData(tuple);
        //LOG_MSG("    feedback ok\n");
    }

    void DoProcessTimeout() {}
};
