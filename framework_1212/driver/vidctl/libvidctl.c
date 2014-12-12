/************************************************************************
 * Copyright (C), 2011, liangzhang@xidian.edu.cn
 * FileName:   libgbshm.c
 * Description:
 * Date:	2011-03-16
 * Version:	V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libvidctl.h"

/******************************************** 
 *    Function    : libgbshm_init
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static LIBVIDCTL_OBJ *libvidctl_init()
{
    int i = 0;
    LIBVIDCTL_OBJ *ptrhandle_vidctl = malloc(sizeof(LIBVIDCTL_OBJ));
    if (ptrhandle_vidctl == NULL)
    {
	SYS_ERROR("malloc phandle failed.\r\n");
	return NULL;
    }
    ptrhandle_vidctl->vidctl_fd = open("/dev/vidctl", O_RDWR);
    if ( ptrhandle_vidctl->vidctl_fd <= 0 )
    {
	SYS_ERROR("open fd failed.\r\n");
	free( ptrhandle_vidctl );
	return NULL;
    }

    return ptrhandle_vidctl;
}
/******************************************** 
 *    Function    : libgbshm_destroy
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libvidctl_destroy(LIBVIDCTL_OBJ *phandle)
{
    if (phandle == NULL)
    {
	SYS_ERROR("ptrhandle_gbshm is NULL.\r\n");
	return succeed_type_failed;
    }
    close(phandle->vidctl_fd);
    free(phandle);
    return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libgbshm_para_get
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libvidctl_enable(LIBVIDCTL_OBJ *ptrhandle_vidctl, enablemode mode, enableresol resol)
{
	if(resol == ENABLE_RESOL_D1)
	{
//		ioctl(ptrhandle_vidctl->vidctl_fd, 0x01, mode);
		ioctl(ptrhandle_vidctl->vidctl_fd, ENABLE_RESOL_D1, mode);
		return succeed_type_succeed;
	}
	else
	{
		ioctl(ptrhandle_vidctl->vidctl_fd, ENABLE_RESOL_CIF, mode);
		return succeed_type_succeed;
	}
}

static succeed_type libvidctl_disable(LIBVIDCTL_OBJ *ptrhandle_vidctl)
{
    ioctl(ptrhandle_vidctl->vidctl_fd, DISABLE_RSZ, NULL);
    return succeed_type_succeed;
}

LIBVIDCTL_OBJ *VIDCTL_INIT()
{
    return libvidctl_init();
}
succeed_type VIDCTL_DESTROY(LIBVIDCTL_OBJ *phandle)
{
    return libvidctl_destroy(phandle);
}

succeed_type VIDCTL_ENABLE(LIBVIDCTL_OBJ *phandle, enablemode mode, enableresol resol)
{
	return libvidctl_enable(phandle, mode, resol);
}

succeed_type VIDCTL_DISABLE(LIBVIDCTL_OBJ *phandle)
{
    return libvidctl_disable(phandle);
}

