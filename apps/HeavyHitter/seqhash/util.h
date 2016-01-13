#ifndef SEQHASH_UTIL_HPP
#define SEQHASH_UTIL_HPP

/*
 * util.h
 */

void gen_change_hh_tbl(hh_tbl_p_t hh_tbl_1, hh_tbl_p_t hh_tbl_2,
		hh_tbl_p_t ret_change_tbl);

void output_cand(unsigned char** cand_list, int no_cand, const char* filename);

#endif
