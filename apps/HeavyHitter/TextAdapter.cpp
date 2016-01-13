/*
 * TextAdapter.cpp
 * - read from pcap file
 */
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "Util.hpp"
#include "TextAdapter.hpp"

/*
 * Constructor
 */
TextAdapter::TextAdapter(std::string filename_par) {
    filename = filename_par;
}

void TextAdapter::initiate(void* useless) {
    fp = fopen(filename.c_str(), "r");

	// open pcap file                                         
	if (fp == NULL) {
		LOG_ERR("cannot open %s\n", filename.c_str());
	}

    buf = (char *)malloc(128*sizeof(char));
}

void* TextAdapter::poll(uint32_t* len) {
    p = fgets(buf, 128, fp);
    if (p == NULL)
        return NULL;

    buf[strlen(buf)-1]='\0';
    *len = strlen(buf);
    return buf;
}
