#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "Worker.hpp"
#include "Dispatcher/ZmqDispatcher.hpp"
#include "DPIBoltComputeThread.hpp"
#include "DPIBoltCollector.hpp"
#include "Flow.hpp"
#include "FlowFieldGrouping.hpp"

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
    Worker<FLOW, MainStateEvent, Dummy>* daemon = new Worker<FLOW, MainStateEvent, Dummy>(argv[2], zkServer ? zkServer : "");

    int numComputeThread = config->get_num_sub_compute_threads();

    DPIBoltCollector mainComputeThread("BM1", numComputeThread);
    daemon->addCollector("BM1", &mainComputeThread);
	
    DPIBoltComputeThread** subComputeThreads = new DPIBoltComputeThread*[numComputeThread];
    for (int i = 0; i < numComputeThread; i++) {
        subComputeThreads[i] = new DPIBoltComputeThread("BS", i);
        subComputeThreads[i]->linkCollector(mainComputeThread.getQueueName(i));
        daemon->addComputeThread(subComputeThreads[i]->getQueueName(), subComputeThreads[i]);
    }
	
    char defaultAddress[] = "tcp://127.0.0.1:12346";
    //DPIBoltDispatcher dispatcher("BD1", config->getstring("inAddress", defaultAddress));
    FlowFieldGrouping lb = FlowFieldGrouping();
    ZmqDispatcher<FLOW> dispatcher("BD1", &lb, true);
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

#ifdef LOAD_BALANCE
    daemon->set_share_state(true);
#endif
    daemon->Start();

    return 0;
}
