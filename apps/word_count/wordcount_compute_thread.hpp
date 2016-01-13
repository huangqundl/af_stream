#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

#include "compute_thread.hpp"
#include "raw_item.hpp"
#include "operator/hashmap_maxnorm_str.hpp"

class ComputeThreadThread : public afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass> {
public:
    ComputeThreadThread(int num_out_queue, afs::RouterBase* r, int num_r_queue, afs::RouterBase* rr) :
        afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass>(num_out_queue, r, num_r_queue, rr) {}

private:

    //unordered_map<int,double> weights;
    afs::HashMapMAXStr* table;
    uint64_t n_;
    uint64_t window_size_;
    std::map<std::string, unsigned long long int> recovery;

    void ComputeThreadInit() {
        Config* config = Config::getInstance();

        double max_state_err = config->getdouble("sys.fault_tolerance.max_state_err", -1);
        table = new afs::HashMapMAXStr(NULL, max_state_err);

        n_ = 0;
        window_size_ = config->getint("sys.window.size_n", 0);
        afs_assert(window_size_, "sys.window.size_n is not set\n");
    }

    void ComputeThreadFinish() {
        Config* config = Config::getInstance();
        char* filename_str = config->getstring("app.word_count.output.filename", NULL);
        afs_assert(filename_str, "app.word_count.output.filename is not set\n");
        std::string filename = filename_str;
        filename += "_" + std::to_string(get_tid());
        FILE* out_file = fopen(filename.c_str(), "w");
        std::unordered_map<std::string, long long> wc = table->get_entire();
        for(auto it = wc.begin(); it != wc.end(); it++){
            fprintf(out_file, "%s %lld\n", it->first.c_str(), it->second);
        }
        fclose(out_file);

        delete table;
    }

    void ComputeThreadRecovery() {
        uint64_t start_ts = now_us();
        char filename[100];
        Config* config = Config::getInstance();
        char* prefix = config->getstring("sys.fault_tolerance.state_backup_prefix", NULL);
        char buf[MAX_RECORD_LENGTH];
        //char key[100];
        sprintf(filename, "%sbak_state_%s_%u", prefix, "Worker", get_tid());
        FILE* file = fopen(filename, "r");
        while (1) {
            if (fgets(buf, MAX_RECORD_LENGTH, file) == NULL) {
                break;
            }
            /*
            char* tok = strtok(buf, " ");
            strcpy(key, tok);
            tok = strtok(NULL, " ");
            unsigned long long int v = strtoull(tok, NULL, 10);
            recovery[key] = v;
            */
             
         }
        uint64_t end_ts = now_us();
        fclose(file);
        LOG_MSG("Recovery time worker %lf\n", (end_ts-start_ts)/1000000.0);
        printf("Recovery time worker %lf %u\n", (end_ts-start_ts)/1000000.0, get_tid());
    }

    void ProcessRecord(afs::RawItem& tuple, uint64_t seq) {
        char* line = tuple.raw_data;
        char* tok = strtok(line, " ");
        while (tok) {
            table->IncrementLL(tok, 1);
            tok = strtok(NULL, " ");
        }

        n_++;
        if (n_ % window_size_ == 0) {
            table->Reset();
        }
    }

    void ProcessTimeout() {
    }
};

