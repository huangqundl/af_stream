#ifndef ANALYZER_PCAP_DECODER_HPP_
#define ANALYZER_PCAP_DECODER_HPP_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <linux/udp.h>
#include <linux/if_ether.h>
#include <pcap.h>
#include <string>

#include "ComputeThread.hpp"

#include "Packet.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "collector_result_writer.hpp"
#include "StFlow.hpp"

class DPIBoltSubComputeThread : public ComputeThread<Packet::TotalField, Packet::TotalField> {
    private:
        Packet p;
        StFlow* st_flow_;                    // flow table
        uint64_t count;
        void* pvHandle;
        unsigned int inspectId;

        // functions from SubComputeThread class
        void finish();
        void process(Packet::TotalField &);
        
        // private functions
        int APPEGNRegInit(const char *rule_lib);
        void packetInspect(struct ip *ip_hdr, int ip_len, FLOW *f);

    public:
        // public functions
        DPIBoltSubComputeThread(std::string name, int id);
        void initialize();
};

#endif // ANALYZER_PCAP_DECODER
