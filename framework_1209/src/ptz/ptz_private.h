#ifndef	_PTZ_PRIVATE_H__ 
#define _PTZ_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "libgbshm.h"
#include "libunix.h"

#define PTZ_POLL_COUNT_MAX	1
typedef struct _PTZ_PRIVATE_HANDLE
{
    Gbshm_Handle gbshm_handle;		
    struct pollfd polltimeout[PTZ_POLL_COUNT_MAX];
    int unix_fd;	
    int active_fdcount;
}PTZ_PRIVATE_HANDLE;

succeed_type ptz_unix(PTZ_PRIVATE_HANDLE *phandle);

#ifdef __cplusplus
}
#endif

#endif  
