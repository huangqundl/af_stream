#ifndef __Flow_hpp__
#define __Flow_hpp__

#include <string.h>

// This class will be transferred on the wire between 32/64 bit machines,
// so we add some dummy char array in the field.
/**
 * Structure of flows
 */
class FLOW {
public:
    /// Basic field of flow
    struct {
        /// source IP
        uint32_t orig_ip;           // key
        /// destination IP
        uint32_t resp_ip;
        /// padding
        char pad[4];
            
        /// start timestamp
        double flow_start_ts;       // values
        /// number of packets
        uint64_t pkt_cnt;
        /// number of bytes
        uint64_t byte_cnt; 
        /// number of events
        uint64_t event_cnt;
    } field;

    /// get number of bytes of metadata
    uint32_t size() {
        return sizeof(field);
    }

    /// get workload
    uint32_t load() {
        return sizeof(field);
    }
};
        

#endif
