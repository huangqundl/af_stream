/********************************************************************
Find large absolute changes between two data streams
G. Cormode 2003,2004

Last modified: 2004-09-14

This work is licensed under the Creative Commons
Attribution-NonCommercial License. To view a copy of this license,
visit http://creativecommons.org/licenses/by-nc/1.0/ or send a letter
to Creative Commons, 559 Nathan Abbott Way, Stanford, California
94305, USA. 
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "prng.h"
#include "massdal.h"
#include "sketch.h"

// qhuang: this is the file "change.c" in original deltoid

/******************************************************************/

#define min(x,y)	((x) < (y) ? (x) : (y))
#define max(x,y)	((x) > (y) ? (x) : (y))

long long fast_abs(long long a) {
	if (a < 0) {
		return -a;
	}
	return a;
}

unsigned long fastfindone(long long *count, int l, long long thresh) 
{
  // internal routine used to find deltoids. 

  int i;
  long long c;
  unsigned long j,k;

  // search through a set of tests to detect whether there is a deltoid there
    // returns 0 if none found, returns id of found item otherwise. 

  j=1;
  k=0;

  if (fast_abs(count[0])<thresh) 
    k=0;
  else
    {
      c = count[0];      
      for (i=l;i>0;i--)
	{
	  // main test: if one side is above threshold and the otherside is not
	  if (((fast_abs(count[i])<thresh) && (fast_abs(c-count[i])<thresh)) ||
	      ((fast_abs(count[i])>=thresh) && (fast_abs(c-count[i])>=thresh)))
	    {
	      k=0;
	      break; 
              // if test fails, bail out
	    }	  
	  if(fast_abs(count[i])>=thresh) 
	    k+=j;
	  j=j<<1;
	  // build the binary representation of the item
	}
    }
  return k;
}

void fast_quitmemory(void * pointer)
     // quit if a memory allocation failed
{
  if (pointer==NULL) exit(1);
}

void fast_loginsert(long long *lists, unsigned long val, int length, long long diff) 
{
  // internal routine used in update
  // lists is a list of 'length' counts
  // val is the item being added
  // diff is the amount (positive or negative)
  //    that its count changes by

  int i;

  // update the logn different tests for a particular item  
  lists[0]+=diff;
  for (i=length;i>0;i--) 
  {
	  if ((val&1)==1)
		  lists[i]+=diff;
	  val>>=1;
  }
}


/******************************************************************/

FastSk_type * FastSk_Init(int depth, int lgn, int hash_row)
{
  // initialize the data structure for finding absolute changes
  // width = 1/eps = width of hash functions
  // depth = number of independent repetitions to avoid misses
  // lgn = number of bits in representation of item indexes

  int i;
  prng_type * prng;
  FastSk_type * absc;
  
  prng=prng_Init(3152131,2);
  // use the random number generator to choose the hash functions

  absc=(FastSk_type *) calloc(1,sizeof(FastSk_type));
  absc->depth=depth;
  absc->width= lgn - (log(depth)/log(2)) + 1;
  absc->size=absc->width*depth;
  absc->hashrow = hash_row;
  absc->lgn = lgn;
    if (absc->hashrow > absc->depth) {
        fprintf(stderr, "hashrow larger than actual rows\n");
    }
  // take the input parameters and put them into the struct


  absc->counts=(long long *) calloc(absc->size,sizeof(long long));
  fast_quitmemory(absc->counts);
  // make space for the hash functions

  absc->testa=(long long *) calloc(hash_row,sizeof(long long));
  absc->testb=(long long *) calloc(hash_row,sizeof(long long));
  fast_quitmemory(absc->testa);
  fast_quitmemory(absc->testb);
  for (i=0;i<hash_row;i++)
    {
      absc->testa[i]=(long long) prng_int(prng);
      if (absc->testa[i]<0) absc->testa[i]= -(absc->testa[i]);
      absc->testb[i]=(long long) prng_int(prng);
      if (absc->testb[i]<0) absc->testb[i]= -(absc->testb[i]);
    }
  // create the hash functions
  prng_Destroy(prng);

  return(absc);
}

void FastSk_Destroy(FastSk_type * absc)
{
  // free up the space that was allocated for the data structure

  int i;

  /*
  for (i=0;i<absc->size;i++)
    free(absc->counts[i]);
  */
  free(absc->counts);
  free(absc->testb);
  free(absc->testa);
  free(absc);
}

unsigned long FastSk_findhashrow(FastSk_type* absc, int i, unsigned long key) {
    unsigned long hash;
    unsigned long l = absc->depth;
    hash = hash31(absc->testa[i],absc->testb[i], key/l);
    hash = hash % l; 
    //fprintf(stderr, "%lu %lu, %lu\n", key % l, hash, (key % l) ^ hash); 
    return (key % l) ^ hash;
}

void FastSk_Update(FastSk_type * absc, unsigned long newitem, 
		long long diff) {
  // routine to update the count of an item
  // newitem is the idenfitier of the item being updated
  // diff is the change, positive or negative
  // absc is the data structure

    int i;
    unsigned long r = 0;

  // for each set of groups, find the group that the item belongs in, update it
    for (i=0;i<absc->hashrow;i++) {
      // use the hash function to find the place where the item belongs
        r = FastSk_findhashrow(absc, i, newitem);
        if (r >= absc->depth) {
           fprintf(stderr, "Error: hash overflow: %lu\n", r); 
        }
        fast_loginsert(&absc->counts[absc->width*r],newitem/absc->depth,absc->width-1,diff);
      // call external routine to update the counts
    }
}

unsigned long FastSk_recovery(FastSk_type* absc, unsigned long x, unsigned long y, int i) {
    unsigned long hash;
    unsigned long l = absc->depth;
    hash = hash31(absc->testa[i],absc->testb[i], x);
    hash = hash % l; 
    return x * l + y ^ hash;
}

unsigned int FastSk_Output(FastSk_type * absc, double thresh, unsigned char** cand_list, unsigned int MAX_CAND, int is_heavy_changer)
{
    // take output from the data structure
    // thresh is the threshold for being a deltoid
    // absc is the data structure that holds the changes

    unsigned long guess;
    unsigned int i,j;
    
    // search for deltoids and put them in a list
    unsigned int cnt = 0;

    for (i=0;i<absc->depth;i++) {
        long long* counts = &(absc->counts[i*absc->width]);

        unsigned long q = fastfindone(counts, absc->width-1, thresh);
        if (q == 0)
            continue;

        for (j=0; j<absc->hashrow; j++) {      
            guess = FastSk_recovery(absc, q, i, j);

            unsigned int t;
            int testval = 0;
            for (t=0; t<absc->hashrow; ++t) {
                unsigned int tmp_r = FastSk_findhashrow(absc, t, guess);
                if (fast_abs(absc->counts[tmp_r*absc->width]) >= thresh) {
                    testval++;
                }
            }

            unsigned int r = FastSk_findhashrow(absc, j, guess);
            if (r == i && guess/absc->depth==q) {
                if ((is_heavy_changer && testval>absc->hashrow/2) || (!is_heavy_changer && testval>=absc->hashrow)) {
                    unsigned int k = 0;
		            for ( ; k<cnt; k++) {
			            if (memcmp(cand_list[k], &guess, absc->lgn/8)==0) {
                            break;
                        }
                    }
                    
                    if (k == cnt) {
                        if (cnt < MAX_CAND) {
                            memcpy(cand_list[cnt++], &guess, absc->lgn/8);
                        }
                        else {
						    fprintf(stderr, "ERR: cand list exhausted\n");
                        }
                    }
                }
            }
		                // if everything worked out OK, add to the output
	    }
    }
    return cnt;
}  

/******** output sketch in plaintext for test (by qhuang, 20130617) */
/*
 * Write to a flie
 */
void FastSk_write_plaintext(FastSk_type *absc, const char* outfile) {
	FILE* fp;
	int i, j;

	fp = fopen(outfile, "w");
    fprintf(fp, "depth: %d\n", absc->depth);
    fprintf(fp, "width: %d\n", absc->width);
	for (i=0; i<absc->depth; ++i) {
        fprintf(fp, "row: %d, testa %lld, testb %lld\n", i, absc->testa[i], absc->testb[i]);
	}
	for (i=0; i<absc->size; ++i) {
        fprintf(fp, "%lld ", absc->counts[i]);
        if ((i+1) % absc->width == 0) {
            fprintf(fp, "\n");
        }
	}
	fclose(fp);
}

/******** reset counter at the end of each time interval (by qhuang, 20130617) */
/*
 * reset the counters
 */
void FastSk_reset(FastSk_type *absc) {
    memset(absc->counts, 0, sizeof(long long)*(absc->size));
}

/******** calculate difference of two adjancent sketches (by qhuang, 20130617) */
/*
 * input: absc - current sketch; absc_old - previous sketch; absc_diff: store result
 */
void FastSk_diff(FastSk_type *absc, FastSk_type *absc_old, FastSk_type *absc_diff) {
	int i;

	for (i=0; i<absc->size; ++i) {
        long long diff = absc->counts[i] - absc_old->counts[i];
            // absc_old->counts[i][j] = absc->counts[i][j];
        absc_diff->counts[i] = diff;
	}
}

/******** copy sketches (by qhuang, 20130617) */
/*
 * input: absc - current sketch; absc_old - previous sketch; absc_diff: store result
 */
void FastSk_copy(FastSk_type *from, FastSk_type *to) {
    memcpy(to->counts, from->counts, sizeof(long long)*(from->size));
}
