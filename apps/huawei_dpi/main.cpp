// Lisence Here

// Application to decode packets into key/value pairs
// Assemble a single worker with dispatcher, compute_threads and collector
// Then Start!

#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"

#include "Worker.hpp"
#include "SpoutDispatcher.hpp"
#include "compute_thread_pcap_decoder.hpp"
#include "collector_result_writer.hpp"

#include "Packet.hpp"
#include "router/PacketFieldBalancer.hpp"

int main(int argc, char* argv[]) {

    // worker name is used to find out the corresponding config section in the config file
    if (argc != 3) {
        LOG_MSG("Requires: Command [Config file] [Worker Name]\n");
        return -1;
    }

	// init configuration
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
    unsigned long long int mem_size = config->get_ull("memSize", 0);
    if (mem_size == 0) {
        LOG_MSG("[mem_size] must be specified in the config file: at least 1\n");
        return -1;
    }

    // create worker
    Worker<Packet::TotalField, Packet::TotalField, Dummy>* worker =
        new Worker<Packet::TotalField, Packet::TotalField, Dummy>(argv[2], zk_server);

    // add collector
    DPIBoltMainComputeThread mainComputeThread("BM1", num_compute_thread);
    worker->addCollector("BM1", &mainComputeThread);
	
    // add compute_threads
    DPIBoltSubComputeThread** subComputeThreads = new DPIBoltSubComputeThread*[num_compute_thread];
    for (int i = 0; i < num_compute_thread; i++) {
        subComputeThreads[i] = new DPIBoltSubComputeThread("BS", i);
        subComputeThreads[i]->linkCollector(mainComputeThread.getQueueName(i));
        worker->addComputeThread(subComputeThreads[i]->getQueueName(), subComputeThreads[i]);
    }
	
    // add dispatcher
    PacketFieldBalancer *lb = new PacketFieldBalancer();
    SpoutDispatcher dispatcher("BD1", filename, mem_size, lb);
    worker->addDispatcher("BD1", &dispatcher);
    for (int i = 0; i < num_compute_thread; i++) {
        dispatcher.addDataQueue(subComputeThreads[i]->getQueueName());
    }

    // Start
    worker->Start();

    return 0;
}
