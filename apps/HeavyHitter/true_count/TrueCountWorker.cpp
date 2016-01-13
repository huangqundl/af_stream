#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <string>

#include "Config.hpp"
#include "Util.hpp"
#include "worker.hpp"
#include "ZmqTimerDispatcher.hpp"
#include "../DetectCollector.hpp"
#include "../Router/DChoice.hpp"
#include "../Packet.hpp"
#include "TrueAnalyzer.hpp"

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
    Worker<Packet::BasicField, Dummy, Dummy>* daemon = new Worker<Packet::BasicField, Dummy, Dummy>(argv[2], zkServer ? zkServer : "");

    int numAnalyzer = config->getint("numSubAnalyzers", 1);

    DetectCollector mainAnalyzer("BM1", numAnalyzer);
    daemon->addCollector("BM1", &mainAnalyzer);
	
    TrueAnalyzer** subAnalyzers = new TrueAnalyzer*[numAnalyzer];
    for (int i = 0; i < numAnalyzer; i++) {
        subAnalyzers[i] = new TrueAnalyzer("BS", i);
        subAnalyzers[i]->linkCollector(mainAnalyzer.getQueueName(i));
        daemon->addAnalyzer(subAnalyzers[i]->getQueueName(), subAnalyzers[i]);
    }
	
    char defaultAddress[] = "tcp://127.0.0.1:12345";
    DChoice lb = DChoice(numAnalyzer, config->getint("DChoice", 1));
    ZmqTimerDispatcher<Packet::BasicField> dispatcher("BD1", &lb, false);
    int numAddr = config->getint("numInAddr", 1);
    for (int i=0; i<numAddr; ++i) {
        char key[20];
        sprintf(key, "inAddress%d", i);
        dispatcher.addAddr(config->getstring(key, defaultAddress));
        LOG_MSG("Connetct to %s\n", config->getstring(key, defaultAddress));
    }
    daemon->addDispatcher("BD1", &dispatcher);
    
    for (int i = 0; i < numAnalyzer; i++) {
        dispatcher.addDataQueue(subAnalyzers[i]->getQueueName());
    }

    daemon->run();

    return 0;
}
