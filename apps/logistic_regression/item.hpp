/// License here

/*
 * Item.hpp
 * - Key/value pairs
 *  */

#ifndef LR_ITEM_HPP_
#define LR_ITEM_HPP_

/**
 * Structure of Item 
 */
class Item {
public:
    size_t tag;
    size_t count;
    double feature[2];

    Item() {}

    void parse(char* buf);
};

#endif
