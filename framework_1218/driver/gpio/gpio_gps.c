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
	GPIO_PRINT(p);
	//设置gpio gpio25 gpio34
    //p->ppinmux_reg->pinmux_3 |= (1<<29);
    //p->ppinmux_reg->pinmux_3 |= (1<<30);


    //p->ppinmux_reg->pinmux_4 |= (1<<14);
    //p->ppinmux_reg->pinmux_4 |= (1<<15);
	
	GPIO_PRINT(p);
    GPIO_DESTROY(p);
    return 0;
}
