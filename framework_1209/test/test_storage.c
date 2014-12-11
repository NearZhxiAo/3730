#include "system_includes.h"
#include "system_debug.h"
#include "libunix.h"

int main(int argc, char **argv)
{
		int sockfd = 0;
		UNIX_CMD_STRUCT cmd;
		alert_event *ptreventvalue = NULL;
		unsigned char filename[64];

		memset(&cmd, 0x00, sizeof(cmd));

		memset(filename, 0x00, 64);

		UNIX_CREATE(UNIXFILE_SOCKET_STORAGE, &sockfd, 0);

		while(1)
		{
				if (succeed_type_succeed == UNIX_POLL(sockfd, -1) )
				{
						//UNIX_RECV(sockfd, filename, &cmd);
						if (cmd.cmd == PARACMD_ALERT_ALERT_EVENT)
						{
								ptreventvalue = (alert_event *)(cmd.cmdinfo);
								SYS_INFO("recv ptreventvalue = 0x%x.\r\n", ptreventvalue->event_type);
						}
				}
		}
		UNIX_DESTROY(UNIXFILE_SOCKET_STORAGE, sockfd);
		return 0;	
}
