/*
 * weight.hpp
 * 
 *
 * */

#include "ComputeThread/ComputeThread.hpp"
#include "item.hpp"
#include "sum.hpp"

/**
 * Specific compute_thread: perform heavy key detection
 */
class Weighter : public ComputeThread<Item, Sum> {

private:
    // statistics
    uint64_t count;

    // to determine timewindow and call reset
    double time_win_start;

    /// range of the distributed weights
    unsigned long long int start, end;
    /// weight values
    double* weight;

    // functions from class ComputeThread
    void process(Item&);
    void finish();
    void recovery_handler();

    void timeoutHandler(Item& tuple);

public:
    // public functions

    /**
     * Constructor
     * @param name The unique name of the ComputeThread
     * @param id The ID of the ComputeThread
     */
    Weighter(std::string name, int id, Config* config);
    void initialize();
};
