#include <string>
#include "Util.hpp"
#include "Daemon.hpp"
#include "Config.hpp"
#include "Packet.hpp"
#include "SpoutDispatcher.hpp"
#include "ZmqMainComputeThread.hpp"
#include "LB/PacketFieldBalancer.hpp"

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
    Daemon<Packet::TotalField, Packet::TotalField, Packet::TotalField>* daemon = new Daemon<Packet::TotalField, Packet::TotalField, Packet::TotalField>(argv[2], zkServer ? zkServer : "");

    PacketFieldBalancer *lb = new PacketFieldBalancer();
    ZmqMainComputeThread<Packet::TotalField> mainComputeThread("SM1", 1, lb);
    daemon->addMainComputeThread("SM1", &mainComputeThread);
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
    PacketFieldBalancer *lb2 = new PacketFieldBalancer();

    SpoutDispatcher dispatcher("SD1", filename, mem_size, lb2);
    daemon->addDispatcher("SD1", &dispatcher);

    dispatcher.addDataQueue(mainComputeThread.getQueueName(0));

    daemon->Start();

    return 0;
}
