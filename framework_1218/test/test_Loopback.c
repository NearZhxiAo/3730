#include "system_includes.h"
#include "system_debug.h"
#include "libcapture.h"
#include "libdisplay.h"
#include "libosd.h"
//#include "libjpeg_encode.h"
#include "libresize.h"
#include <getopt.h>
//#include "libH264Encode.h"
//#include <poll.h>
//#include <sys/time.h>
//#include <time.h>

#define MENU_MAX_NUM 5


//extern void H264_INTI(char *yuvdata, char *jpegdata);
//extern int H264ENC(int *len);
//extern void H264_INIT(char *inbuf,char *outbuf, int _width, int _height, int _framerate, int _bitrate, int _intra);

//extern void H264_ENCODE(int *len);

typedef struct env_args
{
	char *inbuffer;
	char *outbuffer;
	CMEM_AllocParams cmem_params;

}env_args;

typedef struct __videoState
{
	unsigned int bytesGenerated;
	unsigned int isKeyFrame;
}VideoState;

Capture_Handle Capphandle = NULL;
Display_Handle Disphandle = NULL;
env_args gbl_env;
//jpegenc 
FILE *fpjpeg;

//h264enc
FILE *fph264;

static int i_frame_count = 0;
static int p_frame_count = 0;

void cleanup(int sig)
{
	CAPTURE_DESTROY(Capphandle);
	DISPLAY_DESTROY(Disphandle);
}

int CodecInit(char *encoderName)
{
	if(encoderName != NULL)
	{
		gbl_env.cmem_params.type = CMEM_POOL;
		gbl_env.cmem_params.flags = CMEM_NONCACHED;
		gbl_env.cmem_params.alignment = 32;

		if (CMEM_init() < 0)
		{
			printf("CMEM init failed.\r\n");
			return -1;	
		}
		gbl_env.inbuffer  =CMEM_alloc(720*480*2, &gbl_env.cmem_params);
		gbl_env.outbuffer = CMEM_alloc(720*480*2, &gbl_env.cmem_params);

		if(gbl_env.inbuffer == NULL || gbl_env.outbuffer == NULL)
		{
			printf("CMEM_alloc error.\r\n");
			return -1;
		}
		else
		{
			printf("CMEM_alloc success.\r\n");
			printf("inbuffer virtaddr is %x phys addr is %x.\r\n", gbl_env.inbuffer,CMEM_getPhys(gbl_env.inbuffer));
			printf("outbuffer virtaddr is %x phys_addr is %x.\r\n", gbl_env.outbuffer,CMEM_getPhys(gbl_env.outbuffer));
		}

		if(strcmp(encoderName, "h264enc") == 0)
		{
			printf("H264 Encode started.\n");
			fph264 = fopen("out.mp4", "w");
			if (fph264 == NULL)
			{
				SYS_ERROR("open output file error.\r\n");
				return -1;
			}
			H264_INIT(gbl_env.inbuffer, gbl_env.outbuffer, 720, 480, 25,2000000 ,10 );
			//H264_INIT(gbl_env.inbuffer, gbl_env.outbuffer);
		}
		if(strcmp(encoderName, "jpegenc") == 0)
		{
			printf("Jpeg Encode started.\n");
			JPEG_INIT(gbl_env.inbuffer, gbl_env.outbuffer, 720, 480,80 );
		}
	}
	else
	{
		SYS_INFO("No need encode!!\r\n");
	}
	return 0;
}

int main(int argc, char **argv)
{
	char *short_options = "c:t:n:r:";
	struct option long_options[] = 
	{
		{"codec", optional_argument, NULL, 'c'},
		{"h264_time", optional_argument, NULL, 't'},
		{"jpeg_pic_num", optional_argument, NULL, 'n'},
		{"resize", optional_argument, NULL, 'r'},
		{0, 0, 0, 0},
	};
	CaptureBuffer	capbuf;
	DisplayBuffer	disbuf;
	CaptureAttr		capattr;
	DisplayAttr		disattr;
	struct pollfd poll_cap[2];
	pthread_t  scan_input_t;
	//	Resize_Handle rszHandle;
	//	struct timeval start;
	//	struct timeval end;
	ACTION Action;	

	int count = 0;
	int ret;
	DrawType drawtype = CHINESE;
	char *menu[5]={"设置", "分辨率", "帧率", "码率", "颜色"};
	int menuIndex;
	char *text;
	int x, y;
	int i;
	char *encoderName = NULL;
	int time_minute = 0;
	int jpeg_pic_num = 1;
	int jpeg_pic_count = 0;
	char jpeg_name[128];
	struct timeval time_v;
	unsigned int timetemp = 0x00;
	unsigned int start_timestamp = 0x00;
	int init_timetemp = 0;

	//h264
	int h264length;
	VideoState videostate;
	//jpeg
	int jpeglength;
	
	int IsResize = 0;
	ResizeAttr rszAttr;
	Resize_Handle rszHandle;
	unsigned int frame_count = 0;


	FILE *fp;
	FILE *fp_cap;


	x=360;y = 240;
	Action = IDLE;
	menuIndex = 0;

	int c;

	while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1) 
	{
		switch(c)
		{
			case 'c':
				encoderName = optarg;
				break;
			case 't':
				time_minute = atoi(optarg);
				break;
			case 'n':
				jpeg_pic_num = atoi(optarg);
				break;
			case 'r':
				IsResize = atoi(optarg);
				break;
			default:
				SYS_ERROR("%c is unknown options\r\n", c);
		}
	}

#if 0
	if(argc < 2)
	{
		SYS_ERROR("usage: ./%s time \n\r./%s char\n\r ./%s chinese.\r\n",
				__FILE__, __FILE__, __FILE__);
		return 0;
	}


	if(strcmp(argv[1], "time") == 0)
	{
		drawtype = TIME;
		//	text = NULL;
	}

	else if(strcmp(argv[1], "char") == 0)
	{
		//	text="ABCDEFGH";
		drawtype = CHAR;
	}
	else if(strcmp(argv[1], "chinese") == 0)
	{

		drawtype = CHINESE;
		text="西安电子科技大学";
	}
	else
	{
		SYS_ERROR("draw type is unknown\r\n");
		return -1;
	}

#endif

	memset(&capattr, 0x00, sizeof(struct CaptureAttr));
	capattr.bufNum    = 3;
	capattr.userAlloc = 0;
#if 1
	Capphandle = CAPTURE_INIT(&capattr);
	if(Capphandle == NULL)
	{
		SYS_ERROR("Capture_init failed ..\r\n");
		return -1;
	}

	memset(&disattr, 0x00, sizeof(struct DisplayAttr));
	
	if (IsResize == 1)
	{
		printf("resize...........\r\n");
		rszAttr.hWindowType = Resize_FilterType_LOWPASS;
		rszAttr.vWindowType = Resize_FilterType_LOWPASS;
		rszAttr.hFilterType = Resize_WindowType_BLACKMAN;
		rszAttr.vFilterType = Resize_WindowType_BLACKMAN;
		rszAttr.rszRate = 0xe;
		rszAttr.out_width = 400;
		rszAttr.out_height = 300;
		disattr.width 	= 400;
		disattr.height  = 300;
		rszHandle = RESIZE_INIT(&rszAttr);
		if(rszHandle == NULL)
		{
			SYS_ERROR("Resize_init error.\r\n");
			return -1;
		}
	}
	else
	{
		disattr.width 	= 720;
		disattr.height  = 576;
	}
	disattr.bufNum    = 3;
	disattr.userAlloc = 0;

	Disphandle = DISPLAY_INIT(&disattr);
	if(Disphandle == NULL)
	{
		SYS_ERROR("Display_init failed ..\r\n");
		return -1;
	}


	signal(SIGINT, cleanup);

	memset(poll_cap, 0x00, sizeof(poll_cap));

	poll_cap[0].fd = Capphandle->capture_fd;
	poll_cap[0].events = POLLIN;


	poll_cap[1].fd = 0;
	poll_cap[1].events = POLLIN;

	if (CodecInit(encoderName) != 0)
	{
		SYS_ERROR("CodeInit failed\r\n");
		return 0;
	}



#endif

	while(1)
	{
		//SYS_INFO("count %d.\r\n",count++);
		//SYS_INFO("before poll..\r\n");
		int ret;
		//SYS_INFO("after poll..ret %d revents %x\r\n", ret, poll_cap[0].revents);
		if ((ret = poll(poll_cap, 2, -1)) >= 0)
		{
			//如果是h264编码，编码time_minute时间

			if (poll_cap[0].revents)
			{
				//	SYS_INFO("before...\r\n");

				ret = CAPTURE_GET(Capphandle, &capbuf);	
				if(ret < 0)
				{
					SYS_ERROR("Capture_get failed ..\r\n");
					return -1;
				}

				ret = DISPLAY_GET(Disphandle, &disbuf);	
				if(ret < 0)
				{
					SYS_ERROR("Display_get failed ..\r\n");
					return -1;
				}

				switch(Action)
				{
					case IDLE:
						{
							for (i = 0; i < MENU_MAX_NUM; i++)
							{
								if (i == menuIndex)
								{
									set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 0xff00);
								}
								else
								{
									set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 0x4000);
								}
							}
							//四角叠加
							{
								set_osd(capbuf.virtaddr, TIME, NULL, 200, 0, 0xff80);
								set_osd(capbuf.virtaddr, TIME, NULL, 400, 0, 0x8080);
								set_osd(capbuf.virtaddr, TIME, NULL, 200, 460, 0x80c0);
								set_osd(capbuf.virtaddr, TIME, NULL, 400, 460, 0x80);
							}
							break;
						}
					case UP:
						{
							if(menuIndex == 0)
							{
								menuIndex = MENU_MAX_NUM - 1;
								for (i = 0; i < MENU_MAX_NUM; i++)
								{
									if (i == menuIndex)
									{
										set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 255);
									}
									else
									{
										set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 0);
									}
								}

							}
							else
							{
								menuIndex--;
								for (i = 0; i < MENU_MAX_NUM; i++)
								{
									if (i == menuIndex)
									{
										set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 255);
									}
									else
									{
										set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 0);
									}
								}
							}

							Action = IDLE;
							break;
						}
					case DOWN:
						{
							if(menuIndex == 4)
							{
								menuIndex = 0;
								for (i = 0; i < MENU_MAX_NUM; i++)
								{
									if (i == menuIndex)
									{
										set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 255);
									}
									else
									{
										set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 0);
									}
								}

							}
							else
							{
								menuIndex++;
								for (i = 0; i < MENU_MAX_NUM; i++)
								{
									if (i == menuIndex)
									{
										set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 255);
									}
									else
									{
										set_osd(capbuf.virtaddr, drawtype, menu[i], x, (y + (i+1)*20), 0);
									}
								}
							}

							Action = IDLE;
							break;
						}
					default:
						SYS_ERROR("The action is unknown!\r\n");
				}


				if(encoderName != NULL && frame_count % 2 == 0)
				//if(encoderName != NULL )
				{
#if 1
					if(strcmp(encoderName, "h264enc") == 0)
					{
						gettimeofday(&time_v, NULL);
						timetemp = time_v.tv_sec * 1000 + time_v.tv_usec / 1000;
						if( 0 == init_timetemp )
						{
							start_timestamp = timetemp;
							init_timetemp = 1;
						}
						else
						{
							if ( (timetemp - start_timestamp) / 1000 > (time_minute * 60))
							{
								encoderName = NULL;
								fclose(fph264);
								//	CMEM_free(gbl_env.inbuffer, &gbl_env.cmem_params);
								//	CMEM_free(gbl_env.outbuffer, &gbl_env.cmem_params);
								H264_DESTROY();
								SYS_INFO("H264 encode finish, i frame is %d, p frame is %d\r\n", i_frame_count, p_frame_count);
							}
							else
							{
								memcpy(gbl_env.inbuffer, capbuf.virtaddr, 720*480*2);
								H264_ENCODE(&videostate);
								fwrite(gbl_env.outbuffer, videostate.bytesGenerated, 1, fph264);
								if(videostate.isKeyFrame == 1)
								{
								//	SYS_INFO("p frame count is %d\r\n", p_frame_count);
									p_frame_count++;
								}
								else
								{
								//	SYS_INFO("i frame count is %d\r\n", i_frame_count);
									i_frame_count++;
								}
								//SYS_INFO("h264 encode len is %d.\r\n", videostate.bytesGenerated);
							}
						}

					}
#endif
					SYS_INFO("frame count is %d\r\n", frame_count);

					memset(gbl_env.outbuffer, 0x00, 720*480*2);
				}
				frame_count++;
				if (encoderName != NULL)
				{
					if(strcmp(encoderName, "jpegenc") == 0 && jpeg_pic_count < jpeg_pic_num)
					{
						memcpy(gbl_env.inbuffer, capbuf.virtaddr, 720*480*2);
						memset(jpeg_name, 0x00, 128);
						sprintf(jpeg_name, "out%d.jpg", jpeg_pic_count++);
						printf("Line is %d\r\n", __LINE__);

						JPEG_ENCODE(&jpeglength);
						printf("Line is %d\r\n", __LINE__);
						fpjpeg = fopen(jpeg_name, "wb");
						if (fpjpeg != NULL)
						{
							fwrite(gbl_env.outbuffer, jpeglength, 1, fpjpeg);
							fclose(fpjpeg);
							SYS_INFO("jpeg pic num is %s\r\n", jpeg_name);
						}
						else
						{
							SYS_ERROR("open %s failed.\r\n", jpeg_name);
						}
						printf("Line is %d\r\n", __LINE__);
						if (jpeg_pic_count == jpeg_pic_num)
						{
							encoderName = NULL;
							//		CMEM_free(gbl_env.inbuffer, &gbl_env.cmem_params);
							//		CMEM_free(gbl_env.outbuffer, &gbl_env.cmem_params);
							JPEG_DESTROY();
							SYS_INFO("Jpeg encode end..........\r\n");
						}
						memset(gbl_env.outbuffer, 0x00, 720*480*2);
					}
				}


				if (IsResize == 1)
				{

					RESIZE_EXEC(rszHandle, (unsigned long)capbuf.virtaddr, (unsigned long)disbuf.virtaddr);

#if 0
					fp = fopen("resize.yuv","ab+");
					char *buf = (char*)disbuf.virtaddr;
					fwrite(buf, 800 * 600 * 2, 1, fp);
					fclose(fp);
#endif
				}
				else
				{
					memcpy(disbuf.virtaddr, capbuf.virtaddr, 720*576*2);
					//memset(disbuf.virtaddr, 0x00, 800*600);
					//memset(disbuf.virtaddr+800*600, 0xff, 800*600);

				}

				ret = CAPTURE_PUT(Capphandle, &capbuf);
				if(ret < 0)
				{
					SYS_ERROR("Capture_get failed ..\r\n");
					return -1;
				}

				ret = DISPLAY_PUT(Disphandle, &disbuf);
				if(ret < 0)
				{
					SYS_ERROR("Display_get failed ..\r\n");
					return -1;
				}

			}
			if (poll_cap[1].revents)
			{
				char buf[10];
				int n;

				n = read(poll_cap[1].fd, buf, 10);
				if (n > 0)
				{
					buf[n+1] = '\0';
					printf("you input is %s\r\n", buf);
					if(atoi(buf) == 1)
					{
						Action = DOWN;
					}
					else if (atoi(buf) == 2)
					{
						Action = UP;
					}
					else
					{
						SYS_ERROR("input error!!r\n");
					}
				}
				else
				{
					printf("error\r\n");
				}
			}
		}
	}
	return 0;
}
