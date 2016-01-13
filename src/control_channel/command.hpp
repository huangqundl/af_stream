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

#ifndef __ZMQ_COMMAND_HPP_INCLUDED__
#define __ZMQ_COMMAND_HPP_INCLUDED__

#include <stdint.h>
#include "fault_tolerance/backup_item.hpp"

namespace afs_zmq
{

    class object_t;
    class own_t;
    struct i_engine;
    class pipe_t;
    class socket_base_t;

    //  This structure defines the commands that can be sent between threads.

    struct command_t
    {

        enum type_t
        {
            any,
            ready,
            clean,
            finish,
            backup,
            more,
            done,
        } type;

        union {
            struct afs::BackupItem backup_item;
        } args;

        unsigned char data[256];
        unsigned int len;
    };

}    

#endif
