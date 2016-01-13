#include <stdlib.h>
#include <string>

#include "config.hpp"

#include "worker.hpp"

#include "up_thread_trace_error.hpp"
#include "router_rr.hpp"

#include "train_compute_thread_error.hpp"

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
    afs::UpThreadTraceError<afs::RawItem>* i_thread =
        new afs::UpThreadTraceError<afs::RawItem>(router);
    worker->AddUpThread(i_thread);

    for (int i=0; i<num_compute_thread; i++) {
        ComputeThreadThread* compute_thread =
            new ComputeThreadThread(0, NULL);
        worker->AddComputeThread(i, compute_thread);
    }

    //afs::DownThread<afs::RawItem, afs::RawItem>* o_thread = new afs::DownThreadMPSC(num_compute_thread);
    //worker->AddDownThread(o_thread);

    // Start
    worker->Start(recovery);

    return 0;
}
