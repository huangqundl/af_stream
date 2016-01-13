#ifndef __DetectCollector_hpp__
#define __DetectCollector_hpp__

#include "Collector.hpp"

/**
 * Dummy structure for SAND interface
 */
class Dummy {
public:
    /**
     * number of bytes of an instance
     */
    uint32_t size() { return 0; }
};

/**
 * The Colloctor of detect worker
 */
class DetectCollector : public Collector<Dummy, Dummy> {
public:
private:

    void process(Dummy& f) {
    }

public:
    /**
     * Constructor
     * @param name the name of the Collector
     * @param num number of ComputeThreads
     */
    DetectCollector(std::string name, int num): Collector<Dummy, Dummy>(name, num) {
    }

    void initialize() {
        Collector<Dummy, Dummy>::initialize();
    }

    void finish() {
        Collector<Dummy, Dummy>::finish();
    }

};

#endif
