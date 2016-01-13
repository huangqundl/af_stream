/// License here

#ifndef LR_SUM_HPP_
#define LR_SUM_HPP_

/*
 * Sum.hpp
 *  */

/**
 * Structure of Sum 
 */
class Sum {
public:
    size_t seq;
    double sum;

    Sum() {}

    Sum(size_t seq_, double sum_): seq(seq_), sum(sum_) {}

    uint32_t size() {
        return sizeof(size_t)+sizeof(double);
    }

    //void parse(char* buf);
};

#endif
