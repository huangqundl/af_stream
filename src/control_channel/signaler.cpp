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


//  On AIX, poll.h has to be included before zmq.h to get consistent
//  definition of pollfd structure (AIX uses 'reqevents' and 'retnevents'
//  instead of 'events' and 'revents' and defines macros to map from POSIX-y
//  names to AIX-specific names).
#include <poll.h>
#include <stdint.h>

#include "signaler.hpp"
#include "likely.hpp"
#include "config.hpp"
#include "err.hpp"
#include "fd.hpp"
#include "ip.hpp"

#include <sys/eventfd.h>

#include <unistd.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

// Helper to sleep for specific number of milliseconds (or until signal)
//
static int sleep_ms (unsigned int ms_)
{
    if (ms_ == 0)
        return 0;
    return usleep (ms_ * 1000);
}

// Helper to wait on close(), for non-blocking sockets, until it completes
// If EAGAIN is received, will sleep briefly (1-100ms) then try again, until
// the overall timeout is reached.
//
static int close_wait_ms (int fd_, unsigned int max_ms_ = 2000)
{
    unsigned int ms_so_far = 0;
    unsigned int step_ms   = max_ms_ / 10;
    if (step_ms < 1)
        step_ms = 1;

    if (step_ms > 100)
        step_ms = 100;

    int rc = 0;       // do not sleep on first attempt

    do
    {
        if (rc == -1 && errno == EAGAIN)
        {
            sleep_ms (step_ms);
            ms_so_far += step_ms;
        }

        rc = close (fd_);
    } while (ms_so_far < max_ms_ && rc == -1 && errno == EAGAIN);

    return rc;
}

afs_zmq::signaler_t::signaler_t ()
{
    //  Create the socketpair for signaling.
    if (make_fdpair (&r, &w) == 0) {
        unblock_socket (w);
        unblock_socket (r);
    }
    pid = getpid();
}

afs_zmq::signaler_t::~signaler_t ()
{
    int rc = close_wait_ms (r);
    errno_assert (rc == 0);
}

afs_zmq::fd_t afs_zmq::signaler_t::get_fd ()
{
    return r;
}

void afs_zmq::signaler_t::send ()
{
    if (afs_unlikely(pid != getpid())) {
        //printf("Child process %d signaler_t::send returning without sending #1\n", getpid());
        return; // do not send anything in forked child context
    }
    const uint64_t inc = 1;
    ssize_t sz = write (w, &inc, sizeof (inc));
    errno_assert (sz == sizeof (inc));
}

int afs_zmq::signaler_t::wait (int timeout_)
{
    if (afs_unlikely(pid != getpid()))
    {
        // we have forked and the file descriptor is closed. Emulate an interupt
        // response.
        //printf("Child process %d signaler_t::wait returning simulating interrupt #1\n", getpid());
        errno = EINTR;
        return -1;
    }

    struct pollfd pfd;
    pfd.fd = r;
    pfd.events = POLLIN;
    int rc = poll (&pfd, 1, timeout_);
    if (afs_unlikely (rc < 0)) {
        errno_assert (errno == EINTR);
        return -1;
    }
    else
    if (afs_unlikely (rc == 0)) {
        errno = EAGAIN;
        return -1;
    }
    if (afs_unlikely(pid != getpid())) {
        // we have forked and the file descriptor is closed. Emulate an interupt
        // response.
        //printf("Child process %d signaler_t::wait returning simulating interrupt #2\n", getpid());
        errno = EINTR;
        return -1;
    }
    zmq_assert (rc == 1);
    zmq_assert (pfd.revents & POLLIN);
    return 0;
}

void afs_zmq::signaler_t::recv ()
{
    //  Attempt to read a signal.
    uint64_t dummy;
    ssize_t sz = read (r, &dummy, sizeof (dummy));
    errno_assert (sz == sizeof (dummy));

    //  If we accidentally grabbed the next signal along with the current
    //  one, return it back to the eventfd object.
    if (afs_unlikely (dummy == 2)) {
        const uint64_t inc = 1;
        ssize_t sz2 = write (w, &inc, sizeof (inc));
        errno_assert (sz2 == sizeof (inc));
        return;
    }

    zmq_assert (dummy == 1);
}

void afs_zmq::signaler_t::forked()
{
    //  Close file descriptors created in the parent and create new pair
    close (r);
    close (w);
    make_fdpair (&r, &w);
}

//  Returns -1 if we could not make the socket pair successfully
int afs_zmq::signaler_t::make_fdpair (fd_t *r_, fd_t *w_)
{
    fd_t fd = eventfd (0, 0);
    if (fd == -1) {
        errno_assert (errno == ENFILE || errno == EMFILE);
        *w_ = *r_ = -1;
        return -1;
    }
    else {
        *w_ = *r_ = fd;
        return 0;
    }
}
