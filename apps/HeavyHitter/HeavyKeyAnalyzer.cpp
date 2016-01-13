/*
 * HeavyKeyComputeThread.cpp
 * - HeavyKeyComputeThread thread - management on the flow level
 */

#include <inttypes.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include "Common/common.hpp"
#include "Config.hpp"
#include "HeavyKeyComputeThread.hpp"
#include "deltoid/Deltoid.hpp"
#include "count_base/CountBase.hpp"
#include "ld_sketch/LDSketch.hpp"
#include "seqhash/Seqhash.hpp"
#include "fastsketch/FastSketch.hpp"

void HeavyKeyComputeThread::wrap_detect() {
    char buf[20];
    sprintf(buf, "_%d", printCount);
    std::string output_file = this->getQueueName() + buf;

    detector->detect(thresh * MB, output_file);
}

/****************************************************************************
 * functions from class ComputeThreads
 ****************************************************************************/

void HeavyKeyComputeThread::timeoutHandler(Packet::BasicField& bf) {
    //double cur_ts = bf.pkt_ts;
    /*
    if (cur_ts < last_ts) {
        LOG_MSG("Warning: time out of order\n");
    }
    */

    wrap_detect();

    printCount++;
    //last_ts = bf.pkt_ts;
    total_byte = 0;
    total_pkt = 0;
}

void HeavyKeyComputeThread::process(Packet::BasicField& bf) {
    count++;
    if (count == 1) {
        last_ts = 0;
    }

    /*
    double cur_ts = bf.pkt_ts;
    if (cur_ts < last_ts) {
        LOG_MSG("Warning: time out of order\n");
    }
    */

    total_pkt += bf.accumulated_pkt;
    total_byte += bf.accumulated_byte;

    memcpy(key, &bf.src_ip, 4);
    if (key_length == 64)
        memcpy(key+4, &bf.dst_ip, 4);
    long long val = bf.accumulated_byte;
    detector->update(key, val);
}

void HeavyKeyComputeThread::finish() {

    detector->clean();

    LOG_MSG("HeavyKeyComputeThread %d (%d) processed %" PRIu64 "\n", subId, getpid(), count);
    ComputeThread<Packet::BasicField, Dummy>::finish();
}
    
/****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * Constructor
 */
HeavyKeyComputeThread::HeavyKeyComputeThread(std::string name, int id) : ComputeThread<Packet::BasicField, Dummy>(name, id) {
    
    // statistics
    count = 0;
    total_pkt = 0;
    total_byte = 0;

    printCount = 0;
    last_ts = 0;

    Config *config = Config::getInstance();

    // common for heavy hitter/changer problem
    thresh = config->getdouble("thresh", 0);
    key_length = config->getint("keyLength", 64);

    char default_detector[] = "";
    std::string detector_type = config->getstring("DetectType", default_detector);
    if (detector_type == "Deltoid") {
        detector = new Deltoid(config, id);
    }
    else if (detector_type == "CountBase") {
        detector = new CountBase(config, id);
    }
    else if (detector_type == "LDSketch") {
        detector = new LDSketch(config, id);
    }
    else if (detector_type == "Seqhash") {
        detector = new Seqhash(config, id);
    }
    else if (detector_type == "FastSketch") {
        detector = new FastSketch(config, id);
    }
    else {
        LOG_ERR("Error: no Such Detector: %s\n", detector_type.c_str());
    }

    // print config parameters
    if (id == 0) {
        LOG_MSG("Detect Module: %s\n", detector_type.c_str());
        LOG_MSG("%s: thresh (absolute) %lf MB\n", detector->getName().c_str(), thresh);
        detector->print_parameter();
    }
}
    
void HeavyKeyComputeThread::initialize() {

    detector->init();

    ComputeThread<Packet::BasicField, Dummy>::initialize();
}
