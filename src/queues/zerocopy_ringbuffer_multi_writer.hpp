/*
 * MCRingBuffer.hpp
 *
 * Ringbuffer cache-line protection and batch control updates
 */

#ifndef __ZEROCOPY_RINGBUFFER_MULTI_WRITER_HPP_INCLUDED__
#define __ZEROCOPY_RINGBUFFER_MULTI_WRITER_HPP_INCLUDED__

#include <pthread.h>
#include <stdio.h>
#include <atomic>
//#include "ringbuffer_base.hpp"

#define CACHE_LINE_SIZE 64
#define RB_SIZE_MULTI 2048
#define RB_BATCH_MULTI 64
#define MAX_WRITER 8
//#define barrier() __asm__ __volatile__("": : :"memory")
#define barrier()

template <class T> class ZeroRingBufferMulWriter 
//: public RingBufferBase
{

    struct WriterGlobal {
	    std::atomic_uint_fast64_t nextWriteIndex;
        uint64_t wBatchCount;
	    uint64_t localReadIndex;
        char cachePad[CACHE_LINE_SIZE-sizeof(std::atomic_uint_fast64_t)-2*sizeof(uint64_t)];
    };
    
    struct WriterLocal {
        uint64_t nextWriteIndex;
        uint64_t lastWriteIndex;
        int8_t is_start;
	    char cachePad[CACHE_LINE_SIZE-2*sizeof(uint64_t)-sizeof(int8_t)];
    };

    struct ReaderLocal {
	    uint64_t localWriteIndex;
	    uint64_t nextReadIndex;
	    uint64_t rBatchCount;
	    char cachePad[CACHE_LINE_SIZE-3*sizeof(uint64_t)];
    };

	char cachePad0[CACHE_LINE_SIZE];
	volatile int readIndex;
	volatile int writeIndex;
	char cachePad1[CACHE_LINE_SIZE-2*sizeof(int)];
    struct ReaderLocal reader;
    struct WriterGlobal writer_global;
	int max;				// capacity of the buffer
    //int blockOnEmpty;
	int batchSize;
    int num_writer;
	char cachePad4[CACHE_LINE_SIZE-3*sizeof(int)];
    struct WriterLocal writer_local[MAX_WRITER];
	T buffer[RB_SIZE_MULTI];

public:
   
	ZeroRingBufferMulWriter(int w) {
        fprintf(stderr, "ZeroRingBufferMulWriter.hpp is used\n");
		readIndex = 0; 
		writeIndex = 0;
		max = RB_SIZE_MULTI;
        //blockOnEmpty = block;
		batchSize = RB_BATCH_MULTI;
        num_writer = w;
        LOG_MSG("num writer %d\n", num_writer);

        memset(&reader, 0, sizeof(struct ReaderLocal));
        memset(&writer_global, 0, sizeof(struct WriterLocal));
        for (int i=0; i<num_writer; i++) {
            memset(writer_local+i, 0, sizeof(struct WriterLocal));
        }
		memset(buffer, 0, sizeof(buffer));
	}
	
	inline int nextVal(int x) { return (x+1) >= max ? 0 : x+1; }
    //inline int nextVal(int x) { return (x+1)%max; }
    
    inline bool is_full(uint64_t write, uint64_t read) {
        if (read >= num_writer) {
            if (write>read-num_writer && write<=read) {
                return true;
            }
        }
        else {
            if (write>max-num_writer+read || write<=read) {
                return true;
            }
        }
        return false;
        /*
        if (write == read) {
            return true;
        }
        return false;
        */
    }

    void Insert(int thr, void* data, int v=-1) {
        writer_local[thr].nextWriteIndex = writer_global.nextWriteIndex.fetch_add(1);
        uint64_t cur = writer_local[thr].nextWriteIndex % max;
		uint64_t afterNextWriteIndex = nextVal(cur);
		if (is_full(afterNextWriteIndex, writer_global.localReadIndex)) {
		    while (is_full(afterNextWriteIndex, readIndex)) {
                barrier();
			}
			writer_global.localReadIndex = readIndex;
		}
        //LOG_MSG("thr %d with v %d, write slot %lu, local read %lu\n", thr, v, cur, writer_local[thr].localReadIndex);
		buffer[cur] = *((T*)data);
        writer_local[thr].lastWriteIndex = writer_local[thr].nextWriteIndex;
        if (!writer_local[thr].is_start) {
            writer_local[thr].is_start = 1;
        }


        // qhuang: move one to proceed read operation
		//writer.nextWriteIndex = afterNextWriteIndex;
        //writer_local[thr].nextWriteIndex = afterNextWriteIndex;

		//uint64_t localBatch = writer.wBatchCount++;
		uint64_t tmpBatchCnt = writer_global.wBatchCount++;
		//if (tmpBatchCnt % batchSize == batchSize-1) {
		if (writer_local[thr].nextWriteIndex % batchSize == batchSize-1) {
			//writeIndex = afterNextWriteIndex;

            uint64_t slow = writer_local[thr].lastWriteIndex;
            for (int i=0; i<num_writer; i++) {
                if (writer_local[i].lastWriteIndex<slow && i!=thr && writer_local[i].is_start) {
                    slow = writer_local[i].lastWriteIndex;
                }
            }
            //LOG_MSG("update with slow %lu\n", slow);
			writeIndex = slow % max;
			//writer.wBatchCount = 0;
		}
	}

	T* Extract() {
		if (reader.nextReadIndex == reader.localWriteIndex) {
			if (reader.nextReadIndex == writeIndex) {
                //LOG_MSG("    reader empty read slot %lu, global write %d\n", reader.nextReadIndex, writeIndex);
                barrier();
			    readIndex = reader.nextReadIndex;
			    reader.rBatchCount = 0;
                return NULL;
			}
			reader.localWriteIndex = writeIndex;
		}
		return buffer + reader.nextReadIndex;
	}

    int Ack() {
		reader.nextReadIndex = nextVal(reader.nextReadIndex);
		reader.rBatchCount++;
		if (reader.rBatchCount >= batchSize) {
			readIndex = reader.nextReadIndex;
			reader.rBatchCount = 0;
		}
		barrier();
		return 0;
    }

    void Flush(int thr) {
        //writer_local[thr].nextWriteIndex = 0xffffffffffffffff;
        //writer_local[thr].nextWriteIndex = writer_global.nextWriteIndex.fetch_add(1);
        uint64_t wGlobalNextWrite = writer_global.nextWriteIndex;

        uint64_t min = 0xffffffffffffffff;
        //LOG_MSG("thr %d min %lu, fastest write %lu\n", thr, min, wGlobalNextWrite);
        for (int i=0; i<num_writer; i++) {
            if (writer_local[i].lastWriteIndex<min && i!=thr && writer_local[i].is_start) {
                min = writer_local[i].lastWriteIndex;
                /*
                LOG_MSG("    find min thr %d local %lu, min %lu, fastest write %lu\n", i, writer_local[i].nextWriteIndex, min, wGlobalNextWrite);
                */
            }
        }
        if (min == 0xffffffffffffffff) { 
            LOG_MSG("last writer flush()\n");
            min = writer_global.nextWriteIndex.fetch_add(1);
        }
		writeIndex = min % max;
        writer_local[thr].lastWriteIndex = 0xffffffffffffffff;

		readIndex = reader.nextReadIndex;

        LOG_MSG("global write %d, writer view (slow) %lu, writer view (fast) %lu, reader view %lu\n"
                "    global read  %d, writer view %lu, reader view %lu\n",
                writeIndex, min, wGlobalNextWrite, reader.localWriteIndex,
                readIndex, writer_global.localReadIndex, reader.nextReadIndex);
    }
};

#endif 
