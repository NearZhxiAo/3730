#include "client_pc_private.h"

void SendtoRTSP(CLIENT_PC_PRIVATE_HANDLE *phandle,ushort type){                                                   
	//include libunix.h
	UNIX_CMD_STRUCT unixcmd;                                                                      
	memset(&unixcmd,0,sizeof(unixcmd)); 
	switch(type){ 
		//Preview Start Request:                                                         
		case CMD_DTP_PREVIEW_START_REQ:
			unixcmd.cmd = PARACMD_CLIENTPC_PREVIEW_START;//cmd define intercmd.h                                                 
			unixcmd.cmdlen = sizeof(struct preview_start);//ps define intercmds/ps

			//copy to unixcmd.cmdinfo <-sockdetbuf get params from GetParam()
			memcpy(unixcmd.cmdinfo,phandle->socketbuf,unixcmd.cmdlen);                                       

			//ackflag 0:no response, 1:need ack response
			unixcmd.ret_ack=0;                                                                            
			if(UNIX_SEND(phandle->unix_fd,&unixcmd)==-1)
				SYS_ERROR("PreviewStart Sendto RTSP Error:%d\r\n",errno);  
			break;     
			//end preview start request

			//Preview Stop Request
		case CMD_DTP_PREVIEW_STOP_REQ:	
			unixcmd.cmd = PARACMD_CLIENTPC_PREVIEW_STOP;
			unixcmd.cmdlen = sizeof(struct preview_stop);

			memcpy(unixcmd.cmdinfo,phandle->socketbuf,unixcmd.cmdlen);
			unixcmd.ret_ack=0;
			if(UNIX_SEND(phandle->unix_fd,&unixcmd)==-1)
				SYS_ERROR("PreviewStop Sendto RTSP Error:%d\r\n",errno);
			break;
			//end preview stop request

		case CMD_DTP_CALL_START_REQ:	
			unixcmd.cmd = PARACMD_CLIENTPC_CALL_START;
			unixcmd.cmdlen = sizeof(struct call_start);

			memcpy(unixcmd.cmdinfo,phandle->socketbuf,unixcmd.cmdlen);
			unixcmd.ret_ack=0;
			if(UNIX_SEND(phandle->unix_fd,&unixcmd)==-1) 
			{
				SYS_ERROR("PreviewStop Sendto RTSP Error:%d\r\n",errno);
			}

		case CMD_DTP_CALL_STOP_REQ:
			unixcmd.cmd = PARACMD_CLIENTPC_CALL_STOP;
			unixcmd.cmdlen = sizeof(struct call_stop);

			memcpy(unixcmd.cmdinfo,phandle->socketbuf,unixcmd.cmdlen);
			unixcmd.ret_ack=0;
			if(UNIX_SEND(phandle->unix_fd,&unixcmd)==-1) 
			{
				SYS_ERROR("PreviewStop Sendto RTSP Error:%d\r\n",errno);
			}
			break;
		case CMD_DTP_CONTROL_PTZ_REQ:
			unixcmd.cmd     = PARACMD_CLIENTPC_CTL_PTZ_REQ;
			unixcmd.cmdlen = sizeof(struct ctl_ptz_request);
			memcpy(unixcmd.cmdinfo,phandle->socketbuf,unixcmd.cmdlen);
			unixcmd.ret_ack=0;
			if(UNIX_SEND(phandle->unix_fd,&unixcmd)==-1) 
			{
				SYS_ERROR("PreviewStop Sendto RTSP Error:%d\r\n",errno);
			}
			break;
		default:
			break;
	}//end_switch
}

succeed_type client_pc_unix_deal(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	UNIX_CMD_STRUCT unixcmd;
	char achar_remotename[20];
	// int count_huang=106;
	//int count_handle_alrm=106;

	unsigned char handle_alarm_buf[106]={0x44,0x54,0x00,0x01,0x00,0x00,0x20,0xA0,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x26,0x00,0x00,0x07,0x85,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x20,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x01,0x16,0x00,0x0F,0x01,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x01};
	// SYS_WARNING("-----------------------count is %d\r\n",count_huang);

	memset(&unixcmd,0,sizeof(unixcmd));
	memset(achar_remotename,0,sizeof(achar_remotename));
		
	if(UNIX_RECV(phandle->unix_fd,achar_remotename,&unixcmd)==succeed_type_failed)
	{
		SYS_ERROR("Unix Recv from %s Error.\r\n",achar_remotename);
		return succeed_type_failed;
	}
	else
	{
	   SYS_WARNING("$$$$$$$$$----THE CMD IS %d\r\n",unixcmd.cmd);
	   switch (unixcmd.cmd)
	   {
		   //ÊÖ¹¤±¨¾¯
		   case PARACMD_ALERT_DI_EVENT:
		   	SYS_WARNING("Handle Alarm!\r\n");
			break;
			default:
				break;
			
	   }
	}
	//if(strcmp(achar_remotename,UNIXFILE_SOCKET_RTSP)==0)
	return succeed_type_succeed;
}


