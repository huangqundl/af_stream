/*
 * Random.hpp
 * - contain all random (hash) functions
 */

#ifndef __FlowBoltSubComputeThread_hpp__
#define __FlowBoltSubComputeThread_hpp__

#include <stdlib.h>

void init_random(int seed) {
    srand(seed);
}

double get_random() {
    return (double) rand() / RAND_MAX;
}

#endif
