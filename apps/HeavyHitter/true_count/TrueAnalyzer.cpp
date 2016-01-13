/*
 * TrueAnalyzer.cpp
 * - TrueAnalyzer thread - management on the flow level
 */

#include <inttypes.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include "common.hpp"
#include "Config.hpp"
#include "TrueAnalyzer.hpp"

void TrueAnalyzer::printFlow(FLOW& f) {
    char addr1[30];
    char addr2[30];
    fprintf(fp, "origip=%s respip=%s event: %" PRIu64 " packet: %" PRIu64  " byte: %" PRIu64 " start: %lf\n", 
        ip2a(f.field.orig_ip, addr1),
        ip2a(f.field.resp_ip, addr2),
        f.field.event_cnt,
        f.field.pkt_cnt,
        f.field.byte_cnt,
        f.field.flow_start_ts - (uint64_t)first_ts); 
}

/****************************************************************************
 * functions from class Analyzers
 ****************************************************************************/
void TrueAnalyzer::detect() {
    std::vector<FLOW>* all_flows = st_flow_->get_all_flows();
    st_flow_->travel_flows();

    char buf[20];
    sprintf(buf, "_%d", printCount);
    std::string output_file = "true_"+this->getQueueName() + buf;
    fp = fopen(output_file.c_str(), "w");

    uint32_t size = all_flows->size();
    fprintf(fp, "count = %d, flows: %u, total packet: %" PRIu64  " total bytes: %" PRIu64 "\n", printCount++, size, total_pkt, total_byte);
    for (uint32_t i=0; i<size; ++i) {
        FLOW f = (*all_flows)[i];
        if (f.field.byte_cnt >= thresh*MB) {
            printFlow(f);
        }
    }
    fclose(fp);

    st_flow_->flush();
    fprintf(fp, "\n");
}

void TrueAnalyzer::timeoutHandler(Packet::BasicField& bf) {
    /*
    double cur_ts = bf.pkt_ts;
    if (cur_ts < last_ts) {
        LOG_MSG("Warning: time out of order\n");
    }
    */
    LOG_MSG("Timeout\n");

    detect();

    //last_ts = bf.pkt_ts;
    total_byte = 0;
    total_pkt = 0;
}
/****************************************************************************
 * functions from class Analyzers
 ****************************************************************************/
void TrueAnalyzer::process(Packet::BasicField& bf) {
    count++;
    /*
    if (count == 1) {
        first_ts = bf.pkt_ts;
        last_ts = bf.pkt_ts;
    }
    */

	p.get_bf() = bf;
    total_pkt += bf.accumulated_pkt;
    total_byte += bf.accumulated_byte;

    // update flow state
    FLOW* f = st_flow_->update_state(p);
    if (count == 1) {
        char addr1[30];
        LOG_MSG("origip=%s event: %" PRIu64 " packet: %" PRIu64  " byte: %" PRIu64 " start: %lf\n", 
            ip2a(f->field.orig_ip, addr1),
            f->field.event_cnt,
            f->field.pkt_cnt,
            f->field.byte_cnt,
            f->field.flow_start_ts - (uint64_t)first_ts); 
    }
   
    std::vector<FLOW>* unexported_flows = st_flow_->get_all_flows();
    if (unexported_flows->size() > 0) {
        LOG_MSG("Warning: hash table is too small\n");
        unexported_flows->clear();
    }

	p.reset();
}

void TrueAnalyzer::finish() {
    detect();

    LOG_MSG("Analyzer %d (%d) processed %" PRIu64 "\n", subId, getpid(), count);
    Analyzer<Packet::BasicField, Dummy>::finish();
}
    
#ifdef LOAD_BALANCE
int TrueAnalyzer::get_exported_data(Packet::BasicField& bf, char* data, uint32_t* len, uint32_t *key) {
    return 0;
}
    
int TrueAnalyzer::import_and_merge_data(char* data, uint32_t len) {
    return 0;
}
#endif

/****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * Constructor
 */
TrueAnalyzer::TrueAnalyzer(std::string name, int id) : Analyzer<Packet::BasicField, Dummy>(name, id) {
    count = 0;
    total_pkt = 0;
    total_byte = 0;

    printCount = 0;
    last_ts = 0;

    Config *config = Config::getInstance();

    printInterval = config->getint("printTimeInterval", 1);

    thresh = config->getdouble("thresh", 0);

    if (id == 0) {
        LOG_MSG("Display time interval: %u\n", printInterval);
        LOG_MSG("TrueCounter: flow table size: %u\n", config->getint("flowTableSize", 100000));
        LOG_MSG("TrueCounter: flow table timeout: %lf\n", config->getdouble("flowTableTimeout", 600));
        LOG_MSG("TrueCounter: thresh %lf MB\n", thresh);
    }
    st_flow_ = new StFlow(config->getint("flowTableSize", 100000), 
            config->getdouble("flowTableTimeout", 600));
}
    
void TrueAnalyzer::initialize() {
    st_flow_->create_table();

    Analyzer<Packet::BasicField, Dummy>::initialize();
}
