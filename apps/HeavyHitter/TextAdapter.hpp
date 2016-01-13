/*
 * TextAdapter.hpp
 */

#ifndef __TextAdapter_hpp__
#define __TextAdapter_hpp__

#include <linux/if_ether.h>
#include <pcap.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include "Dispatcher/Adapter/Adapter.hpp"

/**
 * Read preprocessed records.
 * Each record has form: sip dip byte
 */
class TextAdapter : public Adapter {
private:
    /**
     * input filename
     */
    std::string filename;

    /**
     * file handler
     */
    FILE* fp;

    /**
     * buffer to read records
     */
    char* buf;

    /**
     * record pointer
     */
    char* p;

public: // from basic Adapter
    void initiate(void*);
    void* poll(uint32_t*);
    
public:
    /**
     * Constructor
     * @param name input file name
     */
	TextAdapter(std::string name);
};

#endif
