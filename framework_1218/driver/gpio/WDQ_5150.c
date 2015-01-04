#include "system_includes.h"
#include "system_debug.h"
#include "libgpio.h"

int main(int argc, char **argv)
{
    LIBGPIO_OBJ *p = GPIO_INIT();

    if (p == NULL)
    {
		SYS_ERROR("libgpio_init failed.\r\n");
		return 0;
    }
	SYS_INFO("BEFORE\r\n");
	I2C_RESET(p);
	printf("111111111dasadsada\r\n");
	I2C_RESET(p);


	GPIO_5150_SETLOW(p);
	printf("set low.\r\n");
	sleep(5);

	GPIO_5150_SETHIGH(p);
	printf("dasadsada\r\n");
	//GPIO_PRINT(p);
    GPIO_DESTROY(p);
    return 0;
}
