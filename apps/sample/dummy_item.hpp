#ifndef __ROLL_DUMMY_ITEM_HPP_INCLUDED__
#define __ROLL_DUMMY_ITEM_HPP_INCLUDED__

#include "wrap_item.hpp"

#define DATA_SIZE 2048
class DummyType {
public:
	unsigned number;
	char dummy[DATA_SIZE];
};

typedef afs::WrapItem<DummyType> WDummyType;

#endif
