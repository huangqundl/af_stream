#ifndef __ROLL_PARSER_KV_HPP_INCLUDED__
#define __ROLL_PARSER_KV_HPP_INCLUDED__

#include "wrap_item.hpp"
#include "parser_base.hpp"

namespace afs {

class KVItem {
public:
    double ts;
    size_t key;
    long long value;

    void parse(char* buf);

};
    
void KVItem::parse(char* buf) {
    char *token;
    char search[] = " ";
    token = strtok(buf, search);    // timestamp
    ts = strtod(token, NULL);
    token = strtok(NULL, search);   // key
    key = strtoul(token, NULL, 10);
    token = strtok(NULL, search);   // value
    value = strtoll(token, NULL, 10);
}

class KVParser : public ParserBase<KVItem> {

private:
    KVItem item;

public:
    void ParseRecord(void* data, uint32_t len) {
        item.parse((char*)data); 
        LOG_MSG("%lf %lu %lld\n", item.ts, item.key, item.value);
    }

    void Init() {}

    void Clean() {}

};

}

#endif
