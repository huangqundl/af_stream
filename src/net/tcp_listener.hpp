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

#ifndef __ZMQ_TCP_LISTENER_HPP_INCLUDED__
#define __ZMQ_TCP_LISTENER_HPP_INCLUDED__

#include "../zmq_fd.hpp"
//#include "own.hpp"
#include <stdint.h>
#include "io_object.hpp"
#include "tcp_address.hpp"
#include "../include/zmq.h"
#include "../thread/in_callback_base.hpp"
#include "../thread/out_callback_base.hpp"

// qhuang: add headers declared in removed files
#include "options.hpp"
//#include "epoll.hpp"

namespace afs_zmq
{

    //class io_thread_t;
    class socket_base_t;

    // qhuang: 0MQ use io_object to maintain poller
    class tcp_listener_t : public io_object_t
                           // public own_t 
    {
    public:

        //tcp_listener_t (zmq::io_thread_t *io_thread_,
        //    zmq::socket_base_t *socket_, const options_t &options_);
        tcp_listener_t (afs::InCallbackBase* in_callback, afs::OutCallbackBase** out_callbacks, const options_t &options_);
        ~tcp_listener_t ();

        //  Set address to listen on.
        int set_address (const char *addr_);

        // Get the bound address for use with wildcard
        int get_address (std::string &addr_);

        // qhuang: 
        void set_poller (epoll_t* poller);

    private:

        // qhuang: maintain poller directly, not need "io_object" any more 
        //epoll_t *poller;
        afs::InCallbackBase* in_callback_;
        afs::OutCallbackBase** out_callbacks_;

        // qhuang: maintain options directly, not need "own" any more
        //  Socket options associated with this object.
        options_t options;

        //  Handlers for incoming commands.
        void process_plug ();
        void process_term (int linger_);

        //  Handlers for I/O events.
        bool in_event ();

        //  Close the listening socket.
        void close ();

        //  Accept the new connection. Returns the file descriptor of the
        //  newly created connection. The function may return retired_fd
        //  if the connection was dropped while waiting in the listen backlog
        //  or was denied because of accept filters.
        //  qhuang: modify to store peer_addr in the argument
        fd_t accept (char* peer_addr);

        //  Address to listen on.
        tcp_address_t address;

        //  Underlying socket.
        fd_t s;

        //  Handle corresponding to the listening socket.
        epoll_t::handle_t handle;

        //  Socket the listerner belongs to.
        //afs_zmq::socket_base_t *socket;

       // String representation of endpoint to bind to
        std::string endpoint;

        tcp_listener_t (const tcp_listener_t&);
        const tcp_listener_t &operator = (const tcp_listener_t&);
    };

}

#endif
