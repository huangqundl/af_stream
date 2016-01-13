#ifndef SEQHASH_HPP
#define SEQHASH_HPP

#include "../BasicDetector.hpp"
extern "C"
{
#include "hh_tbl.h"
#include "util.h"
}

/// Sequential hash
class Seqhash : public BasicDetector {

private:
    /// number of subspaces
    int D;

    /// Number of buckets in each row
    int K;

    /// lengh of each subspace
    int* bd;

    /// number of rows for each subspace
    int* M;

    /// sketch for current epoch
    hh_tbl_p_t hh_tbl_cur;

    /// sketch for last epoch
    hh_tbl_p_t hh_tbl_old;

    /// diffrence of sketches for current and last epoches
    hh_tbl_p_t change_hh_tbl;

private:
    void reset();

    unsigned int fill_in_cand_list(double thresh);

    void print_parameter();

    void calculate_diff();

public:

    /// Constructor
    Seqhash(Config* config, unsigned int id);

    void init();

    void update(unsigned char* key, long long value);

    void print_internal_state(std::string);

    void clean();
};

#endif
