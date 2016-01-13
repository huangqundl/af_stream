#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

#include "compute_thread.hpp"
#include "raw_item.hpp"

class ComputeThreadThreadUpbackup : public afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass> {
public:
    ComputeThreadThreadUpbackup(int num_out_queue, afs::RouterBase* r, int num_r_queue, afs::RouterBase* rr) :
        afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass>(num_out_queue, r, num_r_queue, rr) {}

private:

    //unordered_map<int,double> weights;
    std::unordered_map<std::string, uint64_t> table;
    uint64_t n_;
    uint64_t out_cnt_;
    std::string filename_prefix;

    void ComputeThreadInit() {
        n_ = 0;
        out_cnt_ = 0;

        Config* config = Config::getInstance();
        char* filename_str = config->getstring("app.word_count.output.filename", NULL);
        afs_assert(filename_str, "app.word_count.output.filename is not set\n");
        filename_prefix = filename_str;
    }

    void ComputeThreadFinish() {
        std::string filename = filename_prefix + "_" + std::to_string(get_tid()) + "_" + std::to_string(out_cnt_++);
        FILE* out_file = fopen(filename.c_str(), "w");
        for(auto it = table.begin(); it != table.end(); it++){
            fprintf(out_file, "%s %lu\n", it->first.c_str(), it->second);
        }
        fclose(out_file);
        table.clear();
    }

    void ComputeThreadRecovery() {}

    void ProcessRecord(afs::RawItem& tuple, uint64_t seq) {
        char* line = tuple.raw_data;
        char* tok = strtok(line, " ");
        while (tok) {
            table[tok]++;
            tok = strtok(NULL, " ");
        }

        n_++;
    }

    void ProcessTimeout() {
        std::string filename = filename_prefix + "_" + std::to_string(get_tid()) + "_" + std::to_string(out_cnt_++);
        FILE* out_file = fopen(filename.c_str(), "w");
        for(auto it = table.begin(); it != table.end(); it++){
            fprintf(out_file, "%s %lu\n", it->first.c_str(), it->second);
        }
        fclose(out_file);
        table.clear();

        afs::NullClass item;
        EmitReverseData(item);
        FlushReverseWriter();
    }
};

