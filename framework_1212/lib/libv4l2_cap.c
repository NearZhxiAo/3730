#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <assert.h>

#include<linux/fb.h>
#include<arpa/inet.h>
#include <time.h>
#include "libv4l2_cap.h"

static   int      fd;
static   struct   v4l2_capability   cap;
struct v4l2_fmtdesc fmtdesc;
struct v4l2_format fmt,fmtack;
struct v4l2_streamparm setfps;  
struct v4l2_requestbuffers req;
struct v4l2_buffer buf;
enum v4l2_buf_type type;
unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3];

typedef struct
{
	char cfType[2];         /* ÎÄ¼þÀàÐÍ, ±ØÐëÎª "BM" (0x4D42)*/
	char cfSize[4];         /* ÎÄ¼þµÄ´óÐ¡(×Ö½Ú) */
	char cfReserved[4];     /* ±£Áô, ±ØÐëÎª 0 */
	char cfoffBits[4];      /* Î»Í¼ÕóÁÐÏà¶ÔÓÚÎÄ¼þÍ·µÄÆ«ÒÆÁ¿(×Ö½Ú)*/
}__attribute__((packed)) BITMAPFILEHEADER_B; 

typedef struct
{
	char ciSize[4];         /* size of BITMAPINFOHEADER */
	char ciWidth[4];        /* Î»Í¼¿í¶È(ÏñËØ) */
	char ciHeight[4];       /* Î»Í¼¸ß¶È(ÏñËØ) */
	char ciPlanes[2];       /* Ä¿±êÉè±¸µÄÎ»Æ½ÃæÊý, ±ØÐëÖÃÎª1 */
	char ciBitCount[2];     /* Ã¿¸öÏñËØµÄÎ»Êý, 1,4,8»ò24 */
	char ciCompress[4];     /* Î»Í¼ÕóÁÐµÄÑ¹Ëõ·½·¨,0=²»Ñ¹Ëõ */
	char ciSizeImage[4];    /* Í¼Ïñ´óÐ¡(×Ö½Ú) */
	char ciXPelsPerMeter[4];/* Ä¿±êÉè±¸Ë®Æ½Ã¿Ã×ÏñËØ¸öÊý */
	char ciYPelsPerMeter[4];/* Ä¿±êÉè±¸´¹Ö±Ã¿Ã×ÏñËØ¸öÊý */
	char ciClrUsed[4];      /* Î»Í¼Êµ¼ÊÊ¹ÓÃµÄÑÕÉ«±íµÄÑÕÉ«Êý */
	char ciClrImportant[4]; /* ÖØÒªÑÕÉ«Ë÷ÒýµÄ¸öÊý */
}__attribute__((packed)) BITMAPINFOHEADER_B;       /* Î»Í¼ÐÅÏ¢Í·½á¹¹ */

typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char rev;
}__attribute__((packed)) PIXEL;

BITMAPFILEHEADER_B FileHead;
BITMAPINFOHEADER_B InfoHead;

struct buffer
{
	void * start;
	unsigned int length;
} * buffers;

int init_v4l2(void)
{
	int i;
	int ret = 0;

	//opendev
	if ((fd = open(FILE_VIDEO, O_RDWR)) == -1) 
	{
		printf("Error opening V4L interface\n");
		return (FALSE);
	}

	//query cap
	if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) 
	{
		printf("Error opening device %s: unable to query device.\n",FILE_VIDEO);
		return (FALSE);
	}
	else
	{
		printf("driver:\t\t%s\n",cap.driver);
		printf("card:\t\t%s\n",cap.card);
		printf("bus_info:\t%s\n",cap.bus_info);
		printf("version:\t%d\n",cap.version);
		printf("capabilities:\t%x\n",cap.capabilities);

		if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) 
		{
			printf("Device %s: supports capture.\n",FILE_VIDEO);
		}

		if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) 
		{
			printf("Device %s: supports streaming.\n",FILE_VIDEO);
		}
	} 

	//emu all support fmt
	fmtdesc.index=0;
	fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("Support format:\n");
	while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
	{
		printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
		fmtdesc.index++;
	}

	//set fmt
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
	fmt.fmt.pix.height = IMAGEHEIGHT;
	fmt.fmt.pix.width = IMAGEWIDTH;
	//fmt.fmt.pix.bytesperline = IMAGEWIDTH*2;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
	{
		printf("Unable to set format.1\n");
		return FALSE;
	} 	
	if(ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
	{
		printf("Unable to get format\n");
		return FALSE;
	} 
	{
		printf("fmt.type:\t\t%d\n",fmt.type);
		printf("pix.pixelformat:\t%c%c%c%c\n",fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF,(fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
		printf("pix.height:\t\t%d\n",fmt.fmt.pix.height);
		printf("pix.width:\t\t%d\n",fmt.fmt.pix.width);
		printf("pix.field:\t\t%d\n",fmt.fmt.pix.field);
	}
	//set fps
	setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	setfps.parm.capture.timeperframe.numerator = 10;
	setfps.parm.capture.timeperframe.denominator = 10;

	printf("init %s \t[OK]\n",FILE_VIDEO);

	return TRUE;
}

int start_v4l2(void)
{
	unsigned int n_buffers;

	req.count=MAX_BUFFER_COUNT;
	req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory=V4L2_MEMORY_MMAP;
	if(ioctl(fd,VIDIOC_REQBUFS,&req)==-1)
	{
		printf("request for buffers error\n");
		return(FALSE);
	}

	//mmap for buffers
	buffers = malloc(req.count*sizeof (*buffers));
	if (!buffers) 
	{
		printf ("Out of memory\n");
		return(FALSE);
	}

	for (n_buffers = 0; n_buffers < req.count; n_buffers++) 
	{
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		//query buffers
		if (ioctl (fd, VIDIOC_QUERYBUF, &buf) == -1)
		{
			printf("query buffer error\n");
			return(FALSE);
		}

		buffers[n_buffers].length = buf.length;
		//map
		buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ |PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
		if (buffers[n_buffers].start == MAP_FAILED)
		{
			printf("buffer map error\n");
			return(FALSE);
		}
		//input the cache queue
		if(ioctl(fd,  VIDIOC_QBUF, &buf) == -1)
		{
			return(FALSE);
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl (fd, VIDIOC_STREAMON, &type);


	printf("grab yuyv OK\n");
	return(TRUE);
}

int getcapfd()
{
	return fd;
}


int close_v4l2(void)
{
	if(fd != -1) 
	{
		close(fd);
		return (TRUE);
	}
	return (FALSE);
}

int get_v4l2data(char **data_addr)
{
	memset(&buf, 0x00, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;			
	if(ioctl(fd, VIDIOC_DQBUF, &buf)==-1)
	{
		printf("VIDIOS_DQBUF is failure.\r\n");
		*data_addr = NULL;
		return -1;
	}
	else
	{
		*data_addr = buffers[buf.index].start;
		return 0;
	}
}

int put_v4l2data()
{
	if(ioctl(fd, VIDIOC_QBUF, &buf)==-1)
	{
		printf("VIDIOS_QBUF is failure.\r\n");
		return -1;
	}
	else
	{
		return 0;
	}

}
#if 0
int main(void)
{
	int i;
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;

	if(init_v4l2() == FALSE) 
	{
	
		return(FALSE);
	}

	if(v4l2_grab() == FALSE)
	{
		return(FALSE);
	}
	time_t tstart, tend;
	while(1)
	{

		memset(&buf, 0x00, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;			
		if(ioctl(fd, VIDIOC_DQBUF, &buf)==-1)
		{
			printf("VIDIOS_DQBUF is failure.\r\n");
			continue;	
		}
			
		assert(buf.index < MAX_BUFFER_COUNT);
			
		FILE *fp = fopen("data.yuv", "ab");
		assert(fp != NULL);
		fwrite(buffers[buf.index].start, 1, 720*480*2, fp);
		fclose(fp);
#if 0
		tstart = time(0);			
		yuyv_2_rgb888(buffers[buf.index].start);
		tend = time(0);
		printf("yuyv_2_rgb888=%f milsec\r\n",difftime(tend,tstart)*1000); 
		
		tstart = time(0);
		show_frame();
		tend = time(0);
		printf("show_frame=%f milsec\r\n",difftime(tend,tstart)*1000); 
#endif

		if(ioctl(fd, VIDIOC_QBUF, &buf)==-1)
		{
			printf("VIDIOS_QBUF is failure.\r\n");
			break;
		}
	
	}


//	munmap(fbp, screensize);
//	close(fp);

	close_v4l2();


	return(TRUE);
}
#endif


