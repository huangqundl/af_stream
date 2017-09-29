#ifndef __AFS_THREAD_ID_INCLUDED__
#define __AFS_THREAD_ID_INCLUDED__

#include <stdint.h>

namespace afs {

int64_t thr_id();
void set_thr_id(int64_t id);

}

#endif
