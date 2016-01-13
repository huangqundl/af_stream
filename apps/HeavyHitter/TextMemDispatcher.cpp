/*
 * TextMemDispatcher.cpp
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "Config.hpp"
#include "util.hpp"
#include "TextMemAdapter.hpp"
#include "TextMemDispatcher.hpp"
#include "Router/DChoice.hpp"

/************************************************************************
 * functions from class Dispatcher
 * **********************************************************************/

Packet::BasicField* TextMemDispatcher::process(void* data, uint32_t length) {

    ++i_;
    if (i_ % 1000000 == 0) {
        uint64_t ts = now_us();
        LOG_MSG("throughput: %lf\n", (i_-last_cnt)*1000000.0/(ts-last_ts));
        last_cnt = i_;
        last_ts = ts;
        LOG_DEBUG("nextTuple: number %" PRIu64 "\n", i_);
    }
    if (i_ > pkt_cnt_) {
        LOG_DEBUG("nextTuple: no packet\n");
    }

    p.reset();
    p.get_bf() = *((Packet::BasicField*)data);

    return &(p.get_bf());
}

/************************************************************************
 *
 * Public Functions:
 *
 * **********************************************************************/

TextMemDispatcher::TextMemDispatcher(std::string name, std::string filename, uint64_t size) : Dispatcher<Packet::BasicField>(name) {
    Config *config = Config::getInstance();
    LOG_MSG("Read from preprocessed data (memory): %s\n", filename.c_str());
    adapter = new TextMemAdapter(filename, size);

    router = new DChoice(config->getint("numSubComputeThreads", 1), config->getint("DChoice", 1));
}

void TextMemDispatcher::initialize() {
    last_ts = 0;
    last_cnt = 0;

    LOG_MSG("TextMemDispatcher initializing\n");
    i_ = 0;
    adapter->initiate(&pkt_cnt_);
    Dispatcher<Packet::BasicField>::initialize();
}

void TextMemDispatcher::finish() {
    Dispatcher<Packet::BasicField>::finish();
}
