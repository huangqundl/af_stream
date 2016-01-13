/*
 * hh_tbl.h
 * - heavy hitter random partition
 */

#ifdef __64BIT
#define MAX_NO_CAND 10000000
#else
#define MAX_NO_CAND 100000
#endif

typedef struct hh_tbl_s * hh_tbl_p_t;

/// Seqhash Sketch structure
typedef struct hh_tbl_s {
    /// Counters
	long long*** T;

    /// number of subspaces
	int D;

    /// number of rows for a subspace
	int* M;

    /// number of buckets
	int K;

    /// length of a total key
	int n;

    /// length of a subspace
	int* bd;
} hh_tbl_t;

/// init sketch
hh_tbl_p_t hh_tbl_init(int D, int* M, int K, int n, int* bd);

/// update sketch with one data item
void hh_tbl_encode(hh_tbl_p_t tbl, unsigned char* key, long long val);

/// identify heavy keys
//@param bias epsilon value
//@param miss allowed miss in hash rows for heavy changer detection
unsigned int hh_tbl_decode(hh_tbl_p_t tbl, hh_tbl_p_t tbl_1, hh_tbl_p_t tbl_2, 
		double thresh, double bias, int miss, unsigned char** ret_cand_list);

void hh_tbl_write(hh_tbl_p_t tbl, char* outfile);
void hh_tbl_copy(hh_tbl_p_t tbl, hh_tbl_p_t ret_tbl);
void hh_tbl_reset(hh_tbl_p_t hh_tbl);
void hh_tbl_destroy(hh_tbl_p_t hh_tbl);

/// print out metadata of the sketch
void hh_tbl_print_summary(hh_tbl_p_t hh_tbl);

/// print out sketch to file in plaintext
// @param sk the target sketch
// @param name name of the output file
void hh_tbl_write_plaintext(hh_tbl_p_t sk, const char* name);
