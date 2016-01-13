#include "Seqhash.hpp"

/****************************
 * Private functions
 * *************************/

// reset all internal data structures
void Seqhash::reset() {
    if (is_heavy_changer) {
        hh_tbl_copy(hh_tbl_cur, hh_tbl_old);
    }
    hh_tbl_reset(hh_tbl_cur);
}
    
unsigned int Seqhash::fill_in_cand_list(double thresh) {
    int no_cand = 0;
    if (is_heavy_changer) {
        no_cand = hh_tbl_decode(change_hh_tbl, hh_tbl_cur, hh_tbl_old, thresh, 0, 1, cand_list);
    }
    else {
        no_cand = hh_tbl_decode(change_hh_tbl, NULL, NULL, thresh, 0, 0, cand_list);
    }
    return no_cand;
}


void Seqhash::print_parameter() {
    BasicDetector::print_parameter();
}

void Seqhash::calculate_diff() {
    gen_change_hh_tbl(hh_tbl_cur, hh_tbl_old, change_hh_tbl);
}

/****************************
 * Public functions
 * *************************/

Seqhash::Seqhash(Config* config, unsigned int id): BasicDetector(config, id) {
    name = "Seqhash";
    K = config->getint("width", 1444);

    if (n == 32) {
        D = 9;
        bd = (int*)calloc(D, sizeof(int));
        bd[0] = 16;
        for (int i=1; i<D; ++i) {
            bd[i] = 2;
        }
        M = (int*)calloc(D, sizeof(int));
        M[0] = 4;
        for (int i=1; i<D-1; ++i) {
            M[i] = 2;
        }   
        M[D-1] = 15;
    }
    else if (n==64) {
	    D = 25;
	    bd = (int*)calloc(D, sizeof(int));
	    bd[0] = 16;
	    for (int i=1; i<D; ++i) {
		    bd[i] = 2;
	    }
	    M = (int*)calloc(D, sizeof(int));
	    M[0] = 4;
	    for (int i=1; i<D-1; ++i) {
		    M[i] = 2;
	    }
	    M[D-1] = 16;
    }
}

void Seqhash::init() {

    // create hh table/cache
    hh_tbl_cur = hh_tbl_init(D, M, K, n, bd);
    // debug
    // reset the table
    hh_tbl_reset(hh_tbl_cur);

    if (is_heavy_changer) {
        hh_tbl_old = hh_tbl_init(D, M, K, n, bd);
        change_hh_tbl = hh_tbl_init(D, M, K, n, bd);
    }
    else {
        hh_tbl_old = NULL;
        change_hh_tbl = hh_tbl_cur;
    }

    BasicDetector::init();
    hh_tbl_print_summary(hh_tbl_cur);
}

void Seqhash::update(unsigned char* key, long long value) {
    // TBD
    hh_tbl_encode(hh_tbl_cur, key, value);
}
    
void Seqhash::print_internal_state(std::string output_file) {
    hh_tbl_write_plaintext(hh_tbl_cur, ("sk_cur" + output_file).c_str());
    if (is_heavy_changer) {
        hh_tbl_write_plaintext(hh_tbl_old, ("sk_old" + output_file).c_str());
        hh_tbl_write_plaintext(change_hh_tbl, ("sk_diff" + output_file).c_str());
    }
}

void Seqhash::clean() {
    hh_tbl_destroy(hh_tbl_cur);
    if (is_heavy_changer) {
        hh_tbl_destroy(hh_tbl_old);
        hh_tbl_destroy(change_hh_tbl);
    }
    BasicDetector::clean();
}
