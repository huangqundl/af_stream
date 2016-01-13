#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "Daemon.hpp"
#include "ZmqDispatcher.hpp"
#include "ZmqMainComputeThread.hpp"
#include "DecoderBoltComputeThread.hpp"
#include "Packet.hpp"
#include "RawPacket.hpp"
#include "LB/RawPacketBalancer.hpp"
#include "LB/PacketFieldBalancer.hpp"
#include "LoadBalancer/RoundRobin.hpp"

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
    Daemon<RawPacket, Packet::TotalField, Packet::TotalField>* daemon = new Daemon<RawPacket, Packet::TotalField, Packet::TotalField>(argv[2], zkServer ? zkServer : "");

    int numSubComputeThread = config->get_num_sub_compute_threads();

    PacketFieldBalancer *lb = new PacketFieldBalancer();
    ZmqMainComputeThread<Packet::TotalField> mainComputeThread("BM1", numSubComputeThread, lb);
    daemon->addMainComputeThread("BM1", &mainComputeThread);
    int numOutQueue = config->getint("numOutQueue", 1);
    for (int i = 0; i < numOutQueue; i++) {
        char key[20];
        sprintf(key, "outPort%d", i);
        char address[20];
        sprintf(address, "tcp://*:%d", config->getint(key, 12345));
        mainComputeThread.addDataQueue(address);
    }
	
    DecoderBoltComputeThread** subComputeThreads = new DecoderBoltComputeThread*[numSubComputeThread];
    for (int i = 0; i < numSubComputeThread; i++) {
        subComputeThreads[i] = new DecoderBoltComputeThread("BS", i);
        subComputeThreads[i]->linkMainComputeThread(mainComputeThread.getQueueName(i));
        daemon->addSubComputeThread(subComputeThreads[i]->getQueueName(), subComputeThreads[i]);
    }
	
    char defaultAddress[] = "tcp://127.0.0.1:12346";
    //RawPacketBalancer *rpb = new RawPacketBalancer();
    //ZmqDispatcher<RawPacket> dispatcher("BD1", rpb, true);
    RoundRobin<RawPacket> *rb = new RoundRobin<RawPacket>();
    ZmqDispatcher<RawPacket> dispatcher("BD1", rb, true);
    int numAddr = config->getint("numInAddr", 1);
    for (int i=0; i<numAddr; ++i) {
        char key[20];
        sprintf(key, "inAddress%d", i);
        dispatcher.addAddr(config->getstring(key, defaultAddress));
        LOG_MSG("Connetct to %s\n", config->getstring(key, defaultAddress));
    }
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
