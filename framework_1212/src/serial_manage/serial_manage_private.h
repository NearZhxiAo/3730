#ifndef	_SERIAL_MANAGE_PRIVATE_H__ 
#define _SERIAL_MANAGE_PRIVATE_H__ 
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "libgbshm.h"
#include "libunix.h"

#define SERIAL_MANAGE_POLL_COUNT_MAX	2
#define SERIAL_DEVICE_FILE  /dev/serial


typedef enum _SERIAL_MANAGE_DATA_TYPE
{
	DATA_START,
	DATA_INFRARED,
	DATA_MEASURE,
	DATA_GPS,
	DATA_MAGNET,
	DATA_POWER_MANAGE,
	DATA_OLED1_CTRL,
	DATA_OLED2_CTRL,
	DATA_END,
}SERIAL_MANAGE_DATA_TYPE;


typedef struct _SERIAL_MANAGE_PRIVATE_HANDLE
{
    Gbshm_Handle gbshm_handle;		
    struct pollfd polltimeout[SERIAL_MANAGE_POLL_COUNT_MAX];
    int unix_fd;	
	int serial_fd;
    int active_fdcount;
}SERIAL_MANAGE_PRIVATE_HANDLE;

succeed_type serial_manage_unix(SERIAL_MANAGE_PRIVATE_HANDLE *phandle);
int serial_init();

void *serial_main_func(void *args);

#ifdef __cplusplus
}
#endif

#endif  
