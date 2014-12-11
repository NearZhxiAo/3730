#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int capture_fd;

	capture_fd = open("/dev/video0", O_RDWR | O_NONBLOCK);
	if (capture_fd <= 0 )
	{
		printf("open device /dev/video0 failed.\r\n");
		return -1;
	}

	printf("open success.\r\n");

	input = TVP5146_AMUX_COMPOSITE;
	if ( ioctl(phandle->capture_fd, VIDIOC_S_INPUT, &input) < 0 )
	{
		printf("ioctl VIDIOC_S_INPUT failed.\r\n");
		return -1;
	}


	return 0;
}

