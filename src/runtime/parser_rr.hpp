#ifndef __ROLL_PARSER_RR_HPP_INCLUDED__
#define __ROLL_PARSER_RR_HPP_INCLUDED__

#include "wrap_item.hpp"
#include "parser_base.hpp"
#include "dummy_item.hpp"

namespace afs {

class RRParser : public TraceParserBase<DummyType> {

private:
    int cur_, max_;

public:
    void ParseRecord(void* data, uint32_t len) {
        Emit(*((DummyType*)data), cur_);
        cur_ = (cur_ + 1) % max_;
    }

    void Init() {}

    void Clean() {}

    RRParser(int max) : cur_(0), max_(max) {}
};

}

#endif
