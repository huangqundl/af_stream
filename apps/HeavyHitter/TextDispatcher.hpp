/*
 * TextDispatcher.hpp
 */

#ifndef __TextDispatcher_hpp__
#define __TextDispatcher_hpp__

#include <limits.h>
#include <string>

#include "ingress_base.hpp"
#include "Dispatcher/Router/Router.hpp"
#include "Packet.hpp"

/**
 * The dispatcher equipped with TextAdapther
 */
class TextDispatcher : public Dispatcher<Packet::BasicField> {
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

    Packet::BasicField* process(void*, uint32_t);

public:
    /**
     * Constructor
     * @param name name of the Dispatcher
     * @param filename input file name
     * @param size useless
     */
    TextDispatcher(std::string name, std::string filename, uint64_t size);
    void initialize();
    void finish();
};

#endif
