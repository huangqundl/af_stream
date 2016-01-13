/*
 * BusyRingBuffer.hpp
 * - Ringbuffer based on busy waiting
 */

#ifndef __BusyRingBuffer_hpp__
#define __BusyRingBuffer_hpp__

#include <pthread.h>
#include <stdio.h>

#define RB_SIZE 2000
#define RB_BATCH 50

#define barrier() __asm__ __volatile__("mfence": : :"memory")

template <class T> class RingBuffer {
	volatile int readIndex;
	volatile int writeIndex;
	volatile int max;				// capacity of the buffer
	volatile int blockOnEmpty;
	T buffer[RB_SIZE];

public:
   
	RingBuffer(int size, int wSyncMinCount=1, bool block=true) {
		//fprintf(stderr, "BusyRingBuffer.hpp is used\n");
		if (size < 2) size = 2; // shouldn't allow value less than 2.
		readIndex = 0; 
		writeIndex = 0;
		max = size;
		blockOnEmpty = block;
	}

	inline int nextVal(int x) { return (x+1) >= max ? 0 : x+1; }

	int Insert(T* data) {
		int nextWriteIndex = nextVal(writeIndex);
        /*
		while (nextWriteIndex == readIndex) {
		}
        */
		if (nextWriteIndex == readIndex) {
            return -1;
        }
        buffer[writeIndex] = *data;
		writeIndex = nextWriteIndex;
		return 0;
	}

	int Extract(T* data) {
		while (readIndex == writeIndex) {
			if (!blockOnEmpty) {
				return -1;
			}
		}
        *data = buffer[readIndex];
		readIndex = nextVal(readIndex);
		return 0;
	}

	void PrintStats(char* text) {
        // do nothing
    }
};

#endif 
