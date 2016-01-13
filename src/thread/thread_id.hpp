#ifndef __AFS_THREAD_ID_INCLUDED__
#define __AFS_THREAD_ID_INCLUDED__

#include <stdint.h>

uint32_t thr_id();
void set_thr_id(uint32_t id);

#endif
