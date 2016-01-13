/**
 * Implementation of Naive and Lock-free ring-buffer queues and
 * performance and verification tests.
 *
 * Build with (g++ version must be >= 4.5.0):
 * $ g++ -Wall -std=c++0x -Wl,--no-as-needed -O2 -D DCACHE1_LINESIZE=`getconf LEVEL1_DCACHE_LINESIZE` lockfree_rb_q.cc -lpthread
 *
 * I verified the program with g++ 4.5.3, 4.6.1, 4.6.3 and 4.8.1.
 *
 * Use -std=c++11 instead of -std=c++0x for g++ 4.8.
 *
 * Copyright (C) 2012-2013 Alexander Krizhanovsky (ak@natsys-lab.com).
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * See http://www.gnu.org/licenses/lgpl.html .
 */

#ifndef __MPMC_QUEUE_BOOST_HPP_INCLUDED__
#define __MPMC_QUEUE_BOOST_HPP_INCLUDED__

#ifndef __x86_64__
#warning "The program is developed for x86-64 architecture only."
#endif
#if !defined(DCACHE1_LINESIZE) || !DCACHE1_LINESIZE
#ifdef DCACHE1_LINESIZE
#undef DCACHE1_LINESIZE
#endif
#define DCACHE1_LINESIZE 64
#endif
#define ____cacheline_aligned	__attribute__((aligned(DCACHE1_LINESIZE)))

#include <sys/time.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <immintrin.h>

#include <atomic>
#include <cassert>
#include <iostream>
#include <boost/lockfree/queue.hpp>

#define QUEUE_SIZE	(1024)

template<class T>
class LockFreeQueue {
public:

	void push(const T& x) {
		while (!q_.push(x))
			asm volatile("rep; nop" ::: "memory");
	}

    bool pop(T& ret) {
		if (!q_.pop(ret)) {
			asm volatile("rep; nop" ::: "memory");
            return false;
        }
		return true;
	}

    LockFreeQueue(int writer, int reader) {}

private:
	boost::lockfree::queue<T, boost::lockfree::capacity<QUEUE_SIZE>> q_;
};


#endif
