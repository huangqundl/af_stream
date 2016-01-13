#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "Daemon.hpp"
#include "ZmqDispatcher.hpp"
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
	// init objects
    Config *config = NULL;
    if (argc > 1)
        config = Config::getInstance(argv[1], argv[2]);
    else
        config = Config::getInstance();

    char* zkServer = config->getstring("zookeeperServer", NULL);
    Daemon<Packet::TotalField, Packet::TotalField, Dummy>* daemon = new Daemon<Packet::TotalField, Packet::TotalField, Dummy>(argv[2], zkServer ? zkServer : "");

    int numSubComputeThread = config->get_num_sub_compute_threads();

    DPIBoltMainComputeThread mainComputeThread("DM1", numSubComputeThread);
    daemon->addMainComputeThread("DM1", &mainComputeThread);
	
    DPIBoltSubComputeThread** subComputeThreads = new DPIBoltSubComputeThread*[numSubComputeThread];
    for (int i = 0; i < numSubComputeThread; i++) {
        subComputeThreads[i] = new DPIBoltSubComputeThread("DS", i);
        subComputeThreads[i]->linkMainComputeThread(mainComputeThread.getQueueName(i));
        daemon->addSubComputeThread(subComputeThreads[i]->getQueueName(), subComputeThreads[i]);
    }

    char defaultAddress[] = "tcp://127.0.0.1:12346";
    PacketFieldBalancer *lb = new PacketFieldBalancer();
    ZmqDispatcher<Packet::TotalField> dispatcher("DD1", lb, true);
    int numAddr = config->getint("numInAddr", 1);
    for (int i=0; i<numAddr; ++i) {
        char key[20];
        sprintf(key, "inAddress%d", i);
        dispatcher.addAddr(config->getstring(key, defaultAddress));
        LOG_MSG("Connetct to %s\n", config->getstring(key, defaultAddress));
    }
    daemon->addDispatcher("DD1", &dispatcher);

    for (int i = 0; i < numSubComputeThread; i++) {
        dispatcher.addDataQueue(subComputeThreads[i]->getQueueName());
    }

#ifdef LOAD_BALANCE
    daemon->set_share_state(true);
#endif
    daemon->Start();

    return 0;
}
