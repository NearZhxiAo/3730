#include "ptz_private.h"
PTZ_PRIVATE_HANDLE global_ptz_private_handle;

static void ptz_poll_init(PTZ_PRIVATE_HANDLE *phandle)
{
	memset(phandle->polltimeout, 0x00, sizeof(struct pollfd) * PTZ_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;
	phandle->polltimeout[0].events = POLLIN;

	phandle->active_fdcount = 1;
}
static succeed_type ptz_init(PTZ_PRIVATE_HANDLE *phandle)
{
	if (NULL == phandle)
	{
		SYS_ERROR("in ptz_init phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	
	phandle->gbshm_handle = Gbshm_init();
	if (NULL == phandle->gbshm_handle)
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	if (Unix_create(UNIXFILE_SOCKET_PTZ, &(phandle->unix_fd), 0 ) == succeed_type_failed)
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_PTZ);
		return succeed_type_failed;
	}

	ptz_poll_init(phandle);
	return succeed_type_succeed;
}


#define TEST_IP "www.baidu.com"
int main(int argc, char **argv)
{
	memset(&global_ptz_private_handle, 0x00, sizeof(global_ptz_private_handle));
	ptz_init(&global_ptz_private_handle);	
	SYS_INFO("Enter ptz_manager func.\r\n");
	
	while(1)
	{
		if (poll(global_ptz_private_handle.polltimeout, global_ptz_private_handle.active_fdcount , -1) > 0) 
        {
			SYS_INFO("POLL event found.\r\n");
			if (global_ptz_private_handle.polltimeout[0].revents)
			{
				//do UNIX recv event.
				ptz_unix(&global_ptz_private_handle);
			}
			ptz_poll_init(&global_ptz_private_handle);
		}
	}

	return 0;
}
