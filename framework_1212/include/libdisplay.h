#ifndef	_LIB_DISPLAY_H 
#define	_LIB_DISPLAY_H

#include "system_debug.h"
#include "system_includes.h"
#include "libcmem.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "linux/videodev2.h"
#include "linux/videodev.h"
#include "linux/fb.h"

#ifdef __cplusplus
extern "C"{
#endif

#define NUM_DISPLAY_BUFS	3

static CMEM_AllocParams dis_cmem_param = {CMEM_POOL, CMEM_NONCACHED, 32};

typedef struct DisplayAttr
{
	int bufNum;
	int userAlloc;
	int width;
	int height;
}DisplayAttr;

typedef struct DisplayBuffer 
{
    void			*virtaddr;
    unsigned long	physaddr;
    int				length;
	int				index;
}DisplayBuffer;

typedef struct __Display_Object
{
	int display_fd;
	int resize_fd;
	DisplayBuffer *disBufsPtr;
	struct v4l2_buffer *v4l2buf;
	char *virtDisplays[NUM_DISPLAY_BUFS];
	unsigned int displaySize;
	unsigned int disBufsNum;
	int userAlloc;
}Display_Object;

typedef Display_Object* Display_Handle;

Display_Handle DISPLAY_INIT(DisplayAttr *attr);
int DISPLAY_GET(Display_Handle phandle, DisplayBuffer *buffer);
int DISPLAY_PUT(Display_Handle phandle, DisplayBuffer *buffer);
int DISPLAY_DESTROY(Display_Handle phandle);
#ifdef __cplusplus
}
#endif
#endif  
