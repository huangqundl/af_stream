#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "Daemon.hpp"
#include "SpoutDispatcher.hpp"
#include "DPIBoltSubComputeThread.hpp"
#include "DPIBoltMainComputeThread.hpp"
#include "Packet.hpp"
#include "HashFunc.hpp"
#include "LB/PacketFieldBalancer.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        LOG_MSG("Requires: Command [Config file] [Worker Name]\n");
        return -1;
    }

	// init configuration
    Config *config = NULL;
    if (argc > 1)
        config = Config::getInstance(argv[1], argv[2]);
    else
        config = Config::getInstance();

    char* zkServer = config->getstring("zookeeperServer", NULL);
    Daemon<Packet::TotalField, Packet::TotalField, Dummy>* daemon = new Daemon<Packet::TotalField, Packet::TotalField, Dummy>(argv[2], zkServer ? zkServer : "");

    int numSubComputeThread = config->get_num_sub_compute_threads();

    DPIBoltMainComputeThread mainComputeThread("BM1", numSubComputeThread);
    daemon->addMainComputeThread("BM1", &mainComputeThread);
	
    DPIBoltSubComputeThread** subComputeThreads = new DPIBoltSubComputeThread*[numSubComputeThread];
    for (int i = 0; i < numSubComputeThread; i++) {
        subComputeThreads[i] = new DPIBoltSubComputeThread("BS", i);
        subComputeThreads[i]->linkMainComputeThread(mainComputeThread.getQueueName(i));
        daemon->addSubComputeThread(subComputeThreads[i]->getQueueName(), subComputeThreads[i]);
    }
	
    char defaultName[] = "/opt/storm/trace.pcap.lq";
    string filename = config->getstring("pcapFile", defaultName);
    uint64_t mem_size = config->get_ull("memSize", 2000000000ULL);
    PacketFieldBalancer *lb = new PacketFieldBalancer();

    SpoutDispatcher dispatcher("BD1", filename, mem_size, lb);
    daemon->addDispatcher("BD1", &dispatcher);
    
    for (int i = 0; i < numSubComputeThread; i++) {
        dispatcher.addDataQueue(subComputeThreads[i]->getQueueName());
    }

#ifdef LOAD_BALANCE
    daemon->set_share_state(true);
#endif
    daemon->Start();

    return 0;
}
