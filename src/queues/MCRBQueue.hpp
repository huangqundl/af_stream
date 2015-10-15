/*
 * MCRBQueue.hpp
 *
 * Data Queue to connect two components within same physical machine
 */

#ifndef __MCRBQUEUE_HPP__
#define __MCRBQUEUE_HPP__

#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <errno.h>
#include <semaphore.h>

#include "util.hpp"
#include "BaseQueue.hpp"
//#include "BusyRingBuffer.hpp"
#include "MCRingBuffer.hpp"
//#include "CPBatchRingBuffer3.hpp"

template <class T>
class MCRBQueue : public BaseQueue <T> {
private:
    char* name;
    RingBuffer<T>* rbuf;

public:
    MCRBQueue(const char* ipcName, bool create = true, bool blockOnEmpty = true) {
        name = (char*) malloc(strlen(ipcName) + 1);
        strcpy(name, ipcName);

        //rbuf = new RingBuffer<T>(RB_SIZE, RB_BATCH, blockOnEmpty);

        int shm_fd;

        shm_unlink(name);
        shm_fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
        if (shm_fd == -1)
            LOG_ERR("shm_open() failed: %s\n", strerror(errno));

        if (ftruncate(shm_fd, sizeof(RingBuffer<T>)) == -1) {
            shm_unlink(name);
            LOG_ERR("ftruncate() failed: %s\n", strerror(errno));
        }

        void* ptr = mmap(0, sizeof(RingBuffer<T>), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            shm_unlink(name);
            LOG_ERR("mmap() failed: %s\n", strerror(errno));
        }

        rbuf = (RingBuffer<T>*) ptr;
        RingBuffer<T>* rb = new RingBuffer<T>(RB_SIZE, RB_BATCH, blockOnEmpty);
        memcpy(rbuf, rb, sizeof(RingBuffer<T>));
        delete rb;
    }

    virtual ~MCRBQueue() {
        shm_unlink(name);
        munmap(rbuf, sizeof(RingBuffer<T>));
        free(name);
    }

	int Write(T* data) {
        return rbuf->Insert(data);
    }

	int Read(T* data) {
        return rbuf->Extract(data);
    }

    void PrintStats(char* text) {
        rbuf->PrintStats(text);
    }
};

#endif
