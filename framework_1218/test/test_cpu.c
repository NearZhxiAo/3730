/* @code
 * cpu_main.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <libcpuinfo.h>
int main( int argc, char **argv )
{
	Cpu_Device device;
	int load;
	Cpu_Handle hCpu = NULL;
	Cpu_Attrs cpuAttrs = Cpu_Attrs_DEFAULT;
	//Dmai_init();
	hCpu = CPU_CREATE(&cpuAttrs);
	if(NULL == hCpu)
	{
		 printf("Cpu_create error!!\r\n");
		 return -1;
	}
	CPU_GET_DEVICE(hCpu, &device);
	CPU_GET_LOAD(hCpu, &load);
	printf("Device type is %s and cpu is loaded %d%%\n",
			CPU_GET_DEVICE_NAME(device), load);
	CPU_DELETE(hCpu);
	return 0;
}

/* @endcode
 *
 */

