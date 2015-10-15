#ifndef __ROLL_THREAD_ID_INCLUDED__
#define __ROLL_THREAD_ID_INCLUDED__

static int __thread __thr_id;

/**
 * @return continous thread IDs starting from 0 as opposed to pthread_self().
 */
inline int 
thr_id()
{
	return __thr_id;
}

inline void
set_thr_id(int id)
{
	__thr_id = id;
}

#endif
