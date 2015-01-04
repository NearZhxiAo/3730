#include "serial_manage_private.h"
SERIAL_MANAGE_PRIVATE_HANDLE global_serial_manage_private_handle;
//pthread_t serial_manage_pthread_t;

static void serial_manage_poll_init(SERIAL_MANAGE_PRIVATE_HANDLE *phandle)
{
	memset(phandle->polltimeout, 0x00, sizeof(struct pollfd) * SERIAL_MANAGE_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;
	phandle->polltimeout[0].events = POLLIN;

	phandle->polltimeout[1].fd = phandle->serial_fd;
	phandle->polltimeout[1].events = POLLIN;

	phandle->active_fdcount = 2;
}
static succeed_type serial_manage_init(SERIAL_MANAGE_PRIVATE_HANDLE *phandle)
{
	if (NULL == phandle)
	{
		SYS_ERROR("in serial_manage_init phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	
	phandle->gbshm_handle = GBSHM_INIT();
	if (NULL == phandle->gbshm_handle)
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	if (UNIX_CREATE(UNIXFILE_SOCKET_SERIAL_MANAGE, &(phandle->unix_fd), 0 ) == succeed_type_failed)
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_SERIAL_MANAGE);
		return succeed_type_failed;
	}

	phandle->serial_fd = serial_init();
	serial_manage_poll_init(phandle);
	return succeed_type_succeed;
}


int main(int argc, char **argv)
{
	/*init global_serial_manage_private_handle*/
	memset(&global_serial_manage_private_handle, 0x00, sizeof(global_serial_manage_private_handle));
	serial_manage_init(&global_serial_manage_private_handle);	
	SYS_INFO("Enter serial_manage_manager func.\r\n");
	//pthread_create(&serial_manage_pthread_t, NULL, serial_main_func, (void *)&global_serial_manage_private_handle)
	
	while(1)
	{
		if (poll(global_serial_manage_private_handle.polltimeout, global_serial_manage_private_handle.active_fdcount , -1) > 0) 
        {
			SYS_INFO("POLL event found.\r\n");
			if (global_serial_manage_private_handle.polltimeout[0].revents)
			{
				//do UNIX recv event.
				serial_manage_unix(&global_serial_manage_private_handle);
			}
			else if (global_serial_manage_private_handle.polltimeout[1].revents )
			{
					serial_main_func(&global_serial_manage_private_handle)
			}
			serial_manage_poll_init(&global_serial_manage_private_handle);
		}
	}

	return 0;
}
