#include "ZeroQueue.hpp"

void* zmq_ctx = 0;

void ZeroQueueInit(const int io_threads) {
    if (zmq_ctx != NULL) {
        LOG_MSG("already_create\n");
        return;
    }

    zmq_ctx = zmq_ctx_new();
    if (zmq_ctx == 0)
        LOG_ERR("zmq_ctx_new(): %s\n", zmq_strerror(errno));
    if (zmq_ctx_set(zmq_ctx, ZMQ_IO_THREADS, io_threads) == -1)
        LOG_ERR("zmq_ctx_set(): %s\n", zmq_strerror(errno));

}

void ZeroQueueClean() {
    if (zmq_ctx_destroy(zmq_ctx) == -1)
        LOG_ERR("zmq_ctx_destroy(): %s\n", zmq_strerror(errno));
}
