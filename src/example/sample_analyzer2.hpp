#include "analyzer.hpp"
#include "dummy_item.hpp"

class AnalyzerThread2 : public afs::Analyzer<DummyType, DummyType, afs::NullClass, afs::NullClass> {
public:
    AnalyzerThread2(int num_out_queue, afs::RouterBase* r) :
        afs::Analyzer<DummyType, DummyType, afs::NullClass, afs::NullClass>(num_out_queue, r) {}

private:
    void AnalyzerInit() {}
    void AnalyzerFinish() {}
    void AnalyzerRecovery() {}

    void DoProcessRecord(DummyType& tuple) {
        //LOG_MSG("data number %d\n", tuple.number);
    }
    void DoProcessTimeout() {}
};
