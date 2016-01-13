/*
 * SpoutDispatcher.cpp
 */

#include <stdio.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <inttypes.h>

#include "Config.hpp"
#include "Util.hpp"
#include "Dispatcher/Adapter/PcapMemoryAdapter.hpp"
#include "Dispatcher/Adapter/PcapFileAdapter.hpp"
#include "Dispatcher/Adapter/PcapDirAdapter.hpp"
#include "SpoutDispatcher.hpp"

/************************************************************************
 * functions from class Dispatcher
 * **********************************************************************/

RawPacket* SpoutDispatcher::process(void* data, uint32_t length)
{
    ++i_;

    // get the raw packet timestamp
    double pkt_ts = *(double *)data;

    // data layout: timestamp (8 bytes) + length (4 bytes) + packet content 
    uint8_t *pkt = (uint8_t*) data + 12;

    if (p.decode(pkt, length, pkt_ts)) {
        return &p;
    } else {
        return NULL;
    }
}

/************************************************************************
 *
 * Public Functions:
 *
 * **********************************************************************/

SpoutDispatcher::SpoutDispatcher(std::string name, std::string filename,
        uint64_t size, Router<RawPacket> *r) :
    Dispatcher<RawPacket>(name) {
    Config *config = Config::getInstance();
    if (config->getboolean("readFromFile", false))
        adapter = new PcapFileAdapter(filename);
    else if (config->getboolean("readFromDir", false))
        adapter = new PcapDirAdapter(filename); 
    else
        adapter = new PcapMemoryAdapter(filename, size, config->getint("replayTimes", 1));

    router = r;
}

void SpoutDispatcher::initialize() {
    LOG_MSG("Dispatcher initializing\n");
    i_ = 0;
    adapter->initiate(&pkt_cnt_);
    Dispatcher<RawPacket>::initialize();
}

void SpoutDispatcher::finish() {
    Dispatcher<RawPacket>::finish();
}
