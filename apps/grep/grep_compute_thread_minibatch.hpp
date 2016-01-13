#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

#include "compute_thread.hpp"
#include "raw_item.hpp"

class ComputeThreadThreadMinibatch : public afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass> {
public:
    ComputeThreadThreadMinibatch(int num_out_queue, afs::RouterBase* r, int num_r_queue, afs::RouterBase* rr) :
        afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass>(num_out_queue, r, num_r_queue, rr) {}

private:

    //unordered_map<int,double> weights;
    uint64_t count_;
    uint64_t out_cnt_;
    std::string filename_prefix;

    void ComputeThreadInit() {
        Config* config = Config::getInstance();
        char* filename_str = config->getstring("app.word_count.output.filename", NULL);
        afs_assert(filename_str, "app.word_count.output.filename is not set\n");
        filename_prefix = filename_str;
    }

    void ComputeThreadFinish() {
        std::string filename = filename_prefix + "_" + std::to_string(get_tid()) + "_" + std::to_string(out_cnt_++);
        FILE* out_file = fopen(filename.c_str(), "w");
        fprintf(out_file, "count %lu\n", count_);
        fclose(out_file);
    }

    void ComputeThreadRecovery() {}

    void ProcessRecord(afs::RawItem& tuple, uint64_t seq) {
        char* line = tuple.raw_data;
        if (strstr(line, "the")) {
            count_++;
        }
    }

    void ProcessTimeout() {
        std::string filename = filename_prefix + "_" + std::to_string(get_tid()) + "_" + std::to_string(out_cnt_++);
        FILE* out_file = fopen(filename.c_str(), "w");
        fprintf(out_file, "count %lu\n", count_);
        fclose(out_file);
        count_ = 0;

        afs::NullClass item;
        EmitReverseData(item);
        FlushReverseWriter();
    }
};

