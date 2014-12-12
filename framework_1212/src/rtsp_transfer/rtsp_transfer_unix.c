#include "rtsp_transfer_private.h"
#include<sys/time.h>
#define PORT  1028
#define AUDIO_PORT  1038
#define CHANNEL_ID_VIDEO 0x0001
#define CHANNEL_ID_AUDIO 0x0003

#define RTSP_AUDIO_SEND
#undef RTSP_AUDIO_SEND
succeed_type rtsp_transfer_socket_creat(RTSP_TRANFER_PRIVATE_HANDLE *phandle, unsigned short type)
{
	succeed_type iret = succeed_type_failed;
	/*fill address  info*/
	if(CHANNEL_ID_VIDEO == type)
	{
		phandle->sockaddr.sin_family = AF_INET;
		phandle->sockaddr.sin_port = htons(phandle->cmdinfo.Port);
		phandle->sockaddr.sin_addr.s_addr = inet_addr(phandle->cmdinfo.IPStr);
		bzero(&(phandle->sockaddr.sin_zero),0);
	}
	else if(CHANNEL_ID_AUDIO == type)
	{
		phandle->audio_sockaddr.sin_family = AF_INET;
		phandle->audio_sockaddr.sin_port = htons(phandle->cmdinfo.Port);
		phandle->audio_sockaddr.sin_addr.s_addr = inet_addr(phandle->cmdinfo.IPStr);
		bzero(&(phandle->audio_sockaddr.sin_zero),0);
	}
	else
	{
		SYS_WARNING("\n\r-----channel_type is neithor video nor audio-----\n\r");
		return succeed_type_succeed;
	}
#ifdef RTSP_AUDIO_SEND
	phandle->sockaddr1.sin_family = AF_INET;
	phandle->sockaddr1.sin_port = htons(8550);
	phandle->sockaddr1.sin_addr.s_addr = inet_addr("192.168.0.106");
	bzero(&(phandle->sockaddr1.sin_zero),0);
#endif
	/*record transfer protocol in struct rtp_head_info*/
	phandle->rtp_head_info.trans_protocol = phandle->cmdinfo.IPProtocol;

	/*1.Create socket and connect media server */
	if(UDP == phandle->cmdinfo.IPProtocol)
	{
		if(CHANNEL_ID_VIDEO == type)
		{
			/*create UDP socket and bind local 1028 port */
			iret = SOCKET_CREATE(LIBSK_NETMODE_UDP,PORT,NULL, &(phandle->socket_fd));
			if(iret != succeed_type_succeed)
			{
				SYS_ERROR("\n\r----------SOCKET_CREATE_UDP failed-------------\r\n");
				return iret;
			}
		}
		else if(CHANNEL_ID_AUDIO == type)
		{
			/*create UDP socket for audio and bind local 1038 port */
			iret = SOCKET_CREATE(LIBSK_NETMODE_UDP,AUDIO_PORT,NULL, &(phandle->audio_socket_fd));
			if(iret != succeed_type_succeed)
			{
				SYS_ERROR("\n\r----------SOCKET_CREATE_UDP for audio failed-------------\r\n");
				return iret;
			}
		}

#ifdef RTSP_AUDIO_SEND
		iret = SOCKET_CREATE(LIBSK_NETMODE_UDP,PORT,NULL, &(phandle->socket_fd1));
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\n\r----------SOCKET_CREATE_UDP failed----socket_fd1---------\r\n");
			return iret;
		}
#endif
	}
	else if(TCP == phandle->cmdinfo.IPProtocol)
	{
		if(CHANNEL_ID_VIDEO == type)
		{
			/*1.create TCP socket and bind local 1028 port*/
			iret = SOCKET_CREATE(LIBSK_NETMODE_TCP,PORT,NULL, &(phandle->socket_fd));
			if(iret != succeed_type_succeed)
			{
				SYS_ERROR("\n\r----------SOCKET_CREATE_TCP failed-------------\r\n");
				return iret;
			}
			
			/*2.connect  assigned IP and port*/
			iret = SOCKET_CONNECT(phandle->socket_fd, phandle->cmdinfo.Port, phandle->cmdinfo.IPStr);
		  if(iret != succeed_type_succeed)
		  {
			    SYS_ERROR("\n\r---connect IP = %s,connect port = %d-----",phandle->cmdinfo.IPStr,phandle->cmdinfo.Port);
			    SYS_ERROR("\n\r----------SOCKET_CONNECT failed-----\r\n");
			    return iret;
		  }
		  SYS_WARNING("\n\r-kkkk--connect IP = %s,connect port = %d-----",phandle->cmdinfo.IPStr,phandle->cmdinfo.Port);
			
			
		}
		else if(CHANNEL_ID_AUDIO == type)
		{
			/*1.create TCP socket for audio and bind local 1038 port */
			iret = SOCKET_CREATE(LIBSK_NETMODE_TCP,AUDIO_PORT,NULL, &(phandle->audio_socket_fd));
			if(iret != succeed_type_succeed)
			{
				SYS_ERROR("\n\r----------SOCKET_CREATE_TCP for audio failed-------------\r\n");
				return iret;
			}
			
			/*2.connect  assigned IP and port*/
			iret = SOCKET_CONNECT(phandle->audio_socket_fd, phandle->cmdinfo.Port, phandle->cmdinfo.IPStr);
		  if(iret != succeed_type_succeed)
		  {
			    SYS_ERROR("\n\r---connect IP = %s,connect port = %d-----",phandle->cmdinfo.IPStr,phandle->cmdinfo.Port);
			    SYS_ERROR("\n\r----------SOCKET_CONNECT failed-----\r\n");
			    return iret;
		  }
		  SYS_WARNING("\n\r-kkkk---audio--connect IP = %s,connect port = %d-----",phandle->cmdinfo.IPStr,phandle->cmdinfo.Port);
		}

#ifdef RTSP_AUDIO_SEND
		iret = SOCKET_CREATE(LIBSK_NETMODE_TCP,PORT,NULL, &(phandle->socket_fd1));
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\n\r----------SOCKET_CREATE_TCP failed----socket_fd1---------\r\n");
			return iret;
		}
		
	  iret = SOCKET_CONNECT(phandle->socket_fd1, 8550, "192.168.0.106");
	  if(iret != succeed_type_succeed)
	  {
		    SYS_ERROR("\n\r---connect IP = %s,connect port = %d-----",phandle->cmdinfo.IPStr,phandle->cmdinfo.Port);
		    SYS_ERROR("\n\r----------SOCKET_CONNECT failed-----\r\n");
		    return iret;
	  }
	  else
	  {
		    SYS_WARNING("\n\r-kkkk--connect IP = %s,connect port = %d-----",phandle->cmdinfo.IPStr,phandle->cmdinfo.Port);
		    SYS_WARNING("\n\r-kkkk--connect IP = %s,connect port = %d-----","192.168.0.106",8550);
		    SYS_INFO("\n\r----------AUDIO SOCKET_CONNECT succeed-----\r\n");
	  }	
#endif
	}

	return succeed_type_succeed;
}

void rtsp_transfer_preview_cmd_parse(RTSP_TRANFER_PRIVATE_HANDLE *phandle, preview_start *pstcmd_preview_start)
{
	if(0 == pstcmd_preview_start->option)
	{
		SYS_INFO("\r\n----Receive preview_start CMD with no address info-----\r\n");
	}																    
	else
	{
		/*TCP or UDP*/
		phandle->cmdinfo.IPProtocol = pstcmd_preview_start->pe_net_schedule_address.protocol;
		/*get IP*/
		sprintf(phandle->cmdinfo.IPStr,"%s",pstcmd_preview_start->pe_net_schedule_address.ipaddr);
		/*get port*/
		phandle->cmdinfo.Port = pstcmd_preview_start->pe_net_schedule_address.serverport;
	}
}


static succeed_type rtsp_transfer_unix_preview_start_deal(RTSP_TRANFER_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	succeed_type iret = succeed_type_failed;
	unsigned short channel_type;
	/*get command info from client_pc */
	preview_start stcmd_preview_start;
	preview_start *pstcmd_preview_start =&stcmd_preview_start;
	pstcmd_preview_start = (preview_start *)(pcmd->cmdinfo);
	channel_type = pstcmd_preview_start->pe_channel_id.channel_type;
	switch(phandle->current_state)
	{
		case RTSP_STATE_IDLE:
			rtsp_transfer_preview_cmd_parse(phandle, pstcmd_preview_start);
			iret = rtsp_transfer_socket_creat(phandle,channel_type);
			if(iret != succeed_type_succeed)
			{
				SYS_ERROR("\n\r-------rtsp_transfer_socket_creat() failed--------\n\r");
				return iret;
			}
			if(CHANNEL_ID_VIDEO == channel_type)
			{
				/*copy 4 bytes as SSRC*/
				strncpy(phandle->rtp_head_info.SSRC, ((pstcmd_preview_start->pe_char16) + 12),4);
				phandle->current_state = RTSP_STATE_PREVIEW;
			}
			else if(CHANNEL_ID_AUDIO == channel_type)
			{
				/*copy 4 bytes as AUDIO_SSRC*/
				strncpy(phandle->rtp_head_info.AUDIO_SSRC, ((pstcmd_preview_start->pe_char16) + 12),4);
				phandle->current_state = RTSP_STATE_CALL;
			}
			else
			{
				SYS_WARNING("\n\r--------channel_type is not 0x1 or 0x3------\n\r");
			}
			break;
		case RTSP_STATE_PREVIEW:
		case RTSP_STATE_PREVIEW_PAUSE:
		   	
			if(CHANNEL_ID_AUDIO == channel_type)
			{
				rtsp_transfer_preview_cmd_parse(phandle, pstcmd_preview_start);
				SYS_WARNING("\n\r---------------Receive call_start_cmd at preview state-------------------\r\n");
				iret = rtsp_transfer_socket_creat(phandle,channel_type);
				/*copy 4 bytes as AUDIO_SSRC*/
				strncpy(phandle->rtp_head_info.AUDIO_SSRC, ((pstcmd_preview_start->pe_char16) + 12),4);
				if (RTSP_STATE_PREVIEW == phandle->current_state )
				{
					phandle->current_state = RTSP_STATE_PREVIEW_CALL;
				}
				else
				{
					phandle->current_state = RTSP_STATE_PREVIEW_PAUSE_CALL;
				}
			}
			SYS_WARNING("\n\r-----preview state receive call_start_cmd-------\n\r");
			break;	
			
		case RTSP_STATE_CALL:
		case RTSP_STATE_CALL_PAUSE:
			if(CHANNEL_ID_VIDEO == channel_type)
			{
				rtsp_transfer_preview_cmd_parse(phandle, pstcmd_preview_start);
				iret = rtsp_transfer_socket_creat(phandle,channel_type);
				/*copy 4 bytes as SSRC*/
				strncpy(phandle->rtp_head_info.SSRC, ((pstcmd_preview_start->pe_char16) + 12),4);
				if (RTSP_STATE_CALL == phandle->current_state )
				{
					phandle->current_state = RTSP_STATE_PREVIEW_CALL;
				}
				else
				{
					phandle->current_state = RTSP_STATE_PREVIEW_CALL_PAUSE;
				}
			}
			break;
		default:
			break;
	}
	return succeed_type_succeed;
}

static succeed_type rtsp_transfer_unix_call_start_deal(RTSP_TRANFER_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	/*get command info from client_pc */
	call_start stcmd_call_start;
	call_start *pstcmd_call_start =&stcmd_call_start;
	pstcmd_call_start = (call_start *)(pcmd->cmdinfo);

	/*Create socket and connect media server */
	if(RTSP_STATE_IDLE == phandle->current_state)
	{
		/*TCP or UDP*/
		phandle->cmdinfo.IPProtocol = pstcmd_call_start->pe_netaddress.net_transtype;
		/*get IP*/
		sprintf(phandle->cmdinfo.IPStr,"%s",pstcmd_call_start->pe_netaddress.ip);
		/*get port*/
		phandle->cmdinfo.Port = pstcmd_call_start->pe_netaddress.port;

		/*fill address  info*/
		phandle->sockaddr.sin_family = AF_INET;
		phandle->sockaddr.sin_port = htons(phandle->cmdinfo.Port);
		phandle->sockaddr.sin_addr.s_addr = inet_addr(phandle->cmdinfo.IPStr);
		bzero(&(phandle->sockaddr.sin_zero),0);

		if(UDP == phandle->cmdinfo.IPProtocol)
		{
			/*create UDP socket and bind local 1028 port */
			if ( SOCKET_CREATE(LIBSK_NETMODE_UDP,PORT,NULL, &(phandle->socket_fd)) == succeed_type_failed )
			{
				SYS_ERROR("SOCKET_CREATE UDP failed-----%s.\r\n", "rtsp_transfer_unix_event_start");
				return succeed_type_failed;
			}
		}
		else if(TCP == phandle->cmdinfo.IPProtocol)
		{
			/*create TCP socket and bind local 1028 port */
			if ( SOCKET_CREATE(LIBSK_NETMODE_TCP,PORT,NULL, &(phandle->socket_fd)) == succeed_type_failed )
			{
				SYS_ERROR("SOCKET_CREATE TCP failed-----%s.\r\n", "rtsp_transfer_unix_event_start");
				return succeed_type_failed;
			}
		}

		/*connect  assigned IP and port*/
		if(SOCKET_CONNECT(phandle->socket_fd, phandle->cmdinfo.Port, phandle->cmdinfo.IPStr)
				== succeed_type_failed)
		{
			SYS_ERROR("SOCKET_CONNECT failed-----%s.\r\n", "rtsp_transfer_unix_event_start");
			return succeed_type_failed;
		}
		/*change state to call*/
		phandle->current_state = RTSP_STATE_CALL;
	}
	else
	{
		SYS_INFO("\n\r----Receive client_pc command--call_start at non idle state,Don't deal-------\r\n");
	}

	return succeed_type_succeed;
}

static succeed_type rtsp_transfer_unix_preview_stop_deal(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	/*1.close socket*/
	if(SOCKET_CLOSE(phandle->socket_fd) == succeed_type_failed)
	{
		SYS_ERROR("\n\r %s: SOCKET_CLOSE failed, socket_fd = 0x%x. \r\n", "rtsp_transfer_unix_preview_stop_deal", phandle->socket_fd);
		return succeed_type_failed;
	}
	/*2.chang state */
	switch( phandle->current_state )
	{
        case RTSP_STATE_PREVIEW:
		case RTSP_STATE_PREVIEW_PAUSE:
		    SYS_WARNING("\n At preview or preview_pause state receive preview_stop_cmd, state change to idle__state\r\n");
		    phandle->current_state = RTSP_STATE_IDLE;
			break;
		case RTSP_STATE_PREVIEW_CALL:
		case RTSP_STATE_PREVIEW_PAUSE_CALL:
		    SYS_WARNING("\n At preview_call or preview_pause_call state receive preview_stop_cmd, state change to call_state\r\n");
		    phandle->current_state = RTSP_STATE_CALL;
			break;
		case RTSP_STATE_PREVIEW_CALL_PAUSE:
		case RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE:
		    SYS_WARNING("\n At preview_call_pause or preview_pause_call_pause state receive preview_stop_cmd,state change to call_pause state\r\n");
		    phandle->current_state = RTSP_STATE_CALL_PAUSE;
			break;
		default:
		    SYS_WARNING("\n\r--------Receive preview_stop_cmd at non preview state or preview_call or preview_pause  state-----\r\n");
            break;
	}
	return succeed_type_succeed;
}

static succeed_type rtsp_transfer_unix_call_stop_deal(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
    /*1.close socket*/
	if(SOCKET_CLOSE(phandle->audio_socket_fd) == succeed_type_failed)
	{
		SYS_ERROR("AUDIO_SOCKET_CLOSE failed-----%s.\r\n", "rtsp_transfer_unix_call_stop_deal");
		return succeed_type_failed;
	}
	/*2.chang state */
	switch( phandle->current_state )
	{
        case RTSP_STATE_CALL:
		case RTSP_STATE_CALL_PAUSE:
		    SYS_WARNING("\n At call or call_pause state receive call_stop_cmd, state change to idle__state\r\n");
		    phandle->current_state = RTSP_STATE_IDLE;
			break;
		case RTSP_STATE_PREVIEW_CALL:
		case RTSP_STATE_PREVIEW_CALL_PAUSE:
		    SYS_WARNING("\n At preview_call or preview_call_pause state receive call_stop_cmd, state change to preview_state\r\n");
		    phandle->current_state = RTSP_STATE_PREVIEW;
			break;
		case RTSP_STATE_PREVIEW_PAUSE_CALL:
		case RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE:
		    SYS_WARNING("\n At preview_pause_call or preview_pause_call_pause state receive call_stop_cmd,state change to preview_pause state\r\n");
		    phandle->current_state = RTSP_STATE_PREVIEW_PAUSE;
			break;
		default:
		    SYS_WARNING("\n\r--------Receive call_stop_cmd at non call state or preview_call or call_pause  state-----\r\n");
            break;
	}
	return succeed_type_succeed;
}


static count = 0;
static succeed_type rtsp_transfer_unix_avenc_buffer_new_msg(RTSP_TRANFER_PRIVATE_HANDLE *phandle,UNIX_CMD_STRUCT *pcmd)
{
	succeed_type iret = succeed_type_failed;
	LIBVSSHM_UINT_DATA_I rtsp_transfer_vsshm_unit_buffer;
	buffer_new_msg *prtsp_transfer_buffer_new_msg = NULL;

	struct timeval timea;
	struct timeval timeb;
	gettimeofday(&timea,NULL);
	prtsp_transfer_buffer_new_msg = (buffer_new_msg*)(pcmd->cmdinfo);

	/*preview or call state receive buffer_new_msg  and send */
	if((RTSP_STATE_PREVIEW == phandle->current_state)
			||(RTSP_STATE_PREVIEW_CALL == phandle->current_state)
			||(RTSP_STATE_PREVIEW_CALL_PAUSE == phandle->current_state))
	{
		/*get data info from avenc*/
		iret = VSSHM_GET_DATA(phandle->pvsshm_obj, prtsp_transfer_buffer_new_msg->buffer_id, &rtsp_transfer_vsshm_unit_buffer);
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\r\n-------------VSSHM_GET_DATA failed!!!---------------\r\n");
		}

		/*semaphore reduce 1*/
		VSSHM_GET_COMPLETE(phandle->pvsshm_obj, prtsp_transfer_buffer_new_msg->buffer_id);
		count++;
#ifdef SAVE_DATA
		if(count <1000)
		{
			fwrite(rtsp_transfer_vsshm_unit_buffer.vsshm_context, rtsp_transfer_vsshm_unit_buffer.vsshm_header.data_len, 1, phandle->fp_out1);
		}
		else
		{
			if (phandle->fp_out1 != NULL)
			{
				fclose(phandle->fp_out1);
				phandle->fp_out1 = NULL;
			}
		}
#endif
		/*fill data info in phanle'struct datainfo*/
		get_data_info(phandle,&rtsp_transfer_vsshm_unit_buffer);

		/*send video data*/
		SYS_INFO("\n\r--------the current state is %x,and send video-------\r\n",phandle->current_state);
		iret = rtsp_transfer_send_video(phandle);
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\r\n-------------rtsp_transfer_send_video failed!!!---------------\r\n");
		}
	}
	else /*other state don't receive buffer data*/
	{
		SYS_INFO("\r\n---Recv media data at non preview or preview_call or preview_call_pause state-----\r\n");
		/*semaphore reduce 1*/
		VSSHM_GET_COMPLETE(phandle->pvsshm_obj, prtsp_transfer_buffer_new_msg->buffer_id);
	}

	gettimeofday(&timeb,NULL);

	SYS_INFO("\n\r-------rtsptransfer send 1 frame timeuse: %d" ,1000000 * (timeb.tv_sec - timea.tv_sec) + (timeb.tv_usec - timea.tv_usec));
	return iret;
}

int count0 =0;
static succeed_type rtsp_transfer_unix_audio_buffer_new_msg(RTSP_TRANFER_PRIVATE_HANDLE *phandle,UNIX_CMD_STRUCT *pcmd)
{
	succeed_type iret = succeed_type_failed;
	LIBASSHM_UINT_DATA  rtsp_transfer_asshm_unit_buffer;
	audio_buffer_new_msg  *prtsp_transfer_audio_buffer_new_msg = NULL;
       FILE *fp;
	struct timeval timea;
	struct timeval timeb;
	gettimeofday(&timea,NULL);
	prtsp_transfer_audio_buffer_new_msg = (audio_buffer_new_msg*)(pcmd->cmdinfo);

	/*preview or call state receive audio_buffer_new_msg  and send */
	if((RTSP_STATE_CALL == phandle->current_state)
			||(RTSP_STATE_PREVIEW_CALL == phandle->current_state)
			||(RTSP_STATE_PREVIEW_PAUSE_CALL == phandle->current_state))
	{
		/*get data info from avenc*/
		iret = ASSHM_GET_DATA(phandle->passhm_obj, prtsp_transfer_audio_buffer_new_msg->buffer_id, &rtsp_transfer_asshm_unit_buffer);
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\r\n-------------VSSHM_GET_DATA failed!!!---------------\r\n");
		}

		/*semaphore reduce 1*/
		ASSHM_GET_COMPLETE(phandle->passhm_obj, prtsp_transfer_audio_buffer_new_msg->buffer_id);
		count++;
#ifdef SAVE_DATA_AUDIO
		if(count <1000)
		{
			fwrite(rtsp_transfer_asshm_unit_buffer.asshm_context, rtsp_transfer_asshm_unit_buffer.asshm_header.data_len, 1, phandle->fp_out1);
		}
		else
		{
			if (phandle->fp_out1 != NULL)
			{
				fclose(phandle->fp_out1);
				phandle->fp_out1 = NULL;
			}
		}
#endif
		/*fill data info in phanle'struct audatainfo*/
		get_audiodata_info(phandle,&rtsp_transfer_asshm_unit_buffer);

		/*send audio data*/
		//SYS_WARNING("\n\r--------the current state is %x,and send audio-------\r\n",phandle->current_state);
		iret = rtsp_transfer_send_audio(phandle);
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\r\n-------------rtsp_transfer_send_audio failed!!!---------------\r\n");
			return iret;
		}
		
		//写入音频文件
		/*if(count0 ==0)
		{
			fp = fopen("audio.g711", "w+");
			if( NULL == fp )
			{
				SYS_ERROR("\n\r %s: fopen failed, paudio_obj = 0x%x\r\n", "rtsp_transfer_audio_socket",phandle->paudio_obj);
				return succeed_type_failed;
			}
		}
		count0++;
		if(count0==5)
		{
			count0 = 4;
		}
		int remain_len0;
		remain_len0 = phandle->audatainfo.buf_len;
		if(remain_len0 > DATA_MAX_LEN)
		{
			remain_len0 = DATA_MAX_LEN;
		}
		fwrite(phandle->audatainfo.pbuf, 1, remain_len0, fp);
		*/
	}

	else /*other state don't receive buffer data*/
	{
		SYS_INFO("\r\n--------Recv media data at non call or preview_call or preview_pause_call state-----\r\n");
		/*semaphore reduce 1*/
		ASSHM_GET_COMPLETE(phandle->passhm_obj, prtsp_transfer_audio_buffer_new_msg->buffer_id);
	}

	gettimeofday(&timeb,NULL);

	SYS_INFO("\n\r-------rtsptransfer send a audio_buffer timeuse: %d" ,1000000 * (timeb.tv_sec - timea.tv_sec) + (timeb.tv_usec - timea.tv_usec));
	return succeed_type_succeed;
}


static succeed_type rtsp_transfer_unix_deal( RTSP_TRANFER_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	switch(pcmd->cmd)
	{
		/*preview start request*/
		case PARACMD_CLIENTPC_PREVIEW_START:
			SYS_INFO("----------------receive preview_start-------%s.\r\n","rtsp_transfer_unix_deal");
			if(rtsp_transfer_unix_preview_start_deal(phandle,pcmd) == succeed_type_failed)
			{
				SYS_ERROR("Preview start failed-----%s.\r\n", "rtsp_transfer_unix_deal");
				return succeed_type_failed;
			}
			break;
			/*preview stop request*/
		case PARACMD_CLIENTPC_PREVIEW_STOP:
			SYS_INFO("----------------receive preview_stop-------%s.\r\n","rtsp_transfer_unix_deal'");
			if(rtsp_transfer_unix_preview_stop_deal(phandle)==succeed_type_failed)
			{
				SYS_ERROR("Preview stop failed-----%s.\r\n", "rtsp_transfer_unix_deal");
				return succeed_type_failed;
			}
			break;
			/*call start requet*/
		case PARACMD_CLIENTPC_CALL_START:
			if(rtsp_transfer_unix_call_start_deal(phandle,pcmd) == succeed_type_failed)
			{
				SYS_ERROR("Call start failed-----%s.\r\n", "rtsp_transfer_unix_deal");
				return succeed_type_failed;
			}
			break;
			/*call stop request*/
		case PARACMD_CLIENTPC_CALL_STOP:
			if(rtsp_transfer_unix_call_stop_deal(phandle)==succeed_type_failed)
			{
				SYS_ERROR("Call stop failed-----%s.\r\n", "rtsp_transfer_unix_deal");
				return succeed_type_failed;
			}
			break;
			/*record start request */
		case PARACMD_CLIENTPC_RECORD_START_REPLAY_REQ:
			//rtsp_transfer_unix_record_start_replay_req_deal();
			break;
			/*record stop request*/
		case PARACMD_CLIENTPC_RECORD_STOP_REPLAY_REQ:
			//rtsp_transfer_unix_record_stop_replay_req_deal();
			break;
			/*record download start requst*/
		case PARACMD_CLIENTPC_RECORD_START_DOWNLOAD_REQ:
			//rtsp_transfer_unix_record_start_download_req_deal();
			break;
			/*record download stop request*/
		case PARACMD_CLIENTPC_RECORD_STOP_DOWNLOAD_REQ:
			//rtsp_transfer_unix_record_stop_download_deal();
			break;
			/*control record replay request*/
		case PARACMD_CLIENTPC_CONTROL_RECORD_REPLAY_REQ:
			//rtsp_transfer_unix_control_record_replay_req_deal();
			break;
			/*control record download request*/
		case PARACMD_CLIENTPC_CONTROL_RECORD_DOWNLOAD_REQ:
			//rtsp_transfer_unix_control_record_download_req_eal();
			break;
		case PARACMD_CLIENTPC_CONTROL_NET_STOP:
			SYS_WARNING("\n\r-------Recv NET_STOP CMD from clientPC, and terminate data transmission---\r\n");
			rtsp_transfer_unix_control_net_stop(phandle);
			break;
		case PARACMD_AVENC_BUFFER_NEW_MSG:
			//SYS_WARNING("Recv PARACMD_AVENC_BUFFER_NEW_MSG-----%s.\r\n", "rtsp_transfer_unix_deal");
			rtsp_transfer_unix_avenc_buffer_new_msg(phandle,pcmd);
			break;
		case PARACMD_AVENC_AUDIO_BUFFER_NEW_MSG:
			SYS_INFO("Recv PARACMD_AVENC_AUDIO_BUFFER_NEW_MSG-----%s.\r\n", "rtsp_transfer_unix_deal");
			rtsp_transfer_unix_audio_buffer_new_msg(phandle,pcmd);
			break;
		default:
			break;
	}
	return succeed_type_succeed;
}

succeed_type rtsp_transfer_unix(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	unsigned char remotename[128];
	UNIX_CMD_STRUCT unix_rcv_cmd;

	memset(remotename, 0x00, 128);
	memset(&unix_rcv_cmd, 0x00, sizeof(UNIX_CMD_STRUCT));

	//1. recv the unix event
	if ( UNIX_RECV( phandle->unix_fd, remotename, &unix_rcv_cmd) == succeed_type_failed )
	{
		SYS_ERROR("UNIX RECV failed.\r\n");
		return succeed_type_failed;
	}
	SYS_INFO("Rcv Event from %s in rtsp_transfer.\r\n", remotename);

	//2. deal the receiving event.
	rtsp_transfer_unix_deal(phandle, &unix_rcv_cmd);
	return succeed_type_succeed;
}


succeed_type rtsp_transfer_socket(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	unsigned char recv_buf[MTU];
	unsigned int recv_len = MTU;
	unsigned short Ctrl_cmd_type;

	unsigned char payload; /*payload type*/

	/*receive packet on socket*/
	if((phandle->polltimeout[1].revents &POLLIN) == POLLIN)
	{
		/*receive msg*/
		memset(recv_buf,0,recv_len);
		if(SOCKET_RECV( phandle->socket_fd, recv_buf, &recv_len,&(phandle->sockaddr))
			== succeed_type_failed)
		{
			SYS_ERROR("SOCKET_RECV failed-----%s.\r\n", "rtsp_transfer_socket");
			return succeed_type_failed;
		}

		/*distinguish data packet or control  packet*/
		payload = JudgeRecvData(recv_buf,&payload);
		if(payload == 122)/*control packet*/
		{
			SYS_INFO("Receive control packet.-----%s\r\n", "rtsp_transfer_socket");
			/*parse control packet and get command type*/
			Ctrl_cmd_type = Recv_Buf_Parse(recv_buf,&Ctrl_cmd_type);
			SYS_INFO("-------The control command type is : %x\r\n", Ctrl_cmd_type);

			switch(Ctrl_cmd_type)
			{
				/*pause command */
				case RTP_CTRL_DATA_PAUSE:
					if(phandle->current_state == RTSP_STATE_PREVIEW)
				    {
						SYS_INFO("\n\r At PREVIEW state receive preview_pause CMD-----%s\r\n","rtsp_transfer_socket");
						phandle->current_state = RTSP_STATE_PREVIEW_PAUSE;
				 	}
					if(phandle->current_state == RTSP_STATE_PREVIEW_CALL)
				    {
						SYS_INFO("\n\r At PREVIEW_CALL state receive preview_pause CMD-----%s\r\n","rtsp_transfer_socket");
						phandle->current_state = RTSP_STATE_PREVIEW_PAUSE_CALL;
				 	}
					if(phandle->current_state == RTSP_STATE_PREVIEW_CALL_PAUSE)
				    {
						SYS_INFO("\n\r At PREVIEW_CALL_PAUSE state receive preview_ pause CMD-----%s\r\n","rtsp_transfer_socket");
						phandle->current_state = RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE;
				 	}
					break;
				/*continue command*/	
				case RTP_CTRL_DATA_CONTINUE:
					if(phandle->current_state == RTSP_STATE_PREVIEW_PAUSE)
				    {
						SYS_INFO("\n\r At PREVIEW_PAUSE state receive preview_continue CMD-----%s\r\n","rtsp_transfer_socket");
						phandle->current_state = RTSP_STATE_PREVIEW;
				 	}
					if(phandle->current_state == RTSP_STATE_PREVIEW_PAUSE_CALL)
				    {
						SYS_INFO("\n\r At PREVIEW_PAUSE_CALL state receive preview_continue CMD-----%s\r\n","rtsp_transfer_socket");
						phandle->current_state = RTSP_STATE_PREVIEW_CALL;
				 	}
					if(phandle->current_state == RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE)
				    {
						SYS_INFO("\n\r At PREVIEW_PAUSE_CALL_PAUSE state receive preview_ continue CMD-----%s\r\n","rtsp_transfer_socket");
						phandle->current_state = RTSP_STATE_PREVIEW_CALL_PAUSE;
				 	}
					break;
				case RTP_CTRL_REQ_KEYFRAME:
					phandle->I_frame_sended_flag = FALSE;
					SYS_INFO("\n\r-----the received control command: resend I_frame-----\r\n");
					break;
				default:
					SYS_INFO("-----Received other control packet-----\r\n")
						
                    break;
        	}
		}
		else /*other packet that payload is not 122*/
		{
			SYS_INFO("\n\r----------Receive other packet that payload is not 122.\r\n");
		}
	}
	return succeed_type_succeed;
}

succeed_type rtsp_transfer_audio_socket(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{

	unsigned char recv_buf[MTU];
	unsigned int recv_len = MTU;
	unsigned short Ctrl_cmd_type;
  //	unsignted int heartbeat_time=60;//设置定时器时间
       FILE *fp;
	unsigned char payload; /*payload type*/

	/*receive packet on audio_socket*/
	if((phandle->current_state == RTSP_STATE_CALL) ||
			(phandle->current_state == RTSP_STATE_CALL_PAUSE) ||
			(phandle->current_state == RTSP_STATE_PREVIEW_CALL) ||
			(phandle->current_state == RTSP_STATE_PREVIEW_PAUSE_CALL) ||
			(phandle->current_state == RTSP_STATE_PREVIEW_CALL_PAUSE) ||
			(phandle->current_state == RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE))
	{
		/*receive msg*/
		memset(recv_buf,0,recv_len);
		if(SOCKET_RECV( phandle->audio_socket_fd, recv_buf, &recv_len,&(phandle->audio_sockaddr))
				== succeed_type_failed)
		{
			SYS_ERROR("\n\r--------AUDIO_SOCKET_RECV failed-----%s.\r\n", "rtsp_transfer_audio_socket");
			return succeed_type_failed;
		}
	}
	else/*at other state receive packet and don't deal*/
	{
		return succeed_type_failed;
	}

	/*distinguish data packet or control  packet*/
	payload = JudgeRecvData(recv_buf,&payload);
	if(payload == 122)/*control packet*/
	{
		SYS_ERROR("Receive control packet.-----%s\r\n", "rtsp_transfer_audio_socket");
		/*parse control packet and get command type*/
		Ctrl_cmd_type = Recv_Buf_Parse(recv_buf,&Ctrl_cmd_type);
		SYS_ERROR("-------The control command type is : %x\r\n", Ctrl_cmd_type);
		
		/*sig_alrm(int signo)
			{
			
			}
			if (signal(SIGALRM,sig_alrm)=SIG_ERR)
			{
				SYS_ERROR("\n\r----- signal (SIGALAM) errro.-----\r\n");
				return(heartbeat_time);
			}
		alarm(heartbeat_time);*/
		switch(Ctrl_cmd_type)
		{
			/*pause command 
			case RTP_CTRL_CHANNEL_ACTIVE:
			//	heartbeat_time=60;//设置定时器时间
				
				
				/* if(phandle->current_state==RTSP_STATE_IDLE)
				 {
				 SYS_INFO("\n\r At IDLE state receive channel_active CMD-----%s\r\n","rtsp_transfer_audio_socket");	
				 	 phandle->current_stat=RTSP_STATE_CALL;
				 }
				 if(phandle->current_state==RTSP_STATE_PREVIEW)
				 {
				 SYS_INFO("\n\r At PREVIEW or  PREVIEW_PAUSE state receive channel_active CMD-----%s\r\n","rtsp_transfer_audio_socket");	
				 	phandle->current_stat=RTSP_STATE_PREVIEW_CALL;
				 }
				  if(phandle->current_state ==RTSP_STATE_PREVIEW_PAUSE)
				 {
				 SYS_INFO("\n\r At PREVIEW_PAUSE state receive channel_active CMD-----%s\r\n","rtsp_transfer_audio_socket");	
				 	phandle->current_stat=RTSP_STATE_PREVIEW_PAUSE_CALL;
				 }*/		
			
			
			   
			case RTP_CTRL_DATA_PAUSE:
				if(phandle->current_state == RTSP_STATE_CALL)
		    	{
					SYS_INFO("\n\r At CALL state receive call_pause CMD-----%s\r\n","rtsp_transfer_audio_socket");
					phandle->current_state = RTSP_STATE_CALL_PAUSE;
				}
				if(phandle->current_state == RTSP_STATE_PREVIEW_CALL)
				{
					SYS_INFO("\n\r At PREVIEW_CALL state receive call_pause CMD-----%s\r\n","rtsp_transfer_audio_socket");
					phandle->current_state = RTSP_STATE_PREVIEW_CALL_PAUSE;
				}
				if(phandle->current_state == RTSP_STATE_PREVIEW_PAUSE_CALL)
				{
					SYS_INFO("\n\r At PREVIEW_PAUSE_CALL state receive call_pause CMD-----%s\r\n","rtsp_transfer_audio_socket");
					phandle->current_state = RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE;
				}
			break;
			/*continue command*/	
			case RTP_CTRL_DATA_CONTINUE:
				if(phandle->current_state == RTSP_STATE_CALL_PAUSE)
				{
					SYS_INFO("\n\r At CALL_PAUSE state receive call_continue CMD-----%s\r\n","rtsp_transfer_audio_socket");
					phandle->current_state = RTSP_STATE_CALL;
				}
				if(phandle->current_state == RTSP_STATE_PREVIEW_CALL_PAUSE)
		    		{
					SYS_INFO("\n\r At PREVIEW_CALL_PAUSE state receive call_continue CMD-----%s\r\n","rtsp_transfer_audio_socket");
					phandle->current_state = RTSP_STATE_PREVIEW_CALL;
				}
				if(phandle->current_state == RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE)
		    		{
			    		SYS_INFO("\n\r At PREVIEW_PAUSE_CALL_PAUSE state receive call_continue CMD-----%s\r\n","rtsp_transfer_audio_socket");
					phandle->current_state = RTSP_STATE_PREVIEW_PAUSE_CALL;
				}
			break;	
			case RTP_CTRL_CHANNEL_CLOSE:
				if(phandle->current_state == RTSP_STATE_CALL||phandle->current_state ==RTSP_STATE_CALL_PAUSE)
				{
			        	SYS_INFO("\n\r At CALL or CALL_PAUSE state receive channel_close CMD-----%s\r\n","rtsp_transfer_audio_socket");
				       phandle->current_state=RTSP_STATE_IDLE;
			       }
				   
	
				if(phandle->current_state ==  RTSP_STATE_PREVIEW_CALL|| phandle->current_state ==RTSP_STATE_PREVIEW_CALL_PAUSE)
				{
			       	SYS_INFO("\n\r At PREVIEW_CALL or PREVIEW_CALL_PAUSE state receive channel_close CMD-----%s\r\n","rtsp_transfer_audio_socket");
				    phandle->current_state=RTSP_STATE_PREVIEW;
			       }
				 if(phandle->current_state == RTSP_STATE_PREVIEW_PAUSE_CALL||phandle->current_state == RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE)
				{
			       	SYS_INFO("\n\r At PREVIEW_PAUSE_CALL or PREVIEW_PAUSE_CALL_PAUSE state receive channel_close CMD-----%s\r\n","rtsp_transfer_audio_socket");
				   	 phandle->current_state=RTSP_STATE_PREVIEW_PAUSE;
				 }
				 SOCKET_CLOSE(phandle-> audio_socket_fd);
				 break;
        }
	}
	else /*audio_data packet*/
	{
		/*deal with the received audio packet*/
#ifndef USE_NEW_AUDIO
		if((AUDIO_WRITE(phandle->paudio_obj, recv_buf+12 , AUDIO_A711, recv_len-12) ==
					succeed_type_failed))
#else
		if((AUDIO_WRITE(phandle->paudio_obj, recv_buf+28 , AUDIO_A711, recv_len-28) ==
					succeed_type_failed))
#endif
		{
			SYS_ERROR("\n\r %s: AUDIO_WRITE failed, paudio_obj = 0x%x\r\n", "rtsp_transfer_audio_socket",phandle->paudio_obj);
			return succeed_type_failed;
		}
#if 0
		SYS_WARNING("\n\r--------------------Receive audiodata packet succeed.!!!!!!!!!!!!!!---\r\n");
		fp = fopen("audio.g711", "w+");
		if( NULL == fp )
		{
			SYS_ERROR("\n\r %s: fopen failed, paudio_obj = 0x%x\r\n", "rtsp_transfer_audio_socket",phandle->paudio_obj);
			return succeed_type_failed;
		}
		fwrite(recv_buf+12, 1, recv_len-12, fp);
#endif 
	}
	return succeed_type_succeed;
}

unsigned short Recv_Buf_Parse(char *RecvBuf,unsigned short *CmdType)
{
	RTP_CTRL_HEAD stRtpCtrl;
	RTP_CTRL_HEAD *pstRtpCtrl = &stRtpCtrl;

	memset(pstRtpCtrl,0,sizeof(RTP_CTRL_HEAD));
	pstRtpCtrl = (RTP_CTRL_HEAD*)RecvBuf;
	/*get command  type*/
	*CmdType = ntohs(pstRtpCtrl->CtrlCmdType);
	return *CmdType;
}

unsigned char JudgeRecvData(char *RecvBuf,unsigned char *PayLoad)
{
	RTP_DATA_HEAD stRtpHead;
	RTP_DATA_HEAD *pstRtpHead = &stRtpHead;
	memset(pstRtpHead,0,sizeof(RTP_DATA_HEAD));
	pstRtpHead = (RTP_DATA_HEAD*)RecvBuf;/*12 bytes of header*/
	/*get PayLoad  type*/
	*PayLoad = ((pstRtpHead->Mrk_type)& 0x7f);
	return *PayLoad;
}

succeed_type rtsp_transfer_send_audio(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	unsigned char *send_buf;
	unsigned int send_len = 0;/* length for socket send*/
	void *ptr = NULL;
	long remain_len = 0; /*the remain length of buffer*/
	unsigned char *pstring =NULL;
	unsigned int offset = 0;
	succeed_type iret = succeed_type_failed;

	pstring = (unsigned char*)malloc(DATA_MAX_LEN); /*DATA_MAX_LEN == 1400*/
	memset(pstring,0,DATA_MAX_LEN);

	remain_len = phandle->audatainfo.buf_len;
	/*get audio data from buffer*/
	ptr = phandle->audatainfo.pbuf;

	do
	{
		if(remain_len > DATA_MAX_LEN)
		{
			/*get 1400 data bytes to send*/
			memcpy(pstring, ptr, DATA_MAX_LEN);
			phandle->rtp_head_info.Marker = 0;
			phandle->rtp_head_info.data_len = DATA_MAX_LEN;
		}
		else
		{
			/*send the left bytes less than 1400*/
			memcpy(pstring, ptr, remain_len);
			phandle->rtp_head_info.Marker = 0;
			phandle->rtp_head_info.data_len = remain_len;
		}

		/*send packet*/
		if(0xffff == phandle->a_sequence_number)
		{
			phandle->a_sequence_number = 0;
		}
		phandle->rtp_head_info.seqnumber = phandle->a_sequence_number++; /*每发送一个音频rtp分组，序列号加1*/
		send_buf = fillRtpHead(pstring,&(phandle->rtp_head_info),&send_len, &offset);
		*(send_buf+1) = 0x88;		//in audio send must set to be 0x88

#ifdef SAVE_DATA
		if (count < 1000)
		{
			//printf("FILE: %s, LINE: %d. offset = %d. len = %d.\r\n", __FILE__, __LINE__, offset, send_len - offset);
			fwrite( send_buf + offset, send_len - offset, 1, phandle->fp_out);
		}
		else
		{
			if (phandle->fp_out != NULL)
			{
				fclose(phandle->fp_out);
			}
			phandle->fp_out = NULL;
		}
#endif
		//1. fopen audiofile
		//2. fread , 320  --- send_buf
		//3, fread return <= 0, fseek offset 0 
		iret = SOCKET_SEND( phandle->audio_socket_fd, send_buf, &send_len,&(phandle->audio_sockaddr));
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\n\r---AUDIO-----SOCKET_SEND failed.-----%s\r\n", "rtsp_transfer_send_audio");
			return iret;
		}
#ifdef RTSP_AUDIO_SEND
		iret = SOCKET_SEND( phandle->socket_fd1, send_buf, &send_len,&(phandle->sockaddr1));
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\n\r--socket_fd1-----SOCKET_SEND failed.socket_fd = 0x%x-----%s\r\n",phandle->socket_fd1, "rtsp_transfer_send_audio");
		}
#endif
		ptr += DATA_MAX_LEN;
		remain_len -= DATA_MAX_LEN;
		send_len = 0;
		memset(pstring,0,DATA_MAX_LEN);
		memset(send_buf,0,MTU);
		SYS_INFO("\n\r----AUDIO---SOCKET_SEND succeed.....frame_type = %d----\r\n",phandle->audatainfo.frame_type );

	}while(remain_len > 0);

	if(NULL != pstring)
	{
		free(pstring);
		pstring = NULL;
	}

	return succeed_type_succeed;
}

succeed_type rtsp_transfer_send_video(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	succeed_type iret = succeed_type_failed;
	/*send video data*/
	if((phandle->datainfo.frame_type == FRAME_TYPE_VIDEO_H264) ||
			(phandle->datainfo.frame_type == FRAME_TYPE_VIDEO_MP4))
	{
		SYS_INFO("\n\n\r--------phandle->I_frame_sended_flag = 0x%x!!----------\r\n\n",phandle->I_frame_sended_flag);
		/*if I_frame is not sent*/
		if(FALSE == phandle->I_frame_sended_flag)
		{
			/*judge the current frame*/
			if(I_P_FLAG_I == phandle->datainfo.i_p_flag)
			{
				SYS_WARNING("\n\n\n\r-------------I_frame_isn't sent...send I_frame now------\r\n\n\n");
				/*send I frame*/
				iret = rtsp_transfer_send_I_frame(phandle);
				if(iret != succeed_type_succeed)
				{
					SYS_ERROR("\n\r-----rtsp_transfer_send_I_frame failed.-----\r\n");
					return iret;
				}
				phandle->I_frame_sended_flag = TRUE;
			}
			else
			{
				//SYS_WARNING("\n\r--I_frame isn't sent--The current frame is not I_frame,Don't send\r\n");
				return succeed_type_succeed;
			}
		}
		else /*I_frame is snet*/
		{
			SYS_INFO("\n\r--------I_frame is sent.....send current frame now--------\r\n");
			if(I_P_FLAG_I == phandle->datainfo.i_p_flag)
			{
				/*sned I frame*/
				iret = rtsp_transfer_send_I_frame(phandle);
				if(iret != succeed_type_succeed)
				{
					SYS_ERROR("\n\r-----rtsp_transfer_send_I_frame failed.-----\r\n");
					return iret;
				}
			}
			else if(I_P_FLAG_P == phandle->datainfo.i_p_flag)
			{
				/*send P frame*/
				iret = rtsp_transfer_send_P_frame(phandle);
				if(iret != succeed_type_succeed)
				{
					SYS_ERROR("\n\r-----rtsp_transfer_send_P_frame failed.-----\r\n");
					return iret;
				}
			}
			else
			{
				SYS_WARNING("\n\r----other frame type, do not deal\r\n");
			}
		}
	}
	else
	{
		SYS_WARNING("invalid type.\r\n");
	}
	return succeed_type_succeed;
}


succeed_type rtsp_transfer_send_I_frame(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	succeed_type  iret = succeed_type_failed;
	unsigned char *send_buf;
	unsigned int send_len = 0;/* length for socket send*/
	long remain_len = 0; /*the remain length of buffer*/
	void *ptr = NULL;
	unsigned char *pstring =NULL;
	unsigned int offset = 0;

	int I_first_flag = FALSE; /*flag for I_frame's first packet for exten header */

	pstring = (unsigned char*)malloc(DATA_MAX_LEN); /*DATA_MAX_LEN == 1400*/
	memset(pstring,0,DATA_MAX_LEN);


	remain_len = phandle->datainfo.buf_len;
	/*get data from buffer*/
	ptr = phandle->datainfo.pbuf;

	SYS_INFO("\n\n\n\r-----Entering function: send_I_frame()------------\r\n\n\n");

	do
	{
		if(remain_len > DATA_MAX_LEN)
		{
			/*get 1400 data bytes to send*/
			memcpy(pstring, ptr, DATA_MAX_LEN);
			phandle->rtp_head_info.Marker = 0;
			phandle->rtp_head_info.data_len = DATA_MAX_LEN;
		}
		else
		{
			/*send the left bytes less than 1400*/
			memcpy(pstring, ptr, remain_len);
			phandle->rtp_head_info.Marker = 1;
			phandle->rtp_head_info.data_len = remain_len;
		}

		if(FALSE == I_first_flag)
		{
			/*send I_frame's first packet use exten header*/
			phandle->rtp_head_info.Extension = 1;
			if(0xffff == phandle->v_sequence_number)
			{
				phandle->v_sequence_number = 0;
			}
			phandle->rtp_head_info.seqnumber = phandle->v_sequence_number++; /*每发送一个rtp分组，序列号加1*/
			send_buf = fillRtpExtenHead(pstring, &(phandle->rtp_head_info), &send_len, &offset);
			I_first_flag = TRUE;
		}
		else
		{
			phandle->rtp_head_info.Extension = 0;
			if(0xffff == phandle->v_sequence_number)
			{
				phandle->v_sequence_number = 0;
			}
			phandle->rtp_head_info.seqnumber = phandle->v_sequence_number++; /*每发送一个rtp分组，序列号加1*/
			send_buf = fillRtpHead(pstring,&(phandle->rtp_head_info),&send_len, &offset);
		}
#ifdef SAVE_DATA
		if (count < 1000)
		{
			//printf("FILE: %s, LINE: %d. offset = %d. len = %d.\r\n", __FILE__, __LINE__, offset, send_len - offset);
			fwrite( send_buf + offset, send_len - offset, 1, phandle->fp_out);
		}
		else
		{
			if (phandle->fp_out != NULL)
			{
				fclose(phandle->fp_out);
			}
			phandle->fp_out = NULL;
		}
#endif
		/*send packet*/
		iret = SOCKET_SEND( phandle->socket_fd, send_buf, &send_len,&(phandle->sockaddr));
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\n\r--------SOCKET_SEND failed.socket_fd = 0x%x-----%s\r\n",phandle->socket_fd, "rtsp_transfer_send_I_frame");
			return iret;
		}
#ifdef RTSP_SEND
		iret = SOCKET_SEND( phandle->socket_fd1, send_buf, &send_len,&(phandle->sockaddr1));
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\n\r--socket_fd1-----SOCKET_SEND failed.socket_fd = 0x%x-----%s\r\n",phandle->socket_fd1, "rtsp_transfer_send_I_frame");
		}
#endif
		ptr += DATA_MAX_LEN;
		remain_len -= DATA_MAX_LEN;
		send_len = 0;
		memset(pstring,0,DATA_MAX_LEN);
		memset(send_buf,0,MTU);
		SYS_INFO(".....SOCKET_SEND succeed..........ipflag = 0x%x-----\r\n",phandle->datainfo.i_p_flag );

	}while(remain_len >0);

	if(NULL != pstring)
	{
		free(pstring);
		pstring = NULL;
	}

	return succeed_type_succeed;

}

succeed_type rtsp_transfer_send_P_frame(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	succeed_type  iret = succeed_type_failed;
	unsigned char *send_buf;
	unsigned int send_len = 0;/* length for socket send*/
	long remain_len = 0; /*the remain length of buffer*/
	void *ptr = NULL;
	unsigned char *pstring =NULL;

	unsigned int offset = 0;
	pstring = (unsigned char*)malloc(DATA_MAX_LEN); /*DATA_MAX_LEN == 1400*/
	memset(pstring,0,DATA_MAX_LEN);

	remain_len = phandle->datainfo.buf_len;
	/*get data from buffer*/
	ptr = phandle->datainfo.pbuf;

	SYS_INFO("\n\n\n\r-----Entering function: send_P_frame()------------\r\n\n\n");

	do
	{
		if(remain_len > DATA_MAX_LEN)
		{
			/*get 1400 data bytes to send*/
			memcpy(pstring, ptr, DATA_MAX_LEN);
			phandle->rtp_head_info.Marker = 0;
			phandle->rtp_head_info.data_len = DATA_MAX_LEN;
		}
		else
		{
			/*send the left bytes less than 1400*/
			memcpy(pstring, ptr, remain_len);
			phandle->rtp_head_info.Marker = 1;
			phandle->rtp_head_info.data_len = remain_len;
		}

		/*send packet*/
		if(0xffff == phandle->v_sequence_number)
		{
			phandle->v_sequence_number = 0;
		}
		phandle->rtp_head_info.seqnumber = phandle->v_sequence_number++; /*每发送一个rtp分组，序列号加1*/
		send_buf = fillRtpHead(pstring,&(phandle->rtp_head_info),&send_len, &offset);

#ifdef SAVE_DATA
		if (count < 1000)
		{
			//printf("FILE: %s, LINE: %d. offset = %d. len = %d.\r\n", __FILE__, __LINE__, offset, send_len - offset);
			fwrite( send_buf + offset, send_len - offset, 1, phandle->fp_out);
		}
		else
		{
			if (phandle->fp_out != NULL)
			{
				fclose(phandle->fp_out);
			}
			phandle->fp_out = NULL;
		}
#endif
		iret = SOCKET_SEND( phandle->socket_fd, send_buf, &send_len,&(phandle->sockaddr));
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\n\r-%s: SOCKET_SEND failed-socket_fd = 0x%x,send_len = 0x%x\r\n", "rtsp_transfer_send_P_frame",phandle->socket_fd,send_len);
			return iret;
		}
#ifdef RTSP_SEND
		iret = SOCKET_SEND( phandle->socket_fd1, send_buf, &send_len,&(phandle->sockaddr1));
		if(iret != succeed_type_succeed)
		{
			SYS_ERROR("\n\r--socket_fd1-----SOCKET_SEND failed.socket_fd = 0x%x-----%s\r\n",phandle->socket_fd1, "rtsp_transfer_send_P_frame");
		}
#endif
		ptr += DATA_MAX_LEN;
		remain_len -= DATA_MAX_LEN;
		send_len = 0;
		memset(pstring,0,DATA_MAX_LEN);
		memset(send_buf,0,MTU);
		SYS_INFO(".....SOCKET_SEND succeed..........i_p_flag = 0x%x-----\r\n",phandle->datainfo.i_p_flag );

	}while(remain_len >0);

	if(NULL != pstring)
	{
		free(pstring);
		pstring = NULL;
	}
	return iret;
}

void get_data_info(RTSP_TRANFER_PRIVATE_HANDLE *phandle,LIBVSSHM_UINT_DATA_I *prtsp_transfer_vsshm_unit_buffer)
{
	phandle->datainfo.frame_type = prtsp_transfer_vsshm_unit_buffer->vsshm_header.frame_type;
	phandle->datainfo.i_p_flag = prtsp_transfer_vsshm_unit_buffer->vsshm_header.i_p_flag;
	phandle->datainfo.timestamp = prtsp_transfer_vsshm_unit_buffer->vsshm_header.timestamp;
	phandle->datainfo.pbuf = &(prtsp_transfer_vsshm_unit_buffer->vsshm_context[0]);
	phandle->datainfo.buf_len = prtsp_transfer_vsshm_unit_buffer->vsshm_header.data_len;
	/*certain payload type*/
	if(phandle->datainfo.frame_type == FRAME_TYPE_VIDEO_H264)
	{
		phandle->rtp_head_info.Payload = PAYLOAD_H264;
	}
	else if(phandle->datainfo.frame_type == FRAME_TYPE_VIDEO_MP4)
	{
		phandle->rtp_head_info.Payload = PAYLOAD_MPEG4;
	}
	else
	{
		SYS_INFO("\n\r---------------Receive other payload type-----------\r\n");
	}
	/*get timestamp for rtphead*/
	phandle->rtp_head_info.timestamp = (unsigned int)(phandle->datainfo.timestamp);
}


void get_audiodata_info(RTSP_TRANFER_PRIVATE_HANDLE *phandle,LIBASSHM_UINT_DATA *prtsp_transfer_asshm_unit_buffer)
{
	phandle->audatainfo.frame_type = prtsp_transfer_asshm_unit_buffer->asshm_header.frame_type;
	phandle->audatainfo.timestamp = prtsp_transfer_asshm_unit_buffer->asshm_header.timestamp;
	phandle->audatainfo.pbuf = &(prtsp_transfer_asshm_unit_buffer->asshm_context[0]);
	phandle->audatainfo.buf_len = prtsp_transfer_asshm_unit_buffer->asshm_header.data_len;
	/*certain payload type*/
	if(phandle->audatainfo.frame_type == FRAME_TYPE_AUDIO_A711)
	{
		phandle->rtp_head_info.Payload = PAYLOAD_PCMA;
	}
	else if(phandle->datainfo.frame_type == FRAME_TYPE_AUDIO_U711)
	{
		phandle->rtp_head_info.Payload = PAYLOAD_G7221;
	}
	else
	{
		SYS_INFO("\n\r---------------Receive other payload type in audio_buffer_new_msg----------\r\n");
	}
	/*get timestamp for rtphead*/
	phandle->rtp_head_info.timestamp = (unsigned int)(phandle->audatainfo.timestamp);
}


succeed_type rtsp_transfer_unix_control_net_stop(RTSP_TRANFER_PRIVATE_HANDLE *phandle)
{
	switch(phandle->current_state)
	{
		case RTSP_STATE_PREVIEW:
		case RTSP_STATE_PREVIEW_PAUSE:
	    	if(SOCKET_CLOSE(phandle->socket_fd) == succeed_type_failed)
			{
				SYS_ERROR("\n\rSOCKET_CLOSE failed, socket_fd = 0x%x.\r\n", phandle->socket_fd);
				return succeed_type_failed;
			}
			break;
		case RTSP_STATE_CALL:
		case RTSP_STATE_CALL_PAUSE:
	    	if(SOCKET_CLOSE(phandle->audio_socket_fd) == succeed_type_failed)
			{
				SYS_ERROR("\n\r SOCKET_CLOSE failed, audio_socket_fd = 0x%x.\r\n", phandle->audio_socket_fd);
				return succeed_type_failed;
			}
			break;
		case RTSP_STATE_PREVIEW_CALL:
		case RTSP_STATE_PREVIEW_PAUSE_CALL:
		case RTSP_STATE_PREVIEW_CALL_PAUSE:
		case RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE:
	    	if(SOCKET_CLOSE(phandle->socket_fd) == succeed_type_failed)
			{
				SYS_ERROR("\n\rSOCKET_CLOSE failed, socket_fd = 0x%x.\r\n", phandle->socket_fd);
				return succeed_type_failed;
			}
	    	if(SOCKET_CLOSE(phandle->audio_socket_fd) == succeed_type_failed)
			{
				SYS_ERROR("\n\r SOCKET_CLOSE failed, audio_socket_fd = 0x%x.\r\n", phandle->audio_socket_fd);
				return succeed_type_failed;
			}
            break;
		default:
			break;
	}
    phandle->current_state = RTSP_STATE_IDLE;
    return succeed_type_succeed;
}

