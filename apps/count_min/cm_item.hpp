#include <stdint.h>
#include <utility>
//#include "util.hpp"

#define SK_SIZE 8000
#define MAX_HH 100

struct __attribute__ ((__packed__)) CMItem {
    uint32_t num_counter, num_hh;
    unsigned char raw[SK_SIZE*(sizeof(uint32_t)+sizeof(long long))+MAX_HH*sizeof(unsigned long long int)];
    unsigned char* start;

    CMItem() : num_counter(0), num_hh(0), start(raw) {}

    void AppendCounter(uint32_t index, long long value) {
        //LOG_MSG("index %u value %ld\n", index, value);
        num_counter++;
        *((uint32_t*)start) = index;
        *((long long*)(start+sizeof(uint32_t))) = value;
        start += sizeof(uint32_t)+sizeof(long long);
    }

    void AppendHH(unsigned long long int key) {
        num_hh++;
        *((unsigned long long int*)start) = key;
        start += sizeof(unsigned long long int);
    }

    std::pair<uint32_t, long long> GetCounter(uint32_t n) {
        unsigned char* start = raw + n*(sizeof(uint32_t)+sizeof(long long));
        uint32_t index = *(uint32_t*)start;
        long long value = *(long long*)(start+sizeof(uint32_t));
        return make_pair(index, value);
    }

    unsigned long long int GetKey(uint32_t n) {
        unsigned char* start = raw + num_counter*(sizeof(uint32_t)+sizeof(long long))+n*sizeof(unsigned long long int);
        return *(unsigned long long int*)start;
    }

    uint32_t size() {
        return sizeof(uint32_t) + sizeof(uint32_t) + num_counter*(sizeof(uint32_t)+sizeof(long long)) + num_hh*sizeof(unsigned long long int);
    }
};
