#ifndef	_LIB_CAPTURE_H 
#define	_LIB_CAPTURE_H

#include "system_debug.h"
#include "system_includes.h"
#include "libcmem.h"

#include <asm/types.h>
#include <linux/videodev2.h>
//#include <media/tvp5146.h>
//#include <media/davinci/davinci_vpfe.h>

#ifdef __cplusplus
extern "C"{
#endif

#define WIDTH 720
#define HEIGHT 576

#define VIDEO_DEVICE "/dev/video0"

#define CAP_BUF_NUM	3
#define    TVP5146_AMUX_COMPOSITE  0
#define    TVP5146_AMUX_SVIDEO     1
static CMEM_AllocParams cap_cmem_param = {CMEM_POOL, CMEM_NONCACHED, 32};

typedef struct CaptureAttr
{
		int bufNum;
		int userAlloc;
}CaptureAttr;

typedef struct CaptureBuffer 
{
		void			*virtaddr;
		unsigned long	physaddr;
		int				length;
		int				index;
}CaptureBuffer;

typedef struct __Capture_Object
{
		int capture_fd;
		CaptureBuffer *capBufsPtr;
		struct v4l2_buffer *v4l2buf;
		unsigned int capBufsNum;
		int userAlloc;
}Capture_Object;

typedef Capture_Object* Capture_Handle;

Capture_Handle CAPTURE_INIT(CaptureAttr *attr);
succeed_type CAPTURE_GET(Capture_Handle phandle,CaptureBuffer *buffer);
succeed_type CAPTURE_PUT(Capture_Handle phandle,CaptureBuffer *buffer);
succeed_type CAPTURE_DESTROY(Capture_Handle phandle);

#ifdef __cplusplus
}
#endif
#endif  
