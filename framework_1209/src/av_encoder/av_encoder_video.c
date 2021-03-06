#include "system_debug.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <asm/types.h>
#include <time.h>

#include "av_encoder_private.h"

struct buffer_private *buffers = NULL;
#define MIN_BUFFERS		2


extern AV_ENCODER_PRIVATE_HANDLE global_av_encoder_private_handle;

/******************************************************************************/

int get_buffer_index(void *ptr)
{
	int index =0;
	for(;index < MIN_BUFFERS;index++){
		if(buffers[index].start == ptr){
			//printf("found buffer index:%d,buffers[index].start:%d,ptr:%x,offset:%x\n",index,buffers[index].start,ptr,buffers[index].offset);
			return index;
		}
	}
	printf("not found buffer index\n");
	return -1;
}

int video_buf_deal(int fdCapture, int rsz_fd, FILE *fp)
{	
	//struct v4l2_buffer buf;
	//unsigned int *pbuffer;
	int index = 0;
	CaptureBuffer pbuffer;       //added 2012-1-6	
//	pbuffer = malloc(720*480*2);

#if 0	
	CAPTURE_GET(global_av_encoder_private_handle.pcapture_obj, pbuffer, &buf);
	av_encoder_vidEncRunMain(&global_av_encoder_private_handle,fp);
	CAPTURE_PUT(global_av_encoder_private_handle.pcapture_obj, &buf);
	return 0;
#endif


	CAPTURE_GET(global_av_encoder_private_handle.pcapture_obj, &pbuffer);
	av_encoder_vidEncRunMain(&global_av_encoder_private_handle,fp);
	CAPTURE_PUT(global_av_encoder_private_handle.pcapture_obj, &pbuffer);
}
