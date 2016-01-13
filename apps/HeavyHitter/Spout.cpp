#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "worker.hpp"
#include "SpoutDispatcher.hpp"
#include "TextDispatcher.hpp"
#include "SpoutCollector.hpp"
#include "Router/PacketFieldGrouping.hpp"
#include "Packet.hpp"

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

    int numComputeThread = config->getint("numSubComputeThreads", 1);
    
    Router<Packet::BasicField>* lb = NULL;
    lb = new PacketFieldGrouping();
    SpoutCollector collector("BM1", numComputeThread, lb);
    daemon->addCollector("BM1", &collector);
    int numOutQueue = config->getint("numOutQueue", 1);
    for (int i = 0; i < numOutQueue; i++) {
        char key[20];
        sprintf(key, "outPort%d", i);
        char address[20];
        sprintf(address, "tcp://*:%d", config->getint(key, 12346));
        collector.addDataQueue(address);
    }
	
    char defaultName[] = "~/default_trace";
    string filename = config->getstring("pcapFile", defaultName);
    unsigned long long int mem_size = config->get_ull("memSize", 2000000000ULL);

    Dispatcher<Packet::BasicField>* dispatcher;
    if (config->getboolean("readFromPreprocess", false)) {
        dispatcher = new TextDispatcher("BD1", filename, mem_size);
    }
    else {
        dispatcher = new SpoutDispatcher("BD1", filename, mem_size);
    }
    
    daemon->addDispatcher("BD1", dispatcher);
    
    dispatcher->addDataQueue(collector.getQueueName(0));

    daemon->Start();

    return 0;
}
