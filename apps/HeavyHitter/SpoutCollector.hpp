/*
 * SpoutCollector.hpp
 * in heavy hitter detection
 */

#ifndef __SpoutCollector_hpp__
#define __SpoutCollector_hpp__

#include "common.hpp"
#include "Collector/Collector.hpp"
#include "WrapEvent.hpp"
#include "Config.hpp"
#include "Random.hpp"
#include "HashFunc.hpp"
//#include "HashTable.hpp"
#include <map>

/**
 * Collector to distribute packets to DetectWorker
 */
class SpoutCollector : public Collector<Packet::BasicField, Packet::BasicField> {

private:
    /**
     * Structure of flows
     */
    typedef struct {
        /**
         * source IP address
         */
        uint32_t orig_ip;

        /**
         * destination IP address
         */
        uint32_t resp_ip;

        /**
         * number of packets in the flow
         */
        uint64_t accumulated_pkt;

        /**
         * number of bytes in the flow
         */
        uint64_t accumulated_byte;
    } HASH_FLOW;

    /**
     * a small cache to enhance LD-Sketch by reordering keys
     */
    map<uint64_t, HASH_FLOW> flow_table;

    /**
     * expire strategy: memory efficient heuristic
     */
    unsigned int memory_first;

    /**
     * expire strategy: accuracy efficient heuristic
     */
    unsigned int accuracy_first;

    /**
     * counter of output event
     */
    uint64_t seq_count;

    /**
     * number of choices
     */
    unsigned int d;

    /**
     * select one key from hash table to emit
     */
    void expire() {
        map<uint64_t, HASH_FLOW>::iterator it;
        uint64_t key=0, byte=0;
        for (it=flow_table.begin(); it!=flow_table.end(); ++it) {
            if (key==0 || (memory_first && it->second.accumulated_byte<byte) || (accuracy_first && it->second.accumulated_byte>byte)) {
                key = it->first;
                byte = it->second.accumulated_byte;
            }
        }
        
        Packet::BasicField bf;
        memset(&bf, 0, sizeof(Packet::BasicField));
        bf.src_ip = flow_table[key].orig_ip;
        bf.dst_ip = flow_table[key].resp_ip;
        bf.accumulated_pkt = flow_table[key].accumulated_pkt;
        bf.accumulated_byte = flow_table[key].accumulated_byte;
        bf.seq = seq_count++;
        bf.pre_assign = -1;
        char addr1[30], addr2[30];
        printf("    expire %s %s %lu\n", ip2a(bf.src_ip, addr1), ip2a(bf.dst_ip, addr2), bf.accumulated_byte);
        my_emit(bf);

        flow_table.erase(key);
    }

    /**
     * counter of processed packets
     */
    unsigned int count;

    /**
     * instance of output event
     */
    WrapEvent<Packet::BasicField> tuple;

    /**
     * size of cache
     */
    unsigned int max_cache;

    /**
     * Divide a aggregated key to multiple workers
     * @param bf the event to emit
     */
    void my_emit(Packet::BasicField& bf) {
        int old_index = -1;
        int queue_index = this->router->balance(&bf, &old_index);

        if (bf.accumulated_pkt == 1 || d==1) {
            this->emit(&bf, queue_index, E_NORMAL);
        }
        else {
            unsigned int quo = bf.accumulated_byte / d;
            unsigned int r = bf.accumulated_byte % d;
            for (unsigned int i=0; i<d; ++i) {
                bf.accumulated_pkt = 1;
                bf.accumulated_byte = quo + ( i<r ? 1 : 0);
                bf.pre_assign = i;
                this->emit(&bf, queue_index, E_NORMAL);
            }
        }
    }

    void process(Packet::BasicField& bf) {
        count++;
        //last_ts = bf.pkt_ts;
        char addr1[30], addr2[30];
        printf("%s %s %lu\n", ip2a(bf.src_ip, addr1), ip2a(bf.dst_ip, addr2), bf.accumulated_byte);

        if (max_cache == 0) {
            my_emit(bf);
            return;
        }

        uint64_t key;
        HASH_FLOW* key_f = (HASH_FLOW*)(&key);    // flow that stores the key
        key_f->orig_ip = bf.src_ip;
        key_f->resp_ip = bf.dst_ip;

        if (flow_table.find(key) == flow_table.end()) {
            if (flow_table.size() == max_cache) {
                if (accuracy_first) {
                    uint64_t size = flow_table.size();
                    for (unsigned int i=0; i<size; ++i) {
                        expire();
                    }
                }
                else {
                    expire();
                }
            }
            flow_table[key].orig_ip = bf.src_ip;
            flow_table[key].resp_ip = bf.dst_ip;
            flow_table[key].accumulated_pkt = bf.accumulated_pkt; // should be 1
            flow_table[key].accumulated_byte = bf.accumulated_byte; // should be ip_len
            printf("    new add\n");
        }
        else {
            flow_table[key].accumulated_pkt += 1;
            flow_table[key].accumulated_byte += bf.accumulated_byte;
        }
    }

    void timeoutHandler(Packet::BasicField& bf) {
        LOG_MSG("Timeout event %u\n", count);
        uint64_t size = flow_table.size();
        for (unsigned int i=0; i<size; ++i) {
            expire();
        }
        flow_table.clear();
        for (unsigned int i=0; i<this->dataQueue.size(); ++i) {
            this->emit(&bf, i, E_TIMEOUT);
        }
    }

public:
    /**
     * Constructor
     * @param name name of the Dispatcher
     * @param num number of ComputeThreads
     * @param lb the load balancer used
     */
    SpoutCollector(std::string name, int num, Router<Packet::BasicField>* lb) : Collector<Packet::BasicField, Packet::BasicField>(name, num, lb) {
        count = 0;
        Config *config = Config::getInstance();
        seq_count = 0;
        d = config->getint("DChoice", 1);

        max_cache = config->getint("maxCache", 10000);
        char default_str[] = "none";
        string strategy = config->getstring("strategy", default_str);
        if (strategy=="memory") {
            memory_first = 1;
            accuracy_first = 0;
        }
        else if (strategy=="accuracy") {
            memory_first = 0;
            accuracy_first = 1;
        }
        else {
            max_cache = 0;
            memory_first = 0;
            accuracy_first = 0;
        }
    }

    void finish() {
        Collector<Packet::BasicField, Packet::BasicField>::finish();
    }

    void initialize() {
        //LOG_MSG("SpoutCollector: cache allocated %u\n", flow_table_size_);
        LOG_MSG("SpoutCollector: cache strategy - memory first %u accuracy first %u\n", memory_first, accuracy_first);
        LOG_MSG("SpoutCollector: max cache size %u\n", max_cache);
        LOG_MSG("SpoutCollector: DChoice - %u\n", d);

        Collector<Packet::BasicField, Packet::BasicField>::initialize();
    }
    
    void run() {
        LOG_MSG("SpoutCollector (%d) start to run\n", getpid());

        int i = 0;
        int finishCount = 0;
        ControlMsg msg;
        bool* flag = new bool[this->numComputeThread];
        memset(flag, 0, this->numComputeThread * sizeof(bool));

        while (finishCount < this->numComputeThread) {
		    while (this->queue[i]->Read(&tuple) != 0) {
			    i = i+1 < this->numComputeThread ? i+1 : 0;
		    }
            if (tuple.type == E_TIMEOUT) {
                timeoutHandler(tuple.data);
            }
            else if (tuple.type != E_FINISH) {
                process(tuple.data);
            }
            else {
                if (!flag[i]) {
                    finishCount++;
                    flag[i] = true;
                }
            }
        }

        // output residual flows
        uint64_t size = flow_table.size();
        for (unsigned int i=0; i<size; ++i) {
            expire();
        }
        flow_table.clear();
        for (unsigned int i = 0; i < this->dataQueue.size(); i++) {
            this->emit(NULL, i, E_FINISH);
        }

        make_end_msg(&msg);
        while (this->controlQueue->Write(&msg)==-1);

        delete [] flag;
        finish();
    }

};

#endif
