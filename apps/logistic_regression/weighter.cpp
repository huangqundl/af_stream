/*
 * Weighter.cpp
 * - Weighter thread - management on the flow level
 */

#include <inttypes.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include "Common/common.hpp"
#include "Config.hpp"
#include "weighter.hpp"
#include "Util.hpp"

/****************************************************************************
 * functions from class ComputeThreads
 ****************************************************************************/

void Weighter::timeoutHandler(Item& e) {
}

void Weighter::process(Item& e) {
    count++;

    if (time_win_start == 0) {
    }

    double sum = 0;
    for (size_t i=0; i<end-start+1; i++) {
        sum += e.feature[i] * weight[i];
    }

    Sum o(count, sum);
    emit(o);
}

void Weighter::finish() {
    LOG_MSG("Weighter %d (%d) processed %" PRIu64 "\n", subId, getpid(), count);

    ComputeThread<Item, Sum>::finish();
}

void Weighter::recovery_handler() {
}
    
/****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * Constructor
 */
Weighter::Weighter(std::string name, int id, Config* config) : ComputeThread<Item, Sum>(name, id) {
    
    // statistics
    count = 0;

    // Config *config = Config::getInstance();

    // print config parameters
    if (id == 0) {
        // print log here
    }

}
    
void Weighter::initialize() {
    start = 0;
    end = 1;
    weight = (double*) calloc (2, sizeof(double));
    weight[0] = 2;
    weight[1] = 1;

    ComputeThread<Item, Sum>::initialize();
}
