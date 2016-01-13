/*
 * TextMemAdapter.cpp
 * - read from pcap file
 */
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "Util.hpp"
#include "mem_adapter.hpp"

/*
 * Constructor
 */
TextMemAdapter::TextMemAdapter(std::string filename_par, int m) {
    filename = filename_par;
    max = m;
}

void TextMemAdapter::initiate(void* useless) {
    fp = fopen(filename.c_str(), "r");

    // open pcap file
    if (fp == NULL) {
		LOG_ERR("cannot open %s\n", filename.c_str());
	}

    char* buf = (char*)malloc(128*sizeof(char));
    char* p;
    rawdata = (Item*)calloc(max, sizeof(Item));

    total = 0;
    while (1) {
        p = fgets(buf, 128, fp);
        if (p == NULL)
            break;

        buf[strlen(buf)-1]='\0';

        if (strcmp(buf, "timeout") == 0) {
            continue;
        }

        if (total % 10000000 == 0)
            LOG_MSG("load %d\n", total);

        rawdata[total].parse(buf);
        total++;
        if (total == max)
            break;
    }
    cur = 0;

    LOG_MSG("Complete loading trace: total %d\n", total);
}

void* TextMemAdapter::poll(uint32_t* len) {
    if (cur == total)
        return NULL;
    void* ptr = &rawdata[cur];
    cur++;
    return ptr;
}
