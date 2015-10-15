#include <stdlib.h>
#include <string>

#include "config.hpp"

#include "worker.hpp"
#include "dummy_item.hpp"
#include "in_callback_simple.hpp"
#include "out_callback_simple.hpp"

#include "in_thread_net.hpp"
#include "router_rr.hpp"

#include "bi_analyzer2.hpp"

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
    afs::Worker<DummyType, DummyType, afs::NullClass, DummyType>* worker = 
        new afs::Worker<DummyType, DummyType, afs::NullClass, DummyType>
        (std::string(worker_name), worker_id, std::string(""), num_analyzer);

    char* addr = config->getstring("listen_addr", NULL);
    if (addr == NULL) {
        LOG_MSG("[listen addr] must be specified in the config file\n");
        return -1;
    }

    int num_in = config->getint("num_upstreams", 0);
    afs::InCallbackSimple<DummyType>* in_callback =
        new afs::InCallbackSimple<DummyType>(num_in);
    std::vector<afs::OutCallbackBase*> callbacks;
    for (int i=0; i<num_in; i++) {
        afs::OutCallbackSimple<DummyType>* callback =
            new afs::OutCallbackSimple<DummyType>(num_analyzer);
        callbacks.push_back(callback);
    }
    afs::InThreadNet<DummyType, DummyType>* i_thread =
        new afs::InThreadNet<DummyType, DummyType>(in_callback, &callbacks);
    i_thread->AddSource(addr);
    worker->AddInThread(i_thread);

    for (int i=0; i<num_analyzer; i++) {
        afs::RouterRR* router_tmp = new afs::RouterRR(num_in);
        AnalyzerThread2* analyzer =
            new AnalyzerThread2(num_in, router_tmp);
        worker->AddAnalyzer(i, analyzer);
    }

    // run
    worker->run(recovery);

    return 0;
}
