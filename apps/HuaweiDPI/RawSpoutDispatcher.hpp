#ifndef __RawSpoutDispatcher_hpp__
#define __RawSpoutDispatcher_hpp__

#include <limits.h>
#include <string>

#include "Dispatcher.hpp"
#include "LoadBalancer/LoadBalancer.hpp"
#include "RawPacket.hpp"

typedef struct {
    uint64_t tot_pkt_cnt;           // total # of packets observed
    uint64_t tot_byte_cnt;          // total # of bytes observed
    uint64_t valid_pkt_cnt;         // valid # of packets observed
    uint64_t valid_byte_cnt;        // valid # of bytes observed
    uint64_t used_time;
} PacketStat;

class RawSpoutDispatcher : public Dispatcher<RawPacket> {
private:
    PacketStat packet_stat_;        // packet statistics
    uint64_t i_;
    uint64_t pkt_cnt_;
    uint64_t start_time_;
    uint64_t last_byte;

    double start_ts;
    double last_ts;
    
    RawPacket p;

    RawPacket* process(void*, uint32_t);

    void report_stat();

public:
    RawSpoutDispatcher(std::string, std::string, uint64_t,
            LoadBalancer<RawPacket> *);
    void initialize();
    void finish();
};

#endif
