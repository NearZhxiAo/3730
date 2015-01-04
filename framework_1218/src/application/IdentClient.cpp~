#include "IdentClient.h"
#include "ParamMgr.h"

#define IDENT_CLIENT_SCAN_PORT 8888 
#define IDENT_CLIENT_SVRLISTEN_PORT 8000 
//#define IDENT_CLIENT_TIME_ID_HEARTBEAT 0x00001000
#define IDENT_CLIENT_TIME_ID_DEAD      0x00001001
//#define IDENT_CLIENT_TIME_VAL_HEARTBEAT 20*1000 //ms
#define IDENT_CLIENT_TIME_VAL_DEAD      20*1000*3//ms

static bool g_bIsConnected = false;
bool IdentClient::m_IsSvrConnected = false;
bool IdentClient::m_IsSvrThreadExit = false;

bool g_SimAlarmOne = false; 

static bool SendHeartBeat();

IdentClient::IdentClient()
{
	m_IsSvrConnected = false;
	m_IsSvrThreadExit = false;
	m_tachReg = false;
}

IdentClient::~IdentClient()
{
	m_tachReg = false;
}

bool IdentClient::Initilize()
{
	CreateAllThreads();
	return true;
}

bool IdentClient::CreateAllThreads()
{

	if (pthread_create(&m_hThreadForScanHandle, NULL, ThreadForScanFunc, this))
	{
		SYS_ERROR("Failed to create flash_updatethread_func thread\n");
		return false;
	}


    if (pthread_create(&m_hThreadForCommHandle, NULL, ThreadForCommFunc, this))
	{
		SYS_ERROR("Failed to create flash_updatethread_func thread\n");
		return false;
	}


	return true;
}


void*  IdentClient::ThreadForScanFunc(void * pParam)
{
	IdentClient *pThis = (IdentClient *)pParam;
	int SockScan;
	CMD_TRANSFER cmd_transfer;
	
	if(!UDP_CREATE((int *)&SockScan))
    {
		UDP_BIND(SockScan, IDENT_CLIENT_SCAN_PORT);
		unsigned char RecvSockIP[128] = {0};
		unsigned int RecvSockLen = sizeof(cmd_transfer);
		unsigned short RecvSockPort = 0;
		while(!pThis->m_IsSvrThreadExit)
		{
			memset(&cmd_transfer,0x00,sizeof(cmd_transfer));
			if(0 == UDP_RECV(SockScan,(unsigned char *)&cmd_transfer,&RecvSockLen,(unsigned char *)RecvSockIP,(unsigned short *)&RecvSockPort))
			//if(recvfrom(SockScan,(char *)&cmd_transfer,RecvSockLen,0,&RecvSockAddr,(int *)&RecvSockLen) >= 0)
			{
				int cmd;
				char RecvBuff[1024] = {0};
				if (DECONSTRUCT_CMD(&cmd_transfer,(char *)RecvBuff,&cmd) && (CMD_NUM_SCAN == cmd))//扫描信息
				{
					SERVER_LIST server_list;
					//if (!strcmp((char *)g_stGlobal_Variable.globalflash.sysconfigpara.camipconfig.ipaddr,""))
					{
						char tmp[1024] = {0};
						unsigned long Buff_size = sizeof(server_list.servername);
						//GetComputerName((char *)server_list.servername, &Buff_size); //kuangren
						//设置默认识别器ip地址
						struct hostent *p_HostEnt;
						//p_HostEnt = gethostbyname((char *)server_list.servername); //kuangren
						if(p_HostEnt != NULL)
						{
#if 0
							sprintf( (char *)g_stGlobal_Variable.globalflash.sysconfigpara.camipconfig.ipaddr, "%d.%d.%d.%d",
								( p_HostEnt->h_addr_list[0][0] & 0x00ff ),
								( p_HostEnt->h_addr_list[0][1] & 0x00ff ),
								( p_HostEnt->h_addr_list[0][2] & 0x00ff ),
								( p_HostEnt->h_addr_list[0][3] & 0x00ff ) );
#endif
						}
					}
#if 0
					strcpy((char *)server_list.ipaddr,(char *)g_stGlobal_Variable.globalflash.sysconfigpara.camipconfig.ipaddr);
					if (strcmp((char *)server_list.servername,(char *)g_stGlobal_Variable.globalflash.sysconfigpara.camconfig.dev_name))
					{
						strcpy((char *)server_list.servername,(char *)g_stGlobal_Variable.globalflash.sysconfigpara.camconfig.dev_name);
					}
					server_list.cam_state = g_stGlobal_Variable.globalflash.sysconfigpara.camstate;
					server_list.flagvalid = true;
					CONSTRUCT_CMD(CMD_NUM_SCAN_RESPONSE,(char *)&server_list,sizeof(server_list),&cmd_transfer);
#endif
					UDP_SEND(SockScan,(unsigned char *)&cmd_transfer,sizeof(cmd_transfer),(unsigned char *)RecvSockIP,RecvSockPort);
				}

			}
			sleep(2);
		}
	}
	else
	{
		
	}
	return 0;
}

void*  IdentClient::ThreadForCommFunc(void * pParam)
{
    IdentClient *pThis = (IdentClient *)pParam;
	int SockSvrListen;
	IDENT_CLIENT_CONN_STRUCT ClientConnect;
	int SockSvrMax;
	int iRetApi = -1;
	int SocketTmp;

	iRetApi = TCP_CREATE((int *)&SockSvrListen);
	iRetApi = TCP_BIND(SockSvrListen,IDENT_CLIENT_SVRLISTEN_PORT);
	
	timeval TimeVal; 
	TimeVal.tv_sec = 1;
	TimeVal.tv_usec = 0;

	fd_set SockSvrFdSet;

	SockSvrMax = SockSvrListen + 1;
	while(!pThis->m_IsSvrThreadExit)
	{
 		FD_ZERO(&SockSvrFdSet);
		FD_SET(SockSvrListen,&SockSvrFdSet);
		if(!g_bIsConnected)
		{
			pThis->m_IsSvrConnected = false;
		}
		if(pThis->m_IsSvrConnected)
		{
			FD_SET(ClientConnect.Sock,&SockSvrFdSet);
			SockSvrMax = (SockSvrListen > ClientConnect.Sock) ?  (SockSvrListen+1) : (ClientConnect.Sock+1);
		}

		if (select(SockSvrMax,&SockSvrFdSet,NULL,NULL,&TimeVal) > 0)
		{
			if (FD_ISSET(SockSvrListen,&SockSvrFdSet))
			{
				iRetApi = TCP_ACCEPT(SockSvrListen,(int *)&SocketTmp);
				if(0 == iRetApi)
				{
					int status = 0;
					if(!g_bIsConnected || !pThis->m_IsSvrConnected)
					{
						status = 0;
						TCP_SEND(SocketTmp,(unsigned char *)&status,sizeof(status));
						ClientConnect.Sock = SocketTmp;
						g_stGlobal_Variable.client_tcpsocket.socket_it = SocketTmp;//设置全局的TCP
						//AUTO:
						//开启定时器 ClientConnect.Timer
						pThis->m_IsSvrConnected = true;
						g_bIsConnected = true;
						//SetTimer(pThis->m_hWnd,IDENT_CLIENT_TIME_ID_HEARTBEAT,IDENT_CLIENT_TIME_VAL_HEARTBEAT,pThis->TimerOutProc);
						//SetTimer(pThis->m_hWnd,IDENT_CLIENT_TIME_ID_DEAD,IDENT_CLIENT_TIME_VAL_DEAD,pThis->TimerOutProc);
					}
					else
					{
						status = 1;
						TCP_SEND(SocketTmp,(unsigned char *)&status,sizeof(status));
					}
				}
			}
			if (FD_ISSET(ClientConnect.Sock,&SockSvrFdSet))//接收到客户端命令
			{
				CMD_TRANSFER cmd_transfer;
				int cmd;
				char RecvBuff[1300] = {0};
				unsigned int RecvLen = sizeof(cmd_transfer);
				if (0 == TCP_RECV(ClientConnect.Sock,(unsigned char *)&cmd_transfer,&RecvLen))
				{
// 					if (RecvLen <= 0)//接收到的数据为0 说明socket断开 -1则出错
// 					{
// 						g_bIsConnected = false;
// 						pThis->m_IsSvrConnected = false;
// 						//CString cstrErrorInfo;
// 						//cstrErrorInfo.Format("接受客户端数据出错 断开连接! m_IsSvrConnected = %d\r\n",pThis->m_IsSvrConnected);
// 						//WRITE_LOG(cstrErrorInfo,LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
// 					}
					if(DECONSTRUCT_CMD(&cmd_transfer,(char *)RecvBuff,&cmd))
					{
						//AUTO:
						pThis->CommandProc((unsigned char *)RecvBuff,cmd);//专项处理接收到的消息
					}

					/*switch (cmd)
					{
					case CMD_NUM_PTZ:
						{
							PTZ_PARA ptz_para;
							ptz_para = *(PTZ_PARA *)RecvBuff;					
						}
						break;
					}*/
				}
			}
		}
	}
	return 0;
}

#if 1
bool IdentClient::CommandProc(unsigned char *buf, unsigned int cmd)
{
	switch(cmd)
	{
	case CMD_NUM_PTZ:
		{
			SendParaPtz();
		}
		break;
	case  CMD_NUM_VIDEO:
		{
			SendParaVideo();
		}
		break;
	case  CMD_NUM_ALG:
		{
			SendParaAlg();
		}
		break;
	case  CMD_NUM_ALG_CONF_00:
	case  CMD_NUM_ALG_CONF_01:
	case  CMD_NUM_ALG_CONF_02:
	case  CMD_NUM_ALG_CONF_03:
	case  CMD_NUM_ALG_CONF_04:
		{
			SendParaAlgConf(cmd);
		}
		break;
	case  CMD_NUM_ALG_QUAD_000003:
	case  CMD_NUM_ALG_QUAD_004007:
	case  CMD_NUM_ALG_QUAD_008011:
	case  CMD_NUM_ALG_QUAD_012015:
	case  CMD_NUM_ALG_QUAD_016019:
	case  CMD_NUM_ALG_QUAD_020023:
	case  CMD_NUM_ALG_QUAD_024027:
	case  CMD_NUM_ALG_QUAD_028031:
	case  CMD_NUM_ALG_QUAD_032035:
	case  CMD_NUM_ALG_QUAD_036039:
	case  CMD_NUM_ALG_QUAD_040043:
	case  CMD_NUM_ALG_QUAD_044047:
	case  CMD_NUM_ALG_QUAD_048051:
	case  CMD_NUM_ALG_QUAD_052055:
	case  CMD_NUM_ALG_QUAD_056059:
	case  CMD_NUM_ALG_QUAD_060063:
	case  CMD_NUM_ALG_QUAD_064067:
	case  CMD_NUM_ALG_QUAD_068071:
	case  CMD_NUM_ALG_QUAD_072075:
	case  CMD_NUM_ALG_QUAD_076079:
	case  CMD_NUM_ALG_QUAD_080083:
	case  CMD_NUM_ALG_QUAD_084087:
	case  CMD_NUM_ALG_QUAD_088091:
	case  CMD_NUM_ALG_QUAD_092095:
	case  CMD_NUM_ALG_QUAD_096099:
	case  CMD_NUM_ALG_QUAD_100103:
	case  CMD_NUM_ALG_QUAD_104107:
	case  CMD_NUM_ALG_QUAD_108111:
	case  CMD_NUM_ALG_QUAD_112115:
	case  CMD_NUM_ALG_QUAD_116119:
	case  CMD_NUM_ALG_QUAD_120123:
	case  CMD_NUM_ALG_QUAD_124127:
		{
			SendParaAlgQuad(cmd);
		}
		break;
	case  CMD_NUM_ALG_AREA_000003:
	case  CMD_NUM_ALG_AREA_004007:
	case  CMD_NUM_ALG_AREA_008011:
	case  CMD_NUM_ALG_AREA_012015:
	case  CMD_NUM_ALG_AREA_016019:
	case  CMD_NUM_ALG_AREA_020023:
	case  CMD_NUM_ALG_AREA_024027:
	case  CMD_NUM_ALG_AREA_028031:
		{
			SendParaAlgArea(cmd);		
		}
		break;
	case  CMD_NUM_ALG_LINE_000003:
	case  CMD_NUM_ALG_LINE_004007:
	case  CMD_NUM_ALG_LINE_008011:
	case  CMD_NUM_ALG_LINE_012015:
		{
			SendParaAlgLine(cmd);			
		}
		break;
	case  CMD_NUM_ALG_COUNT_000003:
	case  CMD_NUM_ALG_COUNT_004007:
	case  CMD_NUM_ALG_COUNT_008011:
	case  CMD_NUM_ALG_COUNT_012015:
		{
			SendParaAlgCount(cmd);			
		}
		break;
	case  CMD_NUM_ALG_ENTER_000003:
	case  CMD_NUM_ALG_ENTER_004007:
	case  CMD_NUM_ALG_ENTER_008011:
	case  CMD_NUM_ALG_ENTER_012015:
		{
			SendParaAlgEnter(cmd);				
		}
		break;
	case  CMD_NUM_ALG_OUT_000003:
	case  CMD_NUM_ALG_OUT_004007:
	case  CMD_NUM_ALG_OUT_008011:
	case  CMD_NUM_ALG_OUT_012015:
		{
			SendParaAlgOut(cmd);			
		}
		break;
	
	case  CMD_NUM_ALG_FINDBAG_000003:
	case  CMD_NUM_ALG_FINDBAG_004007:
	case  CMD_NUM_ALG_FINDBAG_008011:
	case  CMD_NUM_ALG_FINDBAG_012015:
		{
			SendParaAlgFindBag(cmd);			
		}
		break;
	case  CMD_NUM_ALG_LOST_000002:
	case  CMD_NUM_ALG_LOST_003005:
		{
			SendParaAlgLost(cmd);			
		}
		break;
	
	case  CMD_NUM_ALG_DIRECT_000003:
	case  CMD_NUM_ALG_DIRECT_004007:
		{
			SendParaAlgDirect(cmd);			
		}
		break;
	case  CMD_NUM_ALG_DIRECT_008011:
	case  CMD_NUM_ALG_DIRECT_012015:
		{
			SendParaAlgDirect(cmd);			
		}
		break;
	
	case  CMD_NUM_ALG_DETECT_000003:
	case  CMD_NUM_ALG_DETECT_004007:
		{
			SendParaAlgDetect(cmd);			
		}
		break;
	case  CMD_NUM_ALG_DETECT_008011:
	case  CMD_NUM_ALG_DETECT_012015:
		{
			SendParaAlgDetect(cmd);			
		}
		break;
	case  CMD_NUM_ALG_PEOPLEDENSE_000003:
	case  CMD_NUM_ALG_PEOPLEDENSE_004007:
	case  CMD_NUM_ALG_PEOPLEDENSE_008011:
	case  CMD_NUM_ALG_PEOPLEDENSE_012015:
		{
			SendParaAlgPeopleDense(cmd);			
		}
		break;

	case  CMD_NUM_ALG_CLUSTERDETE_000003:
	case  CMD_NUM_ALG_CLUSTERDETE_004007:
	case  CMD_NUM_ALG_CLUSTERDETE_008011:
	case  CMD_NUM_ALG_CLUSTERDETE_012015:
		{
			SendParaAlgClusterDete(cmd);			
		}
		break;
	case  CMD_NUM_ALG_MOTIONHISTORY_000003:
	case  CMD_NUM_ALG_MOTIONHISTORY_004007:
	case  CMD_NUM_ALG_MOTIONHISTORY_008011:
	case  CMD_NUM_ALG_MOTIONHISTORY_012015:
		{
			SendParaAlgMotionHistory(cmd);			
		}
		break;

	case  CMD_NUM_ALG_HOVERDETE_000003:
	case  CMD_NUM_ALG_HOVERDETE_004007:
	case  CMD_NUM_ALG_HOVERDETE_008011:
	case  CMD_NUM_ALG_HOVERDETE_012015:
		{
			SendParaAlgHoverDete(cmd);			
		}
		break;
	case  CMD_NUM_ALG_ALARMLINE_000003:
	case  CMD_NUM_ALG_ALARMLINE_004007:
	case  CMD_NUM_ALG_ALARMLINE_008011:
	case  CMD_NUM_ALG_ALARMLINE_012015:
		{
			SendParaAlgAlarmLine(cmd);			
		}
		break;
	case  CMD_NUM_ALG_RUNNING_000003://获取人员奔跑形状8-11个区域配置请求ALG_DETECT_PARA
	case  CMD_NUM_ALG_RUNNING_004007://获取人员奔跑形状12-15个区域配置请求ALG_DETECT_PARA
	case  CMD_NUM_ALG_RUNNING_008011://获取人员奔跑形状12-15个区域配置请求ALG_DETECT_PARA
	case  CMD_NUM_ALG_RUNNING_012015://获取人员奔跑形状12-15个区域配置请求ALG_DETECT_PARA
		{
			SendParaAlgRunning(cmd);			
		}
		break;

	case  CMD_NUM_SYS_CONFIG://获取系统配置请求SYS_CONFIG_PARA
		{
			SendParaSysConfig();
		}
		break;
	case  CMD_NUM_ALARM_YM://获取报警目录信息请求ALARM_YM_PARA
		{
			SendParaAlarmYM();
		}	
		break;
	case  CMD_NUM_PIC_CONTENTS://获取报警照片请求PIC_CONTENTS
		{
			SendParaPicContents(buf);
		}
		break;
	case  CMD_NUM_SYS_STORAGE://获取系统存储参数SYS_STORAGE_PARA
		{
			SendParaSysStorage();
		}
		break;
	case  CMD_NUM_ALARM_DAY://获取报警目录信息（指定年月里面的日消息）ALARM_DAY_PARA
		{
			SendParaAlarmDay();
		}
		break;
	case  CMD_NUM_ALARM_NAME://获取某日内的报警图片名称ALARM_NAME_PARA
		{
			SendParaAlarName();
		}
		break;

	case  CMD_NUM_PTZ_SET://设置服务器云台参数 云台参数（PTZ_PARA）
		{
			PTZ_PARA ptz_para;
			ptz_para = *(PTZ_PARA *)buf;
			FlashParam_SetPtzPara(ptz_para);
			SendParaSetAck(cmd,true);
		}
		break;


	case  CMD_NUM_VIDEO_SET://设置服务器视频一般配置参数 视频源、编码参数、视频色彩配置VIDEO_PARA
		{
#if 1
			VIDEO_PARA video_para;
			video_para = *(VIDEO_PARA *)buf;
			if (video_para.src == VIDEO_SRC_ANALOG && video_para.analoginfo.vframetype != g_stGlobal_Variable.globalflash.videopara.analoginfo.vframetype)
			{
				g_VideoResolutionChanged = true;
			}
			FlashParam_SetVideoPara(video_para);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	case  CMD_NUM_ALG_SET://设置算法参数配置 ALG_PARA
		{
#if 1
			ALG_PARA alg_para;
			alg_para = *(ALG_PARA *)buf;
			FlashParam_SetAlgPara(alg_para);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	case CMD_NUM_ALG_CONF_00_SET:
	case CMD_NUM_ALG_CONF_01_SET:
	case CMD_NUM_ALG_CONF_02_SET:
	case CMD_NUM_ALG_CONF_03_SET:
	case CMD_NUM_ALG_CONF_04_SET:
		{
#if 1
			ALG_PARA_CONF_UINT alg_para_conf_uint;
			alg_para_conf_uint = *(ALG_PARA_CONF_UINT*)buf;
			FlashParam_SetAlgParaConf(&alg_para_conf_uint,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	case  CMD_NUM_SYS_CONFIG_SET://设置算法参数配置 ALG_PARA
		{
			SYS_CONFIG_PARA sys_config_para;
			sys_config_para = *(SYS_CONFIG_PARA *)buf;
			FlashParam_SetSysConfigPara(sys_config_para);
			SendParaSetAck(CMD_NUM_SYS_CONFIG_RESPONSE,false);
		}
		break;
	case  CMD_NUM_ALG_QUAD_000003_SET://设置回字形状前0-3个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_004007_SET://设置回字形状前4-7个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_008011_SET://设置回字形状前8-11个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_012015_SET://设置回字形状前12-15个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_016019_SET://设置回字形状前16-20个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_020023_SET://设置回字形状前20-23个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_024027_SET://设置回字形状前24-27个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_028031_SET://设置回字形状前28-31个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_032035_SET://设置回字形状前32-35个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_036039_SET://设置回字形状前36-39个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_040043_SET://设置回字形状前40-43个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_044047_SET://设置回字形状前44-47个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_048051_SET://设置回字形状前48-51个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_052055_SET://设置回字形状前52-55个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_056059_SET://设置回字形状前56-59个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_060063_SET://设置回字形状前60-63个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_064067_SET://设置回字形状前64-67个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_068071_SET://设置回字形状前68-71个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_072075_SET://设置回字形状前72-75个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_076079_SET://设置回字形状前76-79个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_080083_SET://设置回字形状前80-83个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_084087_SET://设置回字形状前84-87个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_088091_SET://设置回字形状前88-91个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_092095_SET://设置回字形状前92-95个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_096099_SET://设置回字形状前96-100个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_100103_SET://设置回字形状前101-103个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_104107_SET://设置回字形状前104-107个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_108111_SET://设置回字形状前108-111个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_112115_SET://设置回字形状前112-115个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_116119_SET://设置回字形状前116-119个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_120123_SET://设置回字形状前120-123个区域配置ALG_QUAD_PARA
	case  CMD_NUM_ALG_QUAD_124127_SET://设置回字形状前124-127个区域配置ALG_QUAD_PARA
		{
#if 1
			ALG_QUAD_PARA alg_quad_para;
			alg_quad_para = *(ALG_QUAD_PARA *)buf;
			FlashParam_SetAlgQuadPara(&alg_quad_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	case  CMD_NUM_ALG_AREA_000003_SET://设置禁区形状0-3个区域配置ALG_AREA_PARA
	case  CMD_NUM_ALG_AREA_004007_SET://设置禁区形状4-7个区域配置ALG_AREA_PARA
	case  CMD_NUM_ALG_AREA_008011_SET://设置禁区形状8-11个区域配置ALG_AREA_PARA
	case  CMD_NUM_ALG_AREA_012015_SET://设置禁区形状12-15个区域配置ALG_AREA_PARA
	case  CMD_NUM_ALG_AREA_016019_SET://设置禁区形状16-20个区域配置ALG_AREA_PARA
	case  CMD_NUM_ALG_AREA_020023_SET://设置禁区形状20-23个区域配置ALG_AREA_PARA
	case  CMD_NUM_ALG_AREA_024027_SET://设置禁区形状24-27个区域配置ALG_AREA_PARA
	case  CMD_NUM_ALG_AREA_028031_SET://设置禁区形状28-31个区域配置ALG_AREA_PARA
		{
#if 1
			ALG_AREA_PARA alg_area_para;
			alg_area_para = *(ALG_AREA_PARA *)buf;	
			FlashParam_SetAlgAreaPara(&alg_area_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_LINE_000003_SET://设置拌线形状0-3个区域配置ALG_LINE_PARA
    case  CMD_NUM_ALG_LINE_004007_SET://设置拌线形状4-7个区域配置ALG_LINE_PARA
	case  CMD_NUM_ALG_LINE_008011_SET://设置拌线形状8-11个区域配置ALG_LINE_PARA
	case  CMD_NUM_ALG_LINE_012015_SET://设置拌线形状12-15个区域配置ALG_LINE_PARA
		{
#if 1
			ALG_LINE_PARA alg_line_para;
			alg_line_para = *(ALG_LINE_PARA *)buf;
			FlashParam_SetAlgLinePara(&alg_line_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_COUNT_000003_SET://设置计数形状0-3个区域配置ALG_COUNT_PARA
	case  CMD_NUM_ALG_COUNT_004007_SET://设置计数形状4-7个区域配置ALG_COUNT_PARA
	case  CMD_NUM_ALG_COUNT_008011_SET://设置计数形状8-11个区域配置ALG_COUNT_PARA
	case  CMD_NUM_ALG_COUNT_012015_SET://设置计数形状12-15个区域配置ALG_COUNT_PARA
		{
#if 1
			ALG_COUNT_PARA alg_count_para;
			alg_count_para = *(ALG_COUNT_PARA *)buf;
			FlashParam_SetAlgCountPara(&alg_count_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_ENTER_000003_SET://设置目标进入形状0-3个区域配置ALG_ENTER_PARA
	case  CMD_NUM_ALG_ENTER_004007_SET://设置目标进入形状4-7个区域配置ALG_ENTER_PARA
	case  CMD_NUM_ALG_ENTER_008011_SET://设置目标进入形状8-11个区域配置ALG_ENTER_PARA
	case  CMD_NUM_ALG_ENTER_012015_SET://设置目标进入形状12-15个区域配置ALG_ENTER_PARA
		{
#if 1
			ALG_ENTER_PARA alg_enter_para;
			alg_enter_para = *(ALG_ENTER_PARA *)buf;
			FlashParam_SetAlgEnterPara(&alg_enter_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_OUT_000003_SET://设置目标离开形状0-3个区域配置ALG_OUT_PARA
	case  CMD_NUM_ALG_OUT_004007_SET://设置目标离开形状4-7个区域配置ALG_OUT_PARA
	case  CMD_NUM_ALG_OUT_008011_SET://设置目标离开形状8-11个区域配置ALG_OUT_PARA
	case  CMD_NUM_ALG_OUT_012015_SET://设置目标离开形状12-15个区域配置ALG_OUT_PARA
		{
#if 1
			ALG_OUT_PARA alg_out_para;
			alg_out_para = *(ALG_OUT_PARA *)buf;
			FlashParam_SetAlgOutPara(&alg_out_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_FINDBAG_000003_SET://设置物体遗留形状0-2个区域配置ALG_FINDBAG_PARA
	case  CMD_NUM_ALG_FINDBAG_004007_SET://设置物体遗留形状3-5个区域配置ALG_FINDBAG_PARA
	case  CMD_NUM_ALG_FINDBAG_008011_SET://设置物体遗留形状3-5个区域配置ALG_FINDBAG_PARA
	case  CMD_NUM_ALG_FINDBAG_012015_SET://设置物体遗留形状3-5个区域配置ALG_FINDBAG_PARA
		{
#if 1
			ALG_FINDBAG_PARA alg_findbag_para;
			alg_findbag_para = *(ALG_FINDBAG_PARA *)buf;
			FlashParam_SetAlgFindbagPara(&alg_findbag_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_LOST_000002_SET://设置物体消失形状0-2个区域配置ALG_LOST_PARA
	case  CMD_NUM_ALG_LOST_003005_SET://设置物体消失形状3-5个区域配置ALG_LOST_PARA
		{
#if 1
			ALG_LOST_PARA alg_lost_para;
			alg_lost_para = *(ALG_LOST_PARA *)buf;
			FlashParam_SetAlgLostPara(&alg_lost_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_DIRECT_000003_SET://设置方向报警形状0-3个区域配置ALG_DIRECT_PARA
	case  CMD_NUM_ALG_DIRECT_004007_SET://设置方向报警形状4-7个区域配置ALG_DIRECT_PARA
	case  CMD_NUM_ALG_DIRECT_008011_SET://设置方向报警形状8-11个区域配置ALG_DIRECT_PARA
	case  CMD_NUM_ALG_DIRECT_012015_SET://设置方向报警形状12-15个区域配置ALG_DIRECT_PARA
		{
#if 1
			ALG_DIRECT_PARA alg_direct_para;
			alg_direct_para = *(ALG_DIRECT_PARA *)buf;
			FlashParam_SetAlgDirectPara(&alg_direct_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_DETECT_000003_SET://设置入侵侦测形状0-3个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_DETECT_004007_SET://设置入侵侦测形状4-7个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_DETECT_008011_SET://设置入侵侦测形状8-11个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_DETECT_012015_SET://设置入侵侦测形状12-15个区域配置ALG_DETECT_PARA
		{
#if 1
			ALG_DETECT_PARA alg_detect_para;
			alg_detect_para = *(ALG_DETECT_PARA *)buf;
			FlashParam_SetAlgDetectPara(&alg_detect_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	
	case  CMD_NUM_ALG_PEOPLEDENSE_000003_SET://设置人员密度测形状0-3个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_PEOPLEDENSE_004007_SET://设置人员密度形状4-7个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_PEOPLEDENSE_008011_SET://设置人员密度形状8-11个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_PEOPLEDENSE_012015_SET://设置人员密度形状12-15个区域配置ALG_DETECT_PARA
		{
#if 1
			ALG_PEOPLEDENSE_PARA alg_peopeldense_para;
			alg_peopeldense_para = *(ALG_PEOPLEDENSE_PARA *)buf;
			FlashParam_SetAlgPeopleDensePara(&alg_peopeldense_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
		
	case  CMD_NUM_ALG_CLUSTERDETE_000003_SET://设置人员聚集形状0-3个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_CLUSTERDETE_004007_SET://设置人员聚集形状4-7个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_CLUSTERDETE_008011_SET://设置人员聚集形状8-11个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_CLUSTERDETE_012015_SET://设置人员聚集形状12-15个区域配置ALG_DETECT_PARA
		{
#if 1
			ALG_CLUSTERDETE_PARA alg_clusterdete_para;
			alg_clusterdete_para = *(ALG_CLUSTERDETE_PARA *)buf;
			FlashParam_SetAlgClusterDetePara(&alg_clusterdete_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
		
	case  CMD_NUM_ALG_MOTIONHISTORY_000003_SET://设置夜间起床形状0-3个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_MOTIONHISTORY_004007_SET://设置夜间起床形状4-7个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_MOTIONHISTORY_008011_SET://设置夜间起床形状8-11个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_MOTIONHISTORY_012015_SET://设置夜间起床形状12-15个区域配置ALG_DETECT_PARA
		{
#if 1
			ALG_MOTIONHISTORY_PARA alg_motionhistory_para;
			alg_motionhistory_para = *(ALG_MOTIONHISTORY_PARA *)buf;
			FlashParam_SetAlgMotionHistoryPara(&alg_motionhistory_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
		
	case  CMD_NUM_ALG_HOVERDETE_000003_SET://设置徘徊形状0-3个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_HOVERDETE_004007_SET://设置徘徊形状4-7个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_HOVERDETE_008011_SET://设置徘徊形状8-11个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_HOVERDETE_012015_SET://设置徘徊形状12-15个区域配置ALG_DETECT_PARA
		{
#if 1
			ALG_HOVERDETE_PARA alg_hoverdete_para;
			alg_hoverdete_para = *(ALG_HOVERDETE_PARA *)buf;
			FlashParam_SetAlgHoverDetePara(&alg_hoverdete_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;

	case  CMD_NUM_ALG_ALARMLINE_000003_SET://设置摸高形状0-3个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_ALARMLINE_004007_SET://设置摸高形状4-7个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_ALARMLINE_008011_SET://设置摸高形状8-11个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_ALARMLINE_012015_SET://设置摸高形状12-15个区域配置ALG_DETECT_PARA
		{
#if 1
			ALG_ALARMLINE_PARA alg_alarmline_para;
			alg_alarmline_para = *(ALG_ALARMLINE_PARA *)buf;
			FlashParam_SetAlgAlarmLinePara(&alg_alarmline_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;

	case  CMD_NUM_ALG_RUNNING_000003_SET://设置奔跑形状0-3个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_RUNNING_004007_SET://设置奔跑形状4-7个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_RUNNING_008011_SET://设置奔跑形状8-11个区域配置ALG_DETECT_PARA
	case  CMD_NUM_ALG_RUNNING_012015_SET://设置奔跑形状12-15个区域配置ALG_DETECT_PARA
		{
#if 1
			ALG_RUNNINGOBJ_PARA alg_running_para;
			alg_running_para = *(ALG_RUNNINGOBJ_PARA *)buf;
			FlashParam_SetAlgRunningPara(&alg_running_para,cmd);
			SendParaSetAck(cmd,true);
#endif
		}
		break;
	case CMD_NUM_ALG_PARA_AUTO_SET:
		{
#if 0
			//TODO: 算法参数自动生成
			//1.解析接收到的应用参数
			CMD_TRANSFER cmd_transfer;
			ALG_CONF_APP_AUTO_PARA alg_conf_app_auto_para;
			alg_conf_app_auto_para = *(ALG_CONF_APP_AUTO_PARA*)buf;
			unsigned int PointFixedRate = D1_WIDTH/alg_conf_app_auto_para.AlgConfAppUint.iImgWithdVal;
			CParamGenerator paramGenration;
			// 			CString strErrInfo;
			// 			strErrInfo.Format("接收到Index= %dBright = %d,Gray = %d,Sense=%d\r\n",
			// 				alg_conf_app_auto_para.AlgParaIndex,
			// 				alg_conf_app_auto_para.AlgConfAppUint.AlgAppBright,
			// 				alg_conf_app_auto_para.AlgConfAppUint.AlgAppGray,
			// 				alg_conf_app_auto_para.AlgConfAppUint.AlgAppSense);
			//			WRITE_LOG(strErrInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
			paramGenration.Init(alg_conf_app_auto_para.AlgParaIndex,
				alg_conf_app_auto_para.AlgConfAppUint.iImgWithdVal,
				alg_conf_app_auto_para.AlgConfAppUint.iImgHeightVal,
				alg_conf_app_auto_para.AlgConfAppUint.iObjWithdVal/PointFixedRate,
				alg_conf_app_auto_para.AlgConfAppUint.iObjHeightVal/PointFixedRate,
				alg_conf_app_auto_para.AlgConfAppUint.AlgAppBright,
				alg_conf_app_auto_para.AlgConfAppUint.AlgAppGray,
				alg_conf_app_auto_para.AlgConfAppUint.AlgAppSense);
			paramGenration.GetAlgParam((void*)&alg_conf_app_auto_para.AlgConfAppResult);
			//参数生成完成后只要将生成的参数发送到客户端即可 当客户端提交后则可以从新更新到g_stGlobal_Variable
			memset(&cmd_transfer,0x00,sizeof(cmd_transfer));	
			CONSTRUCT_CMD(CMD_NUM_ALG_PARA_AUTO_RESPONSE,(char *)&alg_conf_app_auto_para,sizeof(ALG_CONF_APP_AUTO_PARA),&cmd_transfer);
			TCP_SEND(g_stGlobal_Variable.client_tcpsocket.socket_it,(unsigned char *)&cmd_transfer,sizeof(cmd_transfer));
			//paramGenration.Init();
			//paramGenration.GetAlgParam();
#endif
		}
		break;
	case CMD_NUM_SYS_STORAGE_SET:
		{
#if 0
			SYS_STORAGE_PARA sys_storage_para;
			sys_storage_para = *(SYS_STORAGE_PARA*)buf;
			FlashParam_SetSysStoragePara(sys_storage_para);
			SendParaSetAck(CMD_NUM_SYS_STORAGE_RESPONSE,false);
#endif
		}
		break;
	case  CMD_NUM_PTZ_ACTION_SET://设置云台实时动态参数配置 PTZ_ACTION
		{
#if 1
			PTZ_ACTION_SET ptz_action_set;
			ptz_action_set = *(PTZ_ACTION_SET*)buf;
			switch(ptz_action_set.ptz_action)
			{
			case	PTZ_LEFT:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						if (ptz_action_set.ptz_value > 0)
						{
							g_stGlobal_Variable.globalflash.ptzpara.speed = ptz_action_set.ptz_value;
						}
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzSetLeft(g_stGlobal_Variable.globalflash.ptzpara.speed);
					}
				}
				break;
			case	PTZ_RIGHT:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						if (ptz_action_set.ptz_value > 0)
						{
							g_stGlobal_Variable.globalflash.ptzpara.speed = ptz_action_set.ptz_value;
						}
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzSetRight(g_stGlobal_Variable.globalflash.ptzpara.speed);
					}
				}
				break;
			case	PTZ_UP:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						if (ptz_action_set.ptz_value > 0)
						{
							g_stGlobal_Variable.globalflash.ptzpara.speed = ptz_action_set.ptz_value;
						}
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzSetUp(g_stGlobal_Variable.globalflash.ptzpara.speed);
					}
				}
				break;
			case	PTZ_DOWN:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						if (ptz_action_set.ptz_value > 0)
						{
							g_stGlobal_Variable.globalflash.ptzpara.speed = ptz_action_set.ptz_value;
						}
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzSetDown(g_stGlobal_Variable.globalflash.ptzpara.speed);
					}
				}
				break;
			case	PTZ_ZOOMIN:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzSetZoomIn();
					}
				}
				break;
			case	PTZ_ZOOMOUT:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzSetZoomOut();
					}
				}
				break;
			case	PTZ_AUTO_FOCUS:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzSetAutoFocus();
					}
				}
				break;
			case	PTZ_AUTO_IRIS:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzAutoIRIS();
					}
				}
				break;
			case	PTZ_SETPRESET:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						//TODO:
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzPreSet(ptz_action_set.ptz_value);
					}
				}
				break;
			case	PTZ_GOTOPRESET:
				{
					if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
					{
						//TODO:
						g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzGoToPre(ptz_action_set.ptz_value);
					}
				}
				break;
			case	PTZ_SETSPEED:
				{
					g_stGlobal_Variable.globalflash.ptzpara.speed = ptz_action_set.ptz_value;
				}
				break;
			case	PTZ_ACT_START:
			case	PTZ_ACT_END:
				{
				
				}
				break;
			default:
				break;
			}
			//TODO: //云台联动
#endif
		}
		break;
	case CMD_NUM_VIDEO_ATTRSET_SET:
		{
#if 0
			VIDEO_ATTRSET_STRUCT video_attrset_struct;
			video_attrset_struct = *(VIDEO_ATTRSET_STRUCT *)buf;
			switch (video_attrset_struct.video_attrset)
			{
			case VIDEO_ATTRSET_BRIGHT:
				{
					EteDVR_SetVideoParam(0,0,BRIGHTNESS,(int)video_attrset_struct.video_attrvalue);
					g_stGlobal_Variable.globalflash.videopara.bright = video_attrset_struct.video_attrvalue;
				}
				break;
			case VIDEO_ATTRSET_CONTRAST:
				{
					EteDVR_SetVideoParam(0,0,CONTRAST,(int)video_attrset_struct.video_attrvalue);
					g_stGlobal_Variable.globalflash.videopara.contrast = video_attrset_struct.video_attrvalue;
				}
				break;
			case VIDEO_ATTRSET_SATURATION:
				{
					EteDVR_SetVideoParam(0,0,SATURATION,(int)video_attrset_struct.video_attrvalue);
					g_stGlobal_Variable.globalflash.videopara.saturation = video_attrset_struct.video_attrvalue;
				}
				break;
			case VIDEO_ATTRSET_HUE:
				{
					EteDVR_SetVideoParam(0,0,HUE,(int)video_attrset_struct.video_attrvalue);
					g_stGlobal_Variable.globalflash.videopara.hue = video_attrset_struct.video_attrvalue;
				}
				break;
			default:
				break;
			}

#endif
		}
		break;
	case CMD_NUM_MODE_RUN_SET:
		{
#if 0
			//TODO:接收到切换工作状态消息
			//2.重置算法参数
			//WRITE_LOG(_T("CMD_NUM_MODE_RUN_SET: ResetALGParmbyCurrentTimeIf"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
			if(ResetALGParmbyCurrentTimeIf(true))//设置成功后保存参数
			{
				//1.保存参数
				FlashParam_DownLoad();
				ParseINIFileToXML(0x00000001,"ParamXML\\ptzParam.xml");
				ParseINIFileToXML(0x00000002,"ParamXML\\videoParam.xml");
				ParseINIFileToXML(0x00000004,"ParamXML\\algParam.xml");
				ParseINIFileToXML(0x00000008,"ParamXML\\quadParam.xml");
				ParseINIFileToXML(0x00000010,"ParamXML\\areaParam.xml");
				ParseINIFileToXML(0x00000020,"ParamXML\\lineParam.xml");
				ParseINIFileToXML(0x00000040,"ParamXML\\countParam.xml");
				ParseINIFileToXML(0x00000080,"ParamXML\\enterParam.xml");
				ParseINIFileToXML(0x00000100,"ParamXML\\outParam.xml");
				ParseINIFileToXML(0x00000200,"ParamXML\\lostParam.xml");
				ParseINIFileToXML(0x00000400,"ParamXML\\directParam.xml");
				ParseINIFileToXML(0x00000800,"ParamXML\\detectParam.xml");
				ParseINIFileToXML(0x00001000,"ParamXML\\peopleDenseParam.xml");
				ParseINIFileToXML(0x00002000,"ParamXML\\peopleClusterParam.xml");
				ParseINIFileToXML(0x00004000,"ParamXML\\peopleMotionParam.xml");
				ParseINIFileToXML(0x00008000,"ParamXML\\peopleHoverParam.xml");
				ParseINIFileToXML(0x00010000,"ParamXML\\alarmLineParam.xml");
				ParseINIFileToXML(0x00020000,"ParamXML\\findBagParam.xml");
				ParseINIFileToXML(0x00040000,"ParamXML\\runningParam.xml");
				ParseINIFileToXML(0x00080000,"ParamXML\\sysconfigalgParam.xml");
				ParseINIFileToXML(0x00100000,"ParamXML\\storageParam.xml");
			}
// 			
//			int ALG_index = 0;
// 			ALG_index = ((g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed >= 1 && g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed <= 2) ? g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed - 1 : 0);
// 			g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed = (ALG_TIME_USED)(ALG_index + 1);
			//ALG_SETPARAM(&g_stGlobal_Variable.globalflash,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

			//3.串口初始化
			if (!g_stGlobal_Variable.ptz_ctlhandle.ptz_initd)
			{
				g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzInit(g_stGlobal_Variable.globalflash.ptzpara.commport,
					g_stGlobal_Variable.globalflash.ptzpara.combaud);
			}
			//切到预置位
			g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzGoToPre(PTZ_PREPOINT_NUM_MAX);
			//4.切换调试状态
			//WRITE_LOG(_T("CMD_NUM_MODE_RUN_SET: SwitchMode"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
#if 0
			if (m_pReg && STATE_RUN != m_pReg->GetMode())
			{
				m_pReg->SwitchMode(STATE_RUN);			
			}
#endif
			SendParaSetAck(CMD_NUM_MODE_RUN_RESPONSE,false);
			//WRITE_LOG(_T("CMD_NUM_MODE_RUN_SET: OVER"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
			//回送响应消息
#endif
		}
		break;
	case  CMD_NUM_MODE_DEBUG_SET:
		{
			//TODO:接收到切换调试状态消息
#if 0
			if (m_pReg && STATE_DEBUG != m_pReg->GetMode())
			{
				m_pReg->SwitchMode(STATE_DEBUG);
				g_stGlobal_Variable.globalflash.sysconfigpara.camstate = CAM_STATE_DEBUG;//迁态
			}
#endif
			SendParaSetAck(CMD_NUM_MODE_DEBUG_RESPONSE,false);
		}
		break;
	case  CMD_NUM_VEDIO_REQUEST://设置视频请求
		{
#if 0
			VIDEO_REQUEST_STRUCT video_request_struct = *(VIDEO_REQUEST_STRUCT *)buf;
			UDP_CREATE(&g_stGlobal_Variable.client_udpsocket.socket_it);
			strcpy((char *)g_stGlobal_Variable.client_udpsocket.socket_cip,(char *)video_request_struct.cip);
			g_stGlobal_Variable.client_udpsocket.socket_cport = video_request_struct.iport;
			g_VideoRequestFlag = true;
#endif
		}
		break;
	case  CMD_NUM_VEDIO_STOP://设置视频停止
		{
			if (g_VideoRequestFlag)
			{
				g_VideoRequestFlag = false;
				UDP_CLOSE(g_stGlobal_Variable.client_udpsocket.socket_it);			
			}
		}
		break;
	case  CMD_NUM_KEEPALIVE://心跳消息
		{
			//SendParaSetAck(CMD_NUM_KEEPALIVE_RESPONSE,false);
 			//KillTimer(m_hWnd,IDENT_CLIENT_TIME_ID_DEAD);
 			//SetTimer(m_hWnd,IDENT_CLIENT_TIME_ID_DEAD,IDENT_CLIENT_TIME_VAL_DEAD,TimerOutProc);
		}
		break;
// 	case  CMD_NUM_KEEPALIVE_RESPONSE://心跳消息
// 		{
// 			KillTimer(m_hWnd,IDENT_CLIENT_TIME_ID_DEAD);
// 			SetTimer(m_hWnd,IDENT_CLIENT_TIME_ID_DEAD,IDENT_CLIENT_TIME_VAL_DEAD,TimerOutProc);
// 		}
// 		break;
	case CMD_NUM_DISCONNECT://断开连接
		{
			//KillTimer(m_hWnd,IDENT_CLIENT_TIME_ID_HEARTBEAT);
			//KillTimer(m_hWnd,IDENT_CLIENT_TIME_ID_DEAD);
			SendParaSetAck(CMD_NUM_DISCONNECT_RESPONSE,false);
			//Sleep(1000);//等待一秒
			TCP_CLOSE(g_stGlobal_Variable.client_tcpsocket.socket_it);
			UDP_CLOSE(g_stGlobal_Variable.client_udpsocket.socket_it);
			g_VideoRequestFlag = false;
			g_bIsConnected = false;
			m_IsSvrConnected = false;
		}
		break;
	case CMD_NUM_SIM_ALARM:
		{
			g_SimAlarmOne = true;
		}
		break;
	default:
		break;
	}

	return true;
}
#endif

#if 0
bool IdentClient::SendParaPtz()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{	
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		bRet = CONSTRUCT_CMD(CMD_NUM_PTZ_RESPONSE,(char *)&g_stGlobal_Variable.globalflash.ptzpara,sizeof(PTZ_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaVideo()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		bRet = CONSTRUCT_CMD(CMD_NUM_VIDEO_RESPONSE,(char *)&g_stGlobal_Variable.globalflash.videopara,sizeof(VIDEO_PARA),&m_cmdTransfer);
		
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlg()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		bRet = CONSTRUCT_CMD(CMD_NUM_ALG_RESPONSE,(char *)&g_stGlobal_Variable.globalflash.algpara,sizeof(ALG_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgConf(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_PARA_CONF_UINT *pAlgParaConfUint = (ALG_PARA_CONF_UINT *)&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[0];
		bRet = CONSTRUCT_CMD(num + CMD_NUM_ALG_CONF_00_RESPONSE - CMD_NUM_ALG_CONF_00,(char *)(pAlgParaConfUint+(num - CMD_NUM_ALG_CONF_00)),sizeof(ALG_PARA_CONF_UINT),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgQuad(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_QUAD_PARA *pAlgQuadPara = (ALG_QUAD_PARA *)&g_stGlobal_Variable.globalflash.algquadpara[0];
		bRet = CONSTRUCT_CMD(num + CMD_NUM_ALG_QUAD_000003_RESPONSE - CMD_NUM_ALG_QUAD_000003,(char *)(pAlgQuadPara+(num- CMD_NUM_ALG_QUAD_000003)),sizeof(ALG_QUAD_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgArea(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_AREA_PARA * pAlgAreaPara = (ALG_AREA_PARA *)&g_stGlobal_Variable.globalflash.algareapara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_AREA_000003_RESPONSE - CMD_NUM_ALG_AREA_000003,(char *)(pAlgAreaPara+(num- CMD_NUM_ALG_AREA_000003)),sizeof(ALG_AREA_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgLine(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_LINE_PARA * pAlgLinePara = (ALG_LINE_PARA *)&g_stGlobal_Variable.globalflash.alglinepara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_LINE_000003_RESPONSE - CMD_NUM_ALG_LINE_000003,(char *)(pAlgLinePara+(num- CMD_NUM_ALG_LINE_000003)),sizeof(ALG_LINE_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgCount(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_COUNT_PARA * pAlgCountPara = (ALG_COUNT_PARA *)&g_stGlobal_Variable.globalflash.algcountpara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_COUNT_000003_RESPONSE - CMD_NUM_ALG_COUNT_000003,(char *)(pAlgCountPara+(num- CMD_NUM_ALG_COUNT_000003)),sizeof(ALG_COUNT_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgEnter(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_ENTER_PARA * pAlgEnterPara = (ALG_ENTER_PARA *)&g_stGlobal_Variable.globalflash.algenterpara[0];
		bRet = CONSTRUCT_CMD(num +CMD_NUM_ALG_ENTER_000003_RESPONSE - CMD_NUM_ALG_ENTER_000003,(char *)(pAlgEnterPara+(num- CMD_NUM_ALG_ENTER_000003)),sizeof(ALG_ENTER_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgOut(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_OUT_PARA * pAlgOutPara = (ALG_OUT_PARA *)&g_stGlobal_Variable.globalflash.algoutpara[0];
		bRet = CONSTRUCT_CMD(num+CMD_NUM_ALG_OUT_000003_RESPONSE - CMD_NUM_ALG_OUT_000003,(char *)(pAlgOutPara+(num- CMD_NUM_ALG_OUT_000003)),sizeof(ALG_OUT_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

// bool IdentClient::SendParaAlgStop(int num)
// {
// 	bool bRet = false;
// 	if (m_IsSvrConnected)
// 	{
// 		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
// 		ALG_STOP_PARA * pAlgStopPara = (ALG_STOP_PARA *)&g_stGlobal_Variable.globalflash.algstoppara[0];
// 		bRet = CONSTRUCT_CMD(num + CMD_NUM_ALG_STOP_000002_RESPONSE - CMD_NUM_ALG_STOP_000002,(char *)(pAlgStopPara+(num- CMD_NUM_ALG_STOP_000002)),sizeof(ALG_STOP_PARA),&m_cmdTransfer);
// 		bRet &= SendParaMsg();
// 	}
// 	return bRet;
// }

bool IdentClient::SendParaAlgFindBag(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_FINDBAG_PARA * pAlgStopPara = (ALG_FINDBAG_PARA *)&g_stGlobal_Variable.globalflash.algfindbagpara[0];
		bRet = CONSTRUCT_CMD(num + CMD_NUM_ALG_FINDBAG_000003_RESPONSE - CMD_NUM_ALG_FINDBAG_000003,(char *)(pAlgStopPara+(num- CMD_NUM_ALG_FINDBAG_000003)),sizeof(ALG_FINDBAG_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgLost(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{	
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_LOST_PARA * pAlgLostPara = (ALG_LOST_PARA *)&g_stGlobal_Variable.globalflash.alglostpara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_LOST_000002_RESPONSE - CMD_NUM_ALG_LOST_000002,(char *)(pAlgLostPara+(num- CMD_NUM_ALG_LOST_000002)),sizeof(ALG_LOST_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgDirect(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_DIRECT_PARA * pAlgDirectPara = (ALG_DIRECT_PARA *)&g_stGlobal_Variable.globalflash.algdirectpara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_DIRECT_000003_RESPONSE - CMD_NUM_ALG_DIRECT_000003,(char *)(pAlgDirectPara+(num- CMD_NUM_ALG_DIRECT_000003)),sizeof(ALG_DIRECT_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgDetect(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_DETECT_PARA * pAlgDetectPara = (ALG_DETECT_PARA *)&g_stGlobal_Variable.globalflash.algdetectpara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_DETECT_000003_RESPONSE - CMD_NUM_ALG_DETECT_000003,(char *)(pAlgDetectPara+(num- CMD_NUM_ALG_DETECT_000003)),sizeof(ALG_DETECT_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgPeopleDense(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_PEOPLEDENSE_PARA * pAlgPeopleDensePara = (ALG_PEOPLEDENSE_PARA *)&g_stGlobal_Variable.globalflash.algpeopledectpara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_PEOPLEDENSE_000003_RESPONSE - CMD_NUM_ALG_PEOPLEDENSE_000003,(char *)(pAlgPeopleDensePara+(num- CMD_NUM_ALG_PEOPLEDENSE_000003)),sizeof(ALG_PEOPLEDENSE_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgClusterDete(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_CLUSTERDETE_PARA * pAlgClusterDetePara = (ALG_CLUSTERDETE_PARA *)&g_stGlobal_Variable.globalflash.algclusterdectpara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_CLUSTERDETE_000003_RESPONSE - CMD_NUM_ALG_CLUSTERDETE_000003,(char *)(pAlgClusterDetePara+(num- CMD_NUM_ALG_CLUSTERDETE_000003)),sizeof(ALG_CLUSTERDETE_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgMotionHistory(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_MOTIONHISTORY_PARA * pAlgMotionHistoryPara = (ALG_MOTIONHISTORY_PARA *)&g_stGlobal_Variable.globalflash.algmotionhistorypara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_MOTIONHISTORY_000003_RESPONSE - CMD_NUM_ALG_MOTIONHISTORY_000003,(char *)(pAlgMotionHistoryPara+(num- CMD_NUM_ALG_MOTIONHISTORY_000003)),sizeof(ALG_MOTIONHISTORY_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgHoverDete(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_HOVERDETE_PARA * pAlgHoverDetePara = (ALG_HOVERDETE_PARA *)&g_stGlobal_Variable.globalflash.alghoverdectpara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_HOVERDETE_000003_RESPONSE - CMD_NUM_ALG_HOVERDETE_000003,(char *)(pAlgHoverDetePara+(num- CMD_NUM_ALG_HOVERDETE_000003)),sizeof(ALG_HOVERDETE_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgAlarmLine(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_ALARMLINE_PARA * pAlgAlarmLineDetePara = (ALG_ALARMLINE_PARA *)&g_stGlobal_Variable.globalflash.algalarmlinepara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_ALARMLINE_000003_RESPONSE - CMD_NUM_ALG_ALARMLINE_000003,(char *)(pAlgAlarmLineDetePara+(num- CMD_NUM_ALG_ALARMLINE_000003)),sizeof(ALG_ALARMLINE_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlgRunning(int num)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALG_RUNNINGOBJ_PARA * pAlgRunningDetePara = (ALG_RUNNINGOBJ_PARA *)&g_stGlobal_Variable.globalflash.algrunningpara[0];
		bRet = CONSTRUCT_CMD(num+ CMD_NUM_ALG_RUNNING_000003_RESPONSE - CMD_NUM_ALG_RUNNING_000003,(char *)(pAlgRunningDetePara+(num- CMD_NUM_ALG_RUNNING_000003)),sizeof(ALG_RUNNINGOBJ_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}


bool IdentClient::SendParaSysConfig()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		bRet = CONSTRUCT_CMD(CMD_NUM_SYS_CONFIG_RESPONSE,(char *)&g_stGlobal_Variable.globalflash.sysconfigpara,sizeof(SYS_CONFIG_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlarmYM()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALARM_YM_PARA alarm_ym_para;
		bRet = CONSTRUCT_CMD(CMD_NUM_ALARM_YM_RESPONSE,(char *)&alarm_ym_para,sizeof(ALARM_YM_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaPicContents(unsigned char *fname)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		PIC_CONTENTS alarm_ym_para;
		bRet = CONSTRUCT_CMD(CMD_NUM_PIC_CONTENTS_RESPONSE,(char *)&alarm_ym_para,sizeof(PIC_CONTENTS),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaSysStorage()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		size_t szAllDriveStrings = GetLogicalDriveStrings(0,NULL);
		char *pDriveStrings = new char[szAllDriveStrings + sizeof((""))];
		GetLogicalDriveStrings(szAllDriveStrings,pDriveStrings);
		size_t szDriveString = strlen(pDriveStrings);
		int iMaxnumDrive = sizeof(g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc)/sizeof(g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[0]);
		int DriveIndex = 0;
		for(DriveIndex = 0; DriveIndex < iMaxnumDrive && szDriveString > 0; DriveIndex++)
		{
			strcpy((char *)g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].disk_name,pDriveStrings);
			unsigned long long FreeAv,TotalBytes,FreeBytes;
			if (GetDiskFreeSpaceEx(pDriveStrings,&FreeAv,&TotalBytes,&FreeBytes))
			{
				g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].realsize = TotalBytes.QuadPart/(unsigned long long)(1024*1024);
				g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].emptysize = FreeAv.QuadPart/(unsigned long long)(1024*1024);
			}
			g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_type = (DRIVE_TYPE)GetDriveType(pDriveStrings);
			char drive_fs_type[128];
			if(!GetVolumeInformation(pDriveStrings,NULL,0,NULL,NULL,NULL,drive_fs_type,128))
			{
				g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_fstype = DRIVE_FS_TYPE_UNKNOWN;
			}
			else
			{
				if(!strcmp(drive_fs_type,"NTFS"))
				{
					g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_fstype = DRIVE_FS_TYPE_NTFS;
				}
				else if(!strcmp(drive_fs_type,"FAT32"))
				{
					g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_fstype = DRIVE_FS_TYPE_FAT32;
				}
				else
				{
					g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_fstype = DRIVE_FS_TYPE_UNKNOWN;
				}
			}
			pDriveStrings += szDriveString + 1;
			szDriveString = strlen(pDriveStrings);
		}
		g_stGlobal_Flash.sysstoragepara.storageconfig.diskcount = DriveIndex;
		bRet = CONSTRUCT_CMD(CMD_NUM_SYS_STORAGE_RESPONSE,(char *)&g_stGlobal_Variable.globalflash.sysstoragepara,sizeof(SYS_STORAGE_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlarmDay()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALARM_DAY_PARA alarm_day_para;
		bRet = CONSTRUCT_CMD(CMD_NUM_ALARM_DAY_RESPONSE,(char *)&alarm_day_para,sizeof(ALARM_DAY_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaAlarName()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		ALARM_NAME_PARA alarm_name_para;
		bRet = CONSTRUCT_CMD(CMD_NUM_ALARM_NAME_RESPONSE,(char *)&alarm_name_para,sizeof(ALARM_NAME_PARA),&m_cmdTransfer);
		bRet &= SendParaMsg();
	}
	return bRet;
}


bool IdentClient::SendParaSetAck(int num,bool flag)
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		memset(&m_cmdTransfer,0x00,sizeof(m_cmdTransfer));
		if(flag)
		{
			bRet = CONSTRUCT_CMD(num - CMD_NUM_PTZ_SET + CMD_NUM_PTZ_RESPONSE,NULL,0,&m_cmdTransfer);
		}
		else
		{
			bRet = CONSTRUCT_CMD(num,NULL,0,&m_cmdTransfer);
		}
		bRet &= SendParaMsg();
	}
	return bRet;
}

bool IdentClient::SendParaMsg()
{
	bool bRet = false;
	if (m_IsSvrConnected)
	{
		bRet = TCP_SEND(g_stGlobal_Variable.client_tcpsocket.socket_it,(unsigned char *)&m_cmdTransfer,sizeof(m_cmdTransfer));
	}
	return bRet;
}
#if 0
void CALLBACK IdentClient::TimerOutProc(HWND hWnd,unsigned int nMsg,unsigned int nTimerid,unsigned long dwTime)
{
	switch(nTimerid)
	{
	//case IDENT_CLIENT_TIME_ID_HEARTBEAT:
	//	{
	//		SendHeartBeat();
	//	}
	//	break;
	case IDENT_CLIENT_TIME_ID_DEAD:
		{
			g_VideoRequestFlag = false;
			g_bIsConnected = false;
		}
		break;
	default:
		break;
	}
}
#endif 
#endif
#if 0
bool SendHeartBeat()
{
	bool bRet = false;

	if (g_bIsConnected)
	{
		CMD_TRANSFER cmd_transfer;
		bRet = CONSTRUCT_CMD(CMD_NUM_KEEPALIVE,NULL,0,&cmd_transfer);
		bRet &= TCP_SEND(g_stGlobal_Variable.client_tcpsocket.socket_it,(unsigned char *)&cmd_transfer,sizeof(cmd_transfer));
	}
	return bRet;
}
#endif
