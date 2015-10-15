#ifndef __ROLL_IO_CALLBACK_HPP_INCLUDED__
#define __ROLL_IO_CALLBACK_HPP_INCLUDED__

#include <vector>
#include "net/i_engine.hpp"

namespace afs {

class IOCallback {
protected:
    std::vector<zmq::i_engine*> engines;

public:
    void AddEngine(zmq::i_engine* e) {
        //LOG_MSG("add engine\n");
        engines.push_back(e);
    }
};

}

#endif
