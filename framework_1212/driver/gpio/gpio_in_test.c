#include "system_includes.h"
#include "system_debug.h"
#include "libgpio.h"

int main(int argc, char **argv)
{
    LIBGPIO_OBJ *p = GPIO_INIT();
	int in_data = -1;
    if (p == NULL)
    {
		SYS_ERROR("libgpio_init failed.\r\n");
		return 0;
    }
	GPIO_PRINT(p);
	SYS_INFO("BEFORE\r\n");
	GPIO_GETINPUT(p, 	GPIO_INPUT_DSP_TRIG, &in_data);
	printf("dsp_trig in_data = %d\r\n", in_data);
	GPIO_GETINPUT(p, 	GPIO_INPUT_DSP_KEY_DECT, &in_data);
	printf("key_dect in_data = %d\r\n", in_data);
	
//	GPIO_CLROUTPUT(p, 	GPIO_OUTPUT_DSP_PWR_EN);
//	printf("dsp_en data = 0x%x\r\n",p->pgpio_reg->gpio_set_data23);
	GPIO_SETOUTPUT(p, 	GPIO_OUTPUT_DSP_PWR_EN);
	printf("dsp_en data = 0x%x\r\n",p->pgpio_reg->gpio_set_data23);
    GPIO_DESTROY(p);
    return 0;
}
