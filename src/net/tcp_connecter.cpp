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

#include <new>
#include <string>

#include "tcp_connecter.hpp"
#include "stream_engine.hpp"
//#include "io_thread.hpp"
//#include "platform.hpp"
#include "random.hpp"
#include "err.hpp"
#include "ip.hpp"
#include "tcp.hpp"
#include "address.hpp"
#include "tcp_address.hpp"
//#include "session_base.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

zmq::tcp_connecter_t::tcp_connecter_t (
      //class io_thread_t *io_thread_,
      //class session_base_t *session_,
    int worker_id,
    afs::InCallbackBase* in_callback,
    afs::OutCallbackBase* out_callback,
    const options_t &options_,
    const address_t *addr_,
    bool delayed_start_) :
    //own_t (io_thread_, options_),
    io_object_t (),
    worker_id_(worker_id),
    in_callback_(in_callback),
    out_callback_(out_callback),
    options(options_),
    addr (addr_),
    s (retired_fd),
    handle_valid (false),
    delayed_start (delayed_start_),
    timer_started (false),
    //session (session_),
    current_reconnect_ivl(options.reconnect_ivl)
{
    zmq_assert (addr);
    zmq_assert (addr->protocol == "tcp");
    addr->to_string (endpoint);
    //socket = session-> get_socket();
}

zmq::tcp_connecter_t::~tcp_connecter_t ()
{
    zmq_assert (!timer_started);
    zmq_assert (!handle_valid);
    zmq_assert (s == retired_fd);
}

void zmq::tcp_connecter_t::process_plug ()
{
    if (delayed_start)
        add_reconnect_timer ();
    else
        start_connecting ();
}


// qhuang
void zmq::tcp_connecter_t::set_poller (epoll_t* poller)
{
    io_object_t::set_poller(poller);

    // function in io_object_t::plug
    //handle = add_fd (s);
    //set_pollin (handle);
}

//void zmq::tcp_connecter_t::process_term (int linger_)
void zmq::tcp_connecter_t::terminate ()
{
    if (timer_started) {
        cancel_timer (reconnect_timer_id);
        timer_started = false;
    }

    if (handle_valid) {
        rm_fd (handle);
        handle_valid = false;
    }

    if (s != retired_fd)
        close ();

    // own_t::process_term (linger_);
}

bool zmq::tcp_connecter_t::in_event ()
{
    //  We are not polling for incoming data, so we are actually called
    //  because of error here. However, we can get error on out event as well
    //  on some platforms, so we'll simply handle both events in the same way.
    out_event ();
    return false;
}

bool zmq::tcp_connecter_t::out_event ()
{
    fd_t fd = connect ();
    rm_fd (handle);
    handle_valid = false;

    //  Handle the error condition by attempt to reconnect.
    if (fd == retired_fd) {
        close ();
        add_reconnect_timer();
        return false;
    }

    tune_tcp_socket (fd);
    tune_tcp_keepalives (fd, options.tcp_keepalive, options.tcp_keepalive_cnt, options.tcp_keepalive_idle, options.tcp_keepalive_intvl);

    // qhuang: send worker id
    // current implementation: sync-based
    // refine it with async-based using epoll later on
    while (1) {
        ssize_t nbytes = send (fd, &worker_id_, sizeof(worker_id_), 0);
        if (nbytes == sizeof(worker_id_)) {
            break;
        }
    }

    //  Create the engine object for this connection.
    stream_engine_t *engine = new (std::nothrow)
        stream_engine_t (fd, options, endpoint, in_callback_, out_callback_);
    alloc_assert (engine);

    // qhuang
    engine->plug(io_object_t::get_poller());

    //  Attach the engine to the corresponding session object.
    //send_attach (session, engine);

    //  Shut the connecter down.
    terminate ();

    //socket->event_connected (endpoint, fd);
    return false;
}

void zmq::tcp_connecter_t::timer_event (int id_)
{
    zmq_assert (id_ == reconnect_timer_id);
    timer_started = false;
    start_connecting ();
}

void zmq::tcp_connecter_t::start_connecting ()
{
    //  Open the connecting socket.
    int rc = open ();

    //  Connect may succeed in synchronous manner.
    if (rc == 0) {
        fprintf(stderr, "connect ok\n");
        handle = add_fd (s);
        handle_valid = true;
        out_event ();
    }

    //  Connection establishment may be delayed. Poll for its completion.
    else
    if (rc == -1 && errno == EINPROGRESS) {
        handle = add_fd (s);
        handle_valid = true;
        set_pollout (handle);
        //socket->event_connect_delayed (endpoint, zmq_errno());
    }

    //  Handle any other error condition by eventual reconnect.
    else {
        fprintf(stderr, "connect other\n");
        if (s != retired_fd)
            close ();
        add_reconnect_timer ();
    }
}

void zmq::tcp_connecter_t::add_reconnect_timer()
{
    int rc_ivl = get_new_reconnect_ivl();
    add_timer (rc_ivl, reconnect_timer_id);
    //socket->event_connect_retried (endpoint, rc_ivl);
    timer_started = true;
}

int zmq::tcp_connecter_t::get_new_reconnect_ivl ()
{
    //  The new interval is the current interval + random value.
    int this_interval = current_reconnect_ivl +
        (generate_random () % options.reconnect_ivl);

    //  Only change the current reconnect interval  if the maximum reconnect
    //  interval was set and if it's larger than the reconnect interval.
    if (options.reconnect_ivl_max > 0 && 
        options.reconnect_ivl_max > options.reconnect_ivl) {

        //  Calculate the next interval
        current_reconnect_ivl = current_reconnect_ivl * 2;
        if(current_reconnect_ivl >= options.reconnect_ivl_max) {
            current_reconnect_ivl = options.reconnect_ivl_max;
        }   
    }
    return this_interval;
}

int zmq::tcp_connecter_t::open ()
{
    zmq_assert (s == retired_fd);

    //  Create the socket.
    s = open_socket (addr->resolved.tcp_addr->family (), SOCK_STREAM, IPPROTO_TCP);
    if (s == -1)
        return -1;

    //  On some systems, IPv4 mapping in IPv6 sockets is disabled by default.
    //  Switch it on in such cases.
    if (addr->resolved.tcp_addr->family () == AF_INET6)
        enable_ipv4_mapping (s);

    // Set the socket to non-blocking mode so that we get async connect().
    unblock_socket (s);

    //  Set the socket buffer limits for the underlying socket.
    if (options.sndbuf != 0)
        set_tcp_send_buffer (s, options.sndbuf);
    if (options.rcvbuf != 0)
        set_tcp_receive_buffer (s, options.rcvbuf);

    //  Connect to the remote peer.
    int rc = ::connect (
        s, addr->resolved.tcp_addr->addr (),
        addr->resolved.tcp_addr->addrlen ());

    //  Connect was successfull immediately.
    if (rc == 0)
        return 0;

    //  Translate error codes indicating asynchronous connect has been
    //  launched to a uniform EINPROGRESS.
    if (errno == EINTR)
        errno = EINPROGRESS;
    return -1;
}

zmq::fd_t zmq::tcp_connecter_t::connect ()
{
    //  Async connect has finished. Check whether an error occurred
    int err = 0;
    socklen_t len = sizeof (err);

    int rc = getsockopt (s, SOL_SOCKET, SO_ERROR, (char*) &err, &len);

    //  Assert if the error was caused by 0MQ bug.
    //  Networking problems are OK. No need to assert.

    //  Following code should handle both Berkeley-derived socket
    //  implementations and Solaris.
    if (rc == -1)
        err = errno;
    if (err != 0) {
        errno = err;
        errno_assert (
            errno == ECONNREFUSED ||
            errno == ECONNRESET ||
            errno == ETIMEDOUT ||
            errno == EHOSTUNREACH ||
            errno == ENETUNREACH ||
            errno == ENETDOWN ||
            errno == EINVAL);
        return retired_fd;
    }

    //  Return the newly connected socket.
    fd_t result = s;
    s = retired_fd;
    return result;
}

void zmq::tcp_connecter_t::close ()
{
    zmq_assert (s != retired_fd);
    int rc = ::close (s);
    errno_assert (rc == 0);
    //socket->event_closed (endpoint, s);
    s = retired_fd;
}
