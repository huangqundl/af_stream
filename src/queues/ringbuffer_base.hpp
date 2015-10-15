/*
 * MCRingBuffer.hpp
 *
 * Ringbuffer cache-line protection and batch control updates
 */

#ifndef __RINGBUFFER_BASE_HPP_INCLUDED__
#define __RINGBUFFER_BASE_HPP_INCLUDED__

class RingBufferBase {
public:
    // should be blocking
	virtual void Insert(void* data) = 0;

    // return: NULL failed, otherwise the address
	virtual void* Extract() = 0; 

    virtual int Ack() = 0;
};

#endif 
