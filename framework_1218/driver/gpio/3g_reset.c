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
	printf("3g reset!!\r\n");
	printf("3g set high!!\r\n");
	GPIO_SETOUTPUT(p, 	GPIO_OUTPUT_DSP_3G_HI);
	usleep(500000);
	printf("3g set low!!\r\n");
	GPIO_SETOUTPUT(p, 	GPIO_OUTPUT_DSP_3G_LOW);
	usleep(500000);
	printf("3g set high!!\r\n");
	GPIO_SETOUTPUT(p, 	GPIO_OUTPUT_DSP_3G_HI);

    GPIO_DESTROY(p);
    return 0;
}
