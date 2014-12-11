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

	GPIO_SETOUTPUT(p, 	GPIO_OUTPUT_DSP_TVP5150_LOW);
	sleep(2);
	printf("set 5150 low.\r\n");

	GPIO_SETOUTPUT(p, 	GPIO_OUTPUT_DSP_TVP5150_HI);
	
	printf("set 5150 high.\r\n");
    GPIO_DESTROY(p);
    return 0;
}
