/*
 * ZeroQueue.hpp
 *
 * Implementation of BaseQueue with ZeroMQ
 */

#ifndef ZERO_QUEUE_HPP__
#define ZERO_QUEUE_HPP__

#include <string>

#include <zmq.h>

#include "BaseQueue.hpp"
#include "util.hpp"

extern void* zmq_ctx;

void ZeroQueueInit(const int io_threads);
void ZeroQueueClean();

/*
 * Unidirectional Queue
 *
 * Current implementation:
 * each queue maitains one socket
 * each socket includes one connection
 *
 * Future extension:
 *      ZeroMQ multi-socket and multi-connection feature
 */
template <class T>
class ZeroQueue : BaseQueue<T> {
private:
    void* socket;
    bool sender;

public:
    ZeroQueue(std::string endpoint, bool sender) {
        if (zmq_ctx == 0) {
            LOG_ERR("zmq_ctx has not been initialized\n");
        }

        this->sender = sender;
        if (sender) {
            socket = zmq_socket(zmq_ctx, ZMQ_PUSH);
            if (zmq_bind(socket, endpoint.c_str()) == -1)
                LOG_ERR("zmq_bind(): %s\n", zmq_strerror(errno));
        }
        else {
            socket = zmq_socket(zmq_ctx, ZMQ_PULL);
            if (zmq_connect(socket, endpoint.c_str()) == -1)
                LOG_ERR("zmq_connect(): %s\n", zmq_strerror(errno));
        }
    }

    virtual ~ZeroQueue() {
        if (zmq_close(socket) == -1)
            LOG_ERR("zmq_close(): %s\n", zmq_strerror(errno));
    }

    /*
    void addAddress(std::string addr) {
        if (sender)
            LOG_ERR("only receiver can add address\n");
        if (zmq_connect(socket, addr.c_str()) == -1)
            LOG_ERR("zmq_connect(): %s\n", zmq_strerror(errno));
    }
    */

    /*
     * Insert data.
     * Return 0 if success, otherwise return -1
     */
	inline int Write(T* data) {
        if (zmq_send(socket, data, data->get_size(), 0) != -1) {
            return 0;
        }
        else {
            LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    /*
     * Extract data.
     * Return 0 if success, otherwise return -1
     */
	inline int Read(T* data) {
        if (zmq_recv(socket, data, sizeof(T), 0) != -1) {
            return 0;
        } else {
            LOG_ERR("ZeroQueue.read(): %s\n", zmq_strerror(errno));
            return -1;
        }
    }
};

/*
 * Bidirectional Queue
 *
 * Current implementation:
 * each queue maitains one socket (Req/Rep pattern)
 *
 * Future extension:
 *      ZeroMQ multi-socket and multi-connection feature
 */
template <class T>
class ZeroReqRepQueue : BaseQueue<T> {
private:
    void* socket;

public:
    ZeroReqRepQueue(std::string endpoint, bool upstream) {
        if (zmq_ctx == 0) {
            LOG_ERR("zmq_ctx has not been initialized\n");
        }

        if (upstream) {
            socket = zmq_socket(zmq_ctx, ZMQ_REQ);
            if (zmq_bind(socket, endpoint.c_str()) == -1)
                LOG_ERR("zmq_bind(): %s\n", zmq_strerror(errno));
        }
        else {
            socket = zmq_socket(zmq_ctx, ZMQ_REP);
            if (zmq_connect(socket, endpoint.c_str()) == -1)
                LOG_ERR("zmq_connect(): %s\n", zmq_strerror(errno));
        }
    }

    virtual ~ZeroReqRepQueue() {
        if (zmq_close(socket) == -1)
            LOG_ERR("zmq_close(): %s\n", zmq_strerror(errno));
    }

    /*
     * Insert data.
     * Return 0 if success, otherwise return -1
     */
	inline int Write(T* data) {
        if (zmq_send(socket, data, data->get_size(), 0) != -1) {
            return 0;
        }
        else {
            LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    /*
     * Extract data.
     * Return 0 if success, otherwise return -1
     */
	inline int Read(T* data) {
        if (zmq_recv(socket, data, sizeof(T), 0) != -1) {
            return 0;
        } else {
            LOG_ERR("ZeroQueue.read(): %s\n", zmq_strerror(errno));
            return -1;
        }
    }
};

template <class T>
class ZeroBiQueue : BaseQueue<T> {
private:
    void* forward_socket;
    void* back_socket;
    bool is_upstream;

public:
    ZeroBiQueue(std::string endpoint, bool upstream) {
        if (zmq_ctx == 0) {
            LOG_ERR("zmq_ctx has not been initialized\n");
        }
        this->is_upstream = upstream;

        std::string endpoint_up = endpoint;
        std::string endpoint_down = endpoint;
        endpoint_down[endpoint_down.size()-1] = endpoint_down[endpoint_down.size()-1]+1;

        if (upstream) {
            forward_socket = zmq_socket(zmq_ctx, ZMQ_PUSH);
            if (zmq_connect(forward_socket, endpoint_up.c_str()) == -1)
                LOG_ERR("zmq_bind(): %s\n", zmq_strerror(errno));
            LOG_MSG("Upstream worker: push at %s\n", endpoint_up.c_str());
            back_socket = zmq_socket(zmq_ctx, ZMQ_PULL);
            if (zmq_connect(back_socket, endpoint_down.c_str()) == -1)
                LOG_ERR("zmq_bind(): %s\n", zmq_strerror(errno));
            LOG_MSG("Upstream worker: pull at %s\n", endpoint_down.c_str());
        }
        else {
            forward_socket = zmq_socket(zmq_ctx, ZMQ_PULL);
            if (zmq_bind(forward_socket, endpoint_up.c_str()) == -1)
                LOG_ERR("zmq_connect(): %s\n", zmq_strerror(errno));
            LOG_MSG("Downstream worker: pull at %s\n", endpoint_up.c_str());
            back_socket = zmq_socket(zmq_ctx, ZMQ_PUSH);
            if (zmq_bind(back_socket, endpoint_down.c_str()) == -1)
                LOG_ERR("zmq_connect(): %s\n", zmq_strerror(errno));
            LOG_MSG("Downstream worker: push at %s\n", endpoint_down.c_str());
        }
    }

    virtual ~ZeroBiQueue() {
        if (zmq_close(forward_socket) == -1)
            LOG_ERR("zmq_close(): %s\n", zmq_strerror(errno));
        if (zmq_close(back_socket) == -1)
            LOG_ERR("zmq_close(): %s\n", zmq_strerror(errno));
    }

    /*
     * Insert data.
     * Return 0 if success, otherwise return -1
     */
	inline int Write(T* data) {
        if (is_upstream) {
            int rc = zmq_send(forward_socket, data, data->get_size(), 0);
            if (rc >= 0) {
                return 0;
            }
            else {
                LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
                return -1;
            }
        }
        else {
            int rc = zmq_send(back_socket, data, data->get_size(), 0);
            if (rc >= 0) {
                return 0;
            }
            else {
                LOG_ERR("ZeroQueue.write(): %s\n", zmq_strerror(errno));
                return -1;
            }
        }
    }

    /*
     * Extract data.
     * Return 0 if success, otherwise return -1
     */

    // non-blocking mode
	inline int Read(T* data) {
        if (is_upstream) {
            int rc = zmq_recv(back_socket, data, sizeof(T), ZMQ_DONTWAIT);
            if (rc >= 0) {
                return 0;
            }
            else if (rc == -1 && errno==EAGAIN) {
                return -1;
            }
            else {
                LOG_ERR("ZeroQueue.read(): %s\n", zmq_strerror(errno));
                return -1;
            }
        }
        else {
            int rc = zmq_recv(forward_socket, data, sizeof(T), ZMQ_DONTWAIT);
            if (rc >= 0) {
                return 0;
            }
            else if (rc == -1 && errno==EAGAIN) {
                return -1;
            }
            else {
                LOG_ERR("ZeroQueue.read(): %s\n", zmq_strerror(errno));
                return -1;
            }
        }
    }
};

#endif
