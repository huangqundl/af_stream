#include "analyzer.hpp"
#include "dummy_item.hpp"

class AnalyzerThread1 : public afs::Analyzer<DummyType, DummyType, afs::NullClass, afs::NullClass> {
public:
    AnalyzerThread1(int num_out_queue, afs::RouterBase* r) :
        afs::Analyzer<DummyType, DummyType, afs::NullClass, afs::NullClass>(num_out_queue, r) {}

private:
    void AnalyzerInit() {}
    void AnalyzerFinish() {}
    void AnalyzerRecovery() {}

    void DoProcessRecord(DummyType& tuple) {
        printf("Analyzer %d, data number %u\n", get_tid(), tuple.number);
        EmitData(tuple);
    }
    void DoProcessTimeout() {}
};
