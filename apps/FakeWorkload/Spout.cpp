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
#include "LoadBalancer/PacketFieldGrouping.hpp"
#include "Packet.hpp"
#include "Flow.hpp"

#if LOAD_BALANCE
    #if LOAD_BALANCE_FEEDBACK
        #include "LoadBalancer/PacketFieldGroupingFB.hpp"
        #include "LoadBalancer/FlowQueueSizeGrouping.hpp"
        #include "LoadBalancer/FlowFieldGrouping.hpp"
        #include "LoadBalancer/RoundRobin.hpp"
        #include "LoadBalancer/PacketTokenGrouping.hpp"
        #include "LoadBalancer/PacketTokenGroupingFB.hpp"
        #include "LoadBalancer/PacketTokenGroupingMigrate.hpp"
        #include "LoadBalancer/PacketTokenGroupingMigrateFB.hpp"
    #else
        #include "LoadBalancer/FlowFieldGrouping.hpp"
    #endif
#else
    #include "LoadBalancer/FlowFieldGrouping.hpp"
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
    Worker<Packet::BasicField, Packet::BasicField, Packet::BasicField>* daemon = new Worker<Packet::BasicField, Packet::BasicField, Packet::BasicField>(argv[2], zkServer ? zkServer : "");

    int numComputeThread = config->get_num_sub_compute_threads();
    
    LoadBalancer<Packet::BasicField>* lb = NULL;
#if LOAD_BALANCE
#if LOAD_BALANCE_FEEDBACK
    char default_lb[] = "PacketFieldGrouping";
    string lb_type = config->getstring("CollectorLoadBalancer", default_lb);
    if (lb_type == "PacketFieldGrouping") {
        lb = new PacketFieldGrouping();
    }
    else if (lb_type == "PacketTokenGrouping") {
        lb = new PacketTokenGrouping();
    }
    else if (lb_type == "PacketTokenGroupingMigrate") {
        lb = new PacketTokenGroupingMigrate();
    }
    else if (lb_type == "PacketFieldGroupingFB") {
        lb = new PacketFieldGroupingFB();
    }
    else if (lb_type == "PacketTokenGroupingFB") {
        lb = new PacketTokenGroupingFB();
    }
    else if (lb_type == "PacketTokenGroupingMigrateFB") {
        lb = new PacketTokenGroupingMigrateFB();
    }
    else {
        lb = new PacketFieldGrouping();
    }
#else
    lb = new PacketFieldGrouping();
#endif
#else
    lb = new PacketFieldGrouping();
#endif
    ZmqCollector<Packet::BasicField> mainComputeThread("BM1", numComputeThread, lb);
    daemon->addCollector("BM1", &mainComputeThread);
    int numOutQueue = config->getint("numOutQueue", 1);
    for (int i = 0; i < numOutQueue; i++) {
        char key[20];
        sprintf(key, "outPort%d", i);
        char address[20];
        sprintf(address, "tcp://*:%d", config->getint(key, 12346));
        mainComputeThread.addDataQueue(address);
    }
	
    char defaultName[] = "/opt/storm/trace.pcap.lq";
    string filename = config->getstring("pcapFile", defaultName);
    unsigned long long int mem_size = config->get_ull("memSize", 2000000000ULL);

    SpoutDispatcher dispatcher("BD1", filename, mem_size);
    
    daemon->addDispatcher("BD1", &dispatcher);
    
    dispatcher.addDataQueue(mainComputeThread.getQueueName(0));

    daemon->Start();

    return 0;
}
