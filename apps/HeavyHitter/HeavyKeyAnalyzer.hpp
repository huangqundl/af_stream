/*
 * HeavyKeyComputeThread.hpp
 * 
 * A common Anayzer for heavy hitter/changer detection
 * Use various plug-in detectors
 *
 * */

#ifndef __HeavyKeyComputeThread_hpp__
#define __HeavyKeyComputeThread_hpp__

#include "ComputeThread/ComputeThread.hpp"
#include "DetectCollector.hpp"
#include "Packet.hpp"
#include "BasicDetector.hpp"

/**
 * Specific compute_thread: perform heavy key detection
 */
class HeavyKeyComputeThread : public ComputeThread<Packet::BasicField, Dummy> {

private:

    /**
     * Basic class of various detect algorithms
     */
    BasicDetector* detector;

    /**
     * number of bits of keys
     */
    unsigned int key_length;

    /**
     * heavy key threshold
     */
    double thresh;

    /**
     * store the binary key
     */
    unsigned char key[16];

    // statistics
    /**
     * number of events processed, one event may encapsulate multiple packets
     */
    uint64_t count;
    /**
     * number of packets processed
     */
    uint64_t total_pkt;
    /**
     * number of bytes processed
     */
    uint64_t total_byte;

    // metrics for printing
    /**
     * timestamp
     */
    double last_ts;
    /**
     * number of printing times
     */
    int printCount;

    /**
     * Calling detection APIs of various detection algorithms
     */
    void wrap_detect();

    // functions from class ComputeThread
    void process(Packet::BasicField&);
    void finish();

    void timeoutHandler(Packet::BasicField& tuple);

public:
    // public functions
    /**
     * Constructor
     * @param name The unique name of the ComputeThread
     * @param id The ID of the ComputeThread
     */
    HeavyKeyComputeThread(std::string name, int id);
    void initialize();
};

#endif
