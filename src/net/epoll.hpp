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

#ifndef __ZMQ_EPOLL_HPP_INCLUDED__
#define __ZMQ_EPOLL_HPP_INCLUDED__

#include <vector>
#include <sys/epoll.h>

#include "control_channel/fd.hpp"
//#include "thread.hpp"
#include "poller_base.hpp"

// qhuang: simplified from epoll.hpp in 0MQ

// qhuang: rename the namespace from "zmq" to "zmq"
namespace afs_zmq
{

    struct i_poll_events;

    //  This class implements socket polling mechanism using the Linux-specific
    //  epoll mechanism.

    class epoll_t : public poller_base_t
    {
    public:

        typedef void* handle_t;

        epoll_t ();
        ~epoll_t ();

        //  "poller" concept.
        handle_t add_fd (fd_t fd_, i_poll_events *events_);
        void rm_fd (handle_t handle_);
        void set_pollin (handle_t handle_);
        void reset_pollin (handle_t handle_);
        void set_pollout (handle_t handle_);
        void reset_pollout (handle_t handle_);

        static int max_fds ();

        //  Main event loop.
        void loop ();

    private:

        //  Main epoll file descriptor
        fd_t epoll_fd;

        struct poll_entry_t
        {
            fd_t fd;
            epoll_event ev;
            i_poll_events *events;
        };

        //  List of retired event sources.
        typedef std::vector <poll_entry_t*> retired_t;
        retired_t retired;

        //  If true, thread is in the process of shutting down.
        bool stopping;

        epoll_t (const epoll_t&);
        const epoll_t &operator = (const epoll_t&);
    };

}


#endif
