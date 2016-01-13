/*
 * SpoutDispatcher.hpp
 */

#ifndef __SpoutDispatcher_hpp__
#define __SpoutDispatcher_hpp__

#include <limits.h>
#include <string>

#include "ingress_base.hpp"
#include "Router/Router.hpp"
#include "Packet.hpp"
#include "RawPacket.hpp"

//typedef struct {
//    uint64_t tot_pkt_cnt;           // total # of packets observed
//    uint64_t tot_byte_cnt;          // total # of bytes observed
//    uint64_t valid_pkt_cnt;         // # of valid packets
//    uint64_t valid_byte_cnt;        // # of valid bytes
//    uint64_t used_time;
//} PacketStat;

class SpoutDispatcher : public Dispatcher<RawPacket> {
private:
    uint64_t i_;
    uint64_t pkt_cnt_;
    
    RawPacket p;

    RawPacket* process(void*, uint32_t);

public:
    SpoutDispatcher(std::string, std::string, uint64_t,
            Router<RawPacket> *);
    void initialize();
    void finish();
};

#endif
