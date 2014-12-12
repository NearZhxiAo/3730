/************************************************************************
 * Copyright (C), 2011, liangzhang@xidian.edu.cn
 * FileName:   libgbshm.c
 * Description:
 * Date:	2011-03-16
 * Version:	V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libgps.h"

/******************************************** 
 *    Function    : libgbshm_init
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static LIBGPS_OBJ *libgps_init()
{
	int i = 0;
	LIBGPS_OBJ *ptrhandle_gps = malloc(sizeof(LIBGPS_OBJ));
	if (ptrhandle_gps == NULL)
	{
		SYS_ERROR("malloc phandle failed.\r\n");
		return NULL;
	}
	ptrhandle_gps->gps_fd = open("/dev/gps", O_RDWR);
	if ( ptrhandle_gps->gps_fd <= 0 )
	{
		SYS_ERROR("open fd failed.\r\n");
		free( ptrhandle_gps );
		return NULL;
	}

	return ptrhandle_gps;
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
static succeed_type libgps_destroy(LIBGPS_OBJ *phandle)
{
	if (phandle == NULL)
	{
		SYS_ERROR("ptrhandle_gbshm is NULL.\r\n");
		return succeed_type_failed;
	}
	close(phandle->gps_fd);
	free(phandle);
	return succeed_type_succeed;
}

static succeed_type libgps_read(LIBGPS_OBJ *phandle, unsigned char *pbuf)
{
	if (phandle == NULL)
	{
		SYS_ERROR("ptrhandle_gbshm is NULL.\r\n");
		return succeed_type_failed;
	}
	ioctl(phandle->gps_fd, 0x04, pbuf);
	return succeed_type_succeed;
}

typedef struct SIMCARD_SEND_
{
	unsigned int sendlen;
	unsigned char sendbuf[256];
}SIMCARD_SEND;

static succeed_type libgps_write(LIBGPS_OBJ *phandle, unsigned char *pbuf, unsigned int buflen)
{
	SIMCARD_SEND sendbuf;
	if (phandle == NULL)
	{
		SYS_ERROR("ptrhandle_gbshm is NULL.\r\n");
		return succeed_type_failed;
	}
	memset(&sendbuf, 0x00, sizeof(sendbuf));
	memcpy(sendbuf.sendbuf, pbuf, buflen);
	sendbuf.sendlen = buflen;
	ioctl(phandle->gps_fd, 0x03, &sendbuf);
	return succeed_type_succeed;
}

LIBGPS_OBJ *GPS_INIT()
{
	return libgps_init();
}

succeed_type GPS_READ(LIBGPS_OBJ *phandle, unsigned char *pbuf)
{
	return libgps_read(phandle, pbuf);
}

succeed_type GPS_WRITE(LIBGPS_OBJ *phandle, unsigned char *pbuf, unsigned int len)
{
	return libgps_write(phandle, pbuf, len);
}

succeed_type GPS_DESTROY(LIBGPS_OBJ *phandle)
{
	return libgps_destroy(phandle);
}

