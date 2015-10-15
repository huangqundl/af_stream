/*
 * MCRingBuffer.hpp
 *
 * Ringbuffer cache-line protection and batch control updates
 */

#ifndef __MCRingBuffer_hpp__
#define __MCRingBuffer_hpp__

#include <pthread.h>
#include <stdio.h>

#define CACHE_LINE_SIZE 64
#define RB_SIZE 2000
#define RB_BATCH 50
//#define barrier() __asm__ __volatile__("": : :"memory")
#define barrier()

template <class T> class RingBuffer {
	char cachePad0[CACHE_LINE_SIZE];
	volatile int readIndex;
	volatile int writeIndex;
	char cachePad1[CACHE_LINE_SIZE-2*sizeof(int)];
	int localWriteIndex;
	int nextReadIndex;
	int rBatchCount;
	char cachePad2[CACHE_LINE_SIZE-3*sizeof(int)];
	int localReadIndex;
	int nextWriteIndex;
	int wBatchCount;
	char cachePad3[CACHE_LINE_SIZE-3*sizeof(int)];
	int max;				// capacity of the buffer
    int blockOnEmpty;
	int batchSize;
	char cachePad4[CACHE_LINE_SIZE-3*sizeof(int)];
	T buffer[RB_SIZE];

public:
   
	RingBuffer(int size, int wSync=1, bool block = true) {
        fprintf(stderr, "MCRingBuffer.hpp is used\n");
		if (size < 2) // shouldn't allow value less than 2.
            size = 2;
        else if (size > RB_SIZE)
            size = RB_SIZE;
		readIndex = 0; 
		writeIndex = 0;
		localWriteIndex = 0;
		nextReadIndex = 0;
		rBatchCount = 0;
		localReadIndex = 0;
		nextWriteIndex = 0;
		wBatchCount = 0;
		max = size;
        blockOnEmpty = block;
		batchSize = wSync;
		memset(buffer, 0, sizeof(buffer));
	}
	
	inline int nextVal(int x) { return (x+1) >= max ? 0 : x+1; }
    //inline int nextVal(int x) { return (x+1)%max; }

	int Insert(T* data) {
		int afterNextWriteIndex = nextVal(nextWriteIndex);
		if (afterNextWriteIndex == localReadIndex) {
			while (afterNextWriteIndex == readIndex) {
                barrier();
			}
			localReadIndex = readIndex;
		}
		buffer[nextWriteIndex] = *data;
		nextWriteIndex = afterNextWriteIndex;
		wBatchCount++;
		if (wBatchCount >= batchSize) {
			writeIndex = nextWriteIndex;
			wBatchCount = 0;
		}
		return 0;
	}

	int Extract(T* data) {
		if (nextReadIndex == localWriteIndex) {
			while (nextReadIndex == writeIndex) {
                barrier();
                if (!blockOnEmpty)
                    return -1;
			}
			localWriteIndex = writeIndex;
		}
		*data = buffer[nextReadIndex];
		nextReadIndex = nextVal(nextReadIndex);
		rBatchCount++;
		if (rBatchCount >= batchSize) {
			readIndex = nextReadIndex;
			rBatchCount = 0;
		}
		barrier();
		return 0;
	}

	int Insert(int size, T* data) {
		int afterNextWriteIndex = nextVal(nextWriteIndex);
		if (afterNextWriteIndex == localReadIndex) {
			while (afterNextWriteIndex == readIndex) {
                barrier();
			}
			localReadIndex = readIndex;
		}
		buffer[nextWriteIndex] = *data;
		nextWriteIndex = afterNextWriteIndex;
		wBatchCount++;
		if (wBatchCount >= batchSize) {
			writeIndex = nextWriteIndex;
			wBatchCount = 0;
		}
		return 0;
	}

	int Extract(int size, T* data) {
		if (nextReadIndex == localWriteIndex) {
			while (nextReadIndex == writeIndex) {
                barrier();
                if (!blockOnEmpty)
                    return -1;
			}
			localWriteIndex = writeIndex;
		}
		*data = buffer[nextReadIndex];
		nextReadIndex = nextVal(nextReadIndex);
		rBatchCount++;
		if (rBatchCount >= batchSize) {
			readIndex = nextReadIndex;
			rBatchCount = 0;
		}
		barrier();
		return 0;
	}
};

#endif 
