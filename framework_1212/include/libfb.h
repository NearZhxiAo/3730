#ifndef	_LIB_DISPLAY_H 
#define	_LIB_DISPLAY_H

#include "system_debug.h"
#include "system_includes.h"
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "fcntl.h"
#include "unistd.h"
#include "errno.h"
#include "sys/mman.h"
#include "sys/ioctl.h"
#include "linux/fb.h"
#include "video/davincifb.h"
#include "asm/arch/davinci_resizer.h"
#ifdef __cplusplus
extern "C"{
#endif

#define NUM_DISPLAY_BUFS	3
#define UYVY_BLACK	0x10801080

typedef struct _LIBDISP_OBJ
{
	int display_fd;
	int resize_fd;
	char *virtDisplays[NUM_DISPLAY_BUFS];
	unsigned int displaySize;
}LIBDISP_OBJ;
LIBDISP_OBJ *DISP_INIT();
succeed_type DISP_FLIP(LIBDISP_OBJ *phandle, unsigned int *psrc, unsigned int index);
succeed_type DISP_DESTROY(LIBDISP_OBJ *phandle);
#ifdef __cplusplus
}
#endif
#endif  
