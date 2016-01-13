/*
 * util.c
 * - the functions that are used for experiments
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "hh_tbl.h"
#include "util.h"
//#include "lp_lib.h"

//#define LLONG_MAX 9223372036854775807L

/*
 * print a given key/value pair (assume the key has 4 bytes)
 */
/*
void print_key_val(unsigned char* key, int n, long long val) {
	int i;
	for (i=0; i<n/8; ++i) {
		printf("%d.", key[i]);
	}
	printf(" %.4lf megabytes)\n", (double)val / MB);
}
*/

/*
 * generate change hh table
 */
void gen_change_hh_tbl(hh_tbl_p_t hh_tbl_1, hh_tbl_p_t hh_tbl_2,
		hh_tbl_p_t ret_change_tbl) {
	int i, j, k;

	// error checking
	if (ret_change_tbl == NULL) {
		return;
	}

	if (hh_tbl_2 == NULL) {	
		hh_tbl_copy(hh_tbl_1, ret_change_tbl);
	} else {
		hh_tbl_copy(hh_tbl_2, ret_change_tbl);
		for (i=0; i<ret_change_tbl->D; ++i) {
			for (j=0; j<ret_change_tbl->M[i]; ++j) {
				for (k=0; k<ret_change_tbl->K; ++k) {
					ret_change_tbl->T[i][j][k] -= hh_tbl_1->T[i][j][k];
					// assume all values to be absolute
					ret_change_tbl->T[i][j][k] = 
						ret_change_tbl->T[i][j][k] > 0 ?
						ret_change_tbl->T[i][j][k] : 
						- ret_change_tbl->T[i][j][k];
				}
			}
		}
	}
}


/*
 * find real heavy hitters / heavy changers, return the number of real
 */
/*
int find_real_hh(key_tbl_p_t change_tbl, double thresh, unsigned char**
		ret_real_list) {
	long long val;
	int no_hh = 0;                  // no of heavy hitters
	int i, j;

	// error checking
	if (ret_real_list == NULL) {
		return -1;
	}

	for (i=0; i<change_tbl->tbl_list_size; ++i) {
		for (j=0; j<change_tbl->list_len[i]; ++j) {
			val = change_tbl->list[i][j].byte_cnt;
			val = val > 0 ? val : -val;
			if (val >= thresh * MB) {
				memcpy(ret_real_list[no_hh], change_tbl->list[i][j].key,
						change_tbl->n/8);
				no_hh++;
			}
		}
	}

	// return
	return no_hh;
}
*/


/*
 * Find false positives/negatives
 * flag == 0: print distribution, flag == 1: print false pos/neg rates
 */
/*
void find_false_pos_neg(key_tbl_p_t change_key_tbl, hh_tbl_p_t change_hh_tbl, 
		double thresh, unsigned char** cand_list, int no_cand, char flag) {

	key_tbl_entry_p_t key_entry = NULL;
	char found_flag = 0;
	char key_str[13];
	long long val;
	int no_hh = 0;
	int no_false_pos = 0;
	int no_false_neg = 0;
	int i, j, r;

	// find false positives
	for (r=0; r<no_cand; ++r) {
		found_flag = 0;
		for (i=0; i<change_key_tbl->tbl_list_size && !found_flag; ++i) {
			for (j=0; j<change_key_tbl->list_len[i] && !found_flag; ++j) {
				key_entry = &(change_key_tbl->list[i][j]);
				memcpy(key_str, key_entry->key, change_key_tbl->n/8);
				if (memcmp(cand_list[r], key_str, change_key_tbl->n/8) == 0) {
					found_flag = 1;
				}
			}
		}
		if (!found_flag) {
			// printf("Fp (not exist): ");
			// print_key_val(cand_list[r], change_key_tbl->n, 0);
			if (flag == 0) {
				printf("1.0\n");
			}
			no_false_pos++;
		} else {
			val = key_entry->byte_cnt;
			val = val > 0 ? val : -val;
			if (val < thresh * MB) {
				// debug
				// printf("Fp: ");
				// print_key_val(cand_list[r], change_key_tbl->n,
				//		key_entry->byte_cnt);
				if (flag == 0) {
					printf("%lf\n", (thresh - (double)val / MB) / thresh);
				}
				no_false_pos++;
			} else {
				// assume visit_flag is set to zero before
				key_entry->visit_flag = 1;
			}
		}
	}

	// find false negatives
	for (i=0; i<change_key_tbl->tbl_list_size; ++i) {
		for (j=0; j<change_key_tbl->list_len[i]; ++j) {
			key_entry = &(change_key_tbl->list[i][j]);
			val = key_entry->byte_cnt;
			val = val > 0 ? val : -val;
			if (val >= thresh * MB) {
				if (key_entry->visit_flag == 0) {	// not seen in detection
					// debug
					// printf("Fn: ");
					// print_key_val(key_entry->key, change_key_tbl->n,
					//		key_entry->byte_cnt);
					// memcpy(key_str, key_entry->key, change_key_tbl->n/8);
					if (flag == 0) {
						printf("%lf\n", (thresh - (double)val / MB) / thresh);
					}
					no_false_neg++;
				}
				no_hh++;
			}
		}
	}

	// print results
	// printf("No of candidates = %d\n", no_cand);	
	// printf("No of heavy-changers = %d\n", no_hh);
	// printf("No of false positives = %d\n", no_false_pos);
	// printf("No of false negatives = %d\n", no_false_neg);
	if (flag == 1) {
		printf("%d %d %d %d %lf %lf\n", no_hh, no_cand,
				no_false_pos, no_false_neg, (double)no_false_pos / no_cand,
				(double)no_false_neg / no_hh);	
	}
}
*/

/*
 * Min-based estimation
 */
void esti_min(hh_tbl_p_t hh_tbl, unsigned char** cand_list, int no_cand, 
		double bias, char* outfile) {

	FILE* fp;
	long long min_v;
	int no_bit;
	int i, j, k, r;

	// open the file
	fp = fopen(outfile, "w");
	for (r=0; r<no_cand; ++r) {
		min_v = -1;
		no_bit = 0;
		for (i=0; i<hh_tbl->D; ++i) {
			no_bit += hh_tbl->bd[i];
			if (i != hh_tbl->D-1) 
				continue;
			for (j=0; j<hh_tbl->M[i]; ++j) {
				k = hh_tbl_find(hh_tbl, cand_list[r], 0, no_bit-1, i, j);
				if (min_v == -1 || hh_tbl->T[i][j][k] < min_v) {
					min_v = hh_tbl->T[i][j][k];
				}
			}
		}
		fprintf(fp, "%lf\n", (double)min_v - bias);
	}
	fclose(fp);
}


/*
 * Median-based estimation
 */
/*
void esti_median(hh_tbl_p_t hh_tbl_1, hh_tbl_p_t hh_tbl_2, unsigned char**
		cand_list, int no_cand, long long* ret_esti_val) {

	long long* val;
	int i, j, k;

	// calloc
	val = (long long*)calloc(hh_tbl_1->M[0], sizeof(long long));

	// error checking
	if (hh_tbl_2 == NULL) {
		fprintf(stderr, "ERR: require hh_tbl_2 to be non-null\n");
		return;
	}

	// estimation
	for (i=0; i<no_cand; ++i) {
		for (j=0; j<hh_tbl_1->M[0]; ++j) {
			k = hh_tbl_find(hh_tbl_1, cand_list[i], 0, 17, 0, j);
			val[j] = hh_tbl_2->T[0][j][k] - hh_tbl_1->T[0][j][k];
		}		
	}

	// free
	free(val);
}
*/

/*
 * Estimation III - linear programming based approach 
 */
/*
int esti_lp(hh_tbl_p_t hh_tbl, unsigned char** cand_list, int no_cand, 
		double thresh, double bias, char* outfile, unsigned char**
		ret_cand_list, double* ret_est) {

	FILE* fp; 
	char** a;						// intermediate A matrix
	int bucket;						// bucket
	int no_bit;
	int ret_no_cand = -1;
	lprec *lp;
	double* row;
	int* col;
	int var_cnt;
	double* est;
	int i, j, k, m;

	// calloc
	a = (char**)calloc(hh_tbl->K, sizeof(char*));
	for (i=0; i<hh_tbl->K; ++i) {
		a[i] = (char*)calloc(no_cand, sizeof(char));
	}
	row = (double*)calloc(no_cand + 1, sizeof(double));	
	col = (int*)calloc(no_cand + 1, sizeof(int));	
	est = (double*)calloc(no_cand + 1, sizeof(double));

	// create lp structure
	lp = make_lp(0, no_cand + 1);
	if (lp == NULL) {
		fprintf(stderr, "ERR: unable to create LP model\n");
	}
	set_verbose(lp, IMPORTANT);

	// print the objective function
	set_maxim(lp);
	for (i=0; i<no_cand; ++i) {
		row[i] = 1;	
		col[i] = i+1;
	}
	row[no_cand] = hh_tbl->K;
	col[no_cand] = no_cand+1;
	set_obj_fnex(lp, no_cand+1, row, col);	

	// add constraints
	set_add_rowmode(lp, TRUE);			// set mode TRUE to speed up
	no_bit = 0;
	for (i=0; i<hh_tbl->D; ++i) {
		no_bit += hh_tbl->bd[i];
		for (j=0; j<hh_tbl->M[i]; ++j) {
			// init
			for (k=0; k<hh_tbl->K; ++k) {
				memset(a[k], 0, no_cand * sizeof(char));
			}
			// set a = intermediate matrix
			for (k=0; k<no_cand; ++k) {
				bucket = hh_tbl_find(hh_tbl, cand_list[k], 0, no_bit-1, i, j);
				a[bucket][k] = 1;
			}
			// set row / column of constraint matrix
			for (k=0; k<hh_tbl->K; ++k) {
				if (hh_tbl->T[i][j][k] > 0) {
					memset(row, 0, (no_cand + 1) * sizeof(double));
					memset(col, 0, (no_cand + 1) * sizeof(int));
					var_cnt = 0;
					for (m=0; m<no_cand; ++m) {
						if (a[k][m] == 1) {
							row[var_cnt] = 1.0;
							col[var_cnt++] = m + 1;
						}
					}
					row[var_cnt] = 1.0;
					col[var_cnt++] = no_cand + 1;
					add_constraintex(lp, var_cnt, row, col, LE,
							hh_tbl->T[i][j][k]);
				}
			}
		}
	}
	set_add_rowmode(lp, FALSE);				// close the mode

	// solve lp
	// set_presolve(lp, PRESOLVE_ROWS | PRESOLVE_COLS | PRESOLVE_LINDEP,
	//		get_presolveloops(lp));	
	solve(lp);
	get_variables(lp, est);
	delete_lp(lp);
	for (i=0; i<no_cand; ++i) {
		est[i] = est[i] / MB - bias;
		if (est[i] < 0) {
			est[i] = 0;
		}
	}


	// output the distribution
	if (outfile != NULL) {
		fp = fopen(outfile, "w");
		for (i=0; i<no_cand; ++i) {
			fprintf(fp, "%lf\n", est[i]);
		}
		fclose(fp);
	}

	// return the new candidates
	if (ret_cand_list != NULL) {
		ret_no_cand = 0;
		for (i=0; i<no_cand; ++i) {
			if (est[i] >= thresh) {
				memcpy(ret_cand_list[ret_no_cand], cand_list[i], hh_tbl->n/8);
				if (ret_est != NULL) {
					ret_est[ret_no_cand] = est[i];
				}
				ret_no_cand++;
			}
		}
	} else if (ret_cand_list == NULL && ret_est != NULL) {
		// simply return the estimates
		memcpy(ret_est, est, no_cand * sizeof(double));
	}

	// free
	for (i=0; i<hh_tbl->K; ++i) {
		free(a[i]);
	}
	free(a);
	free(est);
	free(row);
	free(col);

	// return
	return ret_no_cand;
}
*/

/*
 * Estimation II - generate a matrix file
 */
void matrix_gen(hh_tbl_p_t hh_tbl, unsigned char** cand_list, int no_cand, int
		start_part, int end_part, char* outfile_A, char* outfile_y) {
	FILE* fp_A; 
	FILE* fp_y;
	char** a;						// intermediate A matrix
	int bucket;						// bucket
	int no_bit;
	int i, j, k, m;

	// calloc
	a = (char**)calloc(hh_tbl->K, sizeof(char*));
	for (i=0; i<hh_tbl->K; ++i) {
		a[i] = (char*)calloc(no_cand, sizeof(char));
	}
	
	// open the output files	
	fp_A = fopen(outfile_A, "w");
	fp_y = fopen(outfile_y, "w");

	// print the A matrix
	no_bit = 0;
	for (i=0; i<hh_tbl->D; ++i) {
		no_bit += hh_tbl->bd[i];
		if (!(i >= start_part && i <= end_part))
			continue;
		for (j=0; j<hh_tbl->M[i]; ++j) {
			// init
			for (k=0; k<hh_tbl->K; ++k) {
				memset(a[k], 0, no_cand * sizeof(char));
			}
			// set a = intermediate matrix
			for (k=0; k<no_cand; ++k) {
				bucket = hh_tbl_find(hh_tbl, cand_list[k], 0, no_bit-1, i, j);
				a[bucket][k] = 1;
			}	
			for (k=0; k<hh_tbl->K; ++k) {
				for (m=0; m<no_cand; ++m) {
					fprintf(fp_A, "%d ", a[k][m]);
				}
				fprintf(fp_A, "\n");
				fprintf(fp_y, "%lf\n", (double)(hh_tbl->T[i][j][k]));
			}
		}
	}

	// close files
	fclose(fp_A);
	fclose(fp_y);

	// free
	for (i=0; i<hh_tbl->K; ++i) {
		free(a[i]);
	}
	free(a);
}

/*
 * Analyze the non-heavy hitter buckets
 */
/*
double analyze_non_heavy_bucket(hh_tbl_p_t hh_tbl, double thresh, char*
		outfile) {
	FILE* fp = NULL;
	// int no_bit;
	// char*** a;
	int bucket;
	long long* byte_est;
	int no_est;					// no of partitions
	double avg = 0;
	double dummy;
	int i, j, k, m;

	// calloc
	a = (char***)calloc(hh_tbl->D, sizeof(char**));
	for (i=0; i<hh_tbl->D; ++i) {
		a[i] = (char**)calloc(hh_tbl->M[i], sizeof(char*));
		for (j=0; j<hh_tbl->M[i]; ++j) {
			a[i][j] = (char*)calloc(hh_tbl->K, sizeof(char));
		}
	}
	// find no of estimations
	no_est = 0;
	for (i=0; i<hh_tbl->D; ++i) {
		for (j=0; j<hh_tbl->M[i]; ++j) {
			no_est++;
		}
	}
	byte_est = (long long*)calloc(no_est, sizeof(long long));
	
	// set a
	for (i=0; i<no_cand; ++i) {
		no_bit = 0;
		for (j=0; j<hh_tbl->D; ++j) {
			no_bit += hh_tbl->bd[j];
			for (k=0; k<hh_tbl->M[j]; ++k) {
				bucket = hh_tbl_find(hh_tbl, cand_list[i], 0, no_bit-1, j, k);
				a[j][k][bucket] = 1;
			}
		}	
	}
	// open the file
	if (outfile != NULL) {
		fp = fopen(outfile, "w");
	}

	// find E[min(non_hh[0], non_hh[1], ..., non_hh[M(D-1)])]
	srand(23456);
	for (i=0; i<10000; ++i) {
		m = 0;
		for (j=0; j<hh_tbl->D; ++j) {
			if (j <= 4)
				continue;
			for (k=0; k<hh_tbl->M[j]; ++k) {
				bucket = rand() % hh_tbl->K;
				while(hh_tbl->T[j][k][bucket] >= thresh ) {
					bucket = rand() % hh_tbl->K;
				}
				while (a[j][k][bucket] == 1 || hh_tbl->T[j][k][bucket] == 0) {
					bucket = rand() % hh_tbl->K;
				}
				byte_est[m++] = hh_tbl->T[j][k][bucket];
			}
		}
		dummy = UINT_MAX;
		m = 0;
		for (j=0; j<hh_tbl->D; ++j) {
			if (j <= 4)
				continue;
			for (k=0; k<hh_tbl->M[j]; ++k) {
				if (byte_est[m] < dummy) {
					dummy = byte_est[m];
				}
				m++;
			}
		}
		if (outfile != NULL) {
			fprintf(fp, "%lf\n", dummy);
		}
		avg += dummy;
	}
	avg /= 10000;
	// printf("# E[min(non_hh[0..M[D-1]])] = %lf\n", avg / MB);

	// close file
	if (outfile != NULL) {
		fclose(fp);
	}

	// free
	for (i=0; i<hh_tbl->D; ++i) {
		for (j=0; j<hh_tbl->M[i]; ++j) {
			free(a[i][j]);
		}
		free(a[i]);
	}
	free(a);
	free(byte_est);

	// return
	return avg / MB;
}
*/

/*
 * Count the number of hh bucket in each table
 */
/*
void count_hh_bucket(hh_tbl_p_t hh_tbl, double thresh) {
	int cnt;
	int i, j, k;

	for (i=0; i<hh_tbl->D; ++i) {
		for (j=0; j<hh_tbl->M[i]; ++j) {
			cnt = 0;
			for (k=0; k<hh_tbl->K; ++k) {
				if (hh_tbl->T[i][j][k] >= thresh * MB) {
					cnt++;
				}
			}
			printf("Count (%d,%d) = %d\n", i, j, cnt);
		}
	}
}
*/

/*
 * iter-decode, after finding the first set of candidates
 */
/*
int iter_decode(hh_tbl_p_t hh_tbl_1, hh_tbl_p_t hh_tbl_2, int bias, unsigned
		char** cand_list, int no_cand, char* esti_file, unsigned char**
		ret_cand_list) {
	FILE* fp;
	char str[100];
	double* est; 
	hh_tbl_p_t new_hh_tbl_1, new_hh_tbl_2;
	int i;

	// calloc
	est = (double*)calloc(no_cand, sizeof(double));

	// error checking
	if (hh_tbl_2 == NULL) {
		fprintf(stderr, "ERR: require hh_tbl_2 to be non-null\n");
		return -1;
	}

	// obtain the estimates	
	fp = fopen(esti_file, "r");
	if (fp == NULL) {
		fprintf(stderr, "ERR: cannot open %s for iter_decode()\n", esti_file);
		exit(-1);
	}
	i = 0;
	while (fgets(str, sizeof(str), fp) != NULL) {
		fgets(str, sizeof(str), fp);		// read the value
		sscanf(str, "%lf\n", &est[i]);
		i++;
	}
	fclose(fp);

	// init new hh tables
	new_hh_tbl_1 = hh_tbl_init(hh_tbl_1->D, hh_tbl_1->M, hh_tbl_1->K,
			hh_tbl_1->n, hh_tbl_1->bd);	
	hh_tbl_copy(hh_tbl_1, new_hh_tbl_1);
	new_hh_tbl_2 = hh_tbl_init(hh_tbl_2->D, hh_tbl_2->M, hh_tbl_2->K,
			hh_tbl_2->n, hh_tbl_2->bd);	
	hh_tbl_copy(hh_tbl_2, new_hh_tbl_2);

	// update the residual (only for the 1st table)
	for (i=0; i<no_cand; ++i) {

	}	
}
*/


/*
 * Deduction of heavy changes
 */
/*
void deduct_est(hh_tbl_p_t hh_tbl, unsigned char** cand_list, int no_cand,
		double* est) {
	int no_bit;
	int i, j, k, r;

	for (r=0; r<no_cand; ++r) {	
		no_bit = 0;
		for (i=0; i<hh_tbl->D; ++i) {
			no_bit += hh_tbl->bd[i];
			for (j=0; j<hh_tbl->M[i]; ++j) {
				k = hh_tbl_find(hh_tbl, cand_list[r], 0, no_bit-1, i, j);
				hh_tbl->T[i][j][k] -= (long long)(est[r] * MB);
			}
		}
	}
}
*/

/*
 * Start the clock
 */
double start_clock() {
	struct timeval tt;
	gettimeofday(&tt,NULL);
	return tt.tv_sec + (double)tt.tv_usec / 1000000;
}

/*
 * stop the clock
 */
double stop_clock(double ts) {
	struct timeval tt;
	gettimeofday(&tt,NULL);
	return (tt.tv_sec + (double)tt.tv_usec / 1000000) - ts;
}

/* by qhuang 20130618 */
void output_cand(unsigned char** cand_list, int no_cand, const char* filename) {
	FILE* fp;
	int i;

	fp = fopen(filename, "w");
    for (i = 0; i<no_cand; ++i) {
        fprintf(fp, "%u\n", (*((unsigned int*)(cand_list[i]))));
    }
	fclose(fp);
}
