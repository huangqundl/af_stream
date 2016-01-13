#include <string.h>
#include <string>

#include "fault_tolerance/operator_tracker.hpp"
#include "hash.hpp"

unsigned int AwareHash(const unsigned char* data,
        unsigned int n,
        unsigned int hash = 388650253,
	    unsigned int scale = 388650319,
	    unsigned int hardener  = 1176845762) {
	while( n ) {
		hash *= scale;
		hash += *data++;
		n--;
	}
	return hash ^ hardener;
}

namespace afs {

Hash::Hash(size_t max_value): MAX_VALUE(max_value) {
    OperatorTracker* tracker = OperatorTracker::GetInstance();

    /*
    uint32_t key_len = sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+1;
    unsigned char* key = new unsigned char[key_len];
    */
    //sprintf(key, "%s", daemon_name.c_str());
    //memcpy(key+name_len, &component_id, sizeof(uint32_t));

    //key[key_len-1] = 1;
    uint64_t afs_seed = tracker->GetSeed();
    unsigned char* key = (unsigned char*)&afs_seed;
    hash = AwareHash(key, sizeof(uint64_t));
    //key[key_len-1] = 2;
    scale = AwareHash(key, sizeof(uint64_t));
    //key[key_len-1] = 3;
    hardener = AwareHash(key, sizeof(uint64_t));

    //fprintf(stderr, "hash %lu scale %lu hardener %lu\n", hash, scale, hardener);
    //delete [] key;
}

Hash::~Hash() {}

size_t Hash::GetHashValue(unsigned char* key, size_t len) {
    return AwareHash(key, len, hash, scale, hardener) % MAX_VALUE;
} 

} // namespace afs
