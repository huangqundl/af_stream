#include <string>

#include "Util.hpp"
#include "Worker.hpp"
#include "Config.hpp"
#include "Packet.hpp"
#include "SpoutDispatcher.hpp"
#include "Collector/ZmqCollector.hpp"
#include "QueueSizeGrouping.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 3) {
        LOG_MSG("Requires: Command [Config file] [Worker Name]\n");
        return -1;
    }

    Config *config = NULL;
    if (argc > 1)
        config = Config::getInstance(argv[1], argv[2]);
    else
        config = Config::getInstance();

    char* zkServer = config->getstring("zookeeperServer", NULL);
    Worker<Packet::TotalField, Packet::TotalField, Packet::TotalField>* daemon = new Worker<Packet::TotalField, Packet::TotalField, Packet::TotalField>(argv[2], zkServer ? zkServer : "");

    //SpoutCollector mainComputeThread("M1");
    QueueSizeGrouping lb;
    ZmqCollector<Packet::TotalField> mainComputeThread("BM1", 1, &lb);
    daemon->addCollector("M1", &mainComputeThread);
    int numOutQueue = config->getint("numOutQueue", 1);
    for (int i = 0; i < numOutQueue; i++) {
        char key[20];
        sprintf(key, "outPort%d", i);
        char address[20];
        sprintf(address, "tcp://*:%d", config->getint(key, 12345));
        mainComputeThread.addDataQueue(address);
    }
	
    char defaultName[] = "/opt/storm/trace.pcap.lq";
    string filename = config->getstring("pcapFile", defaultName);
    unsigned long long int mem_size = config->get_ull("memSize", 2000000000ULL);

    SpoutDispatcher dispatcher("D1", filename, mem_size);
    daemon->addDispatcher("D1", &dispatcher);
    
    dispatcher.addDataQueue(mainComputeThread.getQueueName(0));

    daemon->Start();

    return 0;
}
