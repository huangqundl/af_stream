#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "Worker.hpp"
#include "Dispatcher/ZmqDispatcher.hpp"
#include "FlowBoltComputeThread.hpp"
#include "Collector/ZmqCollector.hpp"
#include "PacketFieldGrouping.hpp"
#include "Packet.hpp"
#include "Flow.hpp"

#if LOAD_BALANCE
    #if LOAD_BALANCE_FEEDBACK
        #include "FlowFieldGroupingFB.hpp"
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

    //FlowBoltCollector mainComputeThread("BM1", numComputeThread);
#if LOAD_BALANCE
#if LOAD_BALANCE_FEEDBACK
    FlowFieldGroupingFB ffg = FlowFieldGroupingFB(config->getint("initOutQueue", 1));
#else
    FlowFieldGrouping ffg = FlowFieldGrouping();
#endif
#else
    FlowFieldGrouping ffg = FlowFieldGrouping();
#endif
    ZmqCollector<FLOW> mainComputeThread("BM1", numComputeThread, &ffg);
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
	
    char defaultAddress[] = "tcp://127.0.0.1:12345";
    //FlowBoltDispatcher dispatcher("BD1", config->getstring("inAddress", defaultAddress));
    PacketFieldGrouping lb = PacketFieldGrouping();
    //ZmqDispatcher<Packet::TotalField> dispatcher("BD1", config->getstring("inAddress", defaultAddress), &lb);
    ZmqDispatcher<Packet::TotalField> dispatcher("BD1", &lb, false);
    int numAddr = config->getint("numInAddr", 1);
    for (int i=0; i<numAddr; ++i) {
        char key[20];
        sprintf(key, "inAddress%d", i);
        dispatcher.addAddr(config->getstring(key, defaultAddress));
        LOG_MSG("Connetct to %s\n", config->getstring(key, defaultAddress));
    }
    daemon->addDispatcher("BD1", &dispatcher);
    
    for (int i = 0; i < numComputeThread; i++) {
        dispatcher.addDataQueue(subComputeThreads[i]->getQueueName());
    }

    daemon->Start();

    return 0;
}
