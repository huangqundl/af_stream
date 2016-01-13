#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <unordered_map>
#include "../Hash.hpp"

/**
 * Structure of key
 */
typedef struct cnt_tbl_key_s {
    /// 16-byte key
    unsigned char key[16];
} cnt_tbl_key_t;

namespace std {
/**
 * Object for hash
 */
    template<> struct hash<cnt_tbl_key_t> {
        size_t operator()(const cnt_tbl_key_t& v) const {
            return AwareHash(v.key, 16);
        }
    };

/**
 * Object for equality
 */
    template<> struct equal_to<cnt_tbl_key_t> {
        constexpr bool operator() (const cnt_tbl_key_t &x, const cnt_tbl_key_t &y) const {
            return memcmp(x.key, y.key, 16)==0;
        }
    };
}

/// unit of associative array
typedef struct cnt_tbl_s * cnt_tbl_p_t;

/// associative array
typedef struct cnt_tbl_s {
    /// hash table
    std::unordered_map<cnt_tbl_key_t, long long> data;
    /// maximum size of hash table
    unsigned int max_len;
    /// length of keys in bits
    unsigned int n;
} cnt_tbl_t;

/**
 * create an associative array
 * @param length maximum size of associative array
 * @param n length of keys
 * @return a pointer to the associative array
 */
cnt_tbl_p_t cnt_tbl_init(unsigned int length, int n);

/**
 * update the associative array with an data item
 * @param cnt_tbl the target associative array
 * @param key key of the data item
 * @param val value of the data item
 */
void cnt_tbl_update(cnt_tbl_p_t cnt_tbl, unsigned char* key, int val);

/**
 * identify heavy keys
 * @param cnt_tbl_new associative array for current epoch
 * @param cnt_tbl_old associative array for last epoch
 * @param thresh threshold for heavy keys
 * @param cand_list detected keys
 * @return number of detected heavy keys
 */
unsigned int cnt_tbl_find_cand(cnt_tbl_p_t cnt_tbl_new, cnt_tbl_p_t cnt_tbl_old, double thresh, unsigned char** cand_list);

/**
 * copy associative array
 * @param cnt_tbl_from source associative array
 * @param cnt_tbl_to target associative array
 */
void cnt_tbl_copy(cnt_tbl_p_t cnt_tbl_from, cnt_tbl_p_t cnt_tbl_to);

/**
 * reset associative array
 * @param cnt_tbl the target associative array
 */
void cnt_tbl_reset(cnt_tbl_p_t cnt_tbl);

/**
 * reset associative array
 * @param cnt_tbl the target associative array
 * @param output filename of the output file
 */
void cnt_tbl_print(cnt_tbl_p_t cnt_tbl, const char* output);

/**
 * free associative array
 * @param cnt_tbl the target associative array
 */
void cnt_tbl_destroy(cnt_tbl_p_t cnt_tbl);

#endif
