#ifndef _FAST_SKETCH_H
#define _FAST_SKETCH_H

// qhuang: this is the file "change.h" in original deltoid

/// Fast Sketch structure
typedef struct FastSk_type{
    /// number of rows in the single column
    unsigned int depth;

    /// number of counters in each bucket
    unsigned int width;

    /// number of total counters
    int size;

    /// number of rows which a key is hashed to
    int hashrow;

    /// coefficients of hash functions
    long long *testa;

    /// coefficients of hash functions
    long long *testb;

    /// length of keys
    int lgn;

    /// writable variable
    long long * counts;
} FastSk_type;

/// init sketch
// @param d depth of sketch
// @param l length of keys
// @param h number of rows a key hash
// @return the pointer to sketch
FastSk_type * FastSk_Init(int d, int l, int h);

/// update sketch with one data item
// @param sk the target sketch
// @param k key of the data item
// @param v value of the data item
void FastSk_Update(FastSk_type *sk, unsigned long k, long long v); 

/// identify heavy keys
// @param sk the target sketch
// @param t the threshold for detection
// @param cand the detected heavy keys
// @param max_cand the maximum number of detected heavy keys allowed
// @param hc whether perform heavy changer detection, otherwise heavh hitter
// @return the number of detected keys
unsigned FastSk_Output(FastSk_type *sk, double t, unsigned char** cand, unsigned int max_cand, int hc); 

/// free the sketch
// @param sk the target sketch
void FastSk_Destroy(FastSk_type *sk);

/// print out sketch to file
// @param sk the target sketch
// @param name name of the output file
void FastSk_write(FastSk_type *sk, char* name);

/// print out sketch to file in plaintext
// @param sk the target sketch
// @param name name of the output file
void FastSk_write_plaintext(FastSk_type *sk, const char* name);

/// reset the sketch
// @param sk the target sketch
void FastSk_reset(FastSk_type *sk);

/// calculate the difference of two sketches
// @param sk1 
// @param sk2
// @param res sk1-sk2
void FastSk_diff(FastSk_type *sk1, FastSk_type *sk2, FastSk_type *res);

/// copy a sketch
// @param from the source sketch
// @param to the target sketch
void FastSk_copy(FastSk_type *from, FastSk_type *to);

#endif
