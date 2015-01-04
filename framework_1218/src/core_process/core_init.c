#include "core_private.h"
#include "libcmem.h"

CMEM_AllocParams cmem_params;

void core_poll_init(CORE_PROCESS_PRIVATE_HANDLE *phandle)
{
	memset(phandle->polltimeout, 0x00, sizeof(struct pollfd) * CORE_PROCESS_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;		
	phandle->polltimeout[0].events = POLLIN;

	phandle->polltimeout[1].fd = phandle->capture_fd;
	phandle->polltimeout[1].events = POLLIN ;
	
	phandle->active_fdcount = 2;
}

succeed_type core_init(CORE_PROCESS_PRIVATE_HANDLE *phandle)
{

	if (NULL == phandle) 
    {
		SYS_ERROR("CORE_PROCESS_PRIVATE_HANDLE must be null.\r\n");
		return succeed_type_failed;
	}

	phandle->gbshm_handle = Gbshm_init();
	if (NULL == phandle->gbshm_handle) 
    {
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

#if 0
    CaptureAttr attr = Cap_Attrs_DEFAULT;	
    phandle->capture_handle = Capture_init(&attr);
    if (NULL == phandle->capture_handle) 
    {
		SYS_ERROR("capture_handle failed.\r\n");
		return succeed_type_failed;
	}
	phandle->capture_fd = phandle->capture_handle->capture_fd;

    phandle->display_handle = Display_init(&Dis_Attrs_DEFAULT);
    if (NULL == phandle->display_handle) 
    {
		SYS_ERROR("display_handle failed.\r\n");
		return succeed_type_failed;
	}
	phandle->display_fd= phandle->display_handle->display_fd;
#endif

    if (Unix_create(UNIXFILE_SOCKET_CORE, &(phandle->unix_fd), 0 ) == succeed_type_failed) 
    {
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_CORE);
		return succeed_type_failed;
	}

    Jpeg_init();

    core_poll_init(phandle);

    if (pthread_mutex_init(&phandle->state_lock, NULL) != 0) 
    {
        SYS_ERROR("pthread_mutex_init mutex_state error.\r\n");
		return succeed_type_failed;
    }

    cmem_params.type        = CMEM_POOL;
    cmem_params.flags       = CMEM_NONCACHED;
    cmem_params.alignment   = 32;

    phandle->jpeg_inbuffer  = (char *)CMEM_alloc(JPEG_BUFFER_SIZE, &cmem_params);
    phandle->jpeg_outbuffer = (char *)CMEM_alloc(JPEG_BUFFER_SIZE, &cmem_params);

    if (phandle->jpeg_inbuffer == NULL || phandle->jpeg_outbuffer == NULL) 
    {
        SYS_ERROR("CMEM_alloc jpeg buffer error.\r\n");
        goto release_buffer;
    }

    Jpeg_init(phandle->jpeg_inbuffer, phandle->jpeg_outbuffer);

    SYS_INFO("core_process success!");
	return succeed_type_succeed;

release_buffer:
    if (phandle->jpeg_inbuffer != NULL) {
        CMEM_free(phandle->jpeg_inbuffer, &cmem_params);
    }

    if (phandle->jpeg_inbuffer != NULL) {
        CMEM_free(phandle->jpeg_outbuffer, &cmem_params);
    }

    return succeed_type_failed;
}
