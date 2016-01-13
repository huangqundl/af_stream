//#ifndef __FiledGrouping_hpp__
//#define __FiledGrouping_hpp__

#include <stdint.h>

#include "Dispatcher/Router/Router.hpp"
#include "../item.hpp"
#include "HashFunc.hpp"

/**
 * specific LoadBalancer by simply hashing keys
 */
class ItemRangeRouter : public Router<Item> {

struct Range {
    unsigned long long int left, right;
};

private:
    Range* ranges;

public:
    /**
     * Constructor
     */
    ItemRangeRouter() {}

	int balance(Item* item, int* flags=NULL) {
        /*
        uint32_t key = item->key;
        return HashFunc((const char*)&key, 4) % attachedQueueCount;
        */
        return 0;
    }
};

//#endif
