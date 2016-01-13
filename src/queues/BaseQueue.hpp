/*
 * BaseQueue.hpp
 *
 * Data Queues to connect:
 *      two components within same worker, or
 *      two workers
 */

#ifndef __BASE_QUEUE_HPP__
#define __BASE_QUEUE_HPP__

template <class T>
class BaseQueue {
public:
    /*
     * Insert data.
     * Return 0 if success, otherwise return -1
     */
	virtual inline int Write(T* data) = 0;

    /*
     * Extract data.
     * Return 0 if success, otherwise return -1
     */
	virtual inline int Read(T* data) = 0;
};

template <class T>
class BaseBiQueue {
public:
    /*
     * Insert data
     * Return 0 if success, otherwise return -1
     */
	virtual int Write(T* data) = 0;

    /*
     * Extract data from upstream
     * Return 0 if success, otherwise return -1
     */
	virtual int Read(T* data) = 0;
};

#endif
