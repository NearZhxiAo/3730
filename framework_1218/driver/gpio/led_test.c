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

	while(1)
	{
#if 1
		GPIO_PWR_RED(p);
		SYS_ERROR("power red \r\n");
		sleep(5);
		GPIO_PWR_GREEN(p);
		SYS_ERROR("power green \r\n");
		sleep(5);
#endif
#if 0
		GPIO_STATE_RED(p);
		SYS_WARNING("state red \r\n");
		sleep(5);
		GPIO_STATE_GREEN(p);
		SYS_ERROR("state green \r\n");
		sleep(5);
#endif
#if 0
		GPIO_3G_GREEN_ON(p);
		SYS_ERROR("3G on \r\n");
		sleep(5);
		GPIO_3G_GREEN_OFF(p);
		SYS_ERROR("3G off \r\n");
		sleep(5);
#endif

	}

	SYS_INFO("CLR\r\n");
    GPIO_DESTROY(p);
    return 0;
}
