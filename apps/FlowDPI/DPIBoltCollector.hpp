/*
 * DPIBoltCollector.hpp
 * - DPIBoltCollector thread - management on the global level
 */

#ifndef __DPIBoltCollector_hpp__
#define __DPIBoltCollector_hpp__

#include "../../Collector/Collector.hpp"
#include "common.hpp"
#include "Packet.hpp"

enum StateEventType { IPPORT_EVENT, NULL_EVENT };

typedef struct {
    double cur_ts;
    StateEventType event_type;
} MainStateEvent;

class Dummy {
public:
    uint32_t size() { return 0; }
    uint32_t get_size() { return 0; }
};

class DPIBoltCollector : public Collector<MainStateEvent, Dummy> {
private:
    void process(MainStateEvent& e);

public:
    DPIBoltCollector(std::string name, int num);
};

#endif
