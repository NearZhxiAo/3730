#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>

int main()
{
	int fd = 0;
	int ret = 0;
	unsigned int index = 0;
	char data[1024] = "11111111";
	struct termios option;


	fd = open("/dev/tts/1",O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd<0)
	{
		perror("open tts 1 error.\r\n");
		return 0;
	}

	ret = tcgetattr(fd,&option);
	printf("ret = %d.\r\n",ret);
#if 1
	ret = cfsetispeed(&option,B9600);
	printf("ret = %d.\r\n",ret);
	ret = cfsetospeed(&option,B9600);
	printf("ret = %d.\r\n",ret);
#endif
#if 0 
	option.c_cflag &= ~PARENB;
	option.c_cflag &= ~CSTOPB;
	option.c_cflag &= ~CSIZE;
	option.c_cflag = ~CS8;
	ret = tcsetattr(fd,TCSANOW,&option);
	printf("ret = %d.\r\n",ret);
#endif 
	//ret = write(fd,data,5);
	memset(data, 0x55, 1024);
#if 0
	while(1)
	{
		ret = write(fd, data, 1024);
		printf("write ret = %d.\r\n", ret);
		sleep(3);
	}
#endif
#if 1
	while(1)
	{
		ret = read(fd, data, 64);
		printf("read return value = %d.\r\n", ret);
		if(0 == ret)
		{
			for (index = 0; index < ret; index++)
			{
				printf("index %d value : 0x%x.\r\n", index, data[index]);
			}
		}
		sleep(3);
	}
#endif
#if 0	
	ret = tcgetattr(fd,&option);
	//printf("ret = %d.\r\n",ret);
	cfsetispeed(&option,B115200);
	//printf("ret = %d.\r\n",ret);
	cfsetospeed(&option,B115200);
	//printf("ret = %d.\r\n",ret);

	//no check 8bit
	option.c_cflag &= ~PARENB;
	option.c_cflag &= ~CSTOPB;
	option.c_cflag &= ~CSIZE;
	option.c_cflag = ~CS8;
	tcsetattr(fd,TCSANOW,&option);

	//Odd 7bit
	option.c_cflag = ~PARENB;
	option.c_cflag &= ~PARODD;
	option.c_cflag &= ~CSTOPB;
	option.c_cflag &= ~CSIZE;
	option.c_cflag = ~CS7;
	tcsetattr(fd,TCSANOW,&option);


	//Even 7bit
	option.c_cflag &= ~PARENB;
	option.c_cflag = ~PARODD;
	option.c_cflag &= ~CSTOPB;
	option.c_cflag &= ~CSIZE;
	option.c_cflag = ~CS7;
	tcsetattr(fd,TCSANOW,&option);

	//Space 7bit
	option.c_cflag &= ~PARENB;
	option.c_cflag &= ~CSTOPB;
	option.c_cflag &= ~CSIZE;
	option.c_cflag = ~CS8;
	tcsetattr(fd,TCSANOW,&option);
	//printf("ret = %d.\r\n",ret);
#endif
	close(fd);


	return 0;
}


