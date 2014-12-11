#include "boa_private.h"

BOA_PRIVATE_HANDLE global_boa_private_handle;
int IsBoaSysQuit = 0;

static void boa_poll_init(BOA_PRIVATE_HANDLE *phandle)
{
	memset(&(phandle->polltimeout), 0x00, sizeof(struct pollfd) * BOA_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;
	phandle->polltimeout[0].events = POLLIN;

	phandle->active_fdcount = 1;

}
static succeed_type boa_init(BOA_PRIVATE_HANDLE *phandle)
{
	if ( NULL == phandle )
	{
		SYS_ERROR("in boa_init phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	//1. 驱动句柄初始化
	phandle->gbshm_handle = Gbshm_init();
	if ( NULL == phandle->gbshm_handle )
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	//2. 创建本进程绑定的UNIX文件，返回值为unix_fd
	if ( Unix_create(UNIXFILE_SOCKET_BOA, &(phandle->unix_fd), 0 ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_BOA);
		return succeed_type_failed;
	}

	//3. 本进程模块的poll初始化函数
	boa_poll_init(phandle);

	return succeed_type_succeed;
}

static succeed_type boa_exit(BOA_PRIVATE_HANDLE *phandle)
{
	Gbshm_destroy(phandle->gbshm_handle);
	if ( Unix_destroy(UNIXFILE_SOCKET_BOA, phandle->unix_fd) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_BOA);
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}

void* boa_pollthread_func(void* arg)
{
	while(0 == IsBoaSysQuit)
	{
		if ( poll( &(global_boa_private_handle.polltimeout), global_boa_private_handle.active_fdcount , -1 ) > 0 )
		{
			SYS_INFO("POLL event found.\r\n");
			if ( global_boa_private_handle.polltimeout[0].revents )
			{
				//do UNIX recv event.
				boa_unix(&global_boa_private_handle);
			}
			if ( global_boa_private_handle.polltimeout[1].revents )
			{
			}
			boa_poll_init(&global_boa_private_handle);
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	pthread_t PollThread;
	memset(&global_boa_private_handle, 0x00, sizeof(global_boa_private_handle));
	boa_init(&global_boa_private_handle);	
	
	if(pthread_create(&PollThread, NULL, boa_pollthread_func, (void *)&global_boa_private_handle))
	{
		SYS_INFO("Enter boa_manager func.\r\n");
		return -1;
	}	
	boa(argc,argv);
	boa_exit(&global_boa_private_handle);
	pthread_join(PollThread, NULL);
	SYS_WARNING("BOA Quit !!\r\n");
	return 0;
}

	
