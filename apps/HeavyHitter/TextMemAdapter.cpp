/*
 * TextMemAdapter.cpp
 * - read from pcap file
 */
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "Util.hpp"
#include "TextMemAdapter.hpp"

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
    rawdata = (Packet::BasicField *)calloc(max, sizeof(Packet::BasicField));

    total = 0;
    unsigned long size = 0;
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

        char *token;
        char search[] = " ";
        token = strtok(buf, search);
        rawdata[total].src_ip = atoi(token);
        token = strtok(NULL, search);
        rawdata[total].dst_ip = atoi(token);
        token = strtok(NULL, search);
        rawdata[total].accumulated_pkt = 1;
        rawdata[total].accumulated_byte = atoi(token);
        rawdata[total].pre_assign = -1;
        rawdata[total].seq = total+1;
        //rawdata[total].pkt_ts = total;
        size += rawdata[total].accumulated_byte;

        total++;
        if (total == max)
            break;
    }
    cur = 0;

    LOG_MSG("Complete loading trace: total %d size: %lu, average: %lu\n", total, size, size/ total);
}

void* TextMemAdapter::poll(uint32_t* len) {
    if (cur == total)
        return NULL;
    void* ptr = &rawdata[cur];
    cur++;
    return ptr;
}
