#ifndef __ROLL_PARSER_EMPTY_HPP_INCLUDED__
#define __ROLL_PARSER_EMPTY_HPP_INCLUDED__

#include "item.hpp"
#include "parser_base.hpp"

namespace afs {

class EmptyItem : public ItemBase {
public:
    double ts;
    size_t key;
    long long value;

    uint32_t get_max_data_size() {
        return sizeof(EmptyItem);
    }

    void parse(char* buf);

};
    
void EmptyItem::parse(char* buf) {
    char *token;
    char search[] = " ";
    token = strtok(buf, search);    // timestamp
    ts = strtod(token, NULL);
    token = strtok(NULL, search);   // key
    key = strtoul(token, NULL, 10);
    token = strtok(NULL, search);   // value
    value = strtoll(token, NULL, 10);
}

class EmptyParser : public ParserBase<EmptyItem> {

private:
    int count_;

public:
    
    EmptyParser() : ParserBase<EmptyItem>(), count_(0) {}

    void ParseRecord(void* data, uint32_t len) {
        //LOG_MSG("%d\n", count_++);
    }

    void Init() {}

    void Clean() {}

};

}

#endif
