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
#include "prng.h"
#include "massdal.h"
#include "sketch.h"

// qhuang: this is the file "change.c" in original deltoid

/******************************************************************/

#define min(x,y)	((x) < (y) ? (x) : (y))
#define max(x,y)	((x) > (y) ? (x) : (y))

long long ll_abs(long long a) {
	if (a < 0) {
		return -a;
	}
	return a;
}

unsigned long absfindone(long long *count, int l, long long thresh) 
{
  // internal routine used to find deltoids. 

  int i;
  long long c;
  unsigned long j,k;

  // search through a set of tests to detect whether there is a deltoid there
    // returns 0 if none found, returns id of found item otherwise. 

  j=1;
  k=0;

  if (ll_abs(count[0])<thresh) 
    k=0;
  else
    {
      c = count[0];      
      for (i=l;i>0;i--)
	{
	  // main test: if one side is above threshold and the otherside is not
	  if (((ll_abs(count[i])<thresh) && (ll_abs(c-count[i])<thresh)) ||
	      ((ll_abs(count[i])>=thresh) && (ll_abs(c-count[i])>=thresh)))
	    {
	      k=0;
	      break; 
              // if test fails, bail out
	    }	  
	  if(ll_abs(count[i])>=thresh) 
	    k+=j;
	  j=j<<1;
	  // build the binary representation of the item
	}
    }
  return k;
}

void quitmemory(void * pointer)
     // quit if a memory allocation failed
{
  if (pointer==NULL) exit(1);
}

void loginsert(long long *lists, unsigned long val, int length, long long diff) 
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

AbsChange_type * AbsChange_Init(int width, int depth, int lgn)
{
  // initialize the data structure for finding absolute changes
  // width = 1/eps = width of hash functions
  // depth = number of independent repetitions to avoid misses
  // lgn = number of bits in representation of item indexes

  int i;
  prng_type * prng;
  AbsChange_type * absc;
  
  prng=prng_Init(3152131,2);
  // use the random number generator to choose the hash functions

  absc=(AbsChange_type *) calloc(1,sizeof(AbsChange_type));
  absc->depth=depth;
  absc->width=width;
  absc->size=width*depth;
  absc->lgn=lgn;
  // take the input parameters and put them into the struct


  absc->testa=(long long *) calloc(depth,sizeof(long long));
  absc->testb=(long long *) calloc(depth,sizeof(long long));
  absc->counts=(long long **) calloc(absc->size,sizeof(long long *));
  quitmemory(absc->testa);
  quitmemory(absc->testb);
  quitmemory(absc->counts);
  // make space for the hash functions

  for (i=0;i<absc->size;i++)
    {
      absc->counts[i]=(long long *) calloc(1+lgn,sizeof(long long));
      quitmemory(absc->counts[i]);
    }
  // make space for the counters

  for (i=0;i<depth;i++)
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

void AbsChange_Destroy(AbsChange_type * absc)
{
  // free up the space that was allocated for the data structure

  int i;

  for (i=0;i<absc->size;i++)
    free(absc->counts[i]);
  free(absc->counts);
  free(absc->testb);
  free(absc->testa);
  free(absc);
}


void AbsChange_Update(AbsChange_type * absc, unsigned long newitem, 
		long long diff) {
  // routine to update the count of an item
  // newitem is the idenfitier of the item being updated
  // diff is the change, positive or negative
  // absc is the data structure

  int i;
  unsigned long hash;

  // for each set of groups, find the group that the item belongs in, update it
  for (i=0;i<absc->depth;i++) 
    {
      hash=hash31(absc->testa[i],absc->testb[i],newitem);
      hash=hash % absc->width; 
      // use the hash function to find the place where the item belongs
      loginsert(absc->counts[i*absc->width+hash],newitem,absc->lgn,diff);
      // call external routine to update the counts
    }
}

unsigned int AbsChange_Output(AbsChange_type * absc, double thresh, unsigned char** cand_list, unsigned int MAX_CAND)
{
    // take output from the data structure
    // thresh is the threshold for being a deltoid
    // absc is the data structure that holds the changes

    unsigned long guess;
    int testval=0;
    unsigned long hash=0;
    unsigned int i,j;
    
    // search for deltoids and put them in a list
    unsigned int cnt = 0;

    for (i=0;i<absc->depth;i++) {
        for (j=0; j<absc->width; j++) {      
	    // go over all the different tests and see if there is a 
	    // deltoid within each test
	        guess=absfindone(absc->counts[testval],absc->lgn,thresh);
	        if (guess>0) {
	            hash=hash31(absc->testa[i],absc->testb[i],guess);
	            hash=hash % (absc->width); 
	            // check item does hash into that bucket... 
                if (hash==j) {
		            // supress duplicates in output
		            // may be a little slow for large lists
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
		                // if everything worked out OK, add to the output
		        }
	        }
	        // advance to next item
	        testval++;
	    }
    }
    return cnt;
}  


/******** Added for offline implementation (by pclee, 060723) */
/*
 * Write to a flie
 */
void AbsChange_write(AbsChange_type *absc, char* outfile) {
	FILE* fp;
	int i, j;

	fp = fopen(outfile, "wb");
	fwrite(&(absc->depth), sizeof(int), 1, fp);
	fwrite(&(absc->width), sizeof(int), 1, fp);
	fwrite(&(absc->lgn), sizeof(int), 1, fp);
	for (i=0; i<absc->depth; ++i) {
		fwrite(&(absc->testa[i]), sizeof(long long), 1, fp);
	}
	for (i=0; i<absc->depth; ++i) {
		fwrite(&(absc->testb[i]), sizeof(long long), 1, fp);
	}
	for (i=0; i<absc->size; ++i) {
		for (j=0; j<1+absc->lgn; ++j) {
			fwrite(&(absc->counts[i][j]), sizeof(long long), 1, fp);
		}
	}
	fclose(fp);
}

/******** output sketch in plaintext for test (by qhuang, 20130617) */
/*
 * Write to a flie
 */
void AbsChange_write_plaintext(AbsChange_type *absc, const char* outfile) {
	FILE* fp;
	int i, j;

	fp = fopen(outfile, "w");
    fprintf(fp, "depth: %d\n", absc->depth);
    fprintf(fp, "width: %d\n", absc->width);
    fprintf(fp, "lgn: %d\n", absc->lgn);
	for (i=0; i<absc->depth; ++i) {
        fprintf(fp, "row: %d, testa %lld, testb %lld\n", i, absc->testa[i], absc->testb[i]);
	}
	for (i=0; i<absc->size; ++i) {
		for (j=0; j<1+absc->lgn; ++j) {
            fprintf(fp, "%lld ", absc->counts[i][j]);
		}
        fprintf(fp, ";    ");
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
void AbsChange_reset(AbsChange_type *absc) {
    int i;
	for (i=0; i<absc->size; ++i) {
        memset(absc->counts[i], 0, sizeof(long long)*(absc->lgn+1));
	}
}

/******** calculate difference of two adjancent sketches (by qhuang, 20130617) */
/*
 * input: absc - current sketch; absc_old - previous sketch; absc_diff: store result
 */
void AbsChange_diff(AbsChange_type *absc, AbsChange_type *absc_old, AbsChange_type *absc_diff) {
	int i, j;

	for (i=0; i<absc->size; ++i) {
		for (j=0; j<1+absc->lgn; ++j) {
            long long diff = absc->counts[i][j] - absc_old->counts[i][j];
            // absc_old->counts[i][j] = absc->counts[i][j];
            absc_diff->counts[i][j] = diff;
		}
	}
}

/******** copy sketches (by qhuang, 20130617) */
/*
 * input: absc - current sketch; absc_old - previous sketch; absc_diff: store result
 */
void AbsChange_copy(AbsChange_type *from, AbsChange_type *to) {
	int i;

	for (i=0; i<from->size; ++i) {
        memcpy(to->counts[i], from->counts[i], sizeof(long long)*(from->lgn+1));
	}
}
