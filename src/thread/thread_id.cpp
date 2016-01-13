#include "thread_id.hpp"

thread_local uint32_t __thr_id = -1;

/**
 * @return continous thread IDs starting from 0 as opposed to pthread_self().
 */
uint32_t thr_id()
{
	return __thr_id;
}

void set_thr_id(uint32_t id)
{
	__thr_id = id;
}
