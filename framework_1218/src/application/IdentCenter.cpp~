#include "IdentCenter.h"
//#include "SuperLog.h"
//#include "process.h"
#include "ParamMgr.h"

bool IdentCenter::m_bIsExit = false;
bool g_bIsLinkConnected = false;
static bool g_bIsDataConnected = false;

static sockaddr_in svr_sockaddr;
static sockaddr_in local_sockaddr;

static TCPMSG_TO_CENTER tcpmsg_to_center;
static TCPMSG_ACK tcpmsg_ack;

static TCP_MSG_T g_TcpMsg;
static UDP_MSG_T g_UdpMsg;



#define TIMER_NO_01 0x00000001
#define TIMER_CONN_OFF 30000*3

IdentCenter::IdentCenter()
{
}

IdentCenter::~IdentCenter()
{
	m_bIsExit = true;
}

bool IdentCenter::Attach(Recognizer *pReg,HWND hwnd)
{
    return true;
}

bool IdentCenter::Detach()
{
    return true;
}

bool IdentCenter::Initilize()
{	
	memset(&tcpmsg_to_center,0x00,sizeof(tcpmsg_to_center));
	if (!strcmp((char *)g_stGlobal_Variable.globalflash.sysconfigpara.camconfig.dev_id,""))
	{
#if 0
		SYSTEMTIME Sys_time;
		GetLocalTime(&Sys_time);
		srand(Sys_time.wMilliseconds);
		sprintf((char *)g_stGlobal_Variable.globalflash.sysconfigpara.camconfig.dev_id,
			"%04d%02d%02d%02d%02d%02d%03d%03d",
			Sys_time.wYear,
			Sys_time.wMonth,
			Sys_time.wDay,
			Sys_time.wHour,
			Sys_time.wMinute,
			Sys_time.wSecond,
			Sys_time.wMilliseconds,
			rand()%1000);		
#endif
	}

	strcmp((char *)tcpmsg_to_center.dev_id,(char *)g_stGlobal_Variable.globalflash.sysconfigpara.camconfig.dev_id);
	if (!strcmp((char *)g_stGlobal_Variable.globalflash.sysconfigpara.camconfig.dev_name,""))
	{
		int Buff_size = 128;
		GetComputerName((char *)g_stGlobal_Variable.globalflash.sysconfigpara.camconfig.dev_name, &Buff_size);
	}
	strcmp((char *)tcpmsg_to_center.dev_name,(char *)g_stGlobal_Variable.globalflash.sysconfigpara.camconfig.dev_name);
	if (!strcmp((char *)g_stGlobal_Variable.globalflash.sysconfigpara.camipconfig.ipaddr,""))
	{
#if 0
		char tmp[128] = {0};
		struct hostent *p_HostEnt;
		p_HostEnt = gethostbyname(tmp);
		if(p_HostEnt != NULL)
		{
			sprintf( (char *)g_stGlobal_Variable.globalflash.sysconfigpara.camipconfig.ipaddr, "%d.%d.%d.%d",
				( p_HostEnt->h_addr_list[0][0] & 0x00ff ),
				( p_HostEnt->h_addr_list[0][1] & 0x00ff ),
				( p_HostEnt->h_addr_list[0][2] & 0x00ff ),
				( p_HostEnt->h_addr_list[0][3] & 0x00ff ) );
		}
#endif
	}
	strcmp((char *)tcpmsg_to_center.dev_name,(char *)g_stGlobal_Variable.globalflash.sysconfigpara.camipconfig.ipaddr);
	FlashParam_DownLoad();

	if (pthread_create(m_hThread4ConnectServer, NULL, ThreadForConnectServer, this))
	{
		SYS_ERROR("Failed to create flash_updatethread_func thread\n");
		return false;
	}

    if (pthread_create(m_hThread4CommFunc, NULL, ThreadForCommFunc, this))
	{
		SYS_ERROR("Failed to create flash_updatethread_func thread\n");
		return false;
	}

    if (pthread_create(m_hThread4NewFileSend, NULL, ThreadForNewFileSend, this))
	{
		SYS_ERROR("Failed to create flash_updatethread_func thread\n");
		return false;
	}

	return true;
}

void  IdentCenter::ThreadForConnectServer(void * pParam)
{
	IdentCenter *pThisIdentCenter = (IdentCenter *)pParam;
	if (NULL == pThisIdentCenter)
	{
		//WRITE_LOG(_T("ThreadForConnectServer 传入参数为NULL!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
		return -1; 
	}

	while(!pThisIdentCenter->m_bIsExit)
	{
		if(!g_bIsLinkConnected)
		{
			try
			{
				if(pThisIdentCenter->ConnectServer())
				{
					g_bIsLinkConnected = true;
				}
				sleep(1);
			}
			catch(...)
			{
				g_bIsLinkConnected = false;
			}
		}
		else
		{
			sleep(3);
		}		
	}
	return 0;
}

void  IdentCenter::ThreadForCommFunc(void * pParam)
{
	IdentCenter *pThisIdentCenter = (IdentCenter *)pParam;
	INIT_WSASOCK();
	int SockSvrListen;
	int SockSvrMax;
	int iRetApi = -1;
	int SocketTmp;
	
	TIMEVAL TimeVal; 
	TimeVal.tv_sec = 1;
	TimeVal.tv_usec = 0;

	FD_SET SockSvrFdSet;

	while(!pThisIdentCenter->m_bIsExit)
	{
 		FD_ZERO(&SockSvrFdSet);
		if(g_bIsLinkConnected)
		{
			FD_SET(g_stGlobal_Variable.center_tcpsocket.socket_it,&SockSvrFdSet);
			SockSvrMax = g_stGlobal_Variable.center_tcpsocket.socket_it + 1;
		}
		else
		{
			sleep(1);
			continue;
		}
		if (select(SockSvrMax,&SockSvrFdSet,NULL,NULL,&TimeVal) > 0)
		{
			if (FD_ISSET(g_stGlobal_Variable.center_tcpsocket.socket_it,&SockSvrFdSet))//接收到了新的链接
			{
				memset(&g_TcpMsg,0x00,sizeof(g_TcpMsg));
				unsigned int iRcvLen = sizeof(sizeof(g_TcpMsg));
				if (!TCP_RECV(g_stGlobal_Variable.center_tcpsocket.socket_it,(unsigned char *)&g_TcpMsg,&iRcvLen))//接收成功
				{
					switch(g_TcpMsg.msg_type)
					{
					case TCP_MSG_TYPE_START:
					case TCP_MSG_TYPE_CONNECT:
						{
							
						}
						break;
					case TCP_MSG_TYPE_CONNECT_ACK:
						{
							memset(&tcpmsg_ack,0x00,TCPMSG_ACK_LEN);
							memcpy(&tcpmsg_ack,g_TcpMsg.msg_buf,TCPMSG_ACK_LEN);
							pThisIdentCenter->m_cstrErrorInfo.Format(
								"ThreadForCommFunc 读卡器收到服务器返回连接成功响应 ACK_port=%d ACK_ip=%s!\r\n",
								tcpmsg_ack.alarm_port,
								tcpmsg_ack.alarm_ipaddr);
							//WRITE_LOG(pThisIdentCenter->m_cstrErrorInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN);
									
							// 更新当前时间
							SetLocalTime(&tcpmsg_ack.alarm_systime);
							if(UDP_CREATE(&g_stGlobal_Variable.center_udpsocket.socket_it))
							{
								g_bIsDataConnected = false;
								pThisIdentCenter->m_cstrErrorInfo.Format("g_UdpSocket创建失败: %d!\r\n", GetLastError());
								//WRITE_LOG(pThisIdentCenter->m_cstrErrorInfo,LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
								break;
							}

							memset(&svr_sockaddr,0x00,sizeof(svr_sockaddr));
#if 0
myaddr.sin_family = AF_INET;         	/* host byte order */
			myaddr.sin_port = htons(port);     	/* short, network byte order */
			myaddr.sin_addr.s_addr = INADDR_ANY;
#endif

							svr_sockaddr.sin_family = AF_INET;
							svr_sockaddr.sin_port = htons(tcpmsg_ack.alarm_port);
							svr_sockaddr.sin_addr.S_un.S_addr = inet_addr((char *)tcpmsg_ack.alarm_ipaddr);
							g_bIsDataConnected = true;
							SetTimer(pThisIdentCenter->m_hwnd,TIMER_NO_01,TIMER_CONN_OFF,pThisIdentCenter->TimerOutProc);
						}
						break;
					case TCP_MSG_TYPE_KEEPALIVE:
						{
							CENT2DEV_KEEPALIVE cent2dev_keepalive;
							memset(&cent2dev_keepalive,0x00,CENT2DEV_KEEPALIVE_LEN);
							memcpy(&cent2dev_keepalive,g_TcpMsg.msg_buf,CENT2DEV_KEEPALIVE_LEN);
							if (MAGIC_CENT2DEV_START == cent2dev_keepalive.magic_start
								&&MAGIC_CENT2DEV_KEEPALIVE == cent2dev_keepalive.keepalivedata
								&&MAGIC_CENT2DEV_END == cent2dev_keepalive.magic_end)
							{
								//WRITE_LOG(_T("收到服务器发送的心跳消息!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN);
								DEV2CENT_KEEPALIVE dev2cent_keepalive;
								dev2cent_keepalive.magic_start = MAGIC_DEV2CENT_START;
								dev2cent_keepalive.keepalivedata = MAGIC_CENT2DEV_KEEPALIVE;
								dev2cent_keepalive.magic_end = MAGIC_CENT2DEV_END;
								memset(&g_TcpMsg,0x00,TCP_MSG_LEN);
								g_TcpMsg.msg_type = TCP_MSG_TYPE_KEEPALIVE_ACK;
								memcpy(g_TcpMsg.msg_buf,&dev2cent_keepalive,DEV2CENT_KEEPALIVE_LEN);
								TCP_SEND(g_stGlobal_Variable.center_tcpsocket.socket_it,(unsigned char *)&g_TcpMsg,sizeof(g_TcpMsg));
							}
							else
							{
								//WRITE_LOG(_T("收到服务器发送的错误心跳消息!\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN);
							}
							KillTimer(pThisIdentCenter->m_hwnd,TIMER_NO_01);
							SetTimer(pThisIdentCenter->m_hwnd,TIMER_NO_01,TIMER_CONN_OFF,pThisIdentCenter->TimerOutProc);
						}
						break;
					case TCP_MSG_TYPE_DISCONNECT:
						break;
					case TCP_MSG_TYPE_DISCONNECT_ACK:
						{
							//WRITE_LOG(_T("收到服务器断开确认消息!\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
						}
						break;
					case TCP_MSG_TYPE_REFUSE:
						{
							//WRITE_LOG(_T("收到服务器拒绝连接消息!\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
							if (g_bIsLinkConnected)
							{
								g_bIsLinkConnected = false;
								g_bIsDataConnected = false;				
							}
						}
						break;
					case TCP_MSG_TYPE_SERVER_QUIT:
						{
							//WRITE_LOG(_T("收到服务器退出消息!\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
							if (g_bIsLinkConnected)
							{
								g_bIsLinkConnected = false;
								g_bIsDataConnected = false;	
							}
						}
						break;
					case TCP_MSG_TYPE_RECONNECT:
						{
							if (g_bIsLinkConnected)
							{
								g_bIsLinkConnected = false;
								g_bIsDataConnected = false;	
							}
						}
						break;
					case TCP_MSG_TYPE_RS485_ACK:
					case TCP_MSG_TYPE_END:	
					default:
						break;
					}/*end switch*/
				}/*end if UDP_RECV*/
				else
				{
					g_bIsLinkConnected = false;
				}
			}/*end if FD_ISSET*/
			else
			{
				//Sleep(1000);
			}
		}/*end select*/
	}/*end while*/
	return 0;
}

void  IdentCenter::ThreadForNewFileSend(void * pParam)
{
	IdentCenter *pThisIdentCenter = (IdentCenter *)pParam;
	DEV2CENT_PIC dev2cent_pic;
	DEV2CENT_ALARMMSG dev2cent_alarmmsg;
	memset(&dev2cent_pic,0x00,sizeof(dev2cent_pic));
	memset(&dev2cent_alarmmsg,0x00,sizeof(dev2cent_alarmmsg));
	char  NewFileName[MAX_PATH] = "";
	while(!pThisIdentCenter->m_bIsExit)
	{
		bool IsSecondSendFile = false;
		FileNameNode_T node;
		EnterCriticalSection(&g_stGlobal_Variable.csFileNameList);
		if(!g_bIsLinkConnected) 
		{
			LeaveCriticalSection(&g_stGlobal_Variable.csFileNameList);
			sleep(2);
			continue;
		}

		if (g_FileNameList.size() == 0)
		{
			if(g_FileNameListReserved.size() == 0)
			{
				LeaveCriticalSection(&g_stGlobal_Variable.csFileNameList);
				sleep(2);
				continue;
			}
			else 
			{
				node = g_FileNameListReserved.front();
				strcpy(NewFileName,node.v_name.c_str());
				g_FileNameListReserved.pop();
				IsSecondSendFile = true;
			}
		}
		else
		{
			node = g_FileNameList.front();
			strcpy(NewFileName,node.v_name.c_str());
			g_FileNameList.pop();
			IsSecondSendFile = false;
		}
		LeaveCriticalSection(&g_stGlobal_Variable.csFileNameList);

		if(!pThisIdentCenter->GetDev2CenMsgByName(dev2cent_pic,dev2cent_alarmmsg,NewFileName))
		{
			//WRITE_LOG(_T("检测到非报警图片!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN);
			continue;
		}
		CFile file;
		char NewFilePath[MAX_PATH] = "";
		sprintf(NewFilePath,"%s",NewFileName);

        int OpenCnt = 0;
		while (!file.Open(NewFilePath,CFile::shareDenyWrite))
		{
			if (OpenCnt >= 20)
			{
				//WRITE_LOG(_T("文件打开失败!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
				break;
			}
			OpenCnt++;
			sleep(1);
		}
		if (OpenCnt >= 20)
		{
			if(!file.Open(NewFilePath,CFile::modeRead))
			{
				if (IsSecondSendFile)
				{
				}
				else if((dev2cent_alarmmsg.alarm_index % 2) == 0)
				{
					g_FileNameListReserved.push(node);
				}
				else
				{
					g_FileNameListReserved.push(node);

					EnterCriticalSection(&g_stGlobal_Variable.csFileNameList);
					if (g_FileNameList.size() == 0 )
					{
					}
					else
					{
						node = g_FileNameList.front();
						g_FileNameListReserved.push(node);
						g_FileNameList.pop();
					}
					LeaveCriticalSection(&g_stGlobal_Variable.csFileNameList);
				}
				//WRITE_LOG(_T("报警图片打开失败!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
				continue;
			}
			else
			{
			}
		}
		unsigned char FileBeginChar[2];
		unsigned char FileEndChar[2];
		file.Read(&FileBeginChar,2);
		file.Seek(file.GetLength()-2,0);
		file.Read(&FileEndChar,2);
		file.SeekToBegin();
		if(FileBeginChar[0] != 0xFF  || FileBeginChar[1] != 0xD8 || FileEndChar[0] != 0xFF  || FileEndChar[1] != 0xD9 )
		{
			//WRITE_LOG(_T("检测JPEG图片数据不完整!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
			if (IsSecondSendFile)
			{
			}
			else if((dev2cent_alarmmsg.alarm_index % 2) == 0)
			{
				g_FileNameListReserved.push(node);
			}
			else
			{
				g_FileNameListReserved.push(node);
				EnterCriticalSection(&g_stGlobal_Variable.csFileNameList);
				if (g_FileNameList.size() == 0 )
				{
				}
				else
				{
					node = g_FileNameList.front();
					g_FileNameListReserved.push(node);
					g_FileNameList.pop();
				}
				LeaveCriticalSection(&g_stGlobal_Variable.csFileNameList);
			}
			continue;
		}
		unsigned int pic_num = 0;
		unsigned int pic_length = file.GetLength();
		static int pic_cnt = 0;
		while (pic_length)
		{
			dev2cent_pic.package_index = ++pic_num;
			memset(dev2cent_pic.msg_contents,0x00,1200);
			unsigned int ret_leng;
			ret_leng = file.Read(dev2cent_pic.msg_contents,1200);
			pic_length -= ret_leng;
			dev2cent_pic.msg_len = ret_leng;
			memset(&g_UdpMsg,0x00,UDP_MSG_LEN);
			g_UdpMsg.msg_type = UDP_MSG_TYPE_PIC;
			if(1200 == ret_leng)
			{
				memcpy(g_UdpMsg.msg_buf,&dev2cent_pic,sizeof(dev2cent_pic));
				sendto(g_stGlobal_Variable.center_udpsocket.socket_it,(char *)&g_UdpMsg,UDP_MSG_LEN,0,(SOCKADDR *)&svr_sockaddr,sizeof(svr_sockaddr));
			}
			else
			{
				dev2cent_pic.package_index = PACKAGE_END;
				memcpy(g_UdpMsg.msg_buf,&dev2cent_pic,sizeof(dev2cent_pic));
				sendto(g_stGlobal_Variable.center_udpsocket.socket_it,(char *)&g_UdpMsg,UDP_MSG_LEN,0,(SOCKADDR *)&svr_sockaddr,sizeof(svr_sockaddr));
			}
			msleep(100);
		}
		file.Close();
		//WRITE_LOG(_T("向服务器上报报警图片成功!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
		memset(&g_UdpMsg,0x00,UDP_MSG_LEN);
		g_UdpMsg.msg_type = UDP_MSG_TYPE_ALARM;
		memcpy(g_UdpMsg.msg_buf,&dev2cent_alarmmsg,DEV2CENT_ALARMMSG_LEN);
		if (sendto(g_stGlobal_Variable.center_udpsocket.socket_it,(char *)&g_UdpMsg,UDP_MSG_LEN,0,(SOCKADDR *)&svr_sockaddr,sizeof(svr_sockaddr)) == UDP_MSG_LEN)
		{
// 			dev2cent_alarmmsg.alarm_index += 1;
// 			memcpy(g_UdpMsg.msg_buf,&dev2cent_alarmmsg,DEV2CENT_ALARMMSG_LEN);
// 			if (sendto(g_UdpSocket,(char *)&g_UdpMsg,UDP_MSG_LEN,0,(SOCKADDR *)&svr_sockaddr,sizeof(svr_sockaddr)) == UDP_MSG_LEN)
// 			{
				//WRITE_LOG(_T("向服务器上报报警信息成功!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN);
// 			}
// 			else
// 			{
// 				WRITE_LOG(_T("Thread4UdpProc 向服务器上报报警信息失败!"),LOG_TYPE_INFO,LOG_LEVEL_RUN);
// 			}
		}
		else
		{
			//WRITE_LOG(_T("向服务器上报报警信息失败!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
		}
	}
	//WRITE_LOG(_T("ThreadForNewFileSend 线程退出!\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN);
	return 0;
}



/************************************************************************/
/* 无超时连接服务器 直到点击取消连接方可停止							*/
/*	                                                                     */
/************************************************************************/
bool IdentCenter::ConnectServer() 
{
	// TODO: Add your control notification handler code here
	SOCKADDR_IN SockAddrSvr;
	SockAddrSvr.sin_family = AF_INET;
	SockAddrSvr.sin_addr.S_un.S_addr = inet_addr((char *)g_stGlobal_Variable.globalflash.sysconfigpara.centeripconfig.ipaddr);
	SockAddrSvr.sin_port = htons(g_stGlobal_Variable.globalflash.sysconfigpara.centeripconfig.center_port);

	//WRITE_LOG(_T("开始连接服务器!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN);
	if(TCP_CREATE(&g_stGlobal_Variable.center_tcpsocket.socket_it)) //创建套接字1--出错 0--成功
	{
		m_cstrErrorInfo.Format(("ConnectServer TCP_CREATE: %d!\r\n"),GetLastError());
		//WRITE_LOG(m_cstrErrorInfo,LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
		return false;
	}

#if 0//tcp连接端不需要进行BIND操作
	if (TCP_BIND(g_stGlobal_Variable.center_tcpsocket.socket_it,g_stGlobal_Variable.center_tcpsocket.socket_cport))
	{
		m_cstrErrorInfo.Format(_T("ConnectServer TCP_BIND: %d!\r\n"),GetLastError());
		WRITE_LOG(m_cstrErrorInfo,LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
		TCP_CLOSE(g_stGlobal_Variable.center_tcpsocket.socket_it);
		return false;
	}
#endif		

	//WRITE_LOG(_T("ConnectServer 连接中心套接字创建成功!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN);
	
	while(TCP_CONNECT(g_stGlobal_Variable.center_tcpsocket.socket_it,
		g_stGlobal_Variable.globalflash.sysconfigpara.centeripconfig.center_port,
		(unsigned char *)g_stGlobal_Variable.globalflash.sysconfigpara.centeripconfig.ipaddr)) //连接服务器
	{
		sleep(2);
	}

	//WRITE_LOG(_T("TcpSocket连接成功!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN);
	memset(&g_TcpMsg,0x00,sizeof(g_TcpMsg));
	g_TcpMsg.msg_type = TCP_MSG_TYPE_CONNECT;
	memcpy(g_TcpMsg.msg_buf, &tcpmsg_to_center, sizeof(tcpmsg_to_center));	
	if (TCP_SEND(g_stGlobal_Variable.center_tcpsocket.socket_it,(unsigned char *)&g_TcpMsg,sizeof(g_TcpMsg)))
	{
		m_cstrErrorInfo.Format(("ConnectServer TCP_SEND: %d!\r\n"),GetLastError());
		//WRITE_LOG(m_cstrErrorInfo,LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
		TCP_CLOSE(g_stGlobal_Variable.center_tcpsocket.socket_it);
		//WRITE_LOG(_T("断开连接服务器!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN);
		return false;
	}
	return true;
}

/************************************************************************/
/* 从报警名获取相应的报警信息                                                                     */
/************************************************************************/
bool IdentCenter::GetDev2CenMsgByName(DEV2CENT_PIC &dev2cent_pic,DEV2CENT_ALARMMSG &dev2cent_alarmmsg,const char fname[])
{
#if 0
	CString strname(fname);
	if (-1 != strname.Find('\\',0)) //说明带有反斜杠
	{
		strname = strname.Right(strname.GetLength() - strname.ReverseFind('\\') - 1);//去除反斜杠操作
	}
	char name[MAX_PATH] = "";
	sprintf(name,"%s",strname);
	//sscanf(fname, "%*[^\\]\\%s", name); //找到第一个\\位置
	//if (!strcmp(name,""))
	//{
	//	strcpy(name,fname);
	//}
	//CString strname(name);
	if (!strname.Find("Bj")) //以第5-6位来判断文件名字是否为报警图片
	{
		return false;
	}
	else
	{
		char tmp[128]="";
		sscanf(name, "%*[^-]-%[^.]", tmp);
		if (!strcmp(tmp,""))
		{
			return false;
		}
		dev2cent_pic.magic_start = MAGIC_PIC_START;
		dev2cent_pic.magic_end = MAGIC_PIC_END;
		dev2cent_pic.alarm_index = atoi(tmp);
		strcpy((char *)dev2cent_pic.alarm_picname,(char *)name);
		
		
		dev2cent_alarmmsg.magic_start = MAGIC_ALARM_START;
		dev2cent_alarmmsg.magic_end = MAGIC_ALARM_END;		
		dev2cent_alarmmsg.msg_type = MSG_ALARM;
		dev2cent_alarmmsg.alarmtype = ALARM_WINDOW;
		dev2cent_alarmmsg.alarm_index = atoi(tmp);
		
#if 0
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		sprintf((char *)dev2cent_alarmmsg.alarm_time,"%04d-%02d-%02d %02d:%02d:%02d",
			systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);
#endif
		
		strcpy((char *)dev2cent_alarmmsg.dev_id,(char *)tcpmsg_to_center.dev_id);
		memset(tmp,0x00,sizeof(tmp));
		sscanf(name, "%[^B]", tmp);
		if (!strcmp(tmp,""))
		{
			return false;
		}
		strcpy((char *)dev2cent_alarmmsg.alarm_area,(char *)tmp);
		strcpy((char *)dev2cent_alarmmsg.alarm_picname,(char *)name);
	}
	//第1区BjFullScreen8&28&01-47.jpg
	//第1区Bj8&28&05-48.jpg	
#endif
	return true;
}


void CALLBACK IdentCenter::TimerOutProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)
{
	switch(nTimerid)
	{
	case TIMER_NO_01:
		{
			g_bIsLinkConnected = false;
			g_bIsDataConnected = false;
		}
		break;
	default:
		break;
	}
}
