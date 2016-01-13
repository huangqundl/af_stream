#ifndef _FAST_SKETCH_HPP
#define _FAST_SKETCH_HPP

#include "../BasicDetector.hpp"
extern "C"
{
#include "sketch.h"
#include "prng.h"
}

/**
 * Detect algorithm of Fast Sketch
 */
class FastSketch : public BasicDetector {

private:
    /// sketch for current epoch
    FastSk_type* absc;

    /// sketch for last epoch
    FastSk_type* absc_old; 

    /// difference of current and last sketches
    FastSk_type* absc_diff; 

    /// number of rows in the single column
    unsigned int depth;

    /// number of rows which a key is hashed to
    int hashrow;

private:
    void reset();

    unsigned int fill_in_cand_list(double thresh);

    void print_parameter();

    void calculate_diff();

public:

    /// Constructor
    FastSketch(Config* config, unsigned int id);

    void init();

    void update(unsigned char* key, long long value);

    void print_internal_state(std::string);

    void clean();
};

#endif
