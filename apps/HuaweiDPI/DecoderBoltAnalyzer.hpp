#ifndef __DecoderBoltComputeThread_hpp__
#define __DecoderBoltComputeThread_hpp__

#include <stdio.h>
#include <string>

#include "Packet.hpp"
#include "RawPacket.hpp"
#include "SubComputeThread.hpp"

class DecoderBoltComputeThread : public SubComputeThread<RawPacket, Packet::TotalField> {
    private:
        Packet p;
        uint64_t count;

        void finish();
        void process(RawPacket &);

    public:
        void initialize() {
            count = 0;
            SubComputeThread<RawPacket, Packet::TotalField>::initialize();
        }
        DecoderBoltComputeThread(std::string name, int id) :
            SubComputeThread<RawPacket, Packet::TotalField>(name, id) {
        }
};

#endif
