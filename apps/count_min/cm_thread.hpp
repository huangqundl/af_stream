#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

#include <set>

#include "cm_item.hpp"
#include "cm_sketch.hpp"

//#include "operator/hashmap_l1norm.hpp"
//#include "operator/count_min.hpp"

class ComputeThreadThread : public afs::ComputeThread<afs::RawItem, struct CMItem, afs::NullClass, afs::NullClass> {
public:
    ComputeThreadThread(int num_downstream) :
        afs::ComputeThread<afs::RawItem, struct CMItem, afs::NullClass, afs::NullClass>(0, num_downstream) {}

private:
    CountMin* cm_sketch;
    //FILE* output;
    unsigned long long int window;
    unsigned long long int byte;
    std::set<unsigned long long int> cand;
    unsigned long long int thresh;
    int r, w;

    void ComputeThreadInit() {
        Config* config = Config::getInstance();

        /*
        char* filename_str = config->getstring("outputFile", NULL);
        afs_assert(filename_str, "outputFile is not set\n");
        std::string filename = filename_str;
        filename += "_" + std::to_string(thr_id());
        output = fopen(filename.c_str(), "w");
        */

        r = config->getint("row", 0);
        afs_assert(r > 0, "row is not set\n");
        w = config->getint("width", 0);
        afs_assert(w > 0, "width is not set\n");
        double d = config->getdouble("sys.fault_tolerance.max_state_err", -1);

        cm_sketch = new CountMin(r, w, d);

        thresh = config->get_ull("thresh", -1);
        afs_assert(thresh>=0, "thresh is not set\n");

        window = config->get_ull("app.cm.window_bytes", 0);
    }

    void ComputeThreadFinish() {
        //TerminateWindow();
        //fclose(output);
        delete cm_sketch;
    }

    void ComputeThreadRecovery() {}

    void TerminateWindow() {
        //fprintf(output, "end window\n");
        //LOG_MSG("end window\n");
        
        CMItem* cm_item = new CMItem;
        for (int i=0; i<r; i++) {
            for (int j=0; j<w; j++) {
                long long v = cm_sketch->GetValue(i, j);
                if (v > 0) {
                    cm_item->AppendCounter(i*w+j, v);
                }
            }
        }
        for (auto x : cand) {
            cm_item->AppendHH(x);
        }

        EmitData(0, *cm_item);

        delete cm_item;

        byte = 0;
        cm_sketch->Clear();
        cand.clear();
    }

    void ProcessData(uint32_t worker, uint32_t thread, uint64_t seq, afs::RawItem& tuple) {
        char* line = tuple.raw_data;
        char* tok = strtok(line, " ");
        tok = strtok(NULL, " ");
        unsigned long long int key = strtoull (tok, NULL, 10);
        tok = strtok(NULL, " ");
        long long value =  strtoll (tok, NULL, 10);
            
        //LOG_MSG("Key %llu value %lld\n", key, value);
        unsigned long long sum = cm_sketch->UpdateSketch(key, value);
        //LOG_MSG(" sum: %llu %llu\n", key, sum);
        if (sum>thresh && cand.find(key)==cand.end()) {
            //fprintf(output, "%llu %llu\n", key, sum);
            cand.insert(key);
        }
        
        byte += value;
        //LOG_MSG("byte %ld\n", byte);
        if (byte > window) {
            TerminateWindow();
        }
    }

    void ProcessPunc() {}

};
