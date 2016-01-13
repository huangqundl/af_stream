/*
 * TextDispatcher.cpp
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "Config.hpp"
#include "Util.hpp"
#include "TextAdapter.hpp"
#include "TextDispatcher.hpp"
#include "Router/PacketFieldGrouping.hpp"

/************************************************************************
 * functions from class Dispatcher
 * **********************************************************************/

Packet::BasicField* TextDispatcher::process(void* data, uint32_t length) {

    ++i_;
    if (i_ % 1000000 == 0) {
        LOG_DEBUG("nextTuple: number %" PRIu64 "\n", i_);
    }
    if (i_ > pkt_cnt_) {
        LOG_DEBUG("nextTuple: no packet\n");
    }

    char* str = (char*) data;
    if (strcmp(str, "timeout") == 0) {
        Packet::BasicField tmpBf;
        memset(&tmpBf, 0, sizeof(tmpBf));
        int size = data_queues.size();
        for (int i=0; i<size; ++i) {
            emit(&tmpBf, i, E_TIMEOUT);
        }
        return NULL;
    }

    p.reset();
    char *token;
    char search[] = " ";

    token = strtok(str, search);
    p.get_bf().src_ip = atoi(token);
    token = strtok(NULL, search);
    p.get_bf().dst_ip = atoi(token);
    token = strtok(NULL, search);
    p.get_bf().accumulated_pkt = 1;
    p.get_bf().accumulated_byte = atoi(token);
    p.get_bf().pre_assign = -1;
    p.get_bf().seq = i_;

    return &(p.get_bf());
}

/************************************************************************
 *
 * Public Functions:
 *
 * **********************************************************************/

TextDispatcher::TextDispatcher(std::string name, std::string filename, uint64_t size) : Dispatcher<Packet::BasicField>(name) {
    LOG_MSG("Read from preprocessed data: %s\n", filename.c_str());
    adapter = new TextAdapter(filename);

    router = new PacketFieldGrouping();
}

void TextDispatcher::initialize() {
    LOG_MSG("TextDispatcher initializing\n");
    i_ = 0;
    adapter->initiate(&pkt_cnt_);
    Dispatcher<Packet::BasicField>::initialize();
}

void TextDispatcher::finish() {
    Dispatcher<Packet::BasicField>::finish();
}
