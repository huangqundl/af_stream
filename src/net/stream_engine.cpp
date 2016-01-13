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

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#include <string.h>
#include <new>

#include "stream_engine.hpp"
//#include "io_thread.hpp"
//#include "session_base.hpp"
#include "v2_encoder.hpp"
#include "v2_decoder.hpp"
#include "null_mechanism.hpp"
//#include "curve_client.hpp"
//#include "curve_server.hpp"
#include "config.hpp"
#include "err.hpp"
#include "ip.hpp"
#include "likely.hpp"
#include "wire.hpp"

// qhuang:
// add return value for in_event() and out_event() to indicate whether to finish
// add return value -2 for push and pull function to indicate finish from application

afs_zmq::stream_engine_t::stream_engine_t (fd_t fd_, const options_t &options_, 
                                       const std::string &endpoint_,
                                       afs::InCallbackBase* in_callback,
                                       afs::OutCallbackBase* out_callback
                                       ) :
    in_callback_(in_callback),
    out_callback_(out_callback),
    s (fd_),
    inpos (NULL),
    insize (0),
    decoder (NULL),
    outpos (NULL),
    outsize (0),
    encoder (NULL),
    handshaking (true),
    greeting_size (v2_greeting_size),
    greeting_bytes_read (0),
    //session (NULL),
    options (options_),
    endpoint (endpoint_),
    plugged (false),
    read_msg (NULL),
    write_msg (NULL),
    io_error (false),
    // subscription_required (false),
    mechanism (NULL),
    input_stopped (false),
    output_stopped (false)
    //socket (NULL)
{
    int rc = tx_msg.init ();
    errno_assert (rc == 0);
    
    //  Put the socket into non-blocking mode.
    unblock_socket (s);

    if (out_callback_==NULL) {
        //LOG_MSG("NULL out_callback_ in construct\n");
    }
    else {
        //LOG_MSG("out_callback_ exists in construct\n");
    }

    /*
    recv_bytes = 0;
    send_bytes = 0;
    start_ts = 0;
    stop_ts = 0;
    handshake_ready = false;
    */

    tx_data = NULL;
    tx_size = 0;
    /*
    if (!get_peer_ip_address (s, peer_address))
        peer_address = "";
    */

}

afs_zmq::stream_engine_t::~stream_engine_t ()
{
    zmq_assert (!plugged);

    if (s != retired_fd) {
        int rc = close (s);
        errno_assert (rc == 0);
        s = retired_fd;
    }

    int rc = tx_msg.close ();
    errno_assert (rc == 0);

    delete encoder;
    delete decoder;
    delete mechanism;
}

void afs_zmq::stream_engine_t::plug (
        //io_thread_t *io_thread_,
    //session_base_t *session_
    epoll_t* poller
    )
{
    zmq_assert (!plugged);
    plugged = true;

    // qhuang:
    /*
    if (in_callback_) {
        LOG_MSG("add in_callback\n");
        in_callback_->AddEngine(this);
    }
    if (out_callback_) {
        LOG_MSG("add out_callback\n");
        out_callback_->AddEngine(this);
    }
    */

    //  Connect to session object.
    /*
    zmq_assert (!session);
    zmq_assert (session_);
    session = session_;
    socket = session-> get_socket ();
    */

    //  Connect to I/O threads poller object.
    //io_object_t::plug (io_thread_);
    // qhuang: TODO set poller
    io_object_t::set_poller(poller);
    handle = add_fd (s);
    io_error = false;

    if (options.raw_sock) {
        /*
        // no handshaking for raw sock, instantiate raw encoder and decoders
        encoder = new (std::nothrow) raw_encoder_t (out_batch_size);
        alloc_assert (encoder);

        decoder = new (std::nothrow) raw_decoder_t (in_batch_size);
        alloc_assert (decoder);

        // disable handshaking for raw socket
        handshaking = false;

        read_msg = &stream_engine_t::pull_msg_from_session;
        write_msg = &stream_engine_t::push_msg_to_session;
        */
    }
    else {
        //  Send the 'length' and 'flags' fields of the identity message.
        //  The 'length' field is encoded in the long format.
        outpos = greeting_send;
        outpos [outsize++] = 0xff;
        put_uint64 (&outpos [outsize], options.identity_size + 1);
        outsize += 8;
        outpos [outsize++] = 0x7f;
    }

    set_pollin (handle);
    set_pollout (handle);
    //  Flush all the data that may have been already received downstream.
    in_event ();
}

void afs_zmq::stream_engine_t::unplug ()
{
    zmq_assert (plugged);
    plugged = false;

    //  Cancel all fd subscriptions.
    if (!io_error)
        rm_fd (handle);

    //  Disconnect from I/O threads poller object.
    io_object_t::unplug ();

    //session = NULL;
}

void afs_zmq::stream_engine_t::terminate ()
{
    //LOG_MSG("stream engine terminate\n");
    unplug ();
    delete this;
}

//unsigned char in_buf[zmq::in_batch_size];
//unsigned char out_buf[zmq::out_batch_size];

bool afs_zmq::stream_engine_t::in_event ()
{
    //LOG_MSG("enter in_event()\n");

    assert (!io_error);

    //  If still handshaking, receive and process the greeting message.
    if (afs_unlikely (handshaking))
        if (!handshake ())
            return false;

    zmq_assert (decoder);

    //  If there has been an I/O error, stop polling.
    if (input_stopped) {
        rm_fd (handle);
        io_error = true;
        return false;
    }

    /*
    if (handshake_ready) {
        int rc = read (in_buf, in_batch_size);
        if (rc <= 0) {
            return false;
        }
        insize = static_cast <size_t> (rc);
        if (recv_bytes==0 && send_bytes==0) {
            start_ts = now_us();
            last_recv_bytes = 0;
        }
        recv_bytes += insize;
        if (recv_bytes-last_recv_bytes > 10000000000) {
            stop_ts = now_us();
            double used_time = (stop_ts - start_ts)/1000000.0;
            LOG_MSG("Stream_engine used time %lf, recv throughput %lf Gpbs/s\n",
                used_time,
                8.0*(recv_bytes-last_recv_bytes)/used_time/1024/1024/1024
                );
            last_recv_bytes = recv_bytes;
            start_ts = stop_ts;
        }
        return false;
    }
    */

    //  If there's no data to process in the buffer...
    if (!insize) {

        //  Retrieve the buffer and read as much data as possible.
        //  Note that buffer can be arbitrarily large. However, we assume
        //  the underlying TCP layer has fixed buffer size and thus the
        //  number of bytes read will be always limited.
        //LOG_MSG("    call read()\n");
        size_t bufsize = 0;
        decoder->get_buffer (&inpos, &bufsize);

        int const rc = read (inpos, bufsize);
        if (rc == 0) {
            /*
            stop_ts = now_us();
            double used_time = (stop_ts - start_ts)/1000000.0;
            LOG_MSG("Stream_engine used time %lf, recv throughput %lf Gpbs/s\n",
                used_time,
                8*recv_bytes/used_time/1024/1024/1024
                );
            */
            //LOG_MSG("error no data read\n");
            error ();
            return false;
        }
        if (rc == -1) {
            /*
            stop_ts = now_us();
            double used_time = (stop_ts - start_ts)/1000000.0;
            LOG_MSG("Stream_engine used time %lf, recv throughput %lf Gpbs/s\n",
                used_time,
                8*recv_bytes/used_time/1024/1024/1024
                );
            */
            //LOG_MSG("error in read\n");
            if (errno != EAGAIN) {
                //LOG_MSG("    exit\n");
                error ();
            }
            return false;
        }

        //  Adjust input size
        insize = static_cast <size_t> (rc);

        /*
        if (recv_bytes==0 && send_bytes==0) {
            start_ts = now_us();
            last_recv_bytes = 0;
        }
        recv_bytes += insize;
        if (recv_bytes-last_recv_bytes > 100000000) {
            stop_ts = now_us();
            double used_time = (stop_ts - start_ts)/1000000.0;
            LOG_MSG("in_event used time %lf, recv throughput %lf Gpbs/s\n",
                used_time,
                8.0*(recv_bytes-last_recv_bytes)/used_time/1024/1024/1024
                );
            last_recv_bytes = recv_bytes;
            start_ts = stop_ts;
        }
        */
    }

    int rc = 0;
    size_t processed = 0;
    //bool stop = false;

    //LOG_MSG("    before decode insize %lu\n", insize);
    while (insize > 0) {
        /*
        if (handshake_ready) {
            int nbytes = insize < in_batch_size ? insize : in_batch_size;
            memcpy(in_buf, inpos, nbytes);
            inpos += nbytes;
            insize -= nbytes;
            //inpos += insize;
            //insize = 0;
        }
        else {
            */

        rc = decoder->decode (inpos, insize, processed);
        zmq_assert (processed <= insize);
        inpos += processed;
        insize -= processed;
        if (rc == 0 || rc == -1)
            break;

        rc = (this->*write_msg) (decoder->msg ());
        if (rc == -1)
            break;
        if (afs_unlikely(rc == -2)) {
            //break;
            /*
            stop_ts = now_us();
            double used_time = (stop_ts - start_ts)/1000000.0;
            LOG_MSG("Stream_engine used time %lf, recv throughput %lf Gpbs/s\n",
                used_time,
                8*recv_bytes/used_time/1024/1024/1024
                );
            */
            input_stopped = true;
        }
        //}
        //LOG_MSG("        in size %lu\n", insize);
    }
    //LOG_MSG("after insize %lu\n", insize);

    //  Tear down the connection if we have failed to decode input data
    //  or the session has rejected the message.
    if (rc == -1) {
        //LOG_MSG("input recv and decode error, reset input\n");
        if (errno != EAGAIN) {
            error ();
            return false;
        }
        input_stopped = true;
        reset_pollin (handle);
    }

    if (afs_unlikely(input_stopped)) {
        //LOG_MSG("    in_event reach finish\n");
        //input_stopped = true;
        reset_pollin (handle);
        if (output_stopped) {
            terminate();
            return true;
        }
    }

    //LOG_MSG("leave in_event() with insize %lu\n", insize);
    return false;

    //session->flush ();
}

bool afs_zmq::stream_engine_t::out_event ()
{
    //LOG_MSG("enter out_event()\n");
    zmq_assert (!io_error);


    int rc = 0;
    //bool stop = false;
    //  If write buffer is empty, try to read new data from the encoder.
    if (!outsize) {

        //  Even when we stop polling as soon as there is no
        //  data to send, the poller may invoke out_event one
        //  more time due to 'speculative write' optimisation.
        if (afs_unlikely (encoder == NULL)) {
            zmq_assert (handshaking);
            return false;
        }

        /*
        if (handshake_ready) {
            outpos = out_buf;
            outsize = out_batch_size;
            //outpos = NULL;
            //outsize = encoder->encode (&outpos, 0);
        }
        else {
        */
        outpos = NULL;
        outsize = encoder->encode (&outpos, 0);
        //}

        while (outsize < out_batch_size) {
            rc = (this->*read_msg) (&tx_msg);
            if (rc == -1)
                break;
            //LOG_MSG("rc in out_event: %d\n", rc);
            encoder->load_msg (&tx_msg, tx_data, tx_size);
            //LOG_MSG(" after load_msg\n");
            unsigned char *bufptr = outpos + outsize;
            size_t n = encoder->encode (&bufptr, out_batch_size - outsize);
            zmq_assert (n > 0);
            if (outpos == NULL)
                outpos = bufptr;
            outsize += n;
            if (afs_unlikely(rc == -2)) {
                //LOG_MSG("out event(): finish with outsize %lu\n", outsize);
                output_stopped = true;
                break;
            }
        }
        //LOG_MSG("break\n");

        //  If there is no data to send, stop polling for output.
        if (outsize == 0) {
            //LOG_MSG("outsize is zero\n");
            //output_stopped = true;
            //reset_pollout (handle);
            return false;
        }
    }

    //  If there are any data to write in write buffer, write as much as
    //  possible to the socket. Note that amount of data to write can be
    //  arbitrarily large. However, we assume that underlying TCP layer has
    //  limited transmission buffer and thus the actual number of bytes
    //  written should be reasonably modest.
    int nbytes = write (outpos, outsize);

    //  IO error has occurred. We stop waiting for output events.
    //  The engine is not terminated until we detect input error;
    //  this is necessary to prevent losing incoming messages.
    if (nbytes == -1) {
        //LOG_MSG("send error, reset output\n");
        reset_pollout (handle);
        return false;
    }

    outpos += nbytes;
    outsize -= nbytes;

    if (afs_unlikely(output_stopped)) {
        outpos = NULL;
        outsize = encoder->encode (&outpos, 0);
        //LOG_MSG("no further output, %lu bytes remain in the buffer\n", outsize);
        while (outsize) {
            int nbytes = write (outpos, outsize);
            outpos += nbytes;
            outsize -= nbytes;
        }
        output_stopped = true;
        reset_pollout (handle);
        if (input_stopped) {
            terminate();
            return true;
        }
    }

    //  If we are still handshaking and there are no data
    //  to send, stop polling for output.
    if (afs_unlikely (handshaking))
        if (outsize == 0)
            reset_pollout (handle);

    //LOG_MSG("leave out_event()\n");
    return false;
}

void afs_zmq::stream_engine_t::restart_output ()
{
    if (afs_unlikely (io_error))
        return;

    if (afs_likely (output_stopped)) {
        //LOG_MSG("restart output\n");
        // qhuang:
        if (!out_callback_) {
            return;
        }
        set_pollout (handle);
        output_stopped = false;
    }

    //  Speculative write: The assumption is that at the moment new message
    //  was sent by the user the socket is probably available for writing.
    //  Thus we try to write the data to socket avoiding polling for POLLOUT.
    //  Consequently, the latency should be better in request/reply scenarios.
    out_event ();
}

void afs_zmq::stream_engine_t::restart_input ()
{
    /*
    zmq_assert (input_stopped);
    zmq_assert (session != NULL);
    zmq_assert (decoder != NULL);
    */

    int rc = (this->*write_msg) (decoder->msg ());
    if (rc == -1) {
        /*
        if (errno == EAGAIN)
            session->flush ();
        else
            error ();
        */
        return;
    }

    while (insize > 0) {
        size_t processed = 0;
        rc = decoder->decode (inpos, insize, processed);
        zmq_assert (processed <= insize);
        inpos += processed;
        insize -= processed;
        if (rc == 0 || rc == -1)
            break;
        rc = (this->*write_msg) (decoder->msg ());
        if (rc == -1)
            break;
    }

    /*
    if (rc == -1 && errno == EAGAIN)
        session->flush ();
    else
    if (rc == -1 || io_error)
        error ();
    else {
        input_stopped = false;
        set_pollin (handle);
        session->flush ();

        //  Speculative read.
        in_event ();
    }
    */
}

bool afs_zmq::stream_engine_t::handshake ()
{
    zmq_assert (handshaking);
    zmq_assert (greeting_bytes_read < greeting_size);
    //  Receive the greeting.
    while (greeting_bytes_read < greeting_size) {
        const int n = read (greeting_recv + greeting_bytes_read,
                            greeting_size - greeting_bytes_read);
        if (n == 0) {
            error ();
            return false;
        }
        if (n == -1) {
            if (errno != EAGAIN)
                error ();
            return false;
        }

        greeting_bytes_read += n;

        //  We have received at least one byte from the peer.
        //  If the first byte is not 0xff, we know that the
        //  peer is using unversioned protocol.
        if (greeting_recv [0] != 0xff)
            break;

        if (greeting_bytes_read < signature_size)
            continue;

        //  Inspect the right-most bit of the 10th byte (which coincides
        //  with the 'flags' field if a regular message was sent).
        //  Zero indicates this is a header of identity message
        //  (i.e. the peer is using the unversioned protocol).
        if (!(greeting_recv [9] & 0x01))
            break;

        //  The peer is using versioned protocol.
        //  Send the major version number.
        if (outpos + outsize == greeting_send + signature_size) {
            if (outsize == 0)
                set_pollout (handle);
            outpos [outsize++] = 3;     //  Major version number
        }

        if (greeting_bytes_read > signature_size) {
            if (outpos + outsize == greeting_send + signature_size + 1) {
                if (outsize == 0)
                    set_pollout (handle);

                //  Use ZMTP/2.0 to talk to older peers.
                if (greeting_recv [10] == ZMTP_1_0
                ||  greeting_recv [10] == ZMTP_2_0)
                    outpos [outsize++] = options.type;
                else {
                    outpos [outsize++] = 0; //  Minor version number
                    memset (outpos + outsize, 0, 20);

                    zmq_assert (options.mechanism == ZMQ_NULL
                            ||  options.mechanism == ZMQ_PLAIN
                            ||  options.mechanism == ZMQ_CURVE);

                    if (options.mechanism == ZMQ_NULL)
                        memcpy (outpos + outsize, "NULL", 4);
                    else
                    if (options.mechanism == ZMQ_PLAIN)
                        memcpy (outpos + outsize, "PLAIN", 5);
                    else
                        memcpy (outpos + outsize, "CURVE", 5);
                    outsize += 20;
                    memset (outpos + outsize, 0, 32);
                    outsize += 32;
                    greeting_size = v3_greeting_size;
                }
            }
        }
    }

    //  Position of the revision field in the greeting.
    const size_t revision_pos = 10;

    //  Is the peer using ZMTP/1.0 with no revision number?
    //  If so, we send and receive rest of identity message
    if (greeting_recv [0] != 0xff || !(greeting_recv [9] & 0x01)) {
        /*
        encoder = new (std::nothrow) v1_encoder_t (out_batch_size);
        alloc_assert (encoder);

        decoder = new (std::nothrow) v1_decoder_t (in_batch_size, options.maxmsgsize);
        alloc_assert (decoder);

        //  We have already sent the message header.
        //  Since there is no way to tell the encoder to
        //  skip the message header, we simply throw that
        //  header data away.
        const size_t header_size = options.identity_size + 1 >= 255 ? 10 : 2;
        unsigned char tmp [10], *bufferp = tmp;

        //  Prepare the identity message and load it into encoder.
        //  Then consume bytes we have already sent to the peer.
        const int rc = tx_msg.init_size (options.identity_size);
        zmq_assert (rc == 0);
        memcpy (tx_msg.data (), options.identity, options.identity_size);
        encoder->load_msg (&tx_msg);
        size_t buffer_size = encoder->encode (&bufferp, header_size);
        zmq_assert (buffer_size == header_size);

        //  Make sure the decoder sees the data we have already received.
        inpos = greeting_recv;
        insize = greeting_bytes_read;

        //  To allow for interoperability with peers that do not forward
        //  their subscriptions, we inject a phantom subscription message
        //  message into the incoming message stream.
        if (options.type == ZMQ_PUB || options.type == ZMQ_XPUB)
            subscription_required = true;

        //  We are sending our identity now and the next message
        //  will come from the socket.
        read_msg = &stream_engine_t::pull_msg_from_session;

        //  We are expecting identity message.
        write_msg = &stream_engine_t::write_identity;
        */
    }
    else
    if (greeting_recv [revision_pos] == ZMTP_1_0) {
        /*
        encoder = new (std::nothrow) v1_encoder_t (
            out_batch_size);
        alloc_assert (encoder);

        decoder = new (std::nothrow) v1_decoder_t (
            in_batch_size, options.maxmsgsize);
        alloc_assert (decoder);
        */
    }
    else
    if (greeting_recv [revision_pos] == ZMTP_2_0) {
        /*
        encoder = new (std::nothrow) v2_encoder_t (out_batch_size);
        alloc_assert (encoder);

        decoder = new (std::nothrow) v2_decoder_t (
            in_batch_size, options.maxmsgsize);
        alloc_assert (decoder);
        */
    }
    else {
        if (out_callback_ == NULL) {
            //LOG_MSG("NULL out callback\n");
        }
        encoder = new (std::nothrow) v2_encoder_t (out_batch_size, out_callback_);
        alloc_assert (encoder);

        decoder = new (std::nothrow) v2_decoder_t (
            in_batch_size, options.maxmsgsize);
        alloc_assert (decoder);

        if (options.mechanism == ZMQ_NULL
        &&  memcmp (greeting_recv + 12, "NULL\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20) == 0) {
            // qhuang: enter here
            mechanism = new (std::nothrow)
                null_mechanism_t (options);
                //null_mechanism_t (session, peer_address, options);
            alloc_assert (mechanism);
        }
        else
        if (options.mechanism == ZMQ_PLAIN
        &&  memcmp (greeting_recv + 12, "PLAIN\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20) == 0) {
            /*
            mechanism = new (std::nothrow)
                plain_mechanism_t (session, peer_address, options);
            alloc_assert (mechanism);
            */
        }
        else {
            error ();
            return false;
        }
        read_msg = &stream_engine_t::next_handshake_command;
        write_msg = &stream_engine_t::process_handshake_command;
    }

    // Start polling for output if necessary.
    if (outsize == 0)
        set_pollout (handle);

    //  Handshaking was successful.
    //  Switch into the normal message flow.
    handshaking = false;

    return true;
}

/*
int zmq::stream_engine_t::read_identity (msg_t *msg_)
{
    int rc = msg_->init_size (options.identity_size);
    errno_assert (rc == 0);
    if (options.identity_size > 0)
        memcpy (msg_->data (), options.identity, options.identity_size);
    read_msg = &stream_engine_t::pull_msg_from_session;
    return 0;
}
*/

    /*
int zmq::stream_engine_t::write_identity (msg_t *msg_)
{
    if (options.recv_identity) {
        msg_->set_flags (msg_t::identity);
        int rc = session->push_msg (msg_);
        errno_assert (rc == 0);
    }
    else {
        int rc = msg_->close ();
        errno_assert (rc == 0);
        rc = msg_->init ();
        errno_assert (rc == 0);
    }

    if (subscription_required)
        write_msg = &stream_engine_t::write_subscription_msg;
    else
        write_msg = &stream_engine_t::push_msg_to_session;

    return 0;
}
    */

int afs_zmq::stream_engine_t::next_handshake_command (msg_t *msg_)
{
    zmq_assert (mechanism != NULL);

    const int rc = mechanism->next_handshake_command (msg_);
    if (rc == 0) {
        msg_->set_flags (msg_t::command);
        if (mechanism->is_handshake_complete ())
            mechanism_ready ();
    }

    return rc;
}

int afs_zmq::stream_engine_t::process_handshake_command (msg_t *msg_)
{
    zmq_assert (mechanism != NULL);
    const int rc = mechanism->process_handshake_command (msg_);
    if (rc == 0) {
        if (mechanism->is_handshake_complete ())
            mechanism_ready ();
        if (output_stopped)
            restart_output ();
    }

    return rc;
}

void afs_zmq::stream_engine_t::zap_msg_available ()
{
    zmq_assert (mechanism != NULL);

    const int rc = mechanism->zap_msg_available ();
    if (rc == -1) {
        error ();
        return;
    }
    if (input_stopped)
        restart_input ();
    if (output_stopped)
        restart_output ();
}

void afs_zmq::stream_engine_t::mechanism_ready ()
{
    /*
    if (options.recv_identity) {
        msg_t identity;
        mechanism->peer_identity (&identity);
        const int rc = session->push_msg (&identity);
        if (rc == -1 && errno == EAGAIN) {
            // If the write is failing at this stage with
            // an EAGAIN the pipe must be being shut down,
            // so we can just bail out of the identity set.
            return;
        }
        errno_assert (rc == 0);
        session->flush ();
    }
    */

    //LOG_MSG("Handshake ready\n\n");
    handshake_ready = true;
    read_msg = &stream_engine_t::pull_and_encode;
    write_msg = &stream_engine_t::decode_and_push;

    // qhuang
    if (!in_callback_) {
        input_stopped = true;
        reset_pollin (handle);
    }
    if (!out_callback_) {
        output_stopped = true;
        reset_pollout (handle);
    }
}

/*
int zmq::stream_engine_t::pull_msg_from_session (msg_t *msg_)
{
    //return session->pull_msg (msg_);
    return 0;
}
*/

/*
int zmq::stream_engine_t::push_msg_to_session (msg_t *msg_)
{
    //return session->push_msg (msg_);
    return 0;
}
*/

int afs_zmq::stream_engine_t::pull_and_encode (msg_t *msg_)
{
    zmq_assert (mechanism != NULL);

    if (out_callback_) {
        //if (afs_unlikely(output_stopped)) {
        //    out_callback_->PullRecord(NULL, NULL);
        //}
        //else {
        //    out_callback_->PullRecord(&data, &len);
        //}
        int rc = out_callback_->PullRecord(&tx_data, &tx_size);
        if (tx_data) {
            msg_->init_size(tx_size);
            //memcpy(msg_->data(), tx_data, tx_size);
            //LOG_MSG("pull: rc %d\n", rc);
            if (rc) {
                //out_callback_->Ack(tx_data);
                return -2;
            }
            /*
            if (mechanism->encode (msg_) == -1) {
                //LOG_MSG("pull: encode error\n");
                return -1;
            }
            */

            //LOG_MSG("pull: normal\n");
            return 0;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}

int afs_zmq::stream_engine_t::decode_and_push (msg_t *msg_)
{
    zmq_assert (mechanism != NULL);

    /*
    if (mechanism->decode (msg_) == -1) {
        LOG_MSG("mechanism decode error\n");
        return -1;
    }
    */
    
    if (in_callback_ == NULL) {
        return 0;
    }

    int rc = in_callback_->PushRecord(msg_->data(), msg_->size());
    //int rc = in_callback_->PushRecord(msg_, 0, this);
    if (rc != 0) {
        // to finish
        //LOG_MSG("finish message is decoded\n");
        return -2;
    }

    return 0;
}

    /*
int zmq::stream_engine_t::push_one_then_decode_and_push (msg_t *msg_)
{
    //const int rc = session->push_msg (msg_);
    if (rc == 0)
        write_msg = &stream_engine_t::decode_and_push;
    return rc;
    return 0;
}
    */

    /*
int zmq::stream_engine_t::write_subscription_msg (msg_t *msg_)
{
    msg_t subscription;

    //  Inject the subscription message, so that also
    //  ZMQ 2.x peers receive published messages.
    int rc = subscription.init_size (1);
    errno_assert (rc == 0);
    *(unsigned char*) subscription.data () = 1;
    rc = session->push_msg (&subscription);
    if (rc == -1)
       return -1;

    write_msg = &stream_engine_t::push_msg_to_session;
    return push_msg_to_session (msg_);
    return 0;
}
    */

void afs_zmq::stream_engine_t::error ()
{
    //zmq_assert (session);
    //socket->event_disconnected (endpoint, s);
    //session->flush ();
    //session->detach ();
    //LOG_MSG("stream engine error\n");
    unplug ();
    delete this;
}

int afs_zmq::stream_engine_t::write (const void *data_, size_t size_)
{
    ssize_t nbytes = send (s, data_, size_, 0);

    //  Several errors are OK. When speculative write is being done we may not
    //  be able to write a single byte from the socket. Also, SIGSTOP issued
    //  by a debugging tool can result in EINTR error.
    if (nbytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK ||
          errno == EINTR))
        return 0;

    //  Signalise peer failure.
    if (nbytes == -1) {
        errno_assert (errno != EACCES
                   && errno != EBADF
                   && errno != EDESTADDRREQ
                   && errno != EFAULT
                   && errno != EINVAL
                   && errno != EISCONN
                   && errno != EMSGSIZE
                   && errno != ENOMEM
                   && errno != ENOTSOCK
                   && errno != EOPNOTSUPP);
        return -1;
    }

    //LOG_MSG("    send nbytes %lu\n", nbytes);
    return static_cast <int> (nbytes);
}

int afs_zmq::stream_engine_t::read (void *data_, size_t size_)
{
    const ssize_t rc = recv (s, data_, size_, 0);

    //  Several errors are OK. When speculative read is being done we may not
    //  be able to read a single byte from the socket. Also, SIGSTOP issued
    //  by a debugging tool can result in EINTR error.
    if (rc == -1) {
        errno_assert (errno != EBADF
                   && errno != EFAULT
                   && errno != EINVAL
                   && errno != ENOMEM
                   && errno != ENOTSOCK);
        if (errno == EWOULDBLOCK || errno == EINTR)
            errno = EAGAIN;
    }

    /*
    if (rc != -1)
        LOG_MSG("    recv nbytes %lu\n", rc);
    */
    return static_cast <int> (rc);
}
