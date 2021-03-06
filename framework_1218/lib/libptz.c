#include "system_includes.h"
#include "system_debug.h"
#include "libptz.h"

static LIBPTZ_OBJ * ptz_init(PTZ_TYPE ptztype)
{
	int i = 0;
	LIBPTZ_OBJ *ptrptz = malloc(sizeof(LIBPTZ_OBJ));
	if (ptrptz == NULL)
	{
		SYS_ERROR("malloc phandle failed.\r\n");
		return NULL;
	}
	ptrptz->pgps = GPS_INIT();
	ptrptz->ptztype = ptztype;
	return ptrptz;
}

static succeed_type ptz_write( LIBPTZ_OBJ *phandle, unsigned char *pbuf, unsigned int len)
{
	if ( (phandle == NULL) || (phandle->pgps == NULL) )
	{
		SYS_ERROR("ptz_write handle is not valid.\r\n");
		return succeed_type_failed;
	}
	
	return GPS_WRITE(phandle, pbuf, len);	
}

LIBPTZ_OBJ * PTZ_INIT( PTZ_TYPE ptztype)
{
	return ptz_init(ptztype);
}

succeed_type PTZ_SET_SPEED(LIBPTZ_OBJ *phandle, PTZ_SPEED speed)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;
	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_UP(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_DOWN(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_LEFT(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_RIGHT(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_ZOOMIN(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_ZOOMOUT(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_AUTOFOCUS(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_AUTOIRIS(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}

succeed_type PTZ_SET_PRESET(LIBPTZ_OBJ *phandle)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}


succeed_type PTZ_GOTO_PRESET(LIBPTZ_OBJ *phandle, unsigned int presetnum)
{
	unsigned char sendbuf[32];
	unsigned int sendlen = 0x00;

	memset(sendbuf, 0x00, 32);

	switch(phandle->ptztype)
	{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
	}
	return ptz_write(phandle, sendbuf, sendlen);
}
