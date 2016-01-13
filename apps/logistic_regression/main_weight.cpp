// Lisence Here

// Application with ROLL
// Assemble a single ROLL worker with dispatcher, compute_threads and collector
// Then Start!

#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"

#include "worker.hpp"
#include "mem_dispatcher.hpp"
#include "weight_collector.hpp"
#include "weighter.hpp"

#include "item.hpp"
#include "sum.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    // worker name is used to find out the corresponding config section in the config file
    if (argc != 4) {
        LOG_MSG("Requires: Command [Config file] [Worker Name] [Is Recovery]\n");
        return -1;
    }

	// init objects
    Config *config = NULL;
    config = Config::getInstance(argv[1], argv[2]);

    // read config paramaters
    char* zk_server = config->getstring("zookeeper_server", NULL);
    if (zk_server == NULL) {
        LOG_MSG("[zookeeper_server] must be specified in the config file\n");
        return -1;
    }
    int num_compute_thread = config->getint("num_compute_threads", 0);
    if (num_compute_thread == 0) {
        LOG_MSG("[num_compute_threads] must be specified in the config file: at least 1\n");
        return -1;
    }
    char* filename = config->getstring("trace_file", NULL);
    if (filename == NULL) {
        LOG_MSG("[trace_file] must be specified in the config file\n");
        return -1;
    }
    unsigned long long int mem_size = config->get_ull("mem_size", 0);
    if (mem_size == 0) {
        LOG_MSG("[mem_size] must be specified in the config file: at least 1\n");
        return -1;
    }

    //afs::Daemon::InitDaemonContext(argv[2]);

    // create worker
    Worker<Item, Sum, Sum>* worker =
        new Worker<Item, Sum, Sum>(argv[2], zk_server);

    // add collector
    WeightCollector collector("BM1", num_compute_thread);
    worker->addCollector("BM1", &collector);
	
    // add compute_threads
    Weighter** compute_threads = new Weighter*[num_compute_thread];
    for (int i = 0; i < num_compute_thread; i++) {
        compute_threads[i] = new Weighter("BS", i, config);
        compute_threads[i]->linkCollector(collector.getQueueName(i));
        worker->addComputeThread(compute_threads[i]->getQueueName(), compute_threads[i]);
    }
	
    // add dispatcher
    Dispatcher<Item>* dispatcher;
    dispatcher = new TextMemDispatcher("BD1", filename, mem_size);
    worker->addDispatcher("BD1", dispatcher);
    for (int i = 0; i < num_compute_thread; i++) {
        dispatcher->addDataQueue(compute_threads[i]->getQueueName());
    }

    // Start
    int recovery = atoi(argv[3]);
    worker->Start(recovery);

    return 0;
}
