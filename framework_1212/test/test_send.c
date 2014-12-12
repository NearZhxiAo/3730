/***************************************
  Program write for test the process conmunication

Examples: test_send program produce alert_event_one, and send it out

Exactly: test_avenc and test_storage can recv this event. 

but test_rtsp can't recv this event. This is the same with the intercmd.def definition

such as: Line 41, for alert_event_one, the dest is storage and avenc

 ***************************************/
#include "system_includes.h"
#include "system_debug.h"
#include "libunix.h"

int main(int argc, char **argv)
{
		int sockfd = 0;
		UNIX_CMD_STRUCT cmd;	
		frameratetype framerate;

		framerate = 0x00;
		memset(&cmd, 0x00, sizeof(cmd));
	//	memset(&framerate, 0x00, sizeof(framerate));

		UNIX_CREATE(UNIXFILE_SOCKET_TEST, &sockfd, 0);

		cmd.cmd = PARACMD_BOA_BOATOAVENC_PARA_SETFRAMERATE;
		cmd.cmdlen = sizeof(framerate);

		memcpy(cmd.cmdinfo, &framerate, sizeof(framerate));

		UNIX_SEND(sockfd, &cmd);
		UNIX_DESTROY(UNIXFILE_SOCKET_TEST, sockfd);
		return 0;

}
