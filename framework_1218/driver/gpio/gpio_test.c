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
	GPIO_SETOUTPUT(p, 	GPIO_OUTPUT_DSP_TVP5150_HI);
//	sleep(2);

	I2C_RESET(p);
	printf("dasadsada\r\n");

	//GPIO_SETOUTPUT(p,GPIO_OUTPUT_PWR_LED_RED);
	//GPIO_SETOUTPUT(p,GPIO_OUTPUT_PWR_LED_GREEN);
	//GPIO_SETOUTPUT(p,GPIO_OUTPUT_STATE_LED_RED);
	//GPIO_SETOUTPUT(p,GPIO_OUTPUT_STATE_LED_GREEN);
	//GPIO_SETOUTPUT(p,GPIO_OUTPUT_DSP_PWR_EN);

	SYS_INFO("SET\r\n");

	//GPIO_PRINT(p);
	//GPIO_CLROUTPUT(p,GPIO_OUTPUT_PWR_LED_RED);
	//GPIO_CLROUTPUT(p,GPIO_OUTPUT_PWR_LED_GREEN);
	//GPIO_CLROUTPUT(p,GPIO_OUTPUT_STATE_LED_RED);
	//GPIO_CLROUTPUT(p,GPIO_OUTPUT_STATE_LED_GREEN);
	//GPIO_CLROUTPUT(p,GPIO_OUTPUT_DSP_PWR_EN);

	SYS_INFO("CLR\r\n");
	//GPIO_PRINT(p);
    GPIO_DESTROY(p);
    return 0;
}
