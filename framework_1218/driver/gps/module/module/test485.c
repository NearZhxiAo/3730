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
	sendbuf.sendlen = 5;
	sendbuf.sendbuf[0] = 0x55;
	sendbuf.sendbuf[1] = 0xAA;
	sendbuf.sendbuf[2] = 0x10;
	sendbuf.sendbuf[3] = 0x23;
	sendbuf.sendbuf[4] = 0x45;
	//sendbuf.sendbuf[2] = 'c';
	//sendbuf.sendbuf[3] = 'd';
	//sendbuf.sendbuf[4] = 'e';
	while(1)
	{
		ioctl(fd, SIMCARD_CMD_SEND, &sendbuf);
		printf("send finished.\r\n");
		sleep(3);
	}
	close(fd);

	return 0;
}
