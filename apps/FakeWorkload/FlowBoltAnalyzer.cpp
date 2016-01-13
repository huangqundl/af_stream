#include "FlowBoltComputeThread.hpp"

#include <inttypes.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

extern "C" {
#include "ip_fragment.h"
}

#include "common.hpp"
#include "Config.hpp"
#include "Workload.hpp"

#define CLEAR_THRESHOLD 0

double first_ts;
void printFlow(FLOW& f) {
    char addr1[30];
    char addr2[30];
    printf("origip=%s respip=%s origport=%u respport=%u proto=%u workload: %" PRIu64 " start: %lf\n", 
        ip2a(f.field.orig_ip, addr1),
        ip2a(f.field.resp_ip, addr2),
        f.field.orig_port,
        f.field.resp_port,
        f.field.proto,
        f.field.o2r_byte_cnt+f.field.r2o_byte_cnt,
        f.field.flow_start_ts - (uint64_t)first_ts); 
}

/****************************************************************************
 * functions from class ComputeThreads
 ****************************************************************************/
void FlowBoltComputeThread::process(Packet::BasicField& bf) {
    count++;

    StartLinearWorkload(bf.ip_dlen, scale);
}

void FlowBoltComputeThread::finish() {
    LOG_MSG("ComputeThread %d (%d) processed %" PRIu64 "\n", subId, getpid(), count);
    ComputeThread<Packet::BasicField, FLOW>::finish();
}
    
#ifdef LOAD_BALANCE
int FlowBoltComputeThread::get_exported_data(Packet::BasicField& bf, char* data, uint32_t* len, uint32_t *key) {
    return 0;
}
    
int FlowBoltComputeThread::import_and_merge_data(char* data, uint32_t len) {
    return 0;
}
#endif

/****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * Constructor
 */
FlowBoltComputeThread::FlowBoltComputeThread(std::string name, int id) : ComputeThread<Packet::BasicField, FLOW>(name, id) {
    count = 0;

    Config *config = Config::getInstance();
    scale = config->getint("workloadScale", 1);
    if (id == 0) {
        LOG_MSG("FakeWorkload: scale %u\n", scale);
    }
}
    
void FlowBoltComputeThread::initialize() {
    ComputeThread<Packet::BasicField, FLOW>::initialize();
}
