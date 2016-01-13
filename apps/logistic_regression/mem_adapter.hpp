/*
 * TextMemAdapter.hpp
 */

#ifndef __TextMemAdapter_hpp__
#define __TextMemAdapter_hpp__

#include <linux/if_ether.h>
#include <pcap.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include "Dispatcher/Adapter/Adapter.hpp"
#include "item.hpp"

/**
 * Read preprocessed records.
 * Load the records into memory first, and then read from the memory.
 */
class TextMemAdapter : public Adapter {
private:
    /**
     * Input filename
     */
    std::string filename;

    /**
     * File handler
     */
    FILE* fp;
    
    /**
     * Pointer to a read record
     */
    Item* rawdata;

    /**
     * current number of processed recoreds
     */
    int cur;

    /**
     * total number of recoreds
     */
    int total;

    /**
     * size of memory buffer
     */
    int max;

public: // from basic Adapter
    void initiate(void*);
    void* poll(uint32_t*);
    
public:
    /**
     * Constructor
     * @param name name of the Dispatcher
     * @param max size of memory buffer
     */
	TextMemAdapter(std::string name, int max);
};

#endif
