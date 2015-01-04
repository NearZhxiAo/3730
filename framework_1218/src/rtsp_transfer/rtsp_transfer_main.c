#include "rtsp_transfer_private.h"

RTSP_TRANFER_PRIVATE_HANDLE global_rtsp_transfer_private_handle;
void sig_handle_proc(int sig);
static int IsRtspSysQuit = 0;

static void rtsp_transfer_poll_init(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	//memset(&(phandle->polltimeout), 0x00, sizeof(struct pollfd) * RTSP_TRANFER_POLL_COUNT_MAX );
	memset(phandle->polltimeout, 0x00, sizeof(struct pollfd) * RTSP_TRANFER_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;
	phandle->polltimeout[0].events = POLLIN;
	phandle->active_fdcount = 1;

	switch(phandle->current_state)
	{
		case RTSP_STATE_START:
			break;
		case RTSP_STATE_IDLE:
			phandle->active_fdcount = 1;
			break;
		case RTSP_STATE_PREVIEW:
		case RTSP_STATE_PREVIEW_PAUSE:
			phandle->polltimeout[1].fd = phandle->socket_fd;
			phandle->polltimeout[1].events = POLLIN;
			phandle->active_fdcount = 2;
			break;
		case RTSP_STATE_PREVIEW_CALL:
        case RTSP_STATE_PREVIEW_PAUSE_CALL:
        case RTSP_STATE_PREVIEW_CALL_PAUSE:
        case RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE:
			phandle->polltimeout[1].fd = phandle->socket_fd;
			phandle->polltimeout[1].events = POLLIN;
			phandle->polltimeout[2].fd = phandle->audio_socket_fd;
			phandle->polltimeout[2].events = POLLIN;
			phandle->active_fdcount = 3;
			break;
		case RTSP_STATE_CALL:
		case RTSP_STATE_CALL_PAUSE:
			phandle->polltimeout[1].fd = phandle->audio_socket_fd;
			phandle->polltimeout[1].events = POLLIN;
			phandle->active_fdcount = 2;
			break;
		case RTSP_STATE_END:
			break;
		default:
			break;
	}
}

static succeed_type rtsp_transfer_init(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	SIGNAL_STRUCT_T signal;
	if ( NULL == phandle )
	{
		SYS_ERROR("in rtsp_transfer_init phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	phandle->pgbshm_obj = GBSHM_INIT();
	if ( NULL == phandle->pgbshm_obj )
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}
	phandle->pvsshm_obj = VSSHM_INIT();
	if ( NULL == phandle->pvsshm_obj )
	{
		SYS_ERROR("VSSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}
	phandle->passhm_obj = ASSHM_INIT();
	if ( NULL == phandle->passhm_obj )
	{
		SYS_ERROR("ASSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}
	phandle->paudio_obj = AUDIO_INIT(AUDIO_MODE_WRITE);
	if ( NULL == phandle->paudio_obj )
	{
		SYS_ERROR("AUDIO_INIT failed.\r\n");
		return succeed_type_failed;
	}

	phandle->datainfo.frame_type = FRAME_TYPE_VIDEO_START;
	phandle->datainfo.i_p_flag = I_P_FLAG_START;
	phandle->datainfo.timestamp = 0;
	phandle->datainfo.pbuf = NULL;
	phandle->datainfo.buf_len = 0;

	/*phandle->cmdinfo.IPProtocol = 0x00;
	  memset(phandle->cmdinfo.IPStr, 0,sizeof(phandle->cmdinfo.IPStr));
	  phandle->cmdinfo.Port = 0;*/

	phandle->I_frame_sended_flag = FALSE;

	srand(time(0));//产生随机数种子
	phandle->v_sequence_number = htons(rand());/*视频序列号随机数*/
	phandle->a_sequence_number = htons(rand()); /*音频序列号随机数*/

	//memset(&(phandle->rtp_head_info), 0, sizeof(phandle->rtp_head_info));

	/*创建本地域套接字*/
	if ( UNIX_CREATE(UNIXFILE_SOCKET_RTSP, &(phandle->unix_fd), 0 ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_RTSP);
		return succeed_type_failed;
	}
#ifdef SAVE_DATA_AUDIO
	phandle->fp_out1 = fopen("rtspout_audio.mp4","ab+");
	if(NULL == phandle->fp_out1)
	{
		SYS_ERROR("fopen  fp_out failed.\r\n" );
		return succeed_type_failed;
	}
#endif
#ifdef SAVE_DATA
	phandle->fp_out = fopen("rtspout.mp4","ab+");
	if(NULL == phandle->fp_out)
	{
		SYS_ERROR("fopen  fp_out failed.\r\n" );
		return succeed_type_failed;
	}
#endif
	GBSHM_PARA_GET(phandle->pgbshm_obj,PARA_CMD_SYSTEM_VIDEO_ENC_PARAM,&(phandle->videoencparam), sizeof(video_enc_param));     
	printf("input mode 0x%x\r\n",phandle->videoencparam.video_input_mode);         
	if(phandle->videoencparam.video_input_mode == VIDEO_INPUT_PAL)
	{
		if(phandle->videoencparam.video_enc_resolution == VIDEO_ENC_D1)
		{
			phandle->rtp_head_info.v_width = 720;
			phandle->rtp_head_info.v_height = 576;
		}
		else if(phandle->videoencparam.video_enc_resolution == VIDEO_ENC_CIF)
		{
			phandle->rtp_head_info.v_width = 360;
			phandle->rtp_head_info.v_height = 288;
		}
		else
		{
			phandle->rtp_head_info.v_width = 720;
			phandle->rtp_head_info.v_height = 576;
		}
	}
	else if(phandle->videoencparam.video_enc_mode == VIDEO_INPUT_NTSC)
	{
		if(phandle->videoencparam.video_enc_resolution == VIDEO_ENC_D1)
		{
			phandle->rtp_head_info.v_width = 720;
			phandle->rtp_head_info.v_height = 480;
		}
		else if(phandle->videoencparam.video_enc_resolution == VIDEO_ENC_CIF)
		{
			phandle->rtp_head_info.v_width = 360;
			phandle->rtp_head_info.v_height = 240;
		}
		else
		{
			phandle->rtp_head_info.v_width = 720;
			phandle->rtp_head_info.v_height = 480;
		}
	} 
	else
	{
		phandle->rtp_head_info.v_width = 720;
		phandle->rtp_head_info.v_height = 480;
	}

	phandle->current_state = RTSP_STATE_IDLE;
	rtsp_transfer_poll_init(phandle);

	memset(&signal, 0x00, sizeof(signal));
	signal.signo[0] = SIGINT;
	signal.sig_phandle[0] = sig_handle_proc;
	SIGNAL_INIT(&signal, 1);

	return succeed_type_succeed;
}

static succeed_type rtsp_transfer_exit(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	if ( NULL == phandle )
  	{           
	   SYS_ERROR("in storage_exit phandle is NULL.\r\n");
       return succeed_type_failed;
  	}       
   	GBSHM_DESTROY(phandle->pgbshm_obj);
   	VSSHM_DESTROY(phandle->pvsshm_obj);
	ASSHM_DESTROY(phandle->passhm_obj);
	AUDIO_DESTROY(phandle->paudio_obj, AUDIO_MODE_WRITE);
  	if ( UNIX_DESTROY(UNIXFILE_SOCKET_RTSP, phandle->unix_fd) == succeed_type_failed )
   	{   
   		SYS_ERROR("UNIX_DESTROY %s failed.\r\n", UNIXFILE_SOCKET_RTSP);
	    return succeed_type_failed;
   	}
#ifdef SAVE_DATA
	if(NULL != phandle->fp_out)
	{
		fclose(phandle->fp_out);
		phandle->fp_out = NULL;
	}
	if(NULL != phandle->fp_out1)
	{
		fclose(phandle->fp_out1);
		phandle->fp_out1 = NULL;
	}
#endif
	return succeed_type_succeed;
}
void sig_handle_proc(int sig)
{
	SYS_WARNING("Rtsp Rcv Signo=%d\r\n",sig);
	IsRtspSysQuit = 1;
}
int main(int argc, char **argv)
{
	memset(&global_rtsp_transfer_private_handle, 0x00, sizeof(global_rtsp_transfer_private_handle));
	rtsp_transfer_init(&global_rtsp_transfer_private_handle);
	SYS_INFO("Enter rtsp_transfer_manager func.\r\n");

	while(0 == IsRtspSysQuit)
	{

		if ( poll( &(global_rtsp_transfer_private_handle.polltimeout), global_rtsp_transfer_private_handle.active_fdcount , -1 ) > 0 )
		{
			SYS_INFO("POLL event found.\r\n");
			if ( global_rtsp_transfer_private_handle.polltimeout[0].revents )
			{
				//接收到可读消息do UNIX recv event.
				rtsp_transfer_unix(&global_rtsp_transfer_private_handle);
			}
			if ( global_rtsp_transfer_private_handle.polltimeout[1].revents )
			{
				//接收到可读消息do socket event.
				switch(global_rtsp_transfer_private_handle.current_state)				
				{
					case RTSP_STATE_PREVIEW:
					case RTSP_STATE_PREVIEW_PAUSE:
					case RTSP_STATE_PREVIEW_CALL:
					case RTSP_STATE_PREVIEW_PAUSE_CALL:
					case RTSP_STATE_PREVIEW_CALL_PAUSE:
					case RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE:
					    rtsp_transfer_socket(&global_rtsp_transfer_private_handle);
					    break;
					case RTSP_STATE_CALL:
					case RTSP_STATE_CALL_PAUSE:
					    //SYS_WARNING("\n\r-----AAAAAAAAAAAAAAA----rtsp_transfer_audio_socket--------\r\n");
					    rtsp_transfer_audio_socket(&global_rtsp_transfer_private_handle);
						break;
					default:
						break;
				}
			}
			if ( global_rtsp_transfer_private_handle.polltimeout[2].revents )
			{
				//接收到可读消息do socket event.
				//SYS_WARNING("\n\r-----BBBBBBBBBBBBBBBBB----rtsp_transfer_audio_socket--------\r\n");
				rtsp_transfer_audio_socket(&global_rtsp_transfer_private_handle);
			}

#if 0				
			else if(!global_rtsp_transfer_private_handle.polltimeout[0].revents)
			{
				SYS_WARNING("Recv unknow msg !!active_fcount = 0x%x\r\n",\
						global_rtsp_transfer_private_handle.active_fdcount);
				SYS_WARNING("global_rtsp_transfer_private_handle.polltimeout[0].events = 0x%x\r\n",\
						global_rtsp_transfer_private_handle.polltimeout[0].events);
				SYS_WARNING("global_rtsp_transfer_private_handle.polltimeout[1].events = 0x%x\r\n",\
						global_rtsp_transfer_private_handle.polltimeout[1].events);
			}
#endif		
			rtsp_transfer_poll_init(&global_rtsp_transfer_private_handle);
		}
	}
	rtsp_transfer_exit(&global_rtsp_transfer_private_handle);
	SYS_WARNING("Rtsp_transfe Quit!!\r\n");
	return 0;
}

