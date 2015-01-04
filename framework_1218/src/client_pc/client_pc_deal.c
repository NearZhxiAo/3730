#include"client_pc_private.h"

extern int gtime_no_heart_res;

succeed_type init_socket(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	SOCKET_CLOSE(phandle->socket_fd);
	phandle->socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(phandle->socket_fd == -1)
	{
		SYS_ERROR("Socket Create Failed.\r\n");
		return succeed_type_failed;
	}
	phandle->seq = 0x0001;
	return succeed_type_succeed;
}
char* md5encode(char *md,const char *passwd,const char *seed)
{
	MD5_CTX ctx1,ctx2;
	MD5_Init(&ctx1);
	MD5_Update(&ctx1,passwd,strlen(passwd));
	MD5_Final(md,&ctx1);
	
	MD5_Init(&ctx2);
	MD5_Update(&ctx2,seed,sizeof(seed));
	MD5_Update(&ctx2,md,sizeof(md));
	MD5_Final(md,&ctx2);
	return md;
}
int init_request_buf(CLIENT_PC_PRIVATE_HANDLE *phandle,int flag)
{
	if(flag==0)
	{
	return requestbuf_step_one(phandle);
	}
	if(flag==1)
	{
	return requestbuf_step_two(phandle);	
	}	
	return 0;
}

/******************************************** 
 *    Function     :  client_pc_login_request(CLIENT_PC_PRIVATE_HANDLE* phandle)
 *    Description :  第一、第二步 登陆请求与响应               
 *    Para        :    [in]: *phandle:
  *   Author      :   向松
 *    Date        : 2011-04-28

 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
succeed_type client_pc_login_request(CLIENT_PC_PRIVATE_HANDLE* phandle)
{
	int i = 0;
	int i_succeed = 0;
	unsigned char *buf = NULL ;
	
	for(;i<2;i++)
	{
		phandle->lens = init_request_buf(phandle,i);
		
		if(SOCKET_SEND(phandle->socket_fd, phandle->socketbuf,&(phandle->lens), NULL)==-1)
		{
			SYS_ERROR("Request%d Send Error:%d.\r\n",i+1,errno);
			return succeed_type_failed;
		}
		SYS_INFO("Request%d Send Ok.\r\n",i+1);
		phandle->lenr = 1024;
		if(SOCKET_RECV(phandle->socket_fd, phandle->socketbuf,&(phandle->lenr), NULL)==-1)
		{
			SYS_ERROR("Request%d Recv Error:%d.\r\n",i+1,errno);
			return succeed_type_failed;
		}
		SYS_INFO("Request%d Recv Ok.\r\n",i+1);
	}

	buf  = GetPEcontext(phandle,CMD_PE_TIME);
	memcpy(&phandle->heart_time,buf,sizeof(phandle->heart_time));
	phandle->heart_time = ntohl(phandle->heart_time);

	SYS_WARNING("phandle->heart_time = %d\r\n",phandle->heart_time);
	if( 0 == phandle->heart_time)
	{
		SYS_WARNING("phandle->heart_time = 0\r\n");
		phandle->heart_time = 10;
	}
	
	/* 如果接收到的结果域result是非负的，则登陆响应时正确的，
	否则是错误的*/
	GetPEcontext(phandle,CMD_PE_RESULT);
	if(memcmp(phandle->pebuf,&i_succeed, 4) >= 0)
	{
		return succeed_type_succeed;
	}
	return succeed_type_failed;
}

/******************************************** 
 *    Function     :  client_pc_connect(CLIENT_PC_PRIVATE_HANDLE *phandle)
 *    Description :  与中心连接函数，包括第一、第二步
                           登陆请求与响应和心跳包构造
 *    Para        :    [in]: *phandle:
  *   Author      :   向松
 *    Date        : 2011-04-28

 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
succeed_type client_pc_connect(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	//if(SOCKET_CONNECT(phandle->socket_fd,5000,SERVER_IP)==succeed_type_failed)
	if(SOCKET_CONNECT(phandle->socket_fd,phandle->net_param.center_port,phandle->net_param.center_ipaddr)==succeed_type_failed)
	{
		SYS_ERROR("Connect Error:%d.\r\n",errno);
		return succeed_type_failed;
	}
	SYS_INFO("Connect OK.\r\n");

	/*登陆请求*/
	if(client_pc_login_request(phandle)==succeed_type_failed)
	{
		SYS_ERROR("Login Request Failed.\r\n");
		init_socket(phandle);
		return succeed_type_failed;
	}
	else
	{   
		SYS_INFO("Login Request Success.\r\n");
		SYS_INFO("Init heartbeatbuf.\r\n");

		/*构造心跳包*/
		heartbeat(phandle);
	}
	return succeed_type_succeed;
}

/******************************************** 
 *    Function     : bufhandle(CLIENT_PC_PRIVATE_HANDLE *phandle)
 *    Description : 收到服务器指令后的处理函数                    
 *    Para        :    [in]: *phandle:
 *    Author      :   向松
 *    Date        : 2011-04-28
 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
void bufhandle(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	ushort type = 0;
	memcpy(&type,phandle->socketbuf+6,2);
	uchar *buf = NULL;
	ushort channel_type = 0;
	switch(ntohs(type))
	{ 
	       //收到心跳包命令
		case CMD_DTP_HEART_BEAT_RES:
			gtime_no_heart_res =0;
			SYS_WARNING("HEAT response.--------and------gtime_no_heart_res has been set to 0.\r\n");
			
			break;

		//收到启动视频命令
		case CMD_DTP_PREVIEW_START_REQ:
			SYS_WARNING("Preview Start Request.\r\n");
			buf = GetPEcontext(phandle,CMD_PE_CHANNEL_ID);
			memcpy(&channel_type, buf,sizeof(channel_type));
			channel_type = htons(channel_type);
			if ( CMD_VIEW_FLAG == channel_type)
			{
				phandle->lens = Message_response(phandle,CMD_DTP_PREVIEW_START_RES, CMD_VIEW_FLAG);
			}
			else if(CMD_AUDIO_FLAG == channel_type)
			{
				phandle->lens = Message_response(phandle,CMD_DTP_PREVIEW_START_RES, CMD_AUDIO_FLAG);
			}
			
			if(SOCKET_SEND(phandle->socket_fd,phandle->socketbuf2,&(phandle->lens),NULL) == succeed_type_failed)
			{
				SYS_ERROR("Preview Start Response Error Send.\r\n");
			}
			GetParam(phandle,CMD_DTP_PREVIEW_START_REQ);
			SendtoRTSP(phandle,CMD_DTP_PREVIEW_START_REQ);
			break;

		//收到停止视频命令?		case CMD_DTP_PREVIEW_STOP_REQ:
			SYS_WARNING("Preview Stop Request.\r\n");
			phandle->lens = Message_response(phandle, CMD_DTP_PREVIEW_STOP_RES, 0);
			if(SOCKET_SEND(phandle->socket_fd,phandle->socketbuf2,&(phandle->lens),NULL) == succeed_type_failed)
			{
				SYS_ERROR("Preview Stop Response Send Error.\r\n");
			}
			GetParam(phandle,CMD_DTP_PREVIEW_STOP_REQ);
			SendtoRTSP(phandle,CMD_DTP_PREVIEW_STOP_REQ);
			break;

	       //收到启动音频命令
		case CMD_DTP_CALL_START_REQ:
			SYS_INFO("Vidio Start Request.\r\n");
			phandle->lens = Message_response(phandle, CMD_DTP_CALL_START_RES, 0);
			if(SOCKET_SEND(phandle->socket_fd,phandle->socketbuf2,&(phandle->lens),NULL) == succeed_type_failed)
			{
				SYS_ERROR("Preview Stop Response Send Error.\r\n");
			}
			GetParam(phandle,CMD_DTP_PREVIEW_STOP_REQ);
			SendtoRTSP(phandle,CMD_DTP_PREVIEW_STOP_REQ);
			break;

		  //收到停止音频处理
		case CMD_DTP_CALL_STOP_REQ:
			SYS_INFO("Vidio Stop Request.\r\n");
			phandle->lens = Message_response(phandle, CMD_DTP_CALL_STOP_RES, 0);
			if(SOCKET_SEND(phandle->socket_fd,phandle->socketbuf2,&(phandle->lens),NULL) == succeed_type_failed)
			{
				SYS_ERROR("Preview Stop Response Send Error.\r\n");
			}
			GetParam(phandle,CMD_DTP_PREVIEW_STOP_REQ);
			SendtoRTSP(phandle,CMD_DTP_PREVIEW_STOP_REQ);
			break;
			
		case CMD_DTP_CONTROL_PTZ_REQ:
			SYS_INFO("Dpt Control Request.\r\n");
			phandle->lens = Message_response(phandle, CMD_DTP_CONTROL_PTZ_RES, 0);
			if(SOCKET_SEND(phandle->socket_fd,phandle->socketbuf2,&(phandle->lens),NULL) == succeed_type_failed)
			{
				SYS_ERROR("Preview Stop Response Send Error.\r\n");
			}
			GetParam(phandle,CMD_DTP_CONTROL_PTZ_REQ);
			SendtoRTSP(phandle,CMD_DTP_CONTROL_PTZ_REQ);
			break;
			
		case CMD_DTP_CONTROL_ADDCPS_REQ:
			SYS_INFO("Dpt Control Request.\r\n");
			phandle->lens = Message_response(phandle, CMD_DTP_CONTROL_ADDCPS_RES,0);
			if(SOCKET_SEND(phandle->socket_fd,phandle->socketbuf2,&(phandle->lens),NULL) == succeed_type_failed)
			{
				SYS_ERROR("Preview Stop Response Send Error.\r\n");
			}
			GetParam(phandle,CMD_DTP_CONTROL_ADDCPS_REQ);
			SendtoRTSP(phandle,CMD_DTP_CONTROL_ADDCPS_REQ);
			break;

		default:
			break;
	}
}

succeed_type client_pc_socket_deal(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	memset(phandle->socketbuf,0,1024);
	memset(phandle->socketbuf2,0,100);
	phandle->lenr = 1024;
	if(SOCKET_RECV(phandle->socket_fd,phandle->socketbuf,&(phandle->lenr),NULL) == succeed_type_failed)
	{
		SYS_ERROR("Socket Recv Error:%d.\r\n",errno);
		return succeed_type_failed;
	}
	if(phandle->lenr>0)
	{
		SYS_INFO("SocketbufHandle Start.\r\n");
		bufhandle(phandle);
		return succeed_type_succeed;
	}
	else
	{
		SYS_ERROR("Recv Nothing.\r\n");
		return succeed_type_failed;
	}
	
	return succeed_type_failed;	
}
