//GPS的测试程序，用于从GPS设备接收GPS所发送过来的数据
//调用顺序是：先POLL查看是否有数据，然后 再调用READ去获取数据
//实际使用中还需要去解析收到的数据
#include "system_includes.h"
#include "system_debug.h"
#include "libgps.h"
#include "sys/poll.h"
int main(int argc, char **argv)
{
	struct pollfd polltimeout;
	unsigned char recvdata[1024];
	unsigned int index = 0;
	LIBGPS_OBJ *p = GPS_INIT();
	if (p == NULL)
	{
		SYS_ERROR("libgbshm_init failed.\r\n");
		return 0;
	}
	memset(&polltimeout, 0x00, sizeof(polltimeout));
	polltimeout.fd = p->gps_fd;
	polltimeout.events = POLLIN;
	printf("\r\n begin to poll\r\n");
	while(1)
	{
		if ( poll( &polltimeout, 1, -1) > 0 )
		{
			memset(recvdata, 0x00, 1024);
			GPS_READ(p, recvdata);
			for (index = 0; index < 320; index++)
			{
				if ( index == 160)
				{
					printf("\r\n\r\n");
				}
				printf("%c", recvdata[index]);
			}
		}
		sleep(5);
	}
	GPS_DESTROY(p);
	return 0;
}
