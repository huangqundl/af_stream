#include <string.h>
#include <string>

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

Hash::Hash(size_t max_value): StatelessOperator(), MAX_VALUE(max_value) {
    std::string daemon_name = daemon->get_daemon_name();
    size_t name_len = daemon_name.size();
    char* key = new char[name_len+sizeof(uint32_t)+1];
    sprintf(key, "%s", daemon_name.c_str());
    memcpy(key+name_len, &component_id, sizeof(uint32_t));
    key[name_len+sizeof(uint32_t)] = 1;
    hash = AwareHash((unsigned char*)key, daemon->get_daemon_name().size()+sizeof(uint32_t));
    key[name_len+sizeof(uint32_t)] = 2;
    scale = AwareHash((unsigned char*)key, daemon->get_daemon_name().size()+sizeof(uint32_t));
    key[name_len+sizeof(uint32_t)] = 3;
    hardener = AwareHash((unsigned char*)key, daemon->get_daemon_name().size()+sizeof(uint32_t));

    //fprintf(stderr, "hash %lu scale %lu hardener %lu\n", hash, scale, hardener);
    delete [] key;
}

Hash::~Hash() {}

size_t Hash::Process(unsigned char* key, size_t len) {
    return AwareHash(key, len, hash, scale, hardener) % MAX_VALUE;
} 

} // namespace afs
