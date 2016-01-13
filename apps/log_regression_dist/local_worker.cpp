#include <stdlib.h>
#include <string>

#include "config.hpp"

#include "worker.hpp"

#include "up_thread_trace.hpp"
#include "router_rr.hpp"

#include "local_compute_thread.hpp"

#include "down_thread_net.hpp"
#include "in_callback_simple.hpp"
#include "out_callback_simple.hpp"

//#include "down_thread_net.hpp"
//#include "out_callback_simple.hpp"

int main(int argc, char* argv[]) {
    // worker name is used to find out the corresponding config section in the config file
    if (argc != 4) {
        LOG_MSG("Requires: Command [Config file] [Worker Name] [Is Recovery]\n");
        return -1;
    }

    char* config_name = argv[1];
    char* worker_name = argv[2];
    int recovery = atoi(argv[3]);

    Config *config = Config::getInstance(config_name, worker_name);

    int num_compute_thread = config->getint("num_compute_threads", 0);
    if (num_compute_thread == 0) {
        LOG_MSG("[num_compute_threads] must be specified in the config file: at least 1\n");
        return -1;
    }
    int num_downstream = config->getint("num_downstreams", 0);

    // create worker
    int worker_id = config->getint("worker_id", -1);
    afs_assert(worker_id>=0, "worker id is not specified in config file\n");
    afs::Worker<afs::RawItem, afs::RawItem, afs::RawItem, afs::NullClass>* worker = 
        new afs::Worker<afs::RawItem, afs::RawItem, afs::RawItem, afs::NullClass>(std::string(worker_name), worker_id, std::string(""), num_compute_thread);

    afs::RouterRR* router = new afs::RouterRR(num_compute_thread);
    afs::UpThreadTrace<afs::RawItem>* i_thread =
        new afs::UpThreadTrace<afs::RawItem>(router);
    worker->AddUpThread(i_thread);

    for (int i=0; i<num_compute_thread; i++) {
        afs::RouterRR* router_tmp = NULL;
        if (num_downstream) {
            router_tmp = new afs::RouterRR(num_downstream);
        }
        ComputeThreadThread* compute_thread =
            new ComputeThreadThread(num_downstream, router_tmp);
        worker->AddComputeThread(i, compute_thread);
    }

    if (num_downstream) {
        afs::InCallbackSimple<afs::RawItem>* in_callback =
            new afs::InCallbackSimple<afs::RawItem>(num_downstream);
        std::vector<afs::OutCallbackBase*> callbacks;
        for (int i=0; i<num_downstream; i++) {
            afs::OutCallbackSimple<afs::RawItem>* callback =
                new afs::OutCallbackSimple<afs::RawItem>(num_compute_thread);
            callbacks.push_back(callback);
        }

        afs::DownThreadNet<afs::RawItem, afs::RawItem>* o_thread = new afs::DownThreadNet<afs::RawItem, afs::RawItem>(num_compute_thread, num_downstream, in_callback, callbacks);
        std::string prefix("downstream_addr");
        for (int i=0; i<num_downstream; i++) {
            char tmp[10];
            sprintf(tmp, "%d", i);
            std::string key = prefix + tmp;
            char* addr = config->getstring(key.c_str(), NULL);
            o_thread->AddDest(i, addr);
        }
        worker->AddDownThread(o_thread);
    }

    // Start
    worker->Start(recovery);

    return 0;
}
