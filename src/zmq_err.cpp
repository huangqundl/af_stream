#include "zmq_err.hpp"

const char *afs_zmq::errno_to_string (int errno_)
{
    switch (errno_) {
    case EFSM:
        return "Operation cannot be accomplished in current state";
    case ENOCOMPATPROTO:
        return "The protocol is not compatible with the socket type";
    case ETERM:
        return "Context was terminated";
    case EMTHREAD:
        return "No thread available";
    default:
        return strerror (errno_);
    }
}

void afs_zmq::zmq_abort(const char *errmsg_)
{
    (void)errmsg_;
    abort ();
}
