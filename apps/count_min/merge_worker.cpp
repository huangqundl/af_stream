#include <stdlib.h>
#include <string>

#include "config.hpp"

#include "thread/worker.hpp"

#include "thread/up_thread_trace.hpp"
#include "thread/up_thread_trace_afs.hpp"
#include "thread/router_rr.hpp"

#include "merge_thread.hpp"
#include "thread/down_thread_net.hpp"

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

    // create worker
    int worker_id = config->getint("worker_id", -1);
    afs_assert(worker_id>=0, "worker id is not specified in config file\n");
    afs::Worker<struct CMItem, afs::NullClass, afs::NullClass, afs::NullClass>* worker = 
        new afs::Worker<struct CMItem, afs::NullClass, afs::NullClass, afs::NullClass>(std::string(worker_name), worker_id, std::string(""), num_compute_thread);

    char* addr = config->getstring("listen_addr", NULL);
    if (addr == NULL) {
        LOG_MSG("[listen addr] must be specified in the config file\n");
        return -1;
    }
    int num_in = config->getint("num_upstreams", 0);
    afs::UpThreadNet<struct CMItem, afs::NullClass>* i_thread =
        new afs::UpThreadNet<struct CMItem, afs::NullClass>(num_in, num_compute_thread);
    i_thread->AddSource(addr);
    worker->AddUpThread(i_thread);

    for (int i=0; i<num_compute_thread; i++) {
        afs::RouterRR* router_tmp = new afs::RouterRR(1);
        MergeThread* compute_thread =
            new MergeThread(num_in, router_tmp);
        worker->AddComputeThread(compute_thread);
    }

    // Start
    worker->Start(recovery);

    return 0;
}
