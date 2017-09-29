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

#ifndef __AFS_THREAD_ID_HPP_INCLUDED__
#define __AFS_THREAD_ID_HPP_INCLUDED__

#include "thread_id.hpp"

namespace afs {

//  unique variable for each thread
thread_local int64_t __thr_id = -1;

/**
 * @return continous thread IDs starting from 0 as opposed to pthread_self().
 */
int64_t thr_id() {
	return __thr_id;
}

void set_thr_id(int64_t id) {
	__thr_id = id;
}

}

#endif
