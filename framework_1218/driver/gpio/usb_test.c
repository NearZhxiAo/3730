#include "system_includes.h"
#include "system_debug.h"
#include "libgpio.h"

int main(int argc, char **argv)
{
    LIBGPIO_OBJ *p = GPIO_INIT();
	int iLoop = 0;
    if (p == NULL)
    {
		SYS_ERROR("libgpio_init failed.\r\n");
		return 0;
    }
	SYS_INFO("BEFORE\r\n");
	printf("USB_STRUCT_LEN/sizeof(int) = 0x%x\r\n",USB_STRUCT_LEN/sizeof(int));
	for(iLoop = 0; iLoop < USB_STRUCT_LEN/sizeof(int) ; iLoop ++)
	{
		if(iLoop < 0x3C)
		{
			printf("USB-REG0x%x:0x%x\r\n",iLoop, p->pusb_reg[iLoop]);
		}
		else
		{
			printf("USB-REG0x%x:0x%x\r\n",iLoop, p->pusb_reg->usb_test[iLoop - 0x3C]);
		}
	}
    GPIO_DESTROY(p);
    return 0;
}
