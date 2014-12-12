#include "misc_private.h"

succeed_type misc_gps(MISC_PRIVATE_HANDLE *phandle)
{
	unsigned char recvdata[1024];
	unsigned int index = 0;

	if (phandle == NULL)
	{
		SYS_ERROR("libgps_params failed.\r\n");
		return succeed_type_failed;
	}
	memset(recvdata, 0x00, 1024);
	GPS_READ(phandle->pgps_obj, recvdata);
	for (index = 0; index < 320; index++)
	{
		if ( index == 160)
		{
	//		printf("\r\n\r\n");
		}
	//	printf("%c", recvdata[index]);
	}
	sleep(5);
	return succeed_type_succeed;
}
