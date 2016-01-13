/*
 * hh_tbl.c
 * - random partition table
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Hash.hpp"
#include "hh_tbl.h"

/*
 * calloc with error message
 */
static void* safe_calloc(size_t nmemb, size_t size, char* name) {
	void* ret;
	ret = calloc(nmemb, size);
	if (ret == NULL) {
		fprintf(stderr, "ERR: cannot alloc %s\n", name);
		exit(-1);
	}
	return ret;
}

/*
 * mangle
 */
static void mangle(const unsigned char* key, unsigned char* ret_key,
		int nbytes) {
	unsigned long long new_key = 0;
	int i;
	for (i=0; i<nbytes; ++i) {
		new_key |= key[nbytes-i-1] << (i * 8);
	}
	new_key = (new_key * 3054961753) & (0xffffffff);
	for (i=0; i<nbytes; ++i) {
		ret_key[i] = (new_key >> (i * 8)) & 0xff;
	}
}

/*
 * unmangle
 */                                                           
static void unmangle(const unsigned char* key, unsigned char* ret_key,       
		int nbytes) {                                         
	// 10001^-1 mod 2^32 = 3472992753                         
	// 1001^-1 mod 2^32 = 3054961753                          
	unsigned long long new_key = 0;                           
	int i;                                                    
	for (i=0; i<nbytes; ++i) {                                
		new_key |= key[i] << (i * 8);                         
	}                                                         
	new_key = (new_key * 1001) & (0xffffffff);                
	for (i=0; i<nbytes; ++i) {                                
		ret_key[nbytes-i-1] = (new_key >> (i * 8)) & 0xff;    
	}                                                         
}

/*
 * Initialize the hh table
 */
hh_tbl_p_t hh_tbl_init(int D, int* M, int K, int n, int* bd) {
	hh_tbl_p_t hh_tbl;
	int i, j;

	// error checking
	if (n / 8 > 13 || n % 8 != 0) {		// maximum # of bits of a key = 104
		fprintf(stderr, "ERR: n too large or n not multiple of 8\n");
		exit(-1);
	}

	// safe calloc
	hh_tbl = (hh_tbl_p_t)safe_calloc(1, sizeof(hh_tbl_t), "hh_tbl");
	hh_tbl->T = (long long***)safe_calloc(D, sizeof(long long**), "tbl->T");
	for (i=0; i<D; ++i) {
		hh_tbl->T[i] = (long long**)safe_calloc(M[i], sizeof(long long*),
				"tbl->T[i]");
		for (j=0; j<M[i]; ++j) {
			hh_tbl->T[i][j] = (long long*)safe_calloc(K, sizeof(long long),
					"tbl->T[i][j]");
		}
	}
	hh_tbl->M = (int*)safe_calloc(D, sizeof(int), "tbl->M");
	hh_tbl->bd = (int*)safe_calloc(D, sizeof(int), "tbl->bd");

	// set parameters
	hh_tbl->D = D;
	memcpy(hh_tbl->M, M, D * sizeof(int));
	hh_tbl->K = K;
	hh_tbl->n = n;
	memcpy(hh_tbl->bd, bd, D * sizeof(int));

	// return
	return hh_tbl;
}

/*
 * Operate - return a random bucket
 */
unsigned int hh_tbl_find(hh_tbl_p_t tbl, unsigned char* key, int start_bit,
		int end_bit, int part_no, int array_no) {
	unsigned char key_str[50];		// assume n/8 + 4 <= 50
	unsigned int oper;
	char bit;
	unsigned int ret_bucket;

	int i;

	// set the key string
	memset(key_str, 0, sizeof(key_str));
	i = start_bit;		// start_bit == 0 in all cases
	while (i <= end_bit) {
		if (end_bit - i + 1 >= 8) {
			key_str[i/8] = key[i/8];	
			i += 8;
		} else {
			bit = (key[i/8] & (1 << (8 - ((i%8) + 1)))) > 0 ?  1 : 0;
			key_str[i/8] |= (bit << (8 - ((i%8) + 1)));
			i++;
		}
	}

	// set the operator and add it to key string
	oper = part_no * tbl->M[part_no] + array_no;
	memcpy(key_str + tbl->n/8, &oper, sizeof(unsigned int));

	/*
	// hash
	MD5_CTX md5;
	unsigned char digest[16];
	MD5_Init(&md5);
	MD5_Update(&md5, key_str, tbl->n/8 + sizeof(unsigned int));
	MD5_Final(digest, &md5);
	memcpy(&ret_bucket, digest, sizeof(unsigned int));	// take 1st 4 bytes
	ret_bucket = (ret_bucket % tbl->K);
	*/

	ret_bucket = AwareHash((char*)key_str, 
			(unsigned int)(tbl->n/8 + sizeof(unsigned int))) % (tbl->K);

	// return
	return ret_bucket;
}


/*
 * Sequential hashign algorithm - encode keys (a.k.a. update step)
 */
void hh_tbl_encode(hh_tbl_p_t tbl, unsigned char* key, long long val) {
	unsigned char key_to_add[13];
	long long val_to_add = 0;
	int no_bit; 
	int i, j, k;

	// mangle
	// mangle(key, key_to_add, tbl->n/8);
	memcpy(key_to_add, key, tbl->n/8 * sizeof(unsigned char));

	// add key/val to table
	val_to_add = val;
	no_bit = 0;
	for (i=0; i<tbl->D; ++i) {
		no_bit += tbl->bd[i];
		for (j=0; j<tbl->M[i]; ++j) {
			k = hh_tbl_find(tbl, key_to_add, 0, no_bit-1, i, j);
			tbl->T[i][j][k] += val_to_add;
		}
	}
}


/*
 * Determine the 0/1 value of a bitmap based on the pre-specified threshold
 * (need to change later) (tbl_2 can be null, if not null, take the absolute
 * change)
 */
void hh_tbl_thresh(hh_tbl_p_t tbl, double thresh, double bias, char***
		ret_bitmap) {
	long long val;
	int i, j, k;

	// error checking	
	if (ret_bitmap == NULL) {
		return;
	}

	// set the bitmap
	for (i=0; i<tbl->D; ++i) {
		for (j=0; j<tbl->M[i]; ++j) {
			for (k=0; k<tbl->K; ++k) {
				val = tbl->T[i][j][k];
				val = val > 0 ? val : -val;
				if (val >= (thresh + bias)) {
					ret_bitmap[i][j][k] = 1;
				}
			}
		}
	}
}


/*
 * Sequential hashing algorithm - decode keys (a.k.a. detection step). 
 * A bias factor is used.
 */
unsigned int hh_tbl_decode(hh_tbl_p_t tbl, hh_tbl_p_t tbl_1, hh_tbl_p_t tbl_2, 
		double thresh, double bias, int miss, unsigned char** ret_cand_list) {
	char*** bitmap;
	unsigned char** cand_list;
	unsigned char **tmp_cand_list;
	unsigned char cand[13];
	unsigned int no_cand;
	int tmp_no_cand;
	int no_miss;
	int* allowed_miss;
	int start_bit, end_bit;
	int no_bit;
	char bit;
	int bucket;

	int i, j, k, m;

	// safe calloc
	bitmap = (char***)safe_calloc(tbl->D, sizeof(char**), "bitmap");
	for (i=0; i<tbl->D; ++i) {
		bitmap[i] = (char**)safe_calloc(tbl->M[i], sizeof(char*),
				"bitmap[i]");
		for (j=0; j<tbl->M[i]; ++j) {
			bitmap[i][j] = (char*)safe_calloc(tbl->K, sizeof(char),
					"bitmap[i][j]");
		}
	}
	cand_list = (unsigned char**)safe_calloc(MAX_NO_CAND, sizeof(unsigned
				char*), "cand_list");
	for (i=0; i<MAX_NO_CAND; ++i) {
		cand_list[i] = (unsigned char*)safe_calloc(tbl->n/8, sizeof(unsigned
					char), "cand_list[i]");
	}
	tmp_cand_list = (unsigned char**)safe_calloc(MAX_NO_CAND, sizeof(unsigned
				char*), "tmp_cand_list");
	for (i=0; i<MAX_NO_CAND; ++i) {
		tmp_cand_list[i] = (unsigned char*)safe_calloc(tbl->n/8,
				sizeof(unsigned char), "tmp_cand_list[i]");
	}
	allowed_miss = (int*)safe_calloc(tbl->D, sizeof(int), "allowed_miss");

	// set the allowed miss
	for (i=0; i<tbl->D; ++i) {
		if (i < tbl->D-1) {
			allowed_miss[i] = tbl->M[i];
		} else {
			allowed_miss[i] = miss;
		}
	}

	// construct the bitmap
	hh_tbl_thresh(tbl, bias, thresh, bitmap);

	// enumerate the candidates
	memset(cand_list[0], 0, tbl->n/8);
	no_cand = 1;
	no_bit = 0;
	for (i=0; i<tbl->D; ++i) {
		// reset the list
		for (j=0; j<no_cand; ++j) {
			memcpy(tmp_cand_list[j], cand_list[j], tbl->n/8);
		}
		tmp_no_cand = no_cand;
		// start comparing
		no_cand = 0;
		start_bit = no_bit;
		no_bit += tbl->bd[i];
		end_bit = start_bit + tbl->bd[i] - 1;
		for (j=0; j<(1<<tbl->bd[i]); ++j) {
			for (k=0; k<tmp_no_cand; ++k) {
				memcpy(cand, tmp_cand_list[k], tbl->n/8);
				for (m=start_bit; m<=end_bit; ++m) {
					bit = (j >> (end_bit-m)) % 2;
					cand[m/8] |= (bit << (8 - ((m%8) + 1)));
				}
				no_miss = 0;
				for (m=0; m<tbl->M[i]; ++m) {
					bucket = hh_tbl_find(tbl, cand, 0, end_bit, i, m);
					if (bitmap[i][m][bucket] == 0) {
						if (tbl_1 != NULL && tbl_2 != NULL) {
							// heavy changer detection
							if (tbl_1->T[i][m][bucket] >= (thresh + bias)
									|| tbl_2->T[i][m][bucket] >= (thresh +
										bias)) {
								no_miss++;
								if (no_miss > allowed_miss[i]) {
									break;
								}
							} else {
								no_miss = allowed_miss[i] + 1;
								break;
							}
						} else {
							// heavy hitter detection (no miss is allowed)
							no_miss = allowed_miss[i] + 1;
							break;
						}
					}
				}
				if (no_miss <= allowed_miss[i]) {
					memcpy(cand_list[no_cand], cand, tbl->n/8);
					no_cand++;
					if (no_cand >= MAX_NO_CAND) {
						fprintf(stderr, "ERR: cand list exhausted\n");
						//exit(-1);
                        //break;
                        return 0;
					}
				}
			}
		}
		// debug
		// fprintf(stderr, "No of candidates in partition %d, %d\n", i,
		// no_cand);
	}

	// return
	if (ret_cand_list != NULL) {
		unsigned char new_key[13];
		for (i=0; i<no_cand; ++i) {
			//unmangle(cand_list[i], new_key, tbl->n/8);
			memcpy(new_key, cand_list[i], tbl->n/8);
			memcpy(ret_cand_list[i], new_key, tbl->n/8);
		}
	}	

	// free
	for (i=0; i<tbl->D; ++i) {
		for (j=0; j<tbl->M[i]; ++j) {
			free(bitmap[i][j]);
		}
		free(bitmap[i]);
	}
	free(bitmap);
	for (i=0; i<MAX_NO_CAND; ++i) {
		free(cand_list[i]);
	}
	free(cand_list);
	for (i=0; i<MAX_NO_CAND; ++i) {
		free(tmp_cand_list[i]);
	}
	free(tmp_cand_list);
	free(allowed_miss);

	// return
	return no_cand;
}


/*
 * Write to a file
 */
void hh_tbl_write(hh_tbl_p_t tbl, char* outfile) {
	FILE* fp; 
	int i, j;

	// open a file
	if ((fp = fopen(outfile, "wb")) == NULL) {
		fprintf(stderr, "ERR: cannot open %s\n", outfile);
		exit(-1);
	}

	// write to a file (including n and bd???)
	fwrite(&(tbl->D), sizeof(int), 1, fp);
	fwrite(tbl->M, sizeof(int), tbl->D, fp);
	fwrite(&(tbl->K), sizeof(int), 1, fp);
	fwrite(&(tbl->n), sizeof(int), 1, fp);
	fwrite(tbl->bd, sizeof(int), tbl->D, fp);
	for (i=0; i<tbl->D; ++i) {
		for (j=0; j<tbl->M[i]; ++j) {
			fwrite(tbl->T[i][j], sizeof(long long), tbl->K, fp);
		}
	}

	// close the file
	fclose(fp);
}


/*
 * Copy the content from one hh-tbl to another (they should have same params)
 */
void hh_tbl_copy(hh_tbl_p_t tbl, hh_tbl_p_t ret_tbl) {
	int i, j;

	if (ret_tbl == NULL) {
		fprintf(stderr, "ERR: ret_tbl in hh_tbl_copy() is NULL\n");
		return;
	}

	for (i=0; i<tbl->D; ++i) {
		for (j=0; j<tbl->M[i]; ++j) {
			memcpy(ret_tbl->T[i][j], tbl->T[i][j], tbl->K * sizeof(long long));
		}
	}
	ret_tbl->D = ret_tbl->D;
	memcpy(ret_tbl->M, tbl->M, tbl->D * sizeof(int));
	ret_tbl->K = tbl->K;
	ret_tbl->n = tbl->n;
	memcpy(ret_tbl->bd, tbl->bd, tbl->D * sizeof(int));
}


/*
 * print summary
 */
void hh_tbl_print_summary(hh_tbl_p_t tbl) {
	int M = 0;
	int i;

	fprintf(stderr, "# n = %d, D = %d, K = %d\n", tbl->n, tbl->D, tbl->K);
	fprintf(stderr, "# bd[0] = %d, bd[1:D-1] = %d\n", tbl->bd[0], tbl->bd[1]);
	for (i=0; i<tbl->D; ++i) {
		M += tbl->M[i];
	}
	fprintf(stderr, "# M[0] = %d, M[1] = %d, M[D-1] = %d, total M = %d\n",
			tbl->M[0], tbl->M[1], tbl->M[tbl->D-1], M);
    fprintf(stderr, "# Total counters: %d\n", M * tbl->K);
}
	
/*
 * reset the counters
 */
void hh_tbl_reset(hh_tbl_p_t hh_tbl) {
	int i, j;
	
	for (i=0; i<hh_tbl->D; ++i) {
		for (j=0; j<hh_tbl->M[i]; ++j) {
			memset(hh_tbl->T[i][j], 0, hh_tbl->K * sizeof(long long)); 
		}
	}
}

/*
 * Destroy
 */
void hh_tbl_destroy(hh_tbl_p_t hh_tbl) {
	int i, j;

	for (i=0; i<hh_tbl->D; ++i) {
		for (j=0; j<hh_tbl->M[i]; ++j) {
			free(hh_tbl->T[i][j]);
		}
		free(hh_tbl->T[i]);
	}
	free(hh_tbl->T);
	free(hh_tbl->M);
	free(hh_tbl->bd);
	free(hh_tbl);
}

/*
 *
 * Write to a file without encoding
 * used for test
 * by qhuang 20130618
 *
 */
void hh_tbl_write_plaintext(hh_tbl_p_t tbl, const char* outfile) {
	FILE* fp; 
	int i, j, k;

	// open a file
	if ((fp = fopen(outfile, "w")) == NULL) {
		fprintf(stderr, "ERR: cannot open %s\n", outfile);
		exit(-1);
	}

	// write to a file (including n and bd???)
    fprintf(fp, "Length of key: %d\n", tbl->n);
	fprintf(fp, "Num of sketches: %d\n", tbl->D);
	fprintf(fp, "Hash row of each sketch:\n  ");
    for (i=0; i<tbl->D; ++i) {
        fprintf(fp, "%d ", tbl->M[i]);
    }
    fprintf(fp, "\n");
	fprintf(fp, "Bit length of each sketch:\n  ");
    for (i=0; i<tbl->D; ++i) {
        fprintf(fp, "%d ", tbl->bd[i]);
    }
    fprintf(fp, "\n");
    fprintf(fp, "Num of buckets: %d\n", tbl->K);

	for (i=0; i<tbl->D; ++i) {
		for (j=0; j<tbl->M[i]; ++j) {
            for (k=0; k<tbl->K; ++k) {
                fprintf(fp, "%lld ", tbl->T[i][j][k]);
            }
            fprintf(fp, ";    ");
		}
        fprintf(fp, "\n");
	}

	// close the file
	fclose(fp);
}
