#ifndef ROLL_UTIL_HPP_
#define ROLL_UTIL_HPP_

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <iostream>
#include <vector>

#include "err.hpp"

#define HLINE "\n=================================================\n"
#define INDENT_HLINE "    =================================================\n"
#define INDENT "    "

// normal greeen 
#define LOG_SYS(...) {fprintf(stderr, "\033[36m" __VA_ARGS__ "\033[0m"); fflush(stderr);}

// highlight red
#define LOG_OUT(...) {fprintf(stderr, "\033[31;1m" __VA_ARGS__ "\033[0m"); fflush(stderr);}

// highlight blue
#define LOG_KEY(...) {fprintf(stderr, "\033[34;1m" __VA_ARGS__ "\033[0m"); fflush(stderr);}

#define LOG_MSG(...) {fprintf(stderr, __VA_ARGS__); fflush(stderr);}
#define LOG_WARN(...) {fprintf(stderr, "[WARN] " __VA_ARGS__); fflush(stderr);}
#define LOG_ERR(...) {fprintf(stderr, "[ERROR] "  __VA_ARGS__); exit(EXIT_FAILURE);}

// in usec (10^-6 second)
inline uint64_t now_us ()
{
    //  Use POSIX gettimeofday function to get precise time.
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec);
}

#define CPU_CORE 32
inline void pin_to_cpu(int cpu) {
	cpu_set_t cpuset;	// set of CPUs allocating to the LVRM

    /*
	if (getuid()) {
        // exit if user is not root
        LOG_ERR("Root required\n");
		return;
	}
    */

	CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset) < 0) {
        fprintf(stderr, "set thread affinity failed\n");
    }
    cpu_set_t get;
    CPU_ZERO(&get);
    if (pthread_getaffinity_np(pthread_self(), sizeof(get), &get) < 0) {
        fprintf(stderr, "get thread affinity failed\n");
    }
    for (int j = 0; j < CPU_CORE; j++) {
        if (CPU_ISSET(j, &get)) {
            LOG_MSG("    running in processor %d\n", j);
        }
    }
}

#endif // ROLL_UTIL_HPP_
