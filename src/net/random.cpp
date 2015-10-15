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

#include <stdlib.h>

#include <unistd.h>
#include <stdint.h>

#include "random.hpp"
#include "clock.hpp"

void zmq::seed_random ()
{
    int pid = (int) getpid ();
    srand ((unsigned int) (clock_t::now_us () + pid));
}

uint32_t zmq::generate_random ()
{
    //  Compensate for the fact that rand() returns signed integer.
    uint32_t low = (uint32_t) rand ();
    uint32_t high = (uint32_t) rand ();
    high <<= (sizeof (int) * 8 - 1);
    return high | low;
}

