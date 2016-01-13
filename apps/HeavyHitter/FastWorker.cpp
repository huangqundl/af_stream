#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "worker.hpp"
#include "SpoutDispatcher.hpp"
#include "TextMemDispatcher.hpp"
#include "DetectCollector.hpp"
#include "Router/DChoice.hpp"
#include "Packet.hpp"
#include "HeavyKeyComputeThread.hpp"

using namespace std;

// integrate the two workers (SpoutWorker and DetectWorker into a single one)

int main(int argc, char* argv[]) {
    if (argc != 3) {
        LOG_MSG("Requires: Command [Config file] [Worker Name]\n");
        return -1;
    }
	// init objects
    Config *config = NULL;
    if (argc > 1)
        config = Config::getInstance(argv[1], argv[2]);
    else
        config = Config::getInstance();

    char* zkServer = config->getstring("zookeeperServer", NULL);
    Worker<Packet::BasicField, Dummy, Dummy>* daemon = new Worker<Packet::BasicField, Dummy, Dummy>(argv[2], zkServer ? zkServer : "");

    int numComputeThread = config->getint("numSubComputeThreads", 1);

    DetectCollector collector("BM1", numComputeThread);
    daemon->addCollector("BM1", &collector);
	
    HeavyKeyComputeThread** compute_threads = new HeavyKeyComputeThread*[numComputeThread];
    for (int i = 0; i < numComputeThread; i++) {
        compute_threads[i] = new HeavyKeyComputeThread("BS", i);
        compute_threads[i]->linkCollector(collector.getQueueName(i));
        daemon->addComputeThread(compute_threads[i]->getQueueName(), compute_threads[i]);
    }
	
    char defaultName[] = "/opt/storm/trace.pcap.lq";
    string filename = config->getstring("pcapFile", defaultName);
    unsigned long long int mem_size = config->get_ull("memSize", 2000000000ULL);
    DChoice lb = DChoice(numComputeThread, config->getint("DChoice", 1));
    Dispatcher<Packet::BasicField>* dispatcher;
    if (config->getboolean("readFromPreprocess", false)) {
        dispatcher = new TextMemDispatcher("BD1", filename, mem_size);
    }
    else {
        dispatcher = new SpoutDispatcher("BD1", filename, mem_size);
    }
    daemon->addDispatcher("BD1", dispatcher);
    
    for (int i = 0; i < numComputeThread; i++) {
        dispatcher->addDataQueue(compute_threads[i]->getQueueName());
    }

    daemon->Start();

    return 0;
}
