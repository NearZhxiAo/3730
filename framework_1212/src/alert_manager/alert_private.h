#ifndef	_ALERT_PRIVATE_H__ 
#define _ALERT_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "libad.h"
#include "libgbshm.h"
#include "libunix.h"
#include "libgpio.h"

#define ALERT_POLL_COUNT_MAX	3
typedef struct _ALERT_PRIVATE_HANDLE
{
    LIBAD_OBJ *pad_obj;
    LIBGPIO_OBJ *pgpio_obj;
    LIBGBSHM_OBJ *pgbshm_obj;		//必须的
    struct pollfd polltimeout[ALERT_POLL_COUNT_MAX];	//必须的
    int unix_fd;	//必须的
    int active_fdcount;	//必须的
	unsigned int power_led;	//0: off , 1, green , 2, red
	unsigned int state_led; //0: off, 1, green, 2, red
	unsigned int G_led; //0: off, 1, green, 2, red
}ALERT_PRIVATE_HANDLE;


succeed_type alert_unix(ALERT_PRIVATE_HANDLE *phandle);
succeed_type alert_ad(ALERT_PRIVATE_HANDLE *phandle);
succeed_type alert_gpio(ALERT_PRIVATE_HANDLE *phandle);

#ifdef __cplusplus
}
#endif
#endif  
