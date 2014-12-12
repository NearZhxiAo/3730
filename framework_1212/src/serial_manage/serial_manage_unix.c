#include "serial_manage_private.h"

static succeed_type serial_manage_unix_deal( SERIAL_MANAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	UNIX_CMD_STRUCT cmd;

	switch(pcmd->cmd)
	{
		case PARACMD_QT_QT_TO_SERIAL_MEASURE:  
			{

			}
		default:
			break;
	}
	return succeed_type_succeed;
}

succeed_type serial_manage_unix(SERIAL_MANAGE_PRIVATE_HANDLE *phandle)
{
	unsigned char remotename[128];
	UNIX_CMD_STRUCT unix_rcv_cmd;

	memset(remotename, 0x00, 128);
	memset(&unix_rcv_cmd, 0x00, sizeof(UNIX_CMD_STRUCT));

	//1. recv the unix event
	if ( UNIX_RECV( phandle->unix_fd, remotename, &unix_rcv_cmd) == succeed_type_failed )
	{
		SYS_ERROR("UNIX RECV failed.\r\n");
		return succeed_type_failed;
	}
	SYS_INFO("Rcv Event from %s in alert.\r\n", remotename); 

	//2. deal the receiving event.
	serial_manage_unix_deal(phandle, &unix_rcv_cmd);
	return succeed_type_succeed;
}
