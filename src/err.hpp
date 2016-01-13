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

#ifndef __ROLL_ERR_HPP_INCLUDED__
#define __ROLL_ERR_HPP_INCLUDED__

//  modify based on err.hpp in 0MQ

//  error codes are defined in afs_errno.h
#include "errno.h"
#include "zmq_err.hpp"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "likely.hpp"

#include <netdb.h>

//  This macro is implemented based on 0MQ zmq_assert().
//  Here, we add error message to the micro.
#define afs_assert(x, ...) \
    do {\
        if (afs_unlikely(!(x))) {\
            fprintf (stderr, "Assertion failed at %s (%s:%d): ", #x, \
                __FILE__, __LINE__);\
            fprintf (stderr, "" __VA_ARGS__);\
            fflush(stderr);\
            exit(EXIT_FAILURE);\
        }\
    } while (0) 

#endif


