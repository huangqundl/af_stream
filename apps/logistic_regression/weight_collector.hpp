#include <unordered_map>

#include "Collector.hpp"
#include "sum.hpp"

/**
 * The Colloctor of detect worker
 */
class WeightCollector : public Collector<Sum, Sum> {
private:

    void process(Sum& e) {
        LOG_MSG("sum %lf\n", e.sum);
    }

public:
    /**
     * Constructor
     * @param name the name of the Collector
     * @param num number of ComputeThreads
     */
    WeightCollector(std::string name, int num): Collector<Sum, Sum>(name, num) {
    }

    void initialize() {
        Collector<Sum, Sum>::initialize();
    }

    void finish() {
        Collector<Sum, Sum>::finish();
    }

};
