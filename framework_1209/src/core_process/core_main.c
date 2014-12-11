#include "core_private.h"

CORE_PROCESS_PRIVATE_HANDLE global_private_handle;

succeed_type core_exit(CORE_PROCESS_PRIVATE_HANDLE *phandle)
{
	if (NULL == phandle) 
    {
		SYS_ERROR("in av_encoder_exit phandle is NULL.\r\n");
		return succeed_type_failed;
	}

	Gbshm_destroy(phandle->gbshm_handle);

	if (Unix_destroy(UNIXFILE_SOCKET_CORE, phandle->unix_fd) == succeed_type_failed) 
    {
		SYS_ERROR("UNIX_DESTROY %s failed.\r\n", UNIXFILE_SOCKET_CORE);
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}

int main(int argc, char **argv)
{
	memset(&global_private_handle, 0x00, sizeof(global_private_handle));
	core_init(&global_private_handle);

	while (1) 
    {     
		SYS_INFO("CORE PROCESS Entering POLL.\r\n");
		if (poll(&(global_private_handle.polltimeout), global_private_handle.active_fdcount, 40000) > 0) 
        {
			if (global_private_handle.polltimeout[0].revents) 
            {		
                process_unix(&global_private_handle);           //unix event
			}

			if (global_private_handle.polltimeout[1].revents) 
            {		
				process_capture(&global_private_handle);        //capture event
			}

			core_poll_init(&global_private_handle);
		}
	}

	core_exit(&global_private_handle);
	return 0;
}
