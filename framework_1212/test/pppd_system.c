#include "system_includes.h"
#include "system_debug.h"
#include "libgpio.h"
LIBGPIO_OBJ *pgpio_obj = NULL;

#define TEST_IP "www.baidu.com"
static void pingnet(LIBGPIO_OBJ *phandle)
{
	unsigned char cmd[128];
	int iping_ret = -1;
	memset(cmd, 0x00, 128);
	sprintf(cmd,"ping -c 1 %s >/dev/null",TEST_IP);

	{
		iping_ret = system(cmd);
		if( 0 == iping_ret) // 0x000-0x3FF
		{
			GPIO_3G_GREEN_ON(phandle);
			printf("green on 3G normal.\r\n");
		}
		else
		{
			GPIO_3G_GREEN_OFF(phandle);
			printf("green on 3G abnormal.\r\n");
		}
	}
}
void pthread_func(void* phandle)
{
	int fd;
	system("/opt/install/sbin/3g_reset &");
	sleep(10);
	while(1)
	{
		fd=open("/var/log/pppfile",O_RDONLY);
		if(fd==-1)
		{	
			system("killall pppd");
			sleep(5);
			system("pppd call ppp0 2 >/dev/null &");
			SYS_WARNING("system pppd!!\r\n");
			sleep(20);
			pingnet(phandle);
		}
		else
		{
			pingnet(phandle);
			close(fd);
			sleep(8);
		}
		sleep(60);
	}
}


int main(int argc, char **argv)
{

	pgpio_obj = GPIO_INIT();
	if ( NULL == pgpio_obj )
	{
		printf("GPIO failed.\r\n");
	}
	pthread_func(pgpio_obj);
	return 0;
}


