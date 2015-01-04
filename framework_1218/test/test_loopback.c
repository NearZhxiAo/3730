#include "system_includes.h"
#include "system_debug.h"
#include "libv4l2_cap.h"
#include "libdisplay.h"
#include "libosd.h"
#include <assert.h>
#include <poll.h>

Display_Handle Disphandle = NULL;

void cleanup(int sig)
{
	close_v4l2();
	Display_destroy(Disphandle);
}


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
#if 0
	gbl_env.cmem_params.type = CMEM_POOL;
	gbl_env.cmem_params.flags = CMEM_NONCACHED;
	gbl_env.cmem_params.alignment = 0;

//	CMEM_init();
//	gbl_env.inbuffer  = CMEM_alloc(IMAGEWIDTH*IMAGEHEIGHT*2, &gbl_env.cmem_params);
//	gbl_env.outbuffer = CMEM_alloc(IMAGEWIDTH*IMAGEHEIGHT*2, &gbl_env.cmem_params);
	if(gbl_env.inbuffer == NULL || gbl_env.outbuffer == NULL)
	{
		printf("CMEM_alloc error.\r\n");
		return -1;
	}
	else
	{
		printf("CMEM_alloc success.\r\n");
	}


	gbl_env.yuv  = malloc(IMAGEWIDTH*IMAGEHEIGHT*2);
	gbl_env.jpeg = malloc(IMAGEWIDTH*IMAGEHEIGHT*2);

	assert(gbl_env.yuv != NULL);
	assert(gbl_env.jpeg != NULL);
#endif


	init_v4l2();
	start_v4l2();

//	uartCreate(&gbl_env.saveFlag);
}



int main(int argc, char **argv)
{
	DisplayBuffer	disbuf;
	DisplayAttr		disattr;
	struct v4l2_buffer v4l2buf;
	char *data_addr;
	DrawType drawtype;
	struct pollfd poll_cap[1];
	int ret;
	char *text;
	int x, y;
	
	x=360;y = 240;

#if 0

	char *text="中华人民共和国";
	drawtype = CHINESE;
	printf("Capture Jpeg Encode demo started.\n");
#endif



	if(argc < 2)
	{
		SYS_ERROR("usage: ./%s time \n\r./%s char\n\r ./%s chinese.\r\n",
					__FILE__, __FILE__, __FILE__);
		return 0;
	}

#if 1
	if(strcmp(argv[1], "time") == 0)
	{
		drawtype = TIME;
		text = NULL;
	}

	else if(strcmp(argv[1], "char") == 0)
	{
		text="ABCDEFGH";
		drawtype = CHAR;
	}
	else if(strcmp(argv[1], "chinese") == 0)
	{
		text="西安电子科技";
		drawtype = CHINESE;
	}
	else
	{
		SYS_ERROR("%s is unknown\r\n", argv[1]);
		return -1;
	}
#endif
#if 0
	if(	env_init() < 0)
	{
		printf("env_init failed\r\n");
		return -1;
	}
#endif
#if 0
	memset(poll_cap, 0x00, sizeof(struct pollfd));

	int fd = open("data.txt",O_RDWR );
	if(fd <=0 )
	{
		printf("open error.\r\n");
		return -1;
	}
	poll_cap[0].fd = getcapfd();
	//poll_cap[0].fd = fd;
	poll_cap[0].events = POLLIN;
#endif

	init_v4l2();
	start_v4l2();





	disattr.bufNum    = 3;
	disattr.userAlloc = 0;
	Disphandle = Display_init(&disattr);
	if(Disphandle == NULL)
	{
		SYS_ERROR("Display_init failed ..\r\n");
		return -1;
	}

	signal(SIGINT, cleanup);


	int n = 0;
	while(1)
	{
		//	if((ret = poll(poll_cap, 1, -1)) >= 0)
		{
			//SYS_INFO("after poll..ret %d revents %x\r\n", ret, poll_cap[0].revents);
			//		if(poll_cap[0].revents)
			{

				ret = get_v4l2data(&data_addr);
				if(ret == -1)
				{
					printf("get_v4l2data failed,continue...\r\n");
					return -1;
				}

				ret = Display_get(Disphandle, &disbuf);	
				if(ret < 0)
				{
					SYS_ERROR("Display_get failed ..\r\n");
					return -1;
				}
				set_osd(data_addr, drawtype, text, x, y);

				memcpy(disbuf.virtaddr, data_addr, 720*480*2);

				ret = put_v4l2data();
				if(ret == -1)
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

			//	else
			{
				//		SYS_ERROR("poll capture event timeout.\r\n");
			}
		}
		//		else
		{
			//			SYS_ERROR("poll error.\r\n");
		}
		//	n++;
	}
	//	cleanup(0);
	return 0;
}
