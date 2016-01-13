/*
 * TextMemDispatcher.hpp
 */

#ifndef __TextMemDispatcher_hpp__
#define __TextMemDispatcher_hpp__

#include <limits.h>
#include <string>

#include "ingress_base.hpp"
#include "Dispatcher/Router/Router.hpp"
#include "Packet.hpp"

/**
 * The dispatcher equipped with TextMemAdapther
 */
class TextMemDispatcher : public Dispatcher<Packet::BasicField> {
private:
    /**
     * count number of packets processed
     */
    uint64_t i_;

    /**
     * maximum number of packets
     */
    uint64_t pkt_cnt_;

    /**
     * instance of a packet
     */
    Packet p;

    /**
     * timestamp
     */
    uint64_t last_ts;

    /**
     * number of packets
     */
    uint64_t last_cnt;

    Packet::BasicField* process(void*, uint32_t);

public:
    /**
     * Constructor
     * @param name name of the Dispatcher
     * @param filename input file name
     * @param size useless
     */
    TextMemDispatcher(std::string name, std::string filename, uint64_t size);
    void initialize();
    void finish();
};

#endif
