#ifndef __Workload_hpp__
#define __Workload_hpp__

#include "Util.hpp"

unsigned int runLinearWorkload(unsigned int size, unsigned int scale) {
    double res = 1;

    for (unsigned int i=1; i<=size*scale; ++i) {
        res *= (double)i;
    }

    return (unsigned int)res;
}

#endif
