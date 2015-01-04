#include "client_pc_private.h"

unsigned char* GetPEcontext(CLIENT_PC_PRIVATE_HANDLE *phandle,int PE_TYPE)
{
	//PEheader type and length
	ushort uflag = 0 ;
	ushort uPEnum = 0;
	uint32 uitotalength = 0;
	int iflag = 0;
	int ioffset = 0;
	ushort uspetype = 0;
	uint32 uipelength = 0;

	memcpy(&uflag,phandle->socketbuf+6,2);
	memcpy(&uPEnum,phandle->socketbuf+8,2);
	memcpy(&uitotalength,phandle->socketbuf+12,4);
	uflag=ntohs(uflag);
	uPEnum=ntohs(uPEnum);
	uitotalength=ntohl(uitotalength);

	if((phandle->lenr)!=uitotalength+68)
	{
		SYS_ERROR("Recv lost data.\r\n");
	}

	memset(phandle->pebuf,0,sizeof(phandle->pebuf));
	for(iflag;iflag<uPEnum&&ioffset<=uitotalength;iflag++,ioffset+=(uipelength+6))
	{
		memcpy(&uspetype,phandle->socketbuf+68+ioffset,2);
		memcpy(&uipelength,phandle->socketbuf+70+ioffset,4);
		uspetype=ntohs(uspetype);
		uipelength=ntohl(uipelength);
		if(uspetype==PE_TYPE)
		{
			memcpy(phandle->pebuf,phandle->socketbuf+68+6+ioffset,uipelength);
			break;
		}
	}
	return phandle->pebuf;
}

int requestbuf_step_one(CLIENT_PC_PRIVATE_HANDLE *phandle)
{                                             
	if( NULL == phandle)
	{
		SYS_ERROR("requestbuf_step_one parameter phandle = NULL!!\r\n");
		return -1;
	}
	//header data:                                                                                
	PE_HEADER ph;                                                                                 
	memset(&ph,'0',sizeof(ph));                                                                   
	ph.flag=htons(CMD_PH_HEAD_FLAG);                                                                        
	ph.version=htons(CMD_MESSAGE_VERSION_FLAG);                                                                     
	ph.zipflag=0;                                                                                 
	ph.msgtype=htons(CMD_DTP_PU_LOGIN_STEP1_REQ);                                                                     
	ph.penum=htons(0x0002);                                                                       
	ph.reserved=htons(0xcccc);                                                                    
	ph.msglen=htonl(0x0024);                                                                      
	ph.seq=htonl(phandle->seq);                                                                   

	//Type:0x1000                                                                                              
	DTP_PU_LOGIN_STEP1_REQ req;                                                                   
	memset(&req,0,sizeof(req));                                                                   

	//PEnum:2                                                                         
	PE pe1,pe2;                                                                                        
	memset(&pe1,0,sizeof(pe1)); 
	memset(&pe2,0,sizeof(pe2));

	//pe1:PE_FD_ID 0x1001                                                                    
	pe1.petype=htons(CMD_PE_FD_ID);                                                                      
	pe1.pelength=htons(0x0012);                                                                    
	//memcpy(req.puid,LOGINID,sizeof(LOGINID));
	memcpy(req.puid,phandle->net_param.center_loginid,sizeof(phandle->net_param.center_loginid));

	//pe2:PE_FD_VERSION 0x101a
	pe2.petype=htons(CMD_PE_FD_VERSION);
	pe2.pelength=htons(0x0006);                                                                
	req.puversion.hardwareversion=htons(0x0);                                                  
	req.puversion.softwareversion=htonl(0x0);                                                  

	//PEheader:                                                                                             
	memcpy(phandle->socketbuf,&ph,sizeof(ph));

	//PE_FD_ID                                                       
	memcpy(phandle->socketbuf+sizeof(ph),&pe1,6);                                                   
	memcpy(phandle->socketbuf+sizeof(ph)+6,&req.puid,18);

	//PE_FD_VERSION                             
	memcpy(phandle->socketbuf+sizeof(ph)+6+18,&pe2,6);                   
	memcpy(phandle->socketbuf+sizeof(ph)+6+18+6,&req.puversion.hardwareversion,2);                   
	memcpy(phandle->socketbuf+sizeof(ph)+6+18+6+2,&req.puversion.softwareversion,4);                   

	//sequence up                                                                                 
	(phandle->seq)++;  
	SYS_INFO("request_step_one() end.\r\n");

	return (68+2*6+ntohs(pe1.pelength)+ntohs(pe2.pelength));
}

int requestbuf_step_two(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	//header data:
	PE_HEADER ph;
	memset(&ph,'0',sizeof(ph));
	ph.flag=htons(CMD_PH_HEAD_FLAG);
	ph.version=htons(CMD_MESSAGE_VERSION_FLAG);
	ph.zipflag=0;
	ph.msgtype=htons(CMD_DTP_PU_LOGIN_STEP2_REQ);
	ph.penum=htons(0x0001);
	ph.reserved=htons(0xcccc);
	ph.msglen=htonl(0x003a);
	ph.seq=htonl(phandle->seq);

	PE pe;
	memset(&pe,0,sizeof(pe));
	pe.petype=htons(0x1000);
	pe.pelength=htons(0x0034);

	DTP_PU_LOGIN_STEP2_REQ req;
	memset(&req,0,sizeof(req));

	sint32 result = 0;
	memcpy(&result,GetPEcontext(phandle,CMD_PE_RESULT),4);
	if(ntohl(result)>0)
	{
		SYS_INFO("Login Request Step One Success.\r\n");
	}
	else
	{
		SYS_ERROR("Login Request Step One Refuesd.\r\n");
		return 0;
	}	

	//SYS_INFO("LoginID:%s.\r\n",LOGINID);
	//memcpy(req.logininfo.puid,LOGINID,sizeof(LOGINID));
	SYS_INFO("LoginID:%s.\r\n",phandle->net_param.center_loginid);
	memcpy(req.logininfo.puid,phandle->net_param.center_loginid,sizeof(phandle->net_param.center_loginid));
	SYS_INFO("req.logininfo.puid = 0s%s",req.logininfo.puid);
	SYS_INFO("GET Seed.\r\n");		
	memcpy(req.logininfo.seed,GetPEcontext(phandle,CMD_PE_CHAR16),sizeof(req.logininfo.seed));
	req.logininfo.netstat=0x0;                                                                     
	SYS_INFO("passwd MD5 encode.\r\n");

	unsigned char md[16];                                                                          
	memset(md,0,16);                                                                               
	char *str = "1234";

	MD5_CTX ctx1,ctx2;
	MD5_Init(&ctx1);
	MD5_Update(&ctx1,str,strlen(str));
	MD5_Final(md,&ctx1);

	MD5_Init(&ctx2);
	MD5_Update(&ctx2,req.logininfo.seed,sizeof(req.logininfo.seed));
	MD5_Update(&ctx2,md,sizeof(md));
	MD5_Final(md,&ctx2);
	memcpy(req.logininfo.passwd,md,16);                                                            

	memset(phandle->socketbuf,'0',sizeof(phandle->socketbuf));
	memcpy(phandle->socketbuf,&ph,sizeof(ph));                                                        
	memcpy(phandle->socketbuf+sizeof(ph),&pe,6);                                                    
	memcpy(phandle->socketbuf+sizeof(ph)+6,&req,sizeof(req));  
	//sequence up:                                                                                 
	(phandle->seq)++;
	SYS_INFO("request_step_two() end.\r\n");
	return (68+6+ntohs(pe.pelength));	
}       

void heartbeat(CLIENT_PC_PRIVATE_HANDLE *phandle){                                            
	PE_HEADER ph;                                                                                 
	memset(&ph,'0',sizeof(ph));                                                                   
	ph.flag=htons(CMD_PH_HEAD_FLAG );                                                                        
	ph.version=htons(CMD_MESSAGE_VERSION_FLAG);                                                                     
	ph.zipflag=0;                                                                                 
	ph.msgtype=htons(CMD_DTP_HEART_BEAT_REQ);                                                                      
	ph.penum=htons(0x0000);                                                                       
	ph.reserved=htons(0xcccc);                                                                    
	ph.msglen=htonl(0x0000);                                                                      
	ph.seq=htonl(0x0003);                                                                         
	memcpy(phandle->heartbeatbuf,&ph,sizeof(ph));                                                 
}

void Preview_Start_GetParam(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	struct preview_start ps;
	ushort temp = 0;
	unsigned char *buf = NULL;

	//serverIP PORT
	uchar a_uchar_ip_port[18];
	memset(&ps,0,sizeof(ps));
	SYS_INFO("Get Preview Start Request Param.\r\n ");

	//PE_CHANNEL_ID 0x1002
	buf = GetPEcontext(phandle,CMD_PE_CHANNEL_ID);
	memcpy(&temp,buf,2);
	temp=ntohs(temp);
	memcpy(&ps.pe_channel_id.channel_type,&temp,2);
	SYS_INFO("Preview Channel Type:%d.\r\n",temp);
	memcpy(&temp,buf+2,2);
	temp=ntohs(temp);
	SYS_INFO("Preview Channel Num:%d.\r\n",temp);
	memcpy(&ps.pe_channel_id.channel_num,&temp,2);

	//option 0:backnothing;1:only NETINFO;2:only CHANNELINFO;3:both
	ps.option = 1;

	//guid PE_CHAR16:0x12
	memcpy(ps.pe_char16,GetPEcontext(phandle,CMD_PE_CHAR16),16);

	//PE_NET_SCHEDULE_ADDRESS:0x1016
	//protocal trans_direction trans_protocol active_flag
	buf=GetPEcontext(phandle,CMD_PE_NET_SCHEDULE_ADDRESS);
	memcpy(&ps.pe_net_schedule_address.protocol,buf,1);
	memcpy(&ps.pe_net_schedule_address.trans_direction,buf+1,1);
	memcpy(&ps.pe_net_schedule_address.trans_protocol,buf+2,1);
	memcpy(&ps.pe_net_schedule_address.active_flag,buf+3,1);
	memset(a_uchar_ip_port,0,18);
	strcpy(a_uchar_ip_port,buf+8);
	SYS_INFO("Preview IP_PORT:%s.\r\n",a_uchar_ip_port);

	int ilen_ip_port = strlen(a_uchar_ip_port);
	ushort usport = 0;
	memcpy(&usport,a_uchar_ip_port+ilen_ip_port-2,2);                                                                
	usport=ntohs(usport);
	SYS_INFO("Preview Port:%d.\r\n",usport);     

	memcpy(ps.pe_net_schedule_address.ipaddr,a_uchar_ip_port,ilen_ip_port-2);                                       
	//memmove(&ps.pe_net_schedule_address.port,&sport,sizeof(sport));                                               
	//ps.pe_net_schedule_address.port = sport;
	memcpy(&(ps.pe_net_schedule_address.serverport), &usport, 2 );

	//copytobuffer                                                                                 
	memset(phandle->socketbuf,0,sizeof(phandle->socketbuf));                                                                
	memcpy(phandle->socketbuf,&ps,sizeof(ps)); 
}

void Preview_Stop_GetParam(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	struct preview_stop psp;
	ushort temp = 0;
	unsigned char *buf = NULL;
	SYS_INFO("Get Preview Stop Request Param.\r\n ");
	memset(&psp,0,sizeof(psp));
	//PE_CHANNEL_ID 0x1002
	buf = GetPEcontext(phandle,CMD_PE_CHANNEL_ID);
	memcpy(&temp,buf,2);
	temp=ntohs(temp);
	memcpy(&psp.pe_channel_id.channel_type,&temp,2);
	memcpy(&temp,buf+2,2);
	temp=ntohs(temp);
	memcpy(&psp.pe_channel_id.channel_num,&temp,2);

	//guid
	memcpy(psp.pe_char16,GetPEcontext(phandle,CMD_PE_CHAR16),16);

	//copytobuffer                                                                                 
	memset(phandle->socketbuf,0,sizeof(phandle->socketbuf));                                                                
	memcpy(phandle->socketbuf,&psp,sizeof(psp)); 
}

void Call_Start_GetParam( CLIENT_PC_PRIVATE_HANDLE *phandle )
{
	struct call_start    cs; 
	unsigned char *buf = NULL ;
	ushort temp = 0;
	memset(&cs,0,sizeof(cs));
	SYS_INFO("Get Preview Start Request Param.\r\n ");
	buf = GetPEcontext(phandle,CMD_PE_CHANNEL_ID);
	memcpy(&temp,buf,2);
	temp=ntohs(temp);
	memcpy(&cs.pe_channel_id.channel_type,&temp,2);
	SYS_INFO("Preview Channel Type:%d.\r\n",temp);
	memcpy(&temp, buf+2, 2);
	temp=ntohs(temp);
	SYS_INFO("Preview Channel Num:%d.\r\n",temp);
	memcpy(&cs.pe_channel_id.channel_num,&temp,2);
	buf = GetPEcontext(phandle,CMD_PE_NETADDRESS);	
}

void Call_Stop_GetParam( CLIENT_PC_PRIVATE_HANDLE *phandle )
{
	struct call_start    cs; 
	unsigned char *buf = NULL ;
	ushort temp = 0;
	memset(&cs,0,sizeof(cs));
	SYS_INFO("Get Preview Start Request Param.\r\n ");
	buf = GetPEcontext(phandle,CMD_PE_CHANNEL_ID);
	memcpy(&temp,buf,2);
	temp=ntohs(temp);
	memcpy(&cs.pe_channel_id.channel_type,&temp,2);
	SYS_INFO("Preview Channel Type:%d.\r\n",temp);
	memcpy(&temp, buf+2, 2);
	temp=ntohs(temp);
	SYS_INFO("Preview Channel Num:%d.\r\n",temp);
}

void Control_Ptz_GetParam( CLIENT_PC_PRIVATE_HANDLE *phandle )
{
	ctl_ptz_request cpr;
	ushort temp = 0;
	uchar *buf = NULL;
	int len = 0;
	memset (&cpr,0,sizeof(cpr));
	SYS_INFO("Get Ptz Request Param.\r\n ");
	//PE_CHANNEL_ID 0x1002
	buf = GetPEcontext(phandle,CMD_PE_PTZ_CMD);

	//拷贝CHANNEL_ID
	memcpy(&temp,buf,sizeof(temp));
	temp=ntohs(temp);
	memcpy(&cpr.pe_channel_id.channel_type,&temp,sizeof(cpr.pe_channel_id.channel_type));
	len = sizeof(cpr.pe_channel_id.channel_type);
	SYS_INFO("Ptz Channel Type:%d.\r\n",temp);
	memcpy(&temp,buf+len,sizeof(temp));
	temp=ntohs(temp);
	SYS_INFO("Ptz Channel Num:%d.\r\n",temp);
	memcpy(&cpr.pe_channel_id.channel_num,&temp,sizeof(cpr.pe_channel_id.channel_num));
	len = len+sizeof(cpr.pe_channel_id.channel_num);

	//拷贝控制命令
	memcpy(&cpr.ctl_cmd, buf+len,sizeof(cpr.ctl_cmd));
	len = len+sizeof(cpr.ctl_cmd);

	//拷贝控制参数
	memcpy(&cpr.ctl_value, buf+len,sizeof(cpr.ctl_value));
	memset(phandle->socketbuf,0,sizeof(phandle->socketbuf));     
	memcpy(phandle->socketbuf,&cpr,sizeof(cpr));
}

void GetParam(CLIENT_PC_PRIVATE_HANDLE *phandle,ushort type)
{
	struct preview_start ps;
	struct preview_stop psp;

	ushort temp = 0;
	unsigned char *buf = NULL ;

	//SYS_INFO("Get Preview Start Request Param.\r\n ");
	switch(type)
	{
		case CMD_DTP_PREVIEW_START_REQ:  
			Preview_Start_GetParam(phandle);
			break;

			//end_preview_start_request
		case CMD_DTP_PREVIEW_STOP_REQ:
			Preview_Stop_GetParam(phandle);
			break;

			//收到音频请求后参数解析处理
		case CMD_DTP_CALL_START_REQ:
			Call_Start_GetParam(phandle);
			break;

			//收到音频停止请求后处理
		case CMD_DTP_CALL_STOP_REQ:
			Call_Stop_GetParam(phandle);
			break;

			//收到摄像机控制请求后处理
		case CMD_DTP_CONTROL_PTZ_REQ:
			Control_Ptz_GetParam(phandle);
			break;
		default:
			break;
	}//end_switch                                                                                          
}        

int Message_response(CLIENT_PC_PRIVATE_HANDLE *phandle, int messege_no, int channel_type )
{
	PE_HEADER ph;
	PE pe;
	int mlen = 0;
	int result = 0;
	char ip_addr[32];
	uint32 len = 0;
	uint32 Ip_Len = 0;
	unsigned short tempport =0;
	memset(&ph,0,sizeof(ph));
	memset(&pe,0,sizeof(pe));
	memset(ip_addr,0,sizeof(ip_addr));
	ph.flag=htons(CMD_PH_HEAD_FLAG);
	ph.zipflag=0;                                                                                 
	ph.msgtype=htons(messege_no); 
	if((channel_type == CMD_AUDIO_FLAG) &&( messege_no == CMD_DTP_PREVIEW_START_RES))
	{
		ph.penum=htons(0x0002); 
		//mlen = 0xa+0x06+strlen(LOCAL_IP)+sizeof(Ip_Len)+sizeof(tempport)+2*sizeof(char);
		mlen = 0xa+0x06+strlen(phandle->net_param.ipaddr)+sizeof(Ip_Len)+sizeof(tempport)+2*sizeof(char);
		//ph.msglen=htonl(37);
		ph.msglen=htonl(mlen); 
	}
	else 
	{
		ph.penum=htons(0x0001); 
		ph.msglen=htonl(0xa); 
	}
	ph.reserved=htons(0xcccc);                                                                                                                                
	memcpy(&ph.seq,phandle->socketbuf+16,4);
	pe.petype=htons(CMD_PE_RESULT);
	pe.pelength=htons(sizeof(result));
	memcpy(phandle->socketbuf2,&ph,sizeof(ph));
	memcpy(phandle->socketbuf2+sizeof(ph),&pe,6);
	memcpy(phandle->socketbuf2+sizeof(ph)+6,&result,sizeof(result));
	if ((channel_type == CMD_AUDIO_FLAG) &&( messege_no == CMD_DTP_PREVIEW_START_RES))
	{
		pe.petype=htons(CMD_PE_NETADDRESS);
		//pe.pelength=htons(strlen(LOCAL_IP)+sizeof(short)+2*sizeof(char)+sizeof(Ip_Len));
		pe.pelength=htons(strlen(phandle->net_param.ipaddr)+sizeof(short)+2*sizeof(char)+sizeof(Ip_Len));
		len = sizeof(PE_HEADER) + 0x06+sizeof(result);
		memcpy(phandle->socketbuf2+len,&pe,6);
		len = len+0x06;
		//memcpy(phandle->socketbuf2+len,ip_addr,strlen(ip_addr));
		//Ip_Len = htonl(strlen(LOCAL_IP));
		Ip_Len = htonl(strlen(phandle->net_param.ipaddr));
		memcpy(phandle->socketbuf2+len, &Ip_Len, sizeof(Ip_Len));
		len = len+sizeof(Ip_Len);
		//memcpy(phandle->socketbuf2+len, LOCAL_IP, strlen(LOCAL_IP));
		//len =len+strlen(LOCAL_IP);
		memcpy(phandle->socketbuf2+len, phandle->net_param.ipaddr, strlen(phandle->net_param.ipaddr));
		len =len+strlen(phandle->net_param.ipaddr);
		//tempport = htons(LOCAL_PORT);
		tempport = htons(phandle->net_param.port);
		memcpy(phandle->socketbuf2+len, &tempport, sizeof(tempport));
		len = len+sizeof(tempport);
		memcpy(phandle->socketbuf2+len,GetPEcontext(phandle,CMD_PE_NET_SCHEDULE_ADDRESS),sizeof(char));
		len = len+2*sizeof(char);
		return len;
	}
	else
	{
		return (sizeof(PE_HEADER) + 0x06 + sizeof(result));
	}
}

