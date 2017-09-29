#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

#include <set>

#include <afstream.h>

#include "cm_item.hpp"
#include "cm_sketch.hpp"

// Note: current implementation does not support sync at the end of window.
// Please guarantee that there is only one window in the input

class MergeThread : public afs::ComputeThread<struct CMItem, afs::NullClass, afs::NullClass, afs::NullClass> {
public:
    MergeThread(int num_upstreams) :
        afs::ComputeThread<struct CMItem, afs::NullClass, afs::NullClass, afs::NullClass>(num_upstreams, 0) {}

private:
    CountMin* cm_sketch;
    FILE* output;
    //unsigned long long int window;
    unsigned long long int byte;
    std::set<unsigned long long int> cand;
    unsigned long long int thresh;
    int r, w;

    void ComputeThreadInit() {
        Config* config = Config::getInstance();

        char* filename_str = config->getstring("outputFile", NULL);
        afs_assert(filename_str, "outputFile is not set\n");
        std::string filename = filename_str;
        filename += "_" + std::to_string(afs::thr_id());
        output = fopen(filename.c_str(), "w");

        r = config->getint("row", 0);
        afs_assert(r > 0, "row is not set\n");
        w = config->getint("width", 0);
        afs_assert(w > 0, "width is not set\n");
        LOG_MSG("width %d\n", w);
        double d = config->getdouble("sys.fault_tolerance.max_state_err", -1);

        cm_sketch = new CountMin(r, w, d);

        thresh = config->get_ull("thresh", -1);
        afs_assert(thresh>=0, "thresh is not set\n");

        //window = config->get_ull("window_bytes", 0);
        //LOG_MSG("window bytes %llu\n", window);
    }

    void ComputeThreadFinish() {
        fclose(output);
        delete cm_sketch;
    }

    void ComputeThreadRecovery() {}

    void ProcessData(uint32_t worker, uint32_t thread, uint64_t seq, struct CMItem &item) {
        LOG_MSG("counter %u, heavy hitter %u\n", item.num_counter, item.num_hh);
        for (uint32_t i=0; i<item.num_counter; i++) {
            std::pair<uint32_t, long long> pair = item.GetCounter(i);
            int k = pair.first / w;
            int j = pair.first % w;
            long long old_v = cm_sketch->GetValue(k, j);
            cm_sketch->SetValue(k, j, old_v+pair.second);
        }
        for (uint32_t i=0; i<item.num_hh; i++) {
            unsigned long long int key = item.GetKey(i);
            unsigned long long int e = cm_sketch->GetEstimate(key);
            //LOG_MSG("key %llu estimate %llu\n", key, e);
            if (e>thresh && cand.find(key)==cand.end()) {
                fprintf(output, "%llu %llu\n", key, e);
                cand.insert(key);
            }
        }
    }

    void ProcessPunc() {}

};
