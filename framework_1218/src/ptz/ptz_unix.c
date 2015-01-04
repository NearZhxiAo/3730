#include "ptz_private.h"

static succeed_type ptz_unix_deal( PTZ_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	UNIX_CMD_STRUCT cmd;

	switch(pcmd->cmd)
	{
		case PARACMD_CORE_PTZ_START_PRESET:
			//do something;
			cmd.cmd = PARACMD_PTZ_PTZ_START_PRESET_ACK;
			Unix_send(phandle->unix_fd, &cmd);
			break;
		case PARACMD_CORE_PTZ_START_SETPRESET:
			//do something;
			cmd.cmd = PARACMD_PTZ_PTZ_START_SETPRESET_ACK;
			Unix_send(phandle->unix_fd, &cmd);
			break;

		default:
			break;
	}
	return succeed_type_succeed;
}

succeed_type ptz_unix(PTZ_PRIVATE_HANDLE *phandle)
{
	unsigned char remotename[128];
	UNIX_CMD_STRUCT unix_rcv_cmd;

	memset(remotename, 0x00, 128);
	memset(&unix_rcv_cmd, 0x00, sizeof(UNIX_CMD_STRUCT));

	//1. recv the unix event
	if ( Unix_recv( phandle->unix_fd, remotename, &unix_rcv_cmd) == succeed_type_failed )
	{
		SYS_ERROR("UNIX RECV failed.\r\n");
		return succeed_type_failed;
	}
	SYS_INFO("Rcv Event from %s in alert.\r\n", remotename); 

	//2. deal the receiving event.
	ptz_unix_deal(phandle, &unix_rcv_cmd);
	return succeed_type_succeed;
}
