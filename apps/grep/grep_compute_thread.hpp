#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

#include "compute_thread.hpp"
#include "raw_item.hpp"
#include "operator/hashmap_maxnorm_str.hpp"

class ComputeThreadThread : public afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass> {
public:
    ComputeThreadThread(int num_out_queue, afs::RouterBase* r) :
        afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass>(num_out_queue, r) {}

private:
    //unordered_map<int,double> weights;
    uint64_t count_;

    void ComputeThreadInit() {
    }

    void ComputeThreadFinish() {
        LOG_MSG("Count %lu\n", count_);
    }

    void ComputeThreadRecovery() {}

    void ProcessRecord(afs::RawItem& tuple, uint64_t seq) {
        char* line = tuple.raw_data;
        if (strstr(line, "the")) {
            count_++;
        }
    }

    void ProcessTimeout() {}
};

