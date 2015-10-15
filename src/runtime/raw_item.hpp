#ifndef __ROLL_RAW_ITEM_HPP_INCLUDED__
#define __ROLL_RAW_ITEM_HPP_INCLUDED__

/// Original data from traces or external sources

#include <string.h>
#include "wrap_item.hpp"

namespace afs {

class RawItem {
public:
    char raw_data[MAX_RECORD_LENGTH];

    RawItem& operator= (RawItem& i) {
        //fprintf(stderr, "size %lu\n", strlen(i.raw_data));
        memcpy(this->raw_data, i.raw_data, strlen(i.raw_data));
        return *this;
    }
};

typedef WrapItem<RawItem> WRawItem;

}

#endif
