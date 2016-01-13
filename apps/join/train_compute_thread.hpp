#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

#include <set>
#include "compute_thread.hpp"
#include "raw_item.hpp"
#include "operator/hashmap_l1norm.hpp"
#include "operator/count_min.hpp"

class ComputeThreadThread : public afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass> {
public:
    ComputeThreadThread(int num_out_queue, afs::RouterBase* r) :
        afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass>(num_out_queue, r) {}

private:
    afs::CountMin* cm_sketch;
    FILE* output;
    unsigned long long int window;
    unsigned long long int byte;
    std::set<unsigned long long int> print;
    unsigned long long int thresh;
    double rate;
    long long sum;

    void ComputeThreadInit() {
        Config* config = Config::getInstance();

        char* filename_str = config->getstring("outputFile", NULL);
        afs_assert(filename_str, "outputFile is not set\n");
        std::string filename = filename_str;
        filename += "_" + std::to_string(get_tid());
        output = fopen(filename.c_str(), "w");

        double d = config->getdouble("sys.fault_tolerance.max_state_err", -1);

        cm_sketch = new afs::CountMin(100, 1, d);

        thresh = config->get_ull("thresh", -1);
        afs_assert(thresh>=0, "thresh is not set\n");

        window = config->get_ull("window_bytes", 0);
        LOG_MSG("window bytes %llu\n", window);

        rate = config->getdouble("sample_rate", 0);
        LOG_MSG("sample rate %lf\n", rate);
    }

    void ComputeThreadFinish() {
        fprintf(output, "%llu %lf\n", byte, sum*1.0/rate);
        fclose(output);
        delete cm_sketch;
    }

    void ComputeThreadRecovery() {}

    void ProcessRecord(afs::RawItem& tuple) {}

    void ProcessRecord(afs::RawItem& tuple, uint64_t seq) {

        char* line = tuple.raw_data;
        char* tok = strtok(line, " ");
        tok = strtok(NULL, " ");
        unsigned long long int key = strtoull (tok, NULL, 10);
        tok = strtok(NULL, " ");
        long long value =  strtoll (tok, NULL, 10);

        if (rand()*1.0/RAND_MAX <= rate) {
            sum = cm_sketch->Update(key, value);
            return;
        }
        
        byte += value;
        if (byte > window) {
            fprintf(output, "%llu %lf\n", byte, sum*1.0/rate);
            byte = 0;
            sum = 0;
            cm_sketch->Reset();
            print.clear();
        }

    }

    void ProcessTimeout() {
        cm_sketch->Error();
    }

};
