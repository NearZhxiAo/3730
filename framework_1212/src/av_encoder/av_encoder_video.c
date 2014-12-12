#include "av_encoder_private.h"

struct buffer_private *buffers = NULL;
#define MIN_BUFFERS		2
#define DISPLAY_SIZE 720 * 576 * 2


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

succeed_type video_buf_deal(AV_ENCODER_PRIVATE_HANDLE *phandle)
{	
	//struct v4l2_buffer buf;
	//unsigned int *pbuffer;
	int index = 0;
	CaptureBuffer pbuffer;       //added 2012-1-6	
	succeed_type ret;
//	pbuffer = malloc(720*480*2);

#if 0	
	CAPTURE_GET(global_av_encoder_private_handle.pcapture_obj, pbuffer, &buf);
	av_encoder_vidEncRunMain(&global_av_encoder_private_handle,fp);
	CAPTURE_PUT(global_av_encoder_private_handle.pcapture_obj, &buf);
	return 0;
#endif

	if(phandle->cam_input_switch == 1)
	{
		//do switch 
		
	}


	ret = CAPTURE_GET(phandle->capHandle, &phandle->capBuf);
	if ( ret == succeed_type_failed )
	{
		SYS_ERROR("Capture_get failed!\r\n");
		return succeed_type_failed;
	}

	ret = DISPLAY_GET(phandle->dispHandle, &phandle->disBuf);
	if ( ret == succeed_type_failed )
	{
		SYS_ERROR("Display_get failed!\r\n");
		return succeed_type_failed;
	}



	if(phandle->resize_enable == 1)
	{
		RESIZE_EXEC(phandle->rszHandle, (unsigned long)phandle->capBuf.virtaddr, (unsigned long)phandle->disBuf.virtaddr);
	}
	else
	{
		memcpy(phandle->disBuf.virtaddr, phandle->capBuf.virtaddr, DISPLAY_SIZE);
	}


	ret = CAPTURE_PUT(phandle->capHandle, &phandle->capBuf);
	if ( ret == succeed_type_failed )
	{
		SYS_ERROR("Capture_get failed!\r\n");
		return succeed_type_failed;
	}

	ret = DISPLAY_PUT(phandle->dispHandle, &phandle->disBuf);
	if ( ret == succeed_type_failed )
	{
		SYS_ERROR("Display_get failed!\r\n");
		return succeed_type_failed;
	}

}
