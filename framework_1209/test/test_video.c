#include "system_debug.h"

int main(int argc, char **argv)
{
	int fd = 0;

//	fd = open("/dev/video0", O_RDWR, 0);
	fd = open("/dev/video0", O_RDWR, 0);

	if (fd < 0 )
	{
		printf("open faile.d\r\n");
	}
	else
	{
		close(fd);
	}
	return 0;
}
