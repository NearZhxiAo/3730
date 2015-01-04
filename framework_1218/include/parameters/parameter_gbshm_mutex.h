#ifndef __PARAMETER_GBSHM_MUTEX_H__
#define __PARAMETER_GBSHM_MUTEX_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct gbshm_mutex
{
		pthread_mutex_t mutex;
}gbshm_mutex;




#ifdef __cplusplus
}
#endif



#endif
