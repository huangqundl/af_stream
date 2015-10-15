/*
    Copyright (c) 2007-2013 Contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "clock.hpp"
#include "likely.hpp"
#include "config.hpp"
#include "err.hpp"
#include "mutex.hpp"

#include <stddef.h>

#include <sys/time.h>

#include <time.h>

zmq::clock_t::clock_t () :
    last_tsc (rdtsc ()),
    last_time (now_us () / 1000)
{
}

zmq::clock_t::~clock_t ()
{
}

uint64_t zmq::clock_t::now_us ()
{


    //  Use POSIX gettimeofday function to get precise time.
    struct timeval tv;
    int rc = gettimeofday (&tv, NULL);
    errno_assert (rc == 0);
    return (tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec);

}

uint64_t zmq::clock_t::now_ms ()
{
    uint64_t tsc = rdtsc ();

    //  If TSC is not supported, get precise time and chop off the microseconds.
    if (!tsc)
    {
        return now_us () / 1000;
    }

    //  If TSC haven't jumped back (in case of migration to a different
    //  CPU core) and if not too much time elapsed since last measurement,
    //  we can return cached time value.
    if (afs_likely (tsc - last_tsc <= (clock_precision / 2) && tsc >= last_tsc))
        return last_time;

    last_tsc = tsc;
    last_time = now_us () / 1000;
    return last_time;
}

uint64_t zmq::clock_t::rdtsc ()
{
    return 0;
}
