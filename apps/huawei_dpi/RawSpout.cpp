#include <string>
#include "Util.hpp"
#include "RawPacket.hpp"
#include "Daemon.hpp"
#include "Config.hpp"
#include "RawSpoutDispatcher.hpp"
#include "ZmqMainComputeThread.hpp"
#include "LB/RawPacketBalancer.hpp"
#include "LoadBalancer/RoundRobin.hpp"

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
    Daemon<RawPacket, RawPacket, RawPacket>* daemon = new Daemon<RawPacket, RawPacket, RawPacket>(argv[2], zkServer ? zkServer : "");

    //RawPacketBalancer *lb = new RawPacketBalancer();
    RoundRobin<RawPacket> *lb = new RoundRobin<RawPacket>();
    ZmqMainComputeThread<RawPacket> mainComputeThread("RM1", 1, lb);
    daemon->addMainComputeThread("RM1", &mainComputeThread);
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
    uint64_t mem_size = config->get_ull("memSize", 2000000000ULL);

    RoundRobin<RawPacket> *rb = new RoundRobin<RawPacket>();
    RawSpoutDispatcher dispatcher("RD1", filename, mem_size, rb);
    daemon->addDispatcher("RD1", &dispatcher);

    dispatcher.addDataQueue(mainComputeThread.getQueueName(0));

    daemon->Start();

    return 0;
}
