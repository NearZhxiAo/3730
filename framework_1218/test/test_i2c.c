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



int main(int argc, char **argv)
{
	unsigned char dev_5150addr = 0xB8;
	unsigned int i = 0;
	unsigned char regvalue = 0x00;
	LIBI2C_OBJ *phandle = I2C_INIT(dev_5150addr);

#if 1
	I2C_SETREG(phandle, 0x1C, 0x80);
	for (i  = 0; i < 0xFF; i++)
	{
		if ( I2C_GETREG(phandle, i, &regvalue)!=succeed_type_succeed )
		{
			printf("read error %d \r\n", i);
			break;
		}
		printf("read reg 0x%x value is 0x%x.\r\n", i, regvalue);
		regvalue = 0x00;
	}
#endif
#if 0

	I2C_SETREG(phandle, 0x09, 0x10);
	I2C_GETREG(phandle, 0x09, &regvalue);
	printf("read regvalue 0x%x.\r\n", regvalue);
	I2C_SETREG(phandle, 0x10, 0x10);
#endif

#if 0
	I2C_SETREG(phandle, 0x28, 0x2);  //NTSC
	I2C_GETREG(phandle, 0x28, &regvalue);
	printf(" \r\n  0x28 read regvalue 0x%x.\r\n", regvalue);
#endif

#if 0
	I2C_SETREG(phandle, 0x28, 0x4);  //PAL
	I2C_GETREG(phandle, 0x28, &regvalue);
	printf(" \r\n  0x28 read regvalue 0x%x.\r\n", regvalue);
#endif

	I2C_DESTROY(phandle);	
	return 0;
}
