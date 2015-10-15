#include <stdlib.h>
#include <string>

#include "config.hpp"

#include "worker.hpp"
#include "dummy_item.hpp"

#include "in_thread_trace.hpp"
#include "router_rr.hpp"

#include "sample_analyzer1.hpp"

#include "out_thread_net.hpp"
#include "out_callback_simple.hpp"

int main(int argc, char* argv[]) {
    // worker name is used to find out the corresponding config section in the config file
    if (argc != 4) {
        LOG_MSG("Requires: Command [Config file] [Worker Name] [Is Recovery]\n");
        return -1;
    }

    char* config_name = argv[1];
    char* worker_name = argv[2];
    int recovery = atoi(argv[3]);

    Config *config = NULL;
    config = Config::getInstance(config_name, worker_name);

    int num_analyzer = config->getint("num_analyzers", 0);
    if (num_analyzer == 0) {
        LOG_MSG("[num_analyzers] must be specified in the config file: at least 1\n");
        return -1;
    }
    int num_downstream = config->getint("num_downstreams", 0);

    // create worker
    int worker_id = config->getint("worker_id", -1);
    afs_assert(worker_id>=0, "worker id is not specified in config file\n");
    afs::Worker<DummyType, DummyType>* worker = 
        new afs::Worker<DummyType, DummyType>(std::string(worker_name), worker_id, std::string(""), num_analyzer);

    char* filename = config->getstring("trace_file", NULL);
    if (filename == NULL) {
        LOG_MSG("[trace_file] must be specified in the config file\n");
        return -1;
    }

    //afs::AdapterDisk* adapter = new afs::AdapterDisk(std::string(filename));
    //AdapterDummy* adapter = new AdapterDummy();
    afs::RouterRR* router = new afs::RouterRR(num_analyzer);
    //afs::RRParser* parser = new afs::RRParser(num_analyzer);
    afs::InThreadTrace<DummyType>* i_thread =
        new afs::InThreadTrace<DummyType>(router);
    worker->AddInThread(i_thread);

    for (int i=0; i<num_analyzer; i++) {
        afs::RouterRR* router_tmp = NULL;
        if (num_downstream) {
            router_tmp = new afs::RouterRR(num_downstream);
        }
        AnalyzerThread1* analyzer =
            new AnalyzerThread1(num_downstream, router_tmp);
        worker->AddAnalyzer(i, analyzer);
    }

    if (num_downstream) {
        std::vector<afs::OutCallbackBase*> callbacks;
        for (int i=0; i<num_downstream; i++) {
            afs::OutCallbackSimple<DummyType>* callback =
                new afs::OutCallbackSimple<DummyType>(num_analyzer);
            callbacks.push_back(callback);
        }

        afs::OutThreadNet<DummyType, afs::NullClass>* o_thread
            = new afs::OutThreadNet<DummyType, afs::NullClass>(num_analyzer, num_downstream, NULL, callbacks);
        std::string prefix("downstream_addr");
        for (int i=0; i<num_downstream; i++) {
            char tmp[10];
            sprintf(tmp, "%d", i);
            std::string key = prefix + tmp;
            char* addr = config->getstring(key.c_str(), NULL);
            o_thread->AddDest(i, addr);
        }
        worker->AddOutThread(o_thread);
    }

    // run
    worker->run(recovery);

    return 0;
}
