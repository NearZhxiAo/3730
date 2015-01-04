#include "system_includes.h"
#include "system_debug.h"
#include "libcapture.h"
#include "libdisplay.h"
#include "libresize.h"
#include <poll.h>
#include <sys/time.h>
#include <time.h>

Capture_Handle Capphandle = NULL;
Display_Handle Disphandle = NULL;

void cleanup(int sig)
{
	Capture_destroy(Capphandle);
	Display_destroy(Disphandle);
}


#if 0
typedef struct env_args {
	char *inbuffer;
	char *outbuffer;
	char *yuv;
	char *jpeg;
	int saveFlag;
	CMEM_AllocParams cmem_params;
}env_args;

env_args gbl_env;

int env_init()
{
	gbl_env.cmem_params.type = CMEM_POOL;
	gbl_env.cmem_params.flags = CMEM_NONCACHED;
	gbl_env.cmem_params.alignment = 0;

	CMEM_init();
	gbl_env.inbuffer  = CMEM_alloc(IMAGEWIDTH*IMAGEHEIGHT*2, &gbl_env.cmem_params);
	gbl_env.outbuffer = CMEM_alloc(IMAGEWIDTH*IMAGEHEIGHT*2, &gbl_env.cmem_params);
	if(gbl_env.inbuffer == NULL || gbl_env.outbuffer == NULL)
	{
		printf("CMEM_alloc error.\r\n");
		return -1;
	}
	else
	{
		printf("CMEM_alloc success.\r\n");
	}

	Jpeg_init(gbl_env.inbuffer, gbl_env.outbuffer);

	gbl_env.yuv  = malloc(IMAGEWIDTH*IMAGEHEIGHT*2);
	gbl_env.jpeg = malloc(IMAGEWIDTH*IMAGEHEIGHT*2);

	assert(gbl_env.yuv != NULL);
	assert(gbl_env.jpeg != NULL);



	init_v4l2();
	start_v4l2();

//	uartCreate(&gbl_env.saveFlag);
}

#endif

int main(int argc, char **argv)
{
	CaptureBuffer	capbuf;
	DisplayBuffer	disbuf;
	CaptureAttr		capattr;
	DisplayAttr		disattr;
	struct v4l2_buffer v4l2buf;
	struct pollfd poll_cap[1];
	Resize_Handle rszHandle;
	struct timeval start;
	struct timeval end;
	ResizeAttr rszAttr;

	rszAttr.hWindowType = Resize_FilterType_LOWPASS;
	rszAttr.vWindowType = Resize_FilterType_LOWPASS;
	rszAttr.hFilterType = Resize_WindowType_BLACKMAN;
	rszAttr.vFilterType = Resize_WindowType_BLACKMAN;
	rszAttr.rszRate = 0xe;
	rszAttr.out_width = 352;
	rszAttr.out_height = 288;


	int count = 0;
	int ret;

	memset(&capattr, 0x00, sizeof(struct CaptureAttr));
	capattr.bufNum    = 3;
	capattr.userAlloc = 0;
#if 1
	Capphandle = Capture_init(&capattr);
	if(Capphandle == NULL)
	{
		SYS_ERROR("Capture_init failed ..\r\n");
		return -1;
	}

	memset(&disattr, 0x00, sizeof(struct DisplayAttr));
	disattr.bufNum    = 3;
	disattr.userAlloc = 0;
	disattr.width 	= 720;
	disattr.height  = 480;
	Disphandle = Display_init(&disattr);
	if(Disphandle == NULL)
	{
		SYS_ERROR("Display_init failed ..\r\n");
		return -1;
	}

	signal(SIGINT, cleanup);

#endif
	memset(poll_cap, 0x00, sizeof(struct pollfd));
	poll_cap[0].fd = Capphandle->capture_fd;
	//poll_cap[0].fd = fd;
	poll_cap[0].events = POLLIN;

#if 0
	int fd = open("data.txt",O_RDWR );
	if(fd <=0 )
	{
		printf("open error.\r\n");
		return -1;
	}

	SYS_INFO("POLLIN %x",POLLIN);
	SYS_INFO("POLLOUT %x",POLLOUT);
	SYS_INFO("POLLPRI %x",POLLPRI);
	SYS_INFO("POLLERR %x",POLLERR);

#endif
	rszHandle = Resize_init(&rszAttr);
	if(rszHandle == NULL)
	{
		SYS_ERROR("Resize_init error.\r\n");
		return -1;
	}

	while(1)
	{
		//SYS_INFO("count %d.\r\n",count++);
		//SYS_INFO("before poll..\r\n");
		int ret;
		if((ret = poll(poll_cap, 1, -1)) >= 0)
		{
			//SYS_INFO("after poll..ret %d revents %x\r\n", ret, poll_cap[0].revents);
			if(poll_cap[0].revents)
			{
				ret = Capture_get(Capphandle, &capbuf);	
				if(ret < 0)
				{
					SYS_ERROR("Capture_get failed ..\r\n");
					return -1;
				}

				ret = Display_get(Disphandle, &disbuf);	
				if(ret < 0)
				{
					SYS_ERROR("Display_get failed ..\r\n");
					return -1;
				}

				//SYS_INFO("capbuf physaddr is %x,disbuf phys addr is %x\r\n",capbuf.physaddr,disbuf.physaddr);
				//	gettimeofday(&start,NULL);
				//memcpy(disbuf.virtaddr, capbuf.virtaddr, 720*480*2);
				//Resize_exec(rszHandle, capbuf.physaddr, disbuf.physaddr);
				Resize_exec(rszHandle, (unsigned long)capbuf.virtaddr, (unsigned long)disbuf.virtaddr);
				//	gettimeofday(&end,NULL);
				//	SYS_INFO("%dms", (end.tv_sec*1000 + end.tv_usec/1000) - (start.tv_sec*1000 + start.tv_usec/1000));
				//	SYS_INFO("start: sec%d, usec %d", start.tv_sec, start.tv_usec);
				//	SYS_INFO("end: sec%d, usec %d", end.tv_sec, end.tv_usec);
				//memcpy(disbuf.virtaddr, capbuf.virtaddr, 720*480*2);

				ret = Capture_put(Capphandle, &capbuf);
				if(ret < 0)
				{
					SYS_ERROR("Capture_get failed ..\r\n");
					return -1;
				}

				ret = Display_put(Disphandle, &disbuf);
				if(ret < 0)
				{
					SYS_ERROR("Display_get failed ..\r\n");
					return -1;
				}
			}
			else
			{
				SYS_ERROR("poll capture event timeout.\r\n");
			}
		}
		else
		{
			SYS_ERROR("poll error.\r\n");
		}
	}
	return 0;
}
