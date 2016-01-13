/*
 * DPIBoltComputeThread.hpp
 * - DPIBoltComputeThread thread - management on the flow level
 */

#ifndef __DPIBoltComputeThread_hpp__
#define __DPIBoltComputeThread_hpp__

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

#include "ipq_api.h"

#include "../../ComputeThread/ComputeThread.hpp"
#include "common.hpp"
#include "HashFunc.hpp"
#include "HashTable.hpp"
#include "Flow.hpp"
#include "DPIBoltCollector.hpp"

#define	MAX_OSDPI_IDS 500000
#define	MAX_OSDPI_FLOWS 2000000

typedef struct {
    uint64_t pkt_cnt;
    uint64_t byte_cnt;
    uint64_t protocol_counter[IPOQUE_MAX_SUPPORTED_PROTOCOLS + 1];
    uint64_t protocol_counter_bytes[IPOQUE_MAX_SUPPORTED_PROTOCOLS + 1];
} DpiStat;

class DPIBoltComputeThread : public ComputeThread<FLOW, MainStateEvent> {
    private:
        // event count
        uint64_t count;

        //Packet p;
        FLOW f;

        // detection
        struct ipoque_detection_module_struct *ipoque_struct;
        u32 detection_tick_resolution;

        // results
        uint64_t ip_packet_count;
        uint64_t total_bytes;

        // id tracking
        struct osdpi_ip_addr {
	        u32 is_ip_v6;
            union {
                u32 ipv4;
		        struct in6_addr ipv6;
            };
        };

        typedef struct osdpi_id {
            struct osdpi_ip_addr ip;
            // qhuang: struct ipoque_id_struct *ipoque_id;
            // the size of struct ipoque_id_struct should be determined in runtime
            // the size in X86_64 is 312, so we allocate 400 here
            unsigned char ipoque_id[400];
        } osdpi_id_t;

        u32 size_id_struct;
        HashTable<struct osdpi_id>* osdpi_ids;
        uint32_t id_table_size;
        double id_table_timeout;
        u32 osdpi_id_count;

        static unsigned int id_hash_fcn(const struct osdpi_id *);
        static bool id_cmp_fcn(const struct osdpi_id *, const struct osdpi_id *);
        static void id_init_fcn(struct osdpi_id *, void*); 
        static void id_free_fcn(struct osdpi_id *, void*); 
        static void id_travel_do_fcn(struct osdpi_id *, void*); 

        // flow tracking
        typedef struct osdpi_flow {
            struct ipoque_unique_flow_ipv4_and_6_struct key;
            // qhuang: struct ipoque_flow_struct *ipoque_flow;
            // the size of struct ipoque_id_struct should be determined in runtime
            // the size in X86_64 is 120, so we allocate 200 here
            unsigned char ipoque_flow[200];

            // result only, not used for flow identification
            u32 detected_protocol;
            uint64_t pkt_cnt;
            uint64_t byte_cnt;
            uint8_t is_in_coordinator;
        } osdpi_flow_t;

        u32 size_flow_struct;
        HashTable<struct osdpi_flow>* osdpi_flows;
        uint32_t flow_table_size;
        double flow_table_timeout;
        u32 osdpi_flow_count;

        static unsigned int flow_hash_fcn(const struct osdpi_flow *);
        static bool flow_cmp_fcn(const struct osdpi_flow *, const struct osdpi_flow *);
        static void flow_init_fcn(struct osdpi_flow *, void*); 
        static void flow_free_fcn(struct osdpi_flow *, void*); 
        static void flow_travel_do_fcn(struct osdpi_flow *, void*); 

        // functions from ComputeThread class
        void finish();
        void process(FLOW&);
        
        // private functions
        unsigned int new_flow_processing(const uint64_t time, const struct iphdr *iph, uint16_t ipsize, uint16_t rawsize, uint32_t* ret);
        
        void* get_id(const struct osdpi_ip_addr *ip);
        struct osdpi_flow* get_osdpi_flow(const struct iphdr *iph, u16 ipsize);
        struct osdpi_flow* get_existed_osdpi_flow(const FLOW* flow);
        struct osdpi_flow* insert_osdpi_flow(const FLOW* flow);

        void printResults(void);
        uint64_t failed_lookup_event;

        #ifdef LOAD_BALANCE
        int get_exported_data(FLOW&, ControlMsg* control_msg);
        int import_and_merge_data(char* data, uint32_t len);
        void process_rebalanced_tuple(FLOW&);
        uint32_t get_state_key(FLOW& f, ControlMsg* control_msg) {
            //unsigned int a = f.field.orig_ip + f.field.resp_ip;
            //unsigned short b = f.field.orig_port + f.field.resp_port;
            sprintf(control_msg->key, "dpi%u,%u,%u,%u,%u", f.field.orig_ip, f.field.resp_ip, f.field.orig_port, f.field.resp_port, f.field.proto);
            control_msg->key_length = strlen(control_msg->key);
            return 0;
        }
        #endif

    public:
        // public functions
        DPIBoltComputeThread(std::string name, int id);
        void initialize();
};

#endif
