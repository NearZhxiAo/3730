#include "system_includes.h"
#include "system_debug.h"
#include "libi2c.h"

/* Standard Linux headers */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>


#if 0
//modified by zhoumeng
int main(int argc, char **argv)
{
	unsigned char dev_5150addr = 0x1A << 1;
	unsigned int i = 0;
	unsigned char regvalue = 0x00;
	LIBI2C_OBJ *phandle = I2C_INIT(dev_5150addr);
	for (i  = 0; i < 0x4 ; i++)
	{
		if ( I2C_SETREG(phandle, i, 0x00)!=succeed_type_succeed )
			//if ( I2C_GETREG(phandle, i, &regvalue)!=succeed_type_succeed )
		{
			printf("set error %d \r\n", i);
			break;
		}
		printf("read reg 0x%x value is 0x%x.\r\n", i, regvalue);
		regvalue = 0x00;
	}
	I2C_SETREG(phandle, 0x09, 0x10);
	I2C_GETREG(phandle, 0x09, &regvalue);
	printf("read regvalue 0x%x.\r\n", regvalue);
	I2C_SETREG(phandle, 0x10, 0x10);
	I2C_DESTROY(phandle);	
	return 0;
}
#endif

int main(int argc, char **argv)
{
//	unsigned char dev_aic23addr = 0x1A << 1;
	unsigned char dev_aic23addr = 0x10 << 1;
	printf("addr = 0x10..\r\n");
#if 1
	unsigned int i = 0;
	LIBI2C_OBJ *phandle = I2C_INIT(dev_aic23addr);

	I2C_SETREG16(phandle, 0x00, 0x97);
	I2C_SETREG16(phandle, 0x01, 0x97);
	I2C_SETREG16(phandle, 0x02, 0xf9);
	I2C_SETREG16(phandle, 0x03, 0xf9);
	I2C_SETREG16(phandle, 0x04, 0x124);
	I2C_SETREG16(phandle, 0x05, 0x08);
	I2C_SETREG16(phandle, 0x06, 0x05);
	printf("after setreg ...\r\n\r\n");
#endif

	//	I2C_SETREG(phandle, 0x10, 0x10);
	//	I2C_DESTROY(phandle);	
	return 0;
}
