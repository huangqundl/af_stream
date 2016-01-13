#include "Deltoid.hpp"

/****************************
 * Private functions
 * *************************/

// reset all internal data structures
void Deltoid::reset() {
    if (is_heavy_changer) {
        AbsChange_copy(absc, absc_old);
    }
    AbsChange_reset(absc);
}
    
unsigned int Deltoid::fill_in_cand_list(double thresh) {
    if (is_heavy_changer)
        return AbsChange_Output(absc_diff, thresh, cand_list, MAX_CAND);
    else
        return AbsChange_Output(absc, thresh, cand_list, MAX_CAND);
}


void Deltoid::print_parameter() {
    BasicDetector::print_parameter();
    LOG_MSG("Deltoid: config width %u, depth %u, actual size %u\n", width, depth, width*depth*(n+1));
}

void Deltoid::calculate_diff() {
    AbsChange_diff(absc, absc_old, absc_diff);
}

/****************************
 * Public functions
 * *************************/

Deltoid::Deltoid(Config* config, unsigned int id): BasicDetector(config, id) {
    name = "Deltoid";

    // deltoid:
    width = config->getint("width", 1);
    depth = config->getint("depth", 1);
}

void Deltoid::init() {

    absc = AbsChange_Init(width, depth, n);
    if (is_heavy_changer) {
        absc_old = AbsChange_Init(width, depth, n);
        absc_diff = AbsChange_Init(width, depth, n);
    }
    else {
        absc_old = NULL;
        absc_diff = NULL;
    }

    BasicDetector::init();
}

void Deltoid::update(unsigned char* key, long long value) {
    unsigned long newitem = 0;
    memcpy(&newitem, key, n/8);
    AbsChange_Update(absc, newitem, value);
}
    
void Deltoid::print_internal_state(std::string output_file) {
    AbsChange_write_plaintext(absc, ("sk_cur" + output_file).c_str());
    if (is_heavy_changer) {
        AbsChange_write_plaintext(absc_old, ("sk_old" + output_file).c_str());
        AbsChange_write_plaintext(absc_diff, ("sk_diff" + output_file).c_str());
    }
}

void Deltoid::clean() {
    AbsChange_Destroy(absc);
    if (is_heavy_changer) {
        AbsChange_Destroy(absc_old);
        AbsChange_Destroy(absc_diff);
    }
    BasicDetector::clean();
}
