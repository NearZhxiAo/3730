#ifndef	_MISC_PRIVATE_H__ 
#define _MISC_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "libad.h"
#include "libgbshm.h"
#include "libunix.h"
#include "libsignal.h"
#include "libgpio.h"
#include "libgps.h"
#define MISC_POLL_COUNT_MAX	2
typedef struct _MISC_PRIVATE_HANDLE
{
     LIBGPIO_OBJ *pgpio_obj;
	 Gbshm_Handle gbshm_handle;		//必须的
	 LIBGPS_OBJ *pgps_obj;		//必须的
    struct pollfd polltimeout[MISC_POLL_COUNT_MAX];	//必须的
    int unix_fd;	//必须的
    int active_fdcount;	//必须的
	net_param localnet;
}MISC_PRIVATE_HANDLE;


succeed_type misc_unix(MISC_PRIVATE_HANDLE *phandle);

#ifdef __cplusplus
}
#endif
#endif  
