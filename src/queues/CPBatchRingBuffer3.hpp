/*
 * CPBatchRingBuffer3.hh
 * - Ringbuffer w/ busy waiting, batch processing, cache-line protection
 * - Similar to CPBatchRingbuffer2.hh, but put localReadIndex and
 *   localWriteIndex
 */

#ifndef __CPBatchRingBuffer3_hh__
#define __CPBatchRingBuffer3_hh__

#include <pthread.h>
#include <stdio.h>

#define RB_SIZE 2000
#define RB_BATCH 50

#define CACHE_LINE_SIZE 64
#define EMPTY_RETRY 20000
#define barrier() __asm__ __volatile__("mfence": : :"memory")

#include "Util.hpp"

template <class T> class RingBuffer {
	char cachePad0[CACHE_LINE_SIZE];
	volatile int readIndex;
	volatile int writeIndex;
	char cachePad1[CACHE_LINE_SIZE-2*sizeof(int)];
	// consumer's variables
	int localWriteIndex;
	int nextReadIndex;
	int rBatchCount;
    int emptyRetry;
	unsigned long extracts;
	unsigned long numEmpty;
	char cachePad2[CACHE_LINE_SIZE-4*sizeof(int)-2*sizeof(unsigned long)];
	// producer's variables
	int localReadIndex;
	int nextWriteIndex;
	int wBatchCount;
	unsigned long inserts;
	unsigned long numFull;
	char cachePad3[CACHE_LINE_SIZE-3*sizeof(int)-2*sizeof(unsigned long)];
	int max;				// capacity of the buffer
	int blockOnEmpty;
	int batchSize;
	char cachePad4[CACHE_LINE_SIZE-3*sizeof(int)];
	T buffer[RB_SIZE];

public:
   
	RingBuffer(int size, int wSync=1, bool block=true) {
		fprintf(stderr, "CPBatchRingBuffer3.hh is used\n");
		if (size < 2) size = 2; // shouldn't allow value less than 2.
		readIndex = 0; 
		writeIndex = 0;
		localWriteIndex = 0;
		nextReadIndex = 0;
		rBatchCount = 0;
		extracts = 0;
		numEmpty = 0;
		localReadIndex = 0;
		nextWriteIndex = 0;
		wBatchCount = 0;
		inserts = 0;
		numFull = 0;
		max = size;
		blockOnEmpty = block;
		batchSize = wSync;
		barrier();
	}
	
	inline int nextVal(int x) { return (x+1) >= max ? 0 : x+1; }
   // inline int nextVal(int x) { return (x+1)%max; }

	int Insert(T* data) {
		int afterNextWriteIndex = nextVal(nextWriteIndex);
		if (afterNextWriteIndex == localReadIndex) {
			while (afterNextWriteIndex == readIndex) {
				numFull++;
                //return -1;
			}
			localReadIndex = readIndex;
		}
		buffer[nextWriteIndex] = *data;
		inserts++;
		nextWriteIndex = afterNextWriteIndex;
		wBatchCount++;
		if (wBatchCount >= batchSize) {
			writeIndex = nextWriteIndex;
			wBatchCount = 0;
		}
		return 0;
	}

	int Extract(T *data) {
		if (nextReadIndex == localWriteIndex) {
            emptyRetry=0;
			while (nextReadIndex == writeIndex) {
                if (!blockOnEmpty) {
                    emptyRetry++;
                    if (emptyRetry > EMPTY_RETRY) {
                        numEmpty++;
                        emptyRetry = 0;
                    }
                    return -1;
                }
			    numEmpty++;
                /*
			    if (!blockOnEmpty) {
				    return -1;
			    }
                */
                //return -1;
			}
			localWriteIndex = writeIndex;
		}
		*data = buffer[nextReadIndex];
		extracts++;
		nextReadIndex = nextVal(nextReadIndex);
		rBatchCount++;
		if (rBatchCount >= batchSize) {
			readIndex = nextReadIndex;
			rBatchCount = 0;
		}
		return 0;
	}

	int Insert(int size, T* data) {
		int afterNextWriteIndex = nextVal(nextWriteIndex);
		if (afterNextWriteIndex == localReadIndex) {
			while (afterNextWriteIndex == readIndex) {
				numFull++;
                //return -1;
			}
			localReadIndex = readIndex;
		}
		buffer[nextWriteIndex] = *data;
		inserts++;
		nextWriteIndex = afterNextWriteIndex;
		wBatchCount++;
		if (wBatchCount >= batchSize) {
			writeIndex = nextWriteIndex;
			wBatchCount = 0;
		}
		return 0;
	}

	int Extract(int size, T *data) {
		if (nextReadIndex == localWriteIndex) {
            emptyRetry=0;
			while (nextReadIndex == writeIndex) {
                if (!blockOnEmpty) {
                    emptyRetry++;
                    if (emptyRetry > EMPTY_RETRY) {
                        numEmpty++;
                        emptyRetry = 0;
                    }
                    return -1;
                }
			    numEmpty++;
                /*
			    if (!blockOnEmpty) {
				    return -1;
			    }
                */
                //return -1;
			}
			localWriteIndex = writeIndex;
		}
		*data = buffer[nextReadIndex];
		extracts++;
		nextReadIndex = nextVal(nextReadIndex);
		rBatchCount++;
		if (rBatchCount >= batchSize) {
			readIndex = nextReadIndex;
			rBatchCount = 0;
		}
		return 0;
	}

	void PrintStats(char* text) {
		LOG_MSG("%s inserts %lu extracts %lu numFull %lu numEmpty %lu\n",
				text, inserts, extracts, numFull, numEmpty);
    }
};

#endif 
