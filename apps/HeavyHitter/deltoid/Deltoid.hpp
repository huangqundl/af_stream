#ifndef DELTOID_HPP
#define DELTOID_HPP

#include "../BasicDetector.hpp"
extern "C"
{
#include "sketch.h"
#include "prng.h"
}

/**
 * Detector of Combinational Group Testing (CGT)
 */
class Deltoid : public BasicDetector {

private:
    /// Sketch for current epoch
    AbsChange_type* absc;

    /// Sketch for last epoch
    AbsChange_type* absc_old; 
    
    /// Difference of sketches for current and last epoches
    AbsChange_type* absc_diff; 

    /// number of rows of sketches
    unsigned int depth;
    
    /// number of buckets in each row
    unsigned int width;

private:
    void reset();

    unsigned int fill_in_cand_list(double thresh);

    void print_parameter();

    void calculate_diff();

public:

    /// Constructor
    Deltoid(Config* config, unsigned int id);

    void init();

    void update(unsigned char* key, long long value);

    void print_internal_state(std::string);

    void clean();
};

#endif
