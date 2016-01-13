/*
 * DPIBoltCollector.cpp
 * - DPIBoltCollector thread - management on the global level
 */

#include "common.hpp"
#include "DPIBoltCollector.hpp"

/****************************************************************************
 * Private functions
 ****************************************************************************/
void DPIBoltCollector::process(MainStateEvent& e) {
    LOG_DEBUG("Collector processing\n");
}

/****************************************************************************
 * Public functions
 ****************************************************************************/
DPIBoltCollector::DPIBoltCollector(std::string name, int num): Collector<MainStateEvent, Dummy>(name, num) {
}
