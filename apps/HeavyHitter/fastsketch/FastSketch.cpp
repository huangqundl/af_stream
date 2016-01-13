#include "FastSketch.hpp"

/****************************
 * Private functions
 * *************************/

// reset all internal data structures
void FastSketch::reset() {
    if (is_heavy_changer) {
        FastSk_copy(absc, absc_old);
    }
    FastSk_reset(absc);
}
    
unsigned int FastSketch::fill_in_cand_list(double thresh) {
    if (is_heavy_changer)
        return FastSk_Output(absc_diff, thresh, cand_list, MAX_CAND, is_heavy_changer);
    else
        return FastSk_Output(absc, thresh, cand_list, MAX_CAND, is_heavy_changer);
}


void FastSketch::print_parameter() {
    BasicDetector::print_parameter();
    unsigned int width= n - (log(depth)/log(2)) + 1;
    LOG_MSG("FastSketch: config width %u, depth %u, hashrow: %d, actual size %u\n", width, depth, hashrow, width*depth);
}

void FastSketch::calculate_diff() {
    FastSk_diff(absc, absc_old, absc_diff);
}

/****************************
 * Public functions
 * *************************/

FastSketch::FastSketch(Config* config, unsigned int id): BasicDetector(config, id) {
    name = "FastSketch";

    // Fash:
    depth = config->getint("depth", 1);
    hashrow = config->getint("hashrow", 1);
}

void FastSketch::init() {

    LOG_MSG("enter init\n")
    absc = FastSk_Init(depth, n, hashrow);
    if (is_heavy_changer) {
        absc_old = FastSk_Init(depth, n, hashrow);
        absc_diff = FastSk_Init(depth, n, hashrow);
    }
    else {
        absc_old = NULL;
        absc_diff = NULL;
    }
    LOG_MSG("enter basic detector init\n")

    BasicDetector::init();
    LOG_MSG("leave init\n")
}

void FastSketch::update(unsigned char* key, long long value) {
    unsigned long newitem = 0;
    memcpy(&newitem, key, n/8);
    FastSk_Update(absc, newitem, value);
}
    
void FastSketch::print_internal_state(std::string output_file) {
    FastSk_write_plaintext(absc, ("sk_cur" + output_file).c_str());
    if (is_heavy_changer) {
        FastSk_write_plaintext(absc_old, ("sk_old" + output_file).c_str());
        FastSk_write_plaintext(absc_diff, ("sk_diff" + output_file).c_str());
    }
}

void FastSketch::clean() {
    FastSk_Destroy(absc);
    if (is_heavy_changer) {
        FastSk_Destroy(absc_old);
        FastSk_Destroy(absc_diff);
    }
    BasicDetector::clean();
}
