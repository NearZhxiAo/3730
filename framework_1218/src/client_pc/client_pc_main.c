#include "client_pc_private.h"

//20110414
//#define sec_delay 15

CLIENT_PC_PRIVATE_HANDLE global_client_pc_private_handle;
void sig_sigalrm_proc(int sig);
void sig_sigint_proc(int sig);
int IsClientpcSysQuit = 0;
int gtime_no_heart_res=0;//单位:secs,持续收不到CMS心跳反馈的时间，到60秒时自动挂断PU与CMS的连接
int gcount_sigalrm_handle=0;//进入函数sig_sigalrm_proc的次数


static void client_pc_poll_init(CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	memset(phandle->polltimeout, 0x00, sizeof(struct pollfd) * CLIENT_PC_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;
	phandle->polltimeout[0].events = POLLIN;
	phandle->polltimeout[1].fd = phandle->socket_fd;
	phandle->polltimeout[1].events = POLLIN;
	phandle->active_fdcount = 2;
}
succeed_type client_pc_init( CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	SIGNAL_STRUCT_T signal;
	phandle->pgbshm_obj = GBSHM_INIT();
	if ( NULL == phandle->pgbshm_obj )
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}
	GBSHM_PARA_GET(phandle->pgbshm_obj,PARA_CMD_SYSTEM_NET_PARAM,&(phandle->net_param),sizeof(phandle->net_param)); 
#if 1
	SYS_WARNING("phandle->net_param.ipmode=0x%x ntpaddr=%s ipaddr=%s ipmask=%s ipgateway=%s wireless_ipaddr=%s center_ipaddr=%s center_loginid=%s center_port=%d port=%d \r\n"\
			,phandle->net_param.ipmode
			,phandle->net_param.ntpaddr
			,phandle->net_param.ipaddr
			,phandle->net_param.ipmask
			,phandle->net_param.ipgateway
			,phandle->net_param.wireless_ipaddr
			,phandle->net_param.center_ipaddr
			,phandle->net_param.center_loginid
			,phandle->net_param.center_port
			,phandle->net_param.port
			);
#endif
	if ( UNIX_CREATE(UNIXFILE_SOCKET_CLIENTPC, &(phandle->unix_fd), 0 ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_CLIENTPC);
		return succeed_type_failed;
	}
	phandle->socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(phandle->socket_fd==-1)
	{	SYS_ERROR("Socket Create Failed.\r\n");
		return succeed_type_failed;
	}
	client_pc_poll_init(phandle);
	//信号量初始化操作
	memset(&signal, 0x00, sizeof(signal));
	signal.signo[0] = SIGINT;
	signal.sig_phandle[0] = sig_sigint_proc;
	signal.signo[1] = SIGALRM;
	signal.sig_phandle[1] = sig_sigalrm_proc;
	SIGNAL_INIT(&signal, 2);

	return succeed_type_succeed;
}

succeed_type client_pc_exit( CLIENT_PC_PRIVATE_HANDLE *phandle)
{
	if ( NULL == phandle )
	{
		SYS_ERROR("in storage_exit phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	GBSHM_DESTROY(phandle->pgbshm_obj);
	if ( UNIX_DESTROY(UNIXFILE_SOCKET_CLIENTPC, phandle->unix_fd ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_DESTROY %s failed.\r\n", UNIXFILE_SOCKET_CLIENTPC);
		return succeed_type_failed;
	}
	SOCKET_CLOSE(global_client_pc_private_handle.socket_fd);

	return succeed_type_succeed;
}

void sig_sigint_proc(int sig)
{
	SYS_WARNING("Client_pc Rcv Signo = %d \r\n",sig);
	IsClientpcSysQuit = 1;
}

void sig_sigalrm_proc(int sig)
{
	int htlens = 68;

	if(gtime_no_heart_res==0)
	{
	    gcount_sigalrm_handle=0;
	}
	
	gcount_sigalrm_handle++;//如果溢出怎么处理?
        //计算无心跳回应的持续时间
	gtime_no_heart_res = global_client_pc_private_handle.heart_time/2+(gcount_sigalrm_handle-1)*global_client_pc_private_handle.heart_time;
	SYS_WARNING("--------- -time now is  %d--------- \r\n",gtime_no_heart_res);

	//如果连续60秒没有收到CMS的心跳回应，则执行:
	if(gtime_no_heart_res>=60)
	{
		SYS_WARNING("$$$$$$$$$$$  The Connection between PU and CMS is dead now!$$$$$$$$$$$$$ \r\n");
		SOCKET_CLOSE(global_client_pc_private_handle.socket_fd);
		SYS_WARNING("$$$$$$$$$$$  The Connection between PU and CMS has been closed!$$$$$$$$$$$$$ \r\n");
		//连接断开之后如何处理?
		
		
		
	}
	CLIENT_PC_PRIVATE_HANDLE *phandle = &global_client_pc_private_handle;
	if(SOCKET_SEND(phandle->socket_fd,phandle->heartbeatbuf,&htlens,NULL)==-1)
	{
		SYS_ERROR("heartbeat send failed:%d\r\n",errno);
	}

	
	alarm(global_client_pc_private_handle.heart_time);
	
}

void SignalHandle(int sig)
{
	int htlens = 68;
	CLIENT_PC_PRIVATE_HANDLE *phandle = &global_client_pc_private_handle;
	if(SOCKET_SEND(phandle->socket_fd,phandle->heartbeatbuf,&htlens,NULL)==-1)
	{
		SYS_ERROR("heartbeat send failed:%d\r\n",errno);
	}
	SYS_WARNING("signal Handler send...\r\n");
	alarm(global_client_pc_private_handle.heart_time/2);
}

int main(int argc, char **argv)
{
	int poll_err_count = 0;
	int htlens = 68;
	memset(&global_client_pc_private_handle, 0x00, sizeof(global_client_pc_private_handle));
	SYS_INFO("Enter client pc main func.\r\n");
	client_pc_init(&global_client_pc_private_handle);

	while(0 == IsClientpcSysQuit)
	{		    
		/*连接中心服务器*/
		if ( client_pc_connect(&global_client_pc_private_handle)==succeed_type_failed )
		{
			SYS_ERROR("Connect Failed.\r\n");
			sleep(3);
			continue;
		}
		if(SOCKET_SEND(global_client_pc_private_handle.socket_fd,global_client_pc_private_handle.heartbeatbuf,&htlens,NULL)==-1)
		{
			SYS_ERROR("heartbeat send failed:%d\r\n",errno);
		}
		alarm(global_client_pc_private_handle.heart_time / 2);
		while(0 == IsClientpcSysQuit)
		{
			if ( poll(global_client_pc_private_handle.polltimeout,global_client_pc_private_handle.active_fdcount,1000*global_client_pc_private_handle.heart_time) > 0 )
			{	

				/*接收其他进程数据*/
				if ( global_client_pc_private_handle.polltimeout[0].revents )
				{
					client_pc_unix_deal(&global_client_pc_private_handle);
				}

				/*接收中心服务器数据*/
				if(global_client_pc_private_handle.polltimeout[1].revents)
				{
					SYS_WARNING("recv clientpc event...\r\n");
					poll_err_count = 0;
					client_pc_socket_deal(&global_client_pc_private_handle);
				}
			}
			else
			{
				poll_err_count++;
				if ( poll_err_count==3 )
				{
					poll_err_count = 0;
					SYS_WARNING("NetWork DisConnect .\r\n");
				}
			}
			client_pc_poll_init(&global_client_pc_private_handle);//poll_init
		}//end_small_while
		init_socket(&global_client_pc_private_handle);
		client_pc_poll_init(&global_client_pc_private_handle);//poll_init
		sleep(3);
	}//end_big_while	
	client_pc_exit(&global_client_pc_private_handle);
	SYS_WARNING("Client_pc Quit!!\r\n");
	return 0;
}//end_main
