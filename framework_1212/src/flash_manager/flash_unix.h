#ifndef	_FLASH_UNIX_H__
#define _FLASH_UNIX_H__
#include "flash_private.h"
#ifdef __cplusplus
extern "C"{
#endif
#if 1
static inline void set_saveparam_flag(FLASH_PRIVATE_HANDLE *phandle)
{
	pthread_mutex_lock(&phandle->mutex);
	phandle->opt_flag = FLASH_OPT_SAVE;
	pthread_mutex_unlock(&phandle->mutex);
}

static inline void get_saveparam_flag(FLASH_PRIVATE_HANDLE *phandle,FLASH_OPT_FLAG *opt_flag)
{
	pthread_mutex_lock(&phandle->mutex);
	*opt_flag = phandle->opt_flag;
	pthread_mutex_unlock(&phandle->mutex);
}

static inline void clear_saveparam_flag(FLASH_PRIVATE_HANDLE *phandle)
{
	pthread_mutex_lock(&phandle->mutex);
	phandle->opt_flag = FLASH_OPT_CLEAR;
	pthread_mutex_unlock(&phandle->mutex);
}

#endif
succeed_type flash_unix(FLASH_PRIVATE_HANDLE *phandle);
succeed_type flash_para_save(void *arg);
succeed_type flash_para_load(void *arg);

#ifdef __cplusplus
}
#endif
#endif
