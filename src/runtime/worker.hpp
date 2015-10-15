#ifndef SAND_WORKER_HPP_
#define SAND_WORKER_HPP_

#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>

#include <map>
#include <vector>
#include <string>
#include <deque>

#include <zookeeper.h>

#include "in_thread.hpp"
#include "in_thread_net.hpp"
#include "analyzer.hpp"
#include "out_thread.hpp"
#include "queues/zerocopy_ringbuffer.hpp"
#include "util.hpp"
#include "wrap_item.hpp"

#include "fault_tolerance/state_manager.hpp"
#include "fault_tolerance/data_manager.hpp"

//using namespace std;

zhandle_t* zh = NULL;
std::string zk_server_name = "127.0.0.1:2181";
//clientid_t ZookeeperClient::myid;
//set<string> ZookeeperClient::upstream_workers;
//deque<ControlMsg> ZookeeperClient::remoteCmd;

/**
 * Parent process of a computation node, consisting of: one dispatcher,
 * one collector, multiple analyzers and one Zookeeper client
 * @tparam T1 output class of dispatcher, input class of analyzers
 * @tparam T2 output class of analyzers, input class of collector
 * @tparam T3 output class of collector, input class of next-hop dispatcher
 */

namespace afs {

template <class InT, class OutT, class RInT=NullClass, class ROutT=NullClass>
class Worker {

typedef WrapItem<InT> WInT;
typedef WrapItem<OutT> WOutT;
typedef WrapItem<RInT> WRInT;
typedef WrapItem<ROutT> WROutT;

private:
    int id_;
    std::string worker_name_;

    /**
     * number of analyzers (We supports multiple analyzers within one worker)
     */
    int num_analyzer_;

    /**
     * references to the analyzers
     */
    Analyzer<InT, OutT, RInT, ROutT>** analyzers_;

    InThread<InT, ROutT>* in_thread_;

    OutThread<OutT, RInT>* out_thread_;

    void WaitForChildTerminate();

    void EnableInput(int i);
    void EnableOutput(int i);
    void ConnectAnalyzers(int i);

    void ParseCPUAffinity(char* str);

    void* zmq_ctx;

    void ZeroQueueInit(const int io_threads) {
        if (zmq_ctx != NULL) {
            LOG_MSG("already_create\n");
            return;
        }

        zmq_ctx = zmq_ctx_new();
        if (zmq_ctx == 0)
            LOG_ERR("zmq_ctx_new(): %s\n", zmq_strerror(errno));
        if (zmq_ctx_set(zmq_ctx, ZMQ_IO_THREADS, io_threads) == -1)
            LOG_ERR("zmq_ctx_set(): %s\n", zmq_strerror(errno));
    }

    void ZeroQueueClean() {
        if (zmq_ctx_destroy(zmq_ctx) == -1)
            LOG_ERR("zmq_ctx_destroy(): %s\n", zmq_strerror(errno));
    }

public:
    /**
     * Constructor
     * @param worker_name The unique name of the worker
     * @param addr The IP address of Zookeeper server
     */
    Worker(string worker_name, int id, string zkAddr, int num_analyzer);

    /**
     * execution all components (dispatcher, analyzers, collector)
     */
    void run(bool recovery = false);

    /******************************************************************
     *
     * IMPORTANT Hint:
     * The following three functions add components to the Worker.
     * DON'T initialize resources when creating the class instances.
     * Allocate in their initialize() functions
     *
     ****************************************************************/

    void AddInThread(InThread<InT, ROutT>* in_thread);

    void AddOutThread(OutThread<OutT, RInT>* out_thread);

    /**
     * config Analyzer for the worker
     * @param name Unique name (within the worker) of the Analyzer
     * @param analyzer Reference to the added Analyzer
     */
    void AddAnalyzer(int index, Analyzer<InT, OutT, RInT, ROutT>* analyzer);

    /**
     * config Collector for the worker
     * @param name Unique name (within the worker) of the Collector
     * @param collector Reference to the added Collector
     */
    //void addCollector(string name, Collector<T2, T3>* collector);
};

/*********************************************************************
 *
 * Implementation
 *
 ********************************************************************/

template <class InT, class OutT, class RInT, class ROutT>
Worker<InT, OutT, RInT, ROutT>::Worker(string worker_name, int id, string zkAddr, int num_analyzer) :
    id_(id),
    worker_name_(worker_name),
    num_analyzer_(num_analyzer),
    analyzers_(NULL),
    in_thread_(NULL),
    out_thread_(NULL) {
    if (num_analyzer > 0) {
        analyzers_ = (Analyzer<InT, OutT, RInT, ROutT>**)calloc(num_analyzer, sizeof(Analyzer<InT, OutT, RInT, ROutT>*));
    }

    /*
    if (!addr.empty())
        ZookeeperClient::server_name = addr;
    zkClient = new ZookeeperClient(worker_name);
    */
}

template <class InT, class OutT, class RInT, class ROutT>
void Worker<InT, OutT, RInT, ROutT>::AddInThread(InThread<InT, ROutT>* in_thread) {
    in_thread_ = in_thread;
}

template <class InT, class OutT, class RInT, class ROutT>
void Worker<InT, OutT, RInT, ROutT>::AddOutThread(OutThread<OutT, RInT>* out_thread) {
    out_thread_ = out_thread;
}

template <class InT, class OutT, class RInT, class ROutT>
void Worker<InT, OutT, RInT, ROutT>::AddAnalyzer(int index, Analyzer<InT, OutT, RInT, ROutT>* analyzer) {
    afs_assert((index>=0 && index<num_analyzer_),
            "Add Analyzer: index error %d\n", index);
    analyzers_[index] = analyzer;
}

template <class InT, class OutT, class RInT, class ROutT>
void Worker<InT, OutT, RInT, ROutT>::WaitForChildTerminate() {
}

template <class InT, class OutT, class RInT, class ROutT>
void Worker<InT, OutT, RInT, ROutT>::ParseCPUAffinity(char* str) {
    // worker; analyzer; input; output

    std::vector<std::string> strings;
    char* tok = strtok(str, ";");
    while (tok) {
        strings.push_back(tok);
        tok = strtok(NULL, ";");
    }
    int size = strings.size();
    if (size > 0) {
        // worker
        int cpu = atoi(strings[0].c_str());
        pin_to_cpu(cpu);
    }
    if (size > 1) {
        size_t start = 0;
        size_t found = strings[1].find(',', start);
        int index = 0;
        while (found != std::string::npos) {
            int cpu = atoi(strings[1].substr(start, found-start).c_str());
            analyzers_[index++]->AssignCPU(cpu);
            start = found+1;
            found = strings[1].find(',', start);
        }
    }
    if (size > 2) {
        int cpu = atoi(strings[2].c_str());
        in_thread_->AssignCPU(cpu);
    }
    if (size > 3) {
        int cpu = atoi(strings[3].c_str());
        out_thread_->AssignCPU(cpu);
    }
}

template <class InT, class OutT, class RInT, class ROutT>
void Worker<InT, OutT, RInT, ROutT>::run(bool recovery) {

    ZeroQueueInit(1);

    StateManager::InitStateManagerContext(worker_name_);
    DataManager::InitDataManagerContext(worker_name_);

    LOG_SYS(HLINE "[Worker] Setup inter-thread channels...\n");

    LOG_MSG("    thread IDs...\n");
    in_thread_->set_tid(num_analyzer_);
    in_thread_->set_wid(id_);
    for (int i=0; i<num_analyzer_; i++) {
        analyzers_[i]->set_tid(i);
        analyzers_[i]->set_wid(id_);
    }
    if (out_thread_) {
        out_thread_->set_tid(num_analyzer_+1);
        out_thread_->set_wid(id_);
    }

    LOG_MSG("    CPU affinity...\n");
    Config* config = Config::getInstance();
    char* cpu_affinity = config->getstring("sys.cpu_affinity", NULL);
    if (cpu_affinity) {
        ParseCPUAffinity(cpu_affinity);
    }

    LOG_MSG("    inter-thread channels...\n");
    for (int i=0; i<num_analyzer_; i++) {
        analyzers_[i]->ConnectInThread(in_thread_);
    }
    if (out_thread_) {
        for (int i=0; i<num_analyzer_; i++) {
            analyzers_[i]->ConnectOutThread(out_thread_);
        }
    }

    if (recovery) {
        LOG_MSG("    Recovery mode...\n");
        in_thread_->EnableRecovery();
        for (int i=0; i<num_analyzer_; i++) {
            analyzers_[i]->AnalyzerRecovery();
        }
    }

    LOG_SYS(HLINE "[Worker] Create threads...\n");

    in_thread_->Create();
    in_thread_->WaitReady();

    for (int i=0; i<num_analyzer_; i++) {
        analyzers_[i]->Create();
        analyzers_[i]->WaitReady();
    }

    if (out_thread_) {
        out_thread_->Create();
        out_thread_->WaitReady();
    }

    LOG_SYS(HLINE "[Worker] Start running...\n");
    in_thread_->NotifyStart();
    for (int i=0; i<num_analyzer_; i++) {
        analyzers_[i]->NotifyStart();
    }
    if (out_thread_) {
        out_thread_->NotifyStart();
    }

    in_thread_->WaitFinish1();
    for (int i=0; i<num_analyzer_; i++) {
        analyzers_[i]->WaitFinish1();
    }
    if (out_thread_) {
        out_thread_->WaitFinish1();
    }

    LOG_SYS(HLINE "[Worker] Clean and output...\n");
    in_thread_->NotifyClean();
    in_thread_->WaitFinish2();
    for (int i=0; i<num_analyzer_; i++) {
        analyzers_[i]->NotifyClean();
        analyzers_[i]->WaitFinish2();
    }
    if (out_thread_) {
        out_thread_->NotifyClean();
        out_thread_->WaitFinish2();
    }

    LOG_SYS(HLINE "[Worker] Finish...\n");
    in_thread_->Destroy();
    for (int i=0; i<num_analyzer_; i++) {
        analyzers_[i]->Destroy();
    }
    if (out_thread_) {
        out_thread_->Destroy();
    }

    StateManager::CleanStateManagerContext();
    DataManager::CleanDataManagerContext();

    ZeroQueueClean();
}

}

#endif // SAND_WORKER_HPP_
