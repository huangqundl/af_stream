/*
 * MCRingBuffer.hpp
 *
 * Ringbuffer cache-line protection and batch control updates
 */

#ifndef __ZEROCOPY_RINGBUFFER_HPP_INCLUDED__
#define __ZEROCOPY_RINGBUFFER_HPP_INCLUDED__

#include <pthread.h>
#include <stdio.h>
//#include "ringbuffer_base.hpp"

#define CACHE_LINE_SIZE 64
#define RB_SIZE 2000
#define RB_BATCH 50
//#define barrier() __asm__ __volatile__("": : :"memory")
#define barrier()

template <class T> class ZeroRingBuffer 
//: public RingBufferBase
{
	char cachePad0[CACHE_LINE_SIZE];
	volatile int readIndex;
	volatile int writeIndex;
	char cachePad1[CACHE_LINE_SIZE-2*sizeof(int)];
	int localWriteIndex;        // reader start
	int nextReadIndex;
	int rBatchCount;            // reader end
	char cachePad2[CACHE_LINE_SIZE-3*sizeof(int)];
	int localReadIndex;         // writer start
	int nextWriteIndex;
    int afterNextWriteIndex;
	int wBatchCount;            // writer end
	char cachePad3[CACHE_LINE_SIZE-4*sizeof(int)];
	int max;				// capacity of the buffer
    int blockOnEmpty;
	int batchSize;
	char cachePad4[CACHE_LINE_SIZE-3*sizeof(int)];
	T buffer[RB_SIZE];

public:
   
	ZeroRingBuffer(bool block = true) {
        fprintf(stderr, "ZeroRingBuffer.hpp is used\n");
		readIndex = 0; 
		writeIndex = 0;
		localWriteIndex = 0;
		nextReadIndex = 0;
		rBatchCount = 0;
		localReadIndex = 0;
		nextWriteIndex = 0;
		wBatchCount = 0;
		max = RB_SIZE;
        blockOnEmpty = block;
		batchSize = RB_BATCH;
		memset(buffer, 0, sizeof(buffer));
	}
	
	inline int nextVal(int x) { return (x+1) >= max ? 0 : x+1; }
    //inline int nextVal(int x) { return (x+1)%max; }
    
    int Size() {
        if (afterNextWriteIndex < localReadIndex) {
            return afterNextWriteIndex - localReadIndex + RB_SIZE;
        }
        else {
            return afterNextWriteIndex - localReadIndex;
        }
    }

	//void Insert(T* data) {
    void Insert(void* data) {
		afterNextWriteIndex = nextVal(nextWriteIndex);
		if (afterNextWriteIndex == localReadIndex) {
			while (afterNextWriteIndex == readIndex) {
                barrier();
			}
			localReadIndex = readIndex;
		}
		buffer[nextWriteIndex] = *((T*)data);
		nextWriteIndex = afterNextWriteIndex;
		wBatchCount++;
		if (wBatchCount >= batchSize) {
			writeIndex = nextWriteIndex;
			wBatchCount = 0;
		}
	}

    T* GetSlot() {
		afterNextWriteIndex = nextVal(nextWriteIndex);
		if (afterNextWriteIndex == localReadIndex) {
			while (afterNextWriteIndex == readIndex) {
                barrier();
			}
			localReadIndex = readIndex;
		}
        return buffer + nextWriteIndex;
    }

    void CompleteWrite() {
		nextWriteIndex = afterNextWriteIndex;
		wBatchCount++;
		if (wBatchCount >= batchSize) {
			writeIndex = nextWriteIndex;
			wBatchCount = 0;
		}
    }

	//int Extract(T** data) {
	T* Extract() {
		if (nextReadIndex == localWriteIndex) {
			if (nextReadIndex == writeIndex) {
                barrier();
                return NULL;
			}
			localWriteIndex = writeIndex;
		}
		//*data = buffer + nextReadIndex;
		return buffer + nextReadIndex;
	}

    int Ack() {
		nextReadIndex = nextVal(nextReadIndex);
		rBatchCount++;
		if (rBatchCount >= batchSize) {
			readIndex = nextReadIndex;
			rBatchCount = 0;
		}
		barrier();
		return 0;
    }

    void Flush() {
        writeIndex = nextWriteIndex;
		readIndex = nextReadIndex;
    }
};

#endif 
