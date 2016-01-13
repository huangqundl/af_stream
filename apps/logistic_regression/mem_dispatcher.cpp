/*
 * TextMemDispatcher.cpp
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "util.hpp"
#include "mem_adapter.hpp"
#include "mem_dispatcher.hpp"
#include "router/item_hash_router.hpp"

/************************************************************************
 * functions from class Dispatcher
 * **********************************************************************/

Item* TextMemDispatcher::process(void* data, uint32_t length) {

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

    return (Item*)data;
}

/************************************************************************
 *
 * Public Functions:
 *
 * **********************************************************************/

TextMemDispatcher::TextMemDispatcher(std::string name, std::string filename, uint64_t size) : Dispatcher<Item>(name) {
    LOG_MSG("Read from preprocessed data (memory): %s\n", filename.c_str());

    adapter = new TextMemAdapter(filename, size);
    router = new ItemRangeRouter();
}

void TextMemDispatcher::initialize() {
    last_ts = 0;
    last_cnt = 0;

    LOG_MSG("TextMemDispatcher (%d) initializing\n", getpid());
    i_ = 0;
    adapter->initiate(&pkt_cnt_);
    Dispatcher<Item>::initialize();
}

void TextMemDispatcher::finish() {
    Dispatcher<Item>::finish();
}
