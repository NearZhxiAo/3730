#include "av_encoder_private.h"
#include <assert.h>

struct buffer_private *buffers = NULL;
#define MIN_BUFFERS		2
#define DISPLAY_SIZE 720 * 576 * 2


static int i_frame_count = 0;
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

void *video_enc(void *args)
{

	AV_ENCODER_PRIVATE_HANDLE *phandle = (AV_ENCODER_PRIVATE_HANDLE *)args;
	int index = 0;
	int frame_count = 0;
	LIBVSSHM_I_P_FLAG i_p_flag;
	int sem_value;
	videoStatus video_status;
	int video_seq;
	static int error_num =0;
	int video_frame_seq = -1;

	assert(phandle != NULL);

	while(frame_count <= 600 ) //150
	{
		//			if (phandle->current_state == AVENC_ENC )
		//do enc 
		//i_frame_count++;
		memset(phandle->videoinbuffer, 0x00, CMEM_SIZE(phandle->width, phandle->height));
		memcpy(phandle->videoinbuffer, phandle->sync_buffer + CMEM_SIZE(phandle->width, phandle->height) * index  , CMEM_SIZE(phandle->width, phandle->height));

		index = (++index) % 10;

		H264_ENCODE(&video_status);
		SYS_INFO("frame count is %d\r\n", frame_count++);
#if 1
		if(video_status.isKeyFrame == IVIDEO_I_FRAME)
		{
			i_p_flag = I_P_FLAG_I;
		}
		else
		{
			i_p_flag = I_P_FLAG_P;
		}
		if (VSSHM_GET_SEQ(phandle->pvsshm_obj, i_p_flag, &video_frame_seq, &sem_value) == succeed_type_failed)
		{
			SYS_ERROR("the function VSSHM_GET_SEQ() failed.\r\n");
			return succeed_type_failed;
		} 
		/*»ñµÃµÄÐÅºÅÁ¿*/
		if (sem_value != 0)
		{
			error_num++;
			//SYS_ERROR("\r\n-------------error number = %d sem_valueue = %d--------    -------------\r\n"\
			,error_num, sem_value);
		}
		/*µ÷ÓÃPUTº¯Êý*/
		if (VSSHM_PUT(phandle->pvsshm_obj, video_frame_seq, FRAME_TYPE_VIDEO_H264, phandle->videooutbuffer, video_status.bytesGenerated) == succeed_type_failed)
		{
			SYS_ERROR("the function VSSHM_PUT() failed.phandle->pvsshm_obj = 0x%x\r\n",phandle->pvsshm_obj);
			return succeed_type_failed;
		}
#endif
#if 1

		UNIX_CMD_STRUCT cmd;
		avenc_to_storage_video_buffer_new_msg bufnewmsg;

		bufnewmsg.buffer_id = video_frame_seq;
		cmd.cmd = PARACMD_AVENC_AVENC_TO_STORAGE_VIDEO_BUFFER_NEW_MSG;
		cmd.cmdlen = sizeof(avenc_to_storage_video_buffer_new_msg);
		memcpy(&cmd.cmdinfo, &bufnewmsg, sizeof(bufnewmsg));
		UNIX_SEND(phandle->unix_fd, &cmd);
		//SYS_INFO("in avenc  i_frame_count is %d\r\n", i_frame_count++);
		
		//usleep(1);

#endif
	}
	//if (i_frame_count >= 450)
	{
		UNIX_CMD_STRUCT cmd;
		cmd.cmd = PARACMD_AVENC_AVENC_TO_STORAGE_STOP_STORAGE;
		UNIX_SEND(phandle->unix_fd, &cmd);				

	}

}

succeed_type video_buf_deal(AV_ENCODER_PRIVATE_HANDLE *phandle)
{	
	succeed_type ret;
	int jpeglength;
	int h264length;
	int jpeg_pic_seq = -1;
	int video_frame_seq = -1;
	int video_seq;
	LIBVSSHM_I_P_FLAG i_p_flag;
	int sem_value;
	static int error_num =0;
	static	int frame_count = 0;
	static int index = 0;
	static int index_frame_count = 0;
	videoStatus video_status;

	if(phandle->cam_input_switch == 1)
	{
		//do switch 
		if(phandle->cam_input_type == INPUT_CCD)
		{	//do switch input from CCD to Infrared

		}
		else
		{
			//do swithc input from Infrared to CCD
		}

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

	// is jpeg pic
	//	phandle->Is_jpeg_pic = 1;
	if (phandle->Is_jpeg_pic)
	{
		SYS_INFO("in jpg encode.........\r\n");
		memcpy(phandle->jpeginbuffer, phandle->capBuf.virtaddr, CMEM_SIZE(phandle->width, phandle->height));

		JPEG_ENCODE(&jpeglength);

		if (ASSHM_GET_SEQ(phandle->passhm_obj, &jpeg_pic_seq) == succeed_type_failed)
		{
			SYS_ERROR("ASSHM_GET_SEQ failed!\r\n");
			assert(0);
		}
		if (ASSHM_PUT(phandle->passhm_obj, jpeg_pic_seq, phandle->jpegoutbuffer, jpeglength)== succeed_type_failed)
		{
			SYS_ERROR("ASSHM_GET_SEQ failed!\r\n");
			assert(0);
		}
		UNIX_CMD_STRUCT cmd;
		avenc_to_storage_jpeg_buffer_new_msg bufnewmsg;
		bufnewmsg.buffer_id = jpeg_pic_seq;
		SYS_INFO("before unix send....jpeg pic sequence is %d.....\r\n", jpeg_pic_seq);

		cmd.cmd = PARACMD_AVENC_AVENC_TO_STORAGE_JPEG_BUFFER_NEW_MSG;
		cmd.cmdlen = sizeof(avenc_to_storage_jpeg_buffer_new_msg);
		memcpy(&cmd.cmdinfo, &bufnewmsg, sizeof(bufnewmsg));
		UNIX_SEND(phandle->unix_fd, &cmd);
		SYS_INFO("after unix send.........\r\n");


		phandle->Is_jpeg_pic = 0;
	}

	if (phandle->current_state == AVENC_DEC)
	{
		//do dec
		//1.d:wemux avi h264
		//2.decode h264

	}

	if (phandle->current_state == AVENC_ENC && frame_count % 3 == 0)
	{
		
		memcpy(phandle->videoinbuffer, phandle->capBuf.virtaddr, CMEM_SIZE(phandle->width, phandle->height));

		H264_ENCODE(&video_status);
		//SYS_INFO("frame count is %d\r\n", frame_count++);
#if 1
		if(video_status.isKeyFrame == IVIDEO_I_FRAME)
		{
			i_p_flag = I_P_FLAG_I;
		}
		else
		{
			i_p_flag = I_P_FLAG_P;
		}
		if (VSSHM_GET_SEQ(phandle->pvsshm_obj, i_p_flag, &video_frame_seq, &sem_value) == succeed_type_failed)
		{
			SYS_ERROR("the function VSSHM_GET_SEQ() failed.\r\n");
			return succeed_type_failed;
		} 
		/*»ñµÃµÄÐÅºÅÁ¿*/
		if (sem_value != 0)
		{
			error_num++;
			//SYS_ERROR("\r\n-------------error number = %d sem_valueue = %d--------    -------------\r\n"\
			,error_num, sem_value);
		}
		/*µ÷ÓÃPUTº¯Êý*/
		if (VSSHM_PUT(phandle->pvsshm_obj, video_frame_seq, FRAME_TYPE_VIDEO_H264, phandle->videooutbuffer, video_status.bytesGenerated) == succeed_type_failed)
		{
			SYS_ERROR("the function VSSHM_PUT() failed.phandle->pvsshm_obj = 0x%x\r\n",phandle->pvsshm_obj);
			return succeed_type_failed;
		}
#endif
#if 1

		UNIX_CMD_STRUCT cmd;
		avenc_to_storage_video_buffer_new_msg bufnewmsg;

		bufnewmsg.buffer_id = video_frame_seq;
		cmd.cmd = PARACMD_AVENC_AVENC_TO_STORAGE_VIDEO_BUFFER_NEW_MSG;
		cmd.cmdlen = sizeof(avenc_to_storage_video_buffer_new_msg);
		memcpy(&cmd.cmdinfo, &bufnewmsg, sizeof(bufnewmsg));
		UNIX_SEND(phandle->unix_fd, &cmd);
		//SYS_INFO("in avenc  i_frame_count is %d\r\n", i_frame_count++);

		//usleep(1);

#endif
	}
	if (frame_count >= 1800)
	{
		UNIX_CMD_STRUCT cmd;
		cmd.cmd = PARACMD_AVENC_AVENC_TO_STORAGE_STOP_STORAGE;
		UNIX_SEND(phandle->unix_fd, &cmd);				
		phandle->current_state = AVENC_IDLE;

	}



	if (frame_count <= 1800)
		frame_count++;
//		SYS_INFO("cap frame count is %d\r\n", frame_count);




	if (phandle->current_state == AVENC_DEC) //???
	{
		//copy decode yuv data to display buffer
	}
	else
	{
		//loopback
		if(phandle->resize_enable == 1)
		{
			RESIZE_EXEC(phandle->rszHandle, (unsigned long)phandle->capBuf.virtaddr, (unsigned long)phandle->disBuf.virtaddr);
		}
		else
		{
			memcpy(phandle->disBuf.virtaddr, phandle->capBuf.virtaddr, DISPLAY_SIZE);
		}

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
