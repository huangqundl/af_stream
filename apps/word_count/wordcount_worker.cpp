#include <stdlib.h>
#include <string>

#include "config.hpp"

#include "worker.hpp"

#include "up_thread_trace.hpp"
#include "up_thread_trace_afs.hpp"
#include "up_thread_trace_upbackup.hpp"
#include "up_thread_trace_minibatch.hpp"
#include "router_rr.hpp"

#include "wordcount_compute_thread.hpp"
#include "wordcount_compute_thread_minibatch.hpp"
#include "wordcount_compute_thread_upbackup.hpp"

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
    afs::Worker<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass>* worker = 
        new afs::Worker<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass>(std::string(worker_name), worker_id, std::string(""), num_compute_thread);


    //afs::AdapterDisk* adapter = new afs::AdapterDisk(std::string(filename));
    //afs::AdapterRAM* adapter = new afs::AdapterRAM();
    afs::RouterRR* router = new afs::RouterRR(num_compute_thread);
    //afs::RRParser* parser = new afs::RRParser(num_compute_thread);
    afs::UpThread<afs::RawItem, afs::NullClass>* i_thread = NULL;
    char* in_type = config->getstring("sys.component.inthread.type", NULL);
    afs_assert(in_type, "sys.component.inthread.type is not specified in config file\n");
    if (strcmp(in_type, "UpThread") == 0) {
        i_thread = new afs::UpThreadTrace<afs::RawItem>(router);
    }
    else if (strcmp(in_type, "UpThreadAFS") == 0) {
        i_thread = new afs::UpThreadTraceAFS<afs::RawItem>(router);
    }
    else if (strcmp(in_type, "UpThreadUpbackup") == 0) {
        i_thread = new afs::UpThreadTraceUpbackup<afs::RawItem>(num_compute_thread,router);
    }
    else if (strcmp(in_type, "UpThreadMinibatch") == 0) {
        i_thread = new afs::UpThreadTraceMinibatch<afs::RawItem>(num_compute_thread, router);
    }
    else {
        LOG_ERR("Unknown up_thread_type %s\n", in_type);
    }

    worker->AddUpThread(i_thread);

    for (int i=0; i<num_compute_thread; i++) {
        afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::NullClass>* compute_thread = NULL;
        if (strcmp(in_type, "UpThread")==0
                || strcmp(in_type, "UpThreadAFS")==0) {
            compute_thread = new ComputeThreadThread(0, NULL, 0, NULL);
        }
        else if (strcmp(in_type, "UpThreadMinibatch")==0) {
            afs::RouterRR* router = new afs::RouterRR(1);
            compute_thread = new ComputeThreadThreadMinibatch(0, NULL, 1, router);
        }
        else if (strcmp(in_type, "UpThreadUpbackup")==0) {
            afs::RouterRR* router = new afs::RouterRR(1);
            compute_thread = new ComputeThreadThreadUpbackup(0, NULL, 1, router);
        }
        worker->AddComputeThread(i, compute_thread);
    }

    // Start
    worker->Start(recovery);

    return 0;
}
