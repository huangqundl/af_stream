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

#include "ip.hpp"
#include "tcp.hpp"
#include "err.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

void afs_zmq::tune_tcp_socket (fd_t s_)
{
    //  Disable Nagle's algorithm. We are doing data batching on 0MQ level,
    //  so using Nagle wouldn't improve throughput in anyway, but it would
    //  hurt latency.
    int nodelay = 1;
    int rc = setsockopt (s_, IPPROTO_TCP, TCP_NODELAY, (char*) &nodelay,
        sizeof (int));
    errno_assert (rc == 0);

}

void afs_zmq::set_tcp_send_buffer (fd_t sockfd_, int bufsize_)
{
    const int rc = setsockopt (sockfd_, SOL_SOCKET, SO_SNDBUF,
        (char*) &bufsize_, sizeof bufsize_);
    errno_assert (rc == 0);
}

void afs_zmq::set_tcp_receive_buffer (fd_t sockfd_, int bufsize_)
{
    const int rc = setsockopt (sockfd_, SOL_SOCKET, SO_RCVBUF,
        (char*) &bufsize_, sizeof bufsize_);
    errno_assert (rc == 0);
}

void afs_zmq::tune_tcp_keepalives (fd_t s_, int keepalive_, int keepalive_cnt_, int keepalive_idle_, int keepalive_intvl_)
{
    // These options are used only under certain #ifdefs below.
    (void)keepalive_;
    (void)keepalive_cnt_;
    (void)keepalive_idle_;
    (void)keepalive_intvl_;

    // If none of the #ifdefs apply, then s_ is unused.
    (void)s_;

    //  Tuning TCP keep-alives if platform allows it
    //  All values = -1 means skip and leave it for OS
    if (keepalive_ != -1) {
        int rc = setsockopt (s_, SOL_SOCKET, SO_KEEPALIVE, (char*) &keepalive_, sizeof (int));
        errno_assert (rc == 0);

        if (keepalive_cnt_ != -1) {
            int rc = setsockopt (s_, IPPROTO_TCP, TCP_KEEPCNT, &keepalive_cnt_, sizeof (int));
            errno_assert (rc == 0);
        }

        if (keepalive_idle_ != -1) {
            int rc = setsockopt (s_, IPPROTO_TCP, TCP_KEEPIDLE, &keepalive_idle_, sizeof (int));
            errno_assert (rc == 0);
        }

        if (keepalive_intvl_ != -1) {
            int rc = setsockopt (s_, IPPROTO_TCP, TCP_KEEPINTVL, &keepalive_intvl_, sizeof (int));
            errno_assert (rc == 0);
        }
    }
}
