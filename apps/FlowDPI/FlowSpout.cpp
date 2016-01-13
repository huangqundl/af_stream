#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "Worker.hpp"
#include "SpoutDispatcher.hpp"
#include "FlowBoltComputeThread.hpp"
#include "ZmqCollector.hpp"
#include "PacketFieldGrouping.hpp"
#include "Packet.hpp"
#include "Flow.hpp"

#if LOAD_BALANCE
    #if LOAD_BALANCE_FEEDBACK
        #include "FlowFieldGroupingFB.hpp"
        #include "FlowQueueSizeGrouping.hpp"
        #include "FlowFieldGrouping.hpp"
        #include "FlowHSGrouping.hpp"
        #include "RoundRobin.hpp"
        #include "FlowTokenGrouping.hpp"
        #include "FlowTokenGroupingMigrate.hpp"
    #else
        #include "FlowFieldGrouping.hpp"
    #endif
#else
    #include "FlowFieldGrouping.hpp"
#endif

using namespace std;

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
    Worker<Packet::TotalField, FLOW, FLOW>* daemon = new Worker<Packet::TotalField, FLOW, FLOW>(argv[2], zkServer ? zkServer : "");

    int numComputeThread = config->get_num_sub_compute_threads();
    
    LoadBalancer<FLOW>* lb = NULL;
#if LOAD_BALANCE
#if LOAD_BALANCE_FEEDBACK
    char default_lb[] = "FlowFieldGrouping";
    string lb_type = config->getstring("CollectorLoadBalancer", default_lb);
    if (lb_type == "FlowFieldGrouping") {
        lb = new FlowFieldGrouping();
    }
    else if (lb_type == "FlowFieldGroupingFB") {
        lb = new FlowFieldGroupingFB(config->getint("initOutQueue", 1));
    }
    else if (lb_type == "FlowQueueSizeGrouping") {
        lb = new FlowQueueSizeGrouping();
    }
    else if (lb_type == "FlowHSGrouping") {
        lb = new FlowHSGrouping();
    }
    else if (lb_type == "RoundRobinGrouping") {
        lb = new RoundRobinGrouping();
    }
    else if (lb_type == "FlowTokenGrouping") {
        lb = new FlowTokenGrouping();
    }
    else if (lb_type == "FlowTokenGroupingMigrate") {
        lb = new FlowTokenGroupingMigrate();
    }
    else {
        lb = new FlowFieldGrouping();
    }
#else
    lb = new FlowFieldGrouping();
#endif
#else
    lb = new FlowFieldGrouping();
#endif
    ZmqCollector<FLOW> mainComputeThread("BM1", numComputeThread, lb);
    daemon->addCollector("BM1", &mainComputeThread);
    int numOutQueue = config->getint("numOutQueue", 1);
    for (int i = 0; i < numOutQueue; i++) {
        char key[20];
        sprintf(key, "outPort%d", i);
        char address[20];
        sprintf(address, "tcp://*:%d", config->getint(key, 12346));
        mainComputeThread.addDataQueue(address);
    }
	
    FlowBoltComputeThread** subComputeThreads = new FlowBoltComputeThread*[numComputeThread];
    for (int i = 0; i < numComputeThread; i++) {
        subComputeThreads[i] = new FlowBoltComputeThread("BS", i);
        subComputeThreads[i]->linkCollector(mainComputeThread.getQueueName(i));
        daemon->addComputeThread(subComputeThreads[i]->getQueueName(), subComputeThreads[i]);
    }
	
    char defaultName[] = "/opt/storm/trace.pcap.lq";
    string filename = config->getstring("pcapFile", defaultName);
    unsigned long long int mem_size = config->get_ull("memSize", 2000000000ULL);

    SpoutDispatcher dispatcher("BD1", filename, mem_size);
    
    daemon->addDispatcher("BD1", &dispatcher);
    
    for (int i = 0; i < numComputeThread; i++) {
        dispatcher.addDataQueue(subComputeThreads[i]->getQueueName());
    }

    daemon->Start();

    return 0;
}
