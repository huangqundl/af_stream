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

#include "tcp_listener.hpp"
#include "stream_engine.hpp"
//#include "io_thread.hpp"
//#include "session_base.hpp"
#include "config.hpp"
#include "err.hpp"
#include "ip.hpp"
#include "tcp.hpp"
//#include "socket_base.hpp"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

afs_zmq::tcp_listener_t::tcp_listener_t (
        afs::InCallbackBase* in_callback,
        afs::OutCallbackBase** out_callbacks,
        //io_thread_t *io_thread_,
        //socket_base_t *socket_, 
        const options_t &options_) :
    //own_t (io_thread_, options_),
    //io_object_t (io_thread_),
    io_object_t (),
    in_callback_(in_callback),
    out_callbacks_(out_callbacks),
    options(options_),
    s (retired_fd)
    //socket (socket_)
{
}

afs_zmq::tcp_listener_t::~tcp_listener_t ()
{
    zmq_assert (s == retired_fd);
}

void afs_zmq::tcp_listener_t::process_plug ()
{
    //  Start polling for incoming connections.
    handle = add_fd (s);
    set_pollin (handle);
}

// qhuang
void afs_zmq::tcp_listener_t::set_poller (epoll_t* poller)
{
    io_object_t::set_poller(poller);

    // function in io_object_t::plug
    handle = add_fd (s);
    set_pollin (handle);
}

void afs_zmq::tcp_listener_t::process_term (int linger_)
{
    rm_fd (handle);
    close ();
    //own_t::process_term (linger_);
}

bool afs_zmq::tcp_listener_t::in_event ()
{
    char peer_addr[16];
    memset((void*)peer_addr, 0, sizeof(peer_addr));
    fd_t fd = accept (peer_addr);

    //  If connection was reset by the peer in the meantime, just ignore it.
    //  TODO: Handle specific errors like ENFILE/EMFILE etc.
    if (fd == retired_fd) {
        //socket->event_accept_failed (endpoint, zmq_errno());
        return false;
    }

    tune_tcp_socket (fd);
    tune_tcp_keepalives (fd, options.tcp_keepalive, options.tcp_keepalive_cnt, options.tcp_keepalive_idle, options.tcp_keepalive_intvl);

    //fprintf(stderr, "listener accept %s\n", peer_addr);

    int peer_worker = -1;
    while (1) {
        const ssize_t rc = recv (fd, &peer_worker, sizeof(int), 0);
        if (rc == sizeof(int)) {
            break;
        }
    }
    //fprintf(stderr, "recv from worker %d\n", peer_worker);

    // Create the engine object for this connection.
    // qhuang: cannot 
    // in_callback_->IncNumIn();
    afs::OutCallbackBase* out_callback = NULL;
    if (out_callbacks_) {
        out_callback = out_callbacks_[peer_worker];
    }
    stream_engine_t *engine = new (std::nothrow)
        stream_engine_t (fd, options, endpoint, in_callback_, out_callback);
    alloc_assert (engine);

    engine->plug(io_object_t::get_poller());

    return false;

    //  Choose I/O thread to run connecter in. Given that we are already
    //  running in an I/O thread, there must be at least one available.
    //io_thread_t *io_thread = choose_io_thread (options.affinity);
    //zmq_assert (io_thread);

    //  Create and launch a session object. 
    // qhuang: TODO
    /*
    session_base_t *session = session_base_t::create (io_thread, false, socket,
        options, NULL);
    errno_assert (session);
    session->inc_seqnum ();
    launch_child (session);
    send_attach (session, engine, false);
    socket->event_accepted (endpoint, fd);
    */
}

void afs_zmq::tcp_listener_t::close ()
{
    zmq_assert (s != retired_fd);
    int rc = ::close (s);
    errno_assert (rc == 0);
    //socket->event_closed (endpoint, s);
    s = retired_fd;
}

int afs_zmq::tcp_listener_t::get_address (std::string &addr_)
{
    // Get the details of the TCP socket
    struct sockaddr_storage ss;
    socklen_t sl = sizeof (ss);
    int rc = getsockname (s, (struct sockaddr *) &ss, &sl);

    if (rc != 0) {
        addr_.clear ();
        return rc;
    }

    tcp_address_t addr ((struct sockaddr *) &ss, sl);
    return addr.to_string (addr_);
}

int afs_zmq::tcp_listener_t::set_address (const char *addr_)
{
    //  Convert the textual address into address structure.
    int rc = address.resolve (addr_, true, options.ipv6);
    if (rc != 0)
        return -1;

    //  Create a listening socket.
    s = open_socket (address.family (), SOCK_STREAM, IPPROTO_TCP);

    //  IPv6 address family not supported, try automatic downgrade to IPv4.
    if (address.family () == AF_INET6
    && errno == EAFNOSUPPORT
    && options.ipv6) {
        rc = address.resolve (addr_, true, true);
        if (rc != 0)
            return rc;
        s = ::socket (address.family (), SOCK_STREAM, IPPROTO_TCP);
    }

    if (s == -1)
        return -1;

    //  On some systems, IPv4 mapping in IPv6 sockets is disabled by default.
    //  Switch it on in such cases.
    if (address.family () == AF_INET6)
        enable_ipv4_mapping (s);

    //  Set the socket buffer limits for the underlying socket.
    if (options.sndbuf != 0)
        set_tcp_send_buffer (s, options.sndbuf);
    if (options.rcvbuf != 0)
        set_tcp_receive_buffer (s, options.rcvbuf);

    //  Allow reusing of the address.
    int flag = 1;
    rc = setsockopt (s, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof (int));
    errno_assert (rc == 0);

    address.to_string (endpoint);

    //  Bind the socket to the network interface and port.
    rc = bind (s, address.addr (), address.addrlen ());
    if (rc != 0)
        goto error;

    //  Listen for incomming connections.
    rc = listen (s, options.backlog);
    if (rc != 0)
        goto error;

    //socket->event_listening (endpoint, s);
    return 0;

error:
    int err = errno;
    close ();
    errno = err;
    return -1;
}

afs_zmq::fd_t afs_zmq::tcp_listener_t::accept (char* peer_addr)
{
    //  The situation where connection cannot be accepted due to insufficient
    //  resources is considered valid and treated by ignoring the connection.
    //  Accept one connection and deal with different failure modes.
    zmq_assert (s != retired_fd);

    struct sockaddr_storage ss;
    memset (&ss, 0, sizeof (ss));
    socklen_t ss_len = sizeof (ss);
    fd_t sock = ::accept (s, (struct sockaddr *) &ss, &ss_len);

    if (sock == -1) {
        errno_assert (errno == EAGAIN || errno == EWOULDBLOCK ||
            errno == EINTR || errno == ECONNABORTED || errno == EPROTO ||
            errno == ENOBUFS || errno == ENOMEM || errno == EMFILE ||
            errno == ENFILE);
        return retired_fd;
    }

    if (!options.tcp_accept_filters.empty ()) {
        bool matched = false;
        for (options_t::tcp_accept_filters_t::size_type i = 0; i != options.tcp_accept_filters.size (); ++i) {
            if (options.tcp_accept_filters[i].match_address ((struct sockaddr *) &ss, ss_len)) {
                matched = true;
                break;
            }
        }
        if (!matched) {
            int rc = ::close (sock);
            errno_assert (rc == 0);
            return retired_fd;
        }
    }

    // qhuang
    //sockaddr_in sin;
    //memcpy(&sin, &ss, sizeof(sin));
    //sprintf(peer_addr, inet_ntoa(sin.sin_addr));

    /*
    sockaddr_in* sin = (sockaddr_in*)(&ss);

    fprintf(stderr, "recv sin zero: ");
    for (int i=0; i<8; i++) {
        fprintf(stderr, "%d ", sin->sin_zero[i]);
    }
    fprintf(stderr, "\n");
    */
    //info.port = sin.sin_port;
    //info.sock = new_fd;

    return sock;
}
