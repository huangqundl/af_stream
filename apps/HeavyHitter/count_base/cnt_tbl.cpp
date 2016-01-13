#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>

#include "common.hpp"
#include "cnt_tbl.hpp"

typedef std::unordered_set<cnt_tbl_key_t> myset;

unsigned int cnt_tbl_find_cand(cnt_tbl_p_t cnt_tbl_cur, cnt_tbl_p_t cnt_tbl_old, double thresh, unsigned char** cand_list) {

    myset ret;
    unsigned int no_cand = 0;

    for(std::unordered_map<cnt_tbl_key_t, long long>::iterator it = cnt_tbl_cur->data.begin(); it != cnt_tbl_cur->data.end(); ++it) {
        long long old_value = 0;

        if (cnt_tbl_old != NULL) {
            // heavy changer
            if (cnt_tbl_old->data.find(it->first) != cnt_tbl_old->data.end()) {
                old_value = cnt_tbl_old->data[it->first];
            }
        }

        if (abs(it->second - old_value) >= thresh) {
            ret.insert(it->first);
        }
    }

    if (cnt_tbl_old != NULL) {
        for(std::unordered_map<cnt_tbl_key_t, long long>::iterator it = cnt_tbl_old->data.begin(); it != cnt_tbl_old->data.end(); ++it) {
            long long new_value = 0;

            if (cnt_tbl_cur->data.find(it->first) != cnt_tbl_cur->data.end()) {
                new_value = cnt_tbl_cur->data[it->first];
            }

            if (abs(it->second - new_value) >= thresh) {
                ret.insert(it->first);
            }

        }
    }

    for(std::unordered_set<cnt_tbl_key_t>::iterator set_it = ret.begin(); set_it != ret.end(); ++set_it) {
        cnt_tbl_key_t key = *set_it;
		memcpy(cand_list[no_cand], &key.key, cnt_tbl_cur->n/8);
        no_cand++;
    }

    return no_cand;
}


void cnt_tbl_print(cnt_tbl_p_t cnt_tbl, const char* output) {
    FILE* fp;

	// open a file
	if ((fp = fopen(output, "w")) == NULL) {
		fprintf(stderr, "ERR: cannot open %s\n", output);
		exit(-1);
	}

    unsigned int len = cnt_tbl->data.size();
    fprintf(fp, "length: %u\n", len);
    for(std::unordered_map<cnt_tbl_key_t, long long>::iterator it = cnt_tbl->data.begin(); it != cnt_tbl->data.end(); ++it) {
        cnt_tbl_key_t key = it->first;
        if (cnt_tbl->n == 32) {
            char addr1[30];
            unsigned int* ptr = (unsigned int*) key.key;
            fprintf(fp, "%s %lld\n", ip2a(*ptr, addr1), it->second);
        }
        else {
            char addr1[30];
            char addr2[30];
            unsigned int* ptr = (unsigned int*) key.key;
            fprintf(fp, "%s %s %lld\n", ip2a(*ptr, addr1), ip2a(*(ptr+1), addr2), it->second);
        }
    }

	// close the file
	fclose(fp);
}

void cnt_tbl_copy(cnt_tbl_p_t cnt_tbl_from, cnt_tbl_p_t cnt_tbl_to) {
    cnt_tbl_to->data = cnt_tbl_from->data;
}

void cnt_tbl_reset(cnt_tbl_p_t cnt_tbl) {
    cnt_tbl->data.clear();
}

void cnt_tbl_update(cnt_tbl_p_t cnt_tbl, unsigned char* key_str, int val) {
    cnt_tbl_key_t key;
    memcpy(key.key, key_str, cnt_tbl->n/8);

    if (cnt_tbl->data.find(key) != cnt_tbl->data.end()) {
        cnt_tbl->data[key] += val;
    }
    else {
        if (cnt_tbl->data.size() < cnt_tbl->max_len) {
            cnt_tbl->data[key] = val;
        }
        else {
            // need to modify
            std::unordered_map<cnt_tbl_key_t, long long>::iterator it;
            long long min = val;
            for(it = cnt_tbl->data.begin(); it != cnt_tbl->data.end(); ++it) {
                if (it->second < min) {
                    min = it->second;
                }
            }
            for(it = cnt_tbl->data.begin(); it != cnt_tbl->data.end(); ) {
                it->second -= min;
                if (it->second <= 0) {
                    cnt_tbl->data.erase(it++);
                }
                else {
                    it++;
                }
            }
            if (min < val) {
                if (cnt_tbl->data.size() >= cnt_tbl->max_len) {
                    fprintf(stderr, "Warning: cnt tbl update error\n");
                }
                cnt_tbl->data[key] = val - min;
            }
        }
    }
}

cnt_tbl_p_t cnt_tbl_init(unsigned int length, int n) {
    cnt_tbl_p_t ret = (cnt_tbl_p_t)calloc(1, sizeof(cnt_tbl_t));
    ret->n = n;
    ret->max_len = length;
    return ret;
}

void cnt_tbl_destroy(cnt_tbl_p_t cnt_tbl) {
    free(cnt_tbl);
}
