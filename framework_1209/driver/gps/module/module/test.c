#include <stdio.h>
#include <stdlib.h>
  #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

#include "incserial.h"
int main(int argc, char **argv)
{
	int fd = 0;
	SIMCARD_SEND sendbuf;
	
	fd = open("/dev/485", O_RDWR);
	if (fd < 0)
	{
	    printf("open error.\r\n");
	    return -1;
	}
	memset(&sendbuf, 0x00, sizeof(sendbuf));
	sendbuf.sendlen = 10;
	sendbuf.sendbuf[0] = 'a';
	sendbuf.sendbuf[1] = 'b';
	sendbuf.sendbuf[2] = 'c';
	sendbuf.sendbuf[3] = 'd';
	sendbuf.sendbuf[4] = 'e';
	ioctl(fd, SIMCARD_CMD_SEND, &sendbuf);
	close(fd);

	return 0;
}
