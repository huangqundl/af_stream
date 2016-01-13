#ifndef SKETCH_H
#define SKETCH_H

// qhuang: this is the file "change.h" in original deltoid

/// CGT Sketch
typedef struct AbsChange_type{
    /// number of rows
    unsigned int depth;

    /// number of buckets in each row
    unsigned int width;

    /// length of keys in bits
    int lgn; 

    /// total number of buckets
    int size;

    /// coefficients for hash functions
    long long *testa;

    /// coefficients for hash functions
    long long *testb;

    /// counters in buckets
    long long ** counts;
} AbsChange_type;

/// init sketch
// @param w width of sketch
// @param d depth of sketch
// @param l length of keys
// @return the pointer to sketch
AbsChange_type * AbsChange_Init(int w, int d, int l);

/// update sketch with one data item
// @param sk the target sketch
// @param k key of the data item
// @param v value of the data item
void AbsChange_Update(AbsChange_type *sk, unsigned long k, long long v );

/// identify heavy keys
// @param sk the target sketch
// @param t the threshold for detection
// @param cand the detected heavy keys
// @param max_cand the maximum number of detected heavy keys allowed
// @return the number of detected keys
unsigned int AbsChange_Output(AbsChange_type *sk, double t, unsigned char**, unsigned int); 

/// free the sketch
// @param sk the target sketch
void AbsChange_Destroy(AbsChange_type *sk);

/// print out sketch to file
// @param sk the target sketch
// @param name name of the output file
void AbsChange_write(AbsChange_type *sk, char* name);

/// print out sketch to file in plaintext
// @param sk the target sketch
// @param name name of the output file
void AbsChange_write_plaintext(AbsChange_type *sk, const char* name);

/// reset the sketch
// @param sk the target sketch
void AbsChange_reset(AbsChange_type *sk);

/// calculate the difference of two sketches
// @param sk1 
// @param sk2
// @param res sk1-sk2
void AbsChange_diff(AbsChange_type *sk1, AbsChange_type *sk2, AbsChange_type *res);

/// copy a sketch
// @param from the source sketch
// @param to the target sketch
void AbsChange_copy(AbsChange_type *from, AbsChange_type *to);

#endif
