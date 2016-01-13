#include "CountBase.hpp"

/****************************
 * Private functions
 * *************************/

// reset all internal data structures
void CountBase::reset() {
    if (is_heavy_changer) {
        cnt_tbl_copy(cnt_tbl_cur, cnt_tbl_old);
    }
    cnt_tbl_reset(cnt_tbl_cur);
    total_cur = 0;
}
    
unsigned int CountBase::fill_in_cand_list(double thresh) {
    unsigned no_cand = 0;
    if (is_heavy_changer)
        no_cand = cnt_tbl_find_cand(cnt_tbl_cur, cnt_tbl_old, thresh, cand_list);
    else
        no_cand = cnt_tbl_find_cand(cnt_tbl_cur, cnt_tbl_old, thresh - (double)total_cur/(length+1), cand_list);
    return no_cand;
}


void CountBase::print_parameter() {
    BasicDetector::print_parameter();
    unsigned int real_size = length*(1+(double)n/64);
    LOG_MSG("%s count length: %u, real size %u\n", name.c_str(), length, real_size);
}

void CountBase::calculate_diff() {
}

/****************************
 * Public functions
 * *************************/

CountBase::CountBase(Config* config, unsigned int id): BasicDetector(config, id) {
    name = "CountBase";

    // new detector:
    length = config->getint("length", 1);
}

void CountBase::init() {

    cnt_tbl_cur = cnt_tbl_init(length, n);
    cnt_tbl_reset(cnt_tbl_cur);
    if (is_heavy_changer) {
        cnt_tbl_old = cnt_tbl_init(length, n);
        cnt_tbl_reset(cnt_tbl_old);
    }
    else {
        cnt_tbl_old = NULL;
    }

    total_cur = 0;

    BasicDetector::init();
}

void CountBase::update(unsigned char* key, long long value) {
    // TBD
    total_cur += value;
    cnt_tbl_update(cnt_tbl_cur, key, value);
}
    
void CountBase::print_internal_state(std::string output_file) {
    cnt_tbl_print(cnt_tbl_cur, ("sk_cur" + output_file).c_str());
    if (is_heavy_changer) {
        cnt_tbl_print(cnt_tbl_old, ("sk_old" + output_file).c_str());
    }
}

void CountBase::clean() {
    cnt_tbl_destroy(cnt_tbl_cur);
    if (is_heavy_changer) {
        cnt_tbl_destroy(cnt_tbl_old);
    }
    BasicDetector::clean();
}
