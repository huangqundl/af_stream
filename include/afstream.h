/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   *************************************************************************
   NOTE to contributors. This file comprises the principal public contract
   for AF-Stream API users. Any change to this file
   supplied in a stable release SHOULD not break existing applications.
   In practice this means that the value of constants must not change, and
   that old values may not be reused for new constants.
   *************************************************************************
*/

#ifndef __AFS_H_INCLUDED__
#define __AFS_H_INCLUDED__

/*  Version macros for compile-time API version detection                     */
#define AFS_VERSION_MAJOR 1
#define AFS_VERSION_MINOR 0
#define AFS_VERSION_PATCH 0

#define AFS_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define AFS_VERSION \
    AFS_MAKE_VERSION(AFS_VERSION_MAJOR, AFS_VERSION_MINOR, AFS_VERSION_PATCH)

#include <stdint.h>

/*******************************************************************
 *
 * Third party
 *
 ******************************************************************/

//#include "../third_party/iniparser/src/iniparser.h"

/*******************************************************************
 *
 * Utilities
 *
 ******************************************************************/

#include "../src/config.hpp"
#include "../src/util.hpp"

/*******************************************************************
 *
 * Threads
 *
 ******************************************************************/

#include "../src/thread/worker.hpp"
#include "../src/thread/up_thread_trace.hpp"
//#include "../src/thread/up_thread_trace_afs.hpp"
#include "../src/thread/router_rr.hpp"
#include "../src/thread/down_thread_net.hpp"
#include "../src/thread/compute_thread.hpp"

/*
 * 
 * Operators
 *
 */
#include "../src/fault_tolerance/operator_tracker.hpp"
#include "../src/operator/array_max.hpp"
#include "../src/operator/hash.hpp"

#include "../src/raw_item.hpp"

#endif

