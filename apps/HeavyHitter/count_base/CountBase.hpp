#ifndef COUNTBASE_HPP
#define COUNTBASE_HPP

#include "../BasicDetector.hpp"
#include "cnt_tbl.hpp"

/**
 * Counter based detection algorithm
 */
class CountBase : public BasicDetector {

private:
    /// Length of associative array
    unsigned int length;
    /// Associative array for current epoch
    cnt_tbl_p_t cnt_tbl_cur;
    /// Associative array for last epoch
    cnt_tbl_p_t cnt_tbl_old;
    /// total sum in current epoch
    long long total_cur;

private:
    void reset();

    unsigned int fill_in_cand_list(double thresh);

    void print_parameter();

    void calculate_diff();

public:

    /// Constructor
    /// @param config configuration structure
    /// @param id index of instances (affect hash function)
    CountBase(Config* config, unsigned int id);

    void init();

    void update(unsigned char* key, long long value);

    void print_internal_state(std::string);

    void clean();
};

#endif
