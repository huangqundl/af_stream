#ifndef __Flow_hpp__
#define __Flow_hpp__

#include <inttypes.h>
#include <string.h>

// XXX: should be large enough to contain the whole payload of at least one packet??
#define MAX_PAYLOAD_LENGTH 5000

// This class will be transferred on the wire between 32/64 bit machines,
// so we add some dummy char array in the field.
class FLOW {
public:
    struct Field {
        // key
        uint32_t orig_ip;           // originator IP
        uint32_t resp_ip;           // responder IP
        uint16_t orig_port;         // port # of originator IP
        uint16_t resp_port;         // port # of responder IP
        uint8_t proto;              // protocol
        uint8_t is_tcpsyn:1;        // true if flow open due to TCP syn
        uint8_t is_tcpfin:1;        // true if flow closed due to TCP fin
        uint8_t is_done:1;
        uint8_t has_export:1;
        char pad[2];                // dummy array
            
        // values
        double flow_start_ts;       // flow start time
        double o2r_first_ts;        // o2r start timestamp 
        double r2o_first_ts;        // r2o start timestamp 
        double o2r_last_ts;         // o2r last seen timestamp 
        double r2o_last_ts;         // r2o last seen timestamp 
        uint32_t o2r_pkt_cnt;       // packet count from orig to resp
        uint32_t r2o_pkt_cnt;       // packet count from resp to orig
        uint64_t o2r_byte_cnt;      // byte count from orig to resp
        uint64_t r2o_byte_cnt;      // byte count from resp to orig
        uint64_t o2r_data_cnt;      // data stored in flow
        uint64_t r2o_data_cnt;
        uint64_t export_byte_cnt;   // byte count from resp to orig
        uint32_t export_pkt_cnt;    // packet count from orig to resp
        char pad2[4];               // dummy array
    } field;

    unsigned char payload[MAX_PAYLOAD_LENGTH];

    FLOW& operator = (const FLOW& flow){
        this->field = flow.field;
        if (!this->field.has_export) {
            if (flow.field.o2r_data_cnt+flow.field.r2o_data_cnt > MAX_PAYLOAD_LENGTH) {
                LOG_MSG("\nWarning!!! FLOW too long %" PRIu64 "\n", flow.field.o2r_data_cnt+flow.field.r2o_data_cnt);
            }
            memcpy(this->payload, flow.payload, flow.field.o2r_data_cnt+flow.field.r2o_data_cnt);
        }
        return *this;
    }

    uint32_t size() {
        return sizeof(field) + (field.has_export ? 0 : field.o2r_data_cnt + field.r2o_data_cnt);
    }

    uint32_t load() {
        return field.o2r_data_cnt + field.r2o_data_cnt - field.export_byte_cnt;
    }
};
        

#endif
