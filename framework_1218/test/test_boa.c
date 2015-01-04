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
		int sockfd ;

		UNIX_CMD_STRUCT cmd;	
		frameratetype framerate;
		bitratetype bitrate;

		SYS_INFO("before unix create!!!\r\n");
		bitrate = 1456;
		framerate =3;           // VIDEO_ENC_20FPS

		memset(&cmd, 0x00, sizeof(cmd));
		//memset(&framerate, 0x00, sizeof(framerate));
		SYS_INFO("before unix create!!!\r\n");
		UNIX_CREATE(UNIXFILE_SOCKET_TEST, &sockfd, 0);

		cmd.cmd = PARACMD_BOA_BOATOAVENC_PARA_SETFRAMERATE;
		cmd.cmdlen = sizeof(framerate);

		memcpy(cmd.cmdinfo, &framerate, sizeof(framerate));
#if 0
		cmd.cmd = PARACMD_BOA_BOATOAVENC_PARA_SETBITRATE;
		cmd.cmdlen = sizeof(bitrate);

		memcpy(cmd.cmdinfo, &bitrate, sizeof(bitrate));

#endif

		SYS_INFO("before unix send!!!\r\n");
		UNIX_SEND(sockfd, &cmd);

		SYS_INFO("before unix destroy!!!\r\n");
		UNIX_DESTROY(UNIXFILE_SOCKET_TEST, sockfd);
		return 0;

}
