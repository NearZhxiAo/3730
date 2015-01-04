#ifndef	_BOA_PRIVATE_H__ 
#define _BOA_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "libad.h"
#include "libgbshm.h"
#include "libunix.h"
#include "parameter.h"
#include "intercmd.h"
#include "libsignal.h"

#define BOA_POLL_COUNT_MAX	2
typedef struct _BOA_PRIVATE_HANDLE
{
    Gbshm_Handle gbshm_handle;		//必须的
    struct pollfd polltimeout[BOA_POLL_COUNT_MAX];	//必须的
    int unix_fd;	//必须的
    int active_fdcount;	//必须的
}BOA_PRIVATE_HANDLE;

extern BOA_PRIVATE_HANDLE global_boa_private_handle;
succeed_type boa_unix(BOA_PRIVATE_HANDLE *phandle);

#ifdef __cplusplus
}
#endif
#endif  
