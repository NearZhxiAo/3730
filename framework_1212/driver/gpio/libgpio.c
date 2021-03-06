/************************************************************************
 * Copyright (C), 2011, liangzhang@xidian.edu.cn
 * FileName:   libgpio.c
 * Description:
 * Date:	2011-03-16
 * Version:	V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libgpio.h"

#define WDQ_VERSION
/******************************************** 
 *    Function    : libgpio_init
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static int gpio_output[] = { -1, 25, 26, 22, 20, 0, 10, -1};
static void i2c_reset(LIBGPIO_OBJ *phandle)
{
	I2C_STRUCT *p = NULL;
	p = phandle->pi2c_reg;
	printf("id1 = 0x%x, id2 = 0x%x.\r\n",p->pid1, p->pid2 );
	p->mdr = 0;
	p->psc = 11;
	p->clkl = 5;
	p->clkh = 5;
	p->mdr |= 0x0020;
	return ;
}
static LIBGPIO_OBJ *libgpio_init()
{
	int i = 0;
	LIBGPIO_OBJ *phandle_gpio = malloc(sizeof(LIBGPIO_OBJ));
	if (phandle_gpio == NULL)
	{
		SYS_ERROR("malloc phandle failed.\r\n");
		return NULL;
	}
	phandle_gpio->gpio_fd = open("/dev/gpio", O_RDWR);
	if ( phandle_gpio->gpio_fd <= 0 )
	{
		SYS_ERROR("open fd failed.\r\n");
		free( phandle_gpio );
		return NULL;
	}
	phandle_gpio->gpio_mmap = (unsigned int) mmap(0, GPIOREG_STRUCT_LEN, PROT_READ| PROT_WRITE, MAP_SHARED, phandle_gpio->gpio_fd, GPIOREG_PHYADDR_BASE);
	if (phandle_gpio->gpio_mmap == NULL)
	{
		SYS_ERROR("mmap gpio failed.\r\n");
		free(phandle_gpio);
		return NULL;
	}
    phandle_gpio->pgpio_reg = (GPIOREG_STRUCT *)(phandle_gpio->gpio_mmap);

	phandle_gpio->pinmux_mmap = (unsigned int) mmap(0, PINMUX_STRUCT_LEN, PROT_READ| PROT_WRITE, MAP_SHARED, phandle_gpio->gpio_fd, PINMUX_PHYADDR_BASE);
	if (phandle_gpio->pinmux_mmap == NULL)
	{
		SYS_ERROR("mmap pinmux failed.\r\n");
		free(phandle_gpio);
		return NULL;
	}

    phandle_gpio->ppinmux_reg = (PINMUX_STRUCT *)(phandle_gpio->pinmux_mmap);

	phandle_gpio->i2c_mmap = (unsigned int) mmap(0, I2C_STRUCT_LEN, PROT_READ| PROT_WRITE, MAP_SHARED, phandle_gpio->gpio_fd, I2C_PHYADDR_BASE);
	if (phandle_gpio->i2c_mmap == NULL)
	{
		SYS_ERROR("mmap i2c failed.\r\n");
		free(phandle_gpio);
		return NULL;
	}

    phandle_gpio->pi2c_reg = (I2C_STRUCT *)(phandle_gpio->i2c_mmap);

	phandle_gpio->usb_mmap = (unsigned int) mmap(0, USB_STRUCT_LEN, PROT_READ| PROT_WRITE, MAP_SHARED,\
			phandle_gpio->gpio_fd, USB_PHYADDR_BASE);
	if (phandle_gpio->usb_mmap == NULL)
	{
		SYS_ERROR("mmap usb failed.\r\n");
		free(phandle_gpio);
		return NULL;
	}

    phandle_gpio->pusb_reg = (USB_STRUCT *)(phandle_gpio->usb_mmap);

	return phandle_gpio;
}
/*
static LIBGPIO_OBJ *libgpio_init()
{
	int i = 0;
	LIBGPIO_OBJ *phandle_gpio = malloc(sizeof(LIBGPIO_OBJ));
	if (phandle_gpio == NULL)
	{
		SYS_ERROR("malloc phandle failed.\r\n");
		return NULL;
	}
	phandle_gpio->gpio_fd = open("/dev/gpio", O_RDWR);
	if ( phandle_gpio->gpio_fd <= 0 )
	{
		SYS_ERROR("open fd failed.\r\n");
		free( phandle_gpio );
		return NULL;
	}
	phandle_gpio->gpio_mmap = (unsigned int) mmap(0, GPIOREG_STRUCT_LEN, PROT_READ| PROT_WRITE, MAP_SHARED,\
			phandle_gpio->gpio_fd, GPIOREG_PHYADDR_BASE);
    phandle_gpio->pgpio_reg = (GPIOREG_STRUCT *)(phandle_gpio->gpio_mmap);

	phandle_gpio->pinmux_mmap = (unsigned int) mmap(0, PINMUX_STRUCT_LEN, PROT_READ| PROT_WRITE, MAP_SHARED,\
			phandle_gpio->gpio_fd, PINMUX_PHYADDR_BASE);
    phandle_gpio->ppinmux_reg = (PINMUX_STRUCT *)(phandle_gpio->pinmux_mmap);

	return phandle_gpio;
}
*/
/******************************************** 
 *    Function    : libgpio_destroy
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libgpio_destroy(LIBGPIO_OBJ *phandle)
{
	if (phandle == NULL)
	{
		SYS_ERROR("phandle_gpio is NULL.\r\n");
		return succeed_type_failed;
	}
	munmap(phandle->gpio_mmap, GPIOREG_STRUCT_LEN);
	munmap(phandle->pinmux_mmap, PINMUX_STRUCT_LEN);
	munmap(phandle->i2c_mmap, I2C_STRUCT_LEN);
	free(phandle);
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libgpio_getinput
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libgpio_getinput(LIBGPIO_OBJ *phandle, GPIO_INPUT_DEFINE gpio, int *in_data)
{
	if (phandle == NULL)
	{
		SYS_ERROR("phandle_gpio is NULL.\r\n");
		return succeed_type_failed;
	}
#ifndef WDQ_VERSION
	switch(gpio)
	{
		case GPIO_INPUT_START:
			break;
		case GPIO_INPUT_DSP_TRIG:
			phandle->ppinmux_reg->pinmux_1 &= (~(3<<2)); //set the pin1 2-3 as gpio(91)
			phandle->pgpio_reg->gpio_dir45 |= ((1<<27)); //set the dir45 27 as input 
			if((phandle->pgpio_reg->gpio_in_data45 & (1<<27)) == 0) //set the dir45 27 as input 
			{
				*in_data = 0;
			}
			else
			{
				*in_data = 1;
			}
			break;
		case GPIO_INPUT_DSP_PWR:
			break;
		case GPIO_INPUT_DSP_KEY_DECT:
			phandle->ppinmux_reg->pinmux_1 &= (~(3<<0)); //set the pin1 0-1 as gpio(92)
			phandle->pgpio_reg->gpio_dir45 |= ((1<<28)); //set the dir45 28 as input 
			if((phandle->pgpio_reg->gpio_in_data45 & (1<<28)) == 0) //set the dir45 28 as input 
			{
				*in_data = 0;
			}
			else
			{
				*in_data = 1;
			}
			break;
		case GPIO_INPUT_END:
			break;
		default:
			break;
	}
#endif
	return succeed_type_succeed;
}
/******************************************** 
 *    Function    : libgpio_setoutput
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libgpio_setoutput(LIBGPIO_OBJ *phandle, GPIO_OUTPUT_DEFINE gpio)
{
	if (phandle == NULL)
	{
		SYS_ERROR("phandle_gpio is NULL.\r\n");
		return succeed_type_failed;
	}
#ifndef WDQ_VERSION
	switch(gpio)
	{
		case GPIO_OUTPUT_START:
			break;
		case GPIO_OUTPUT_PWR_LED_RED:	//index = 1
			phandle->ppinmux_reg->pinmux_3 &= (~(1<<25)); //set the pin3 25 as gpio(22)
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<22)); //set the dir01 22 as output 
			phandle->pgpio_reg->gpio_set_data01 = ((1<<22)); //set the gpio 22 as high 
			
			break;
		case GPIO_OUTPUT_PWR_LED_GREEN: //index = 2
			phandle->ppinmux_reg->pinmux_3 &= (~(3<<26)); //set the pin3 26\27 as gpio
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<23)); //set the dir01 23 as output 
			phandle->pgpio_reg->gpio_set_data01 = ((1<<23)); //set the gpio 23 as high 

			break;
		case GPIO_OUTPUT_DSP_PWR_EN:
			phandle->ppinmux_reg->pinmux_4 &= (~(3<<10)); //set the pin1 10\11 as gpio
			phandle->pgpio_reg->gpio_dir23 &= (~(1<<(32 - 32))); //set the dir23 0 as output 
			phandle->pgpio_reg->gpio_set_data23 = ((1<<(32 -32))); //set the dir23 0(gpio32) as high
			break;
		case GPIO_OUTPUT_STATE_LED_RED:	//index = 3
			phandle->ppinmux_reg->pinmux_1 |= ((1<<22)); //set the pin1 index 22 as gpio(1)
			phandle->pgpio_reg->gpio_dir45 &= (~(1<<15)); //set the dir45 15(gpio79) as output 
			phandle->pgpio_reg->gpio_set_data45 = ((1<<15)); //set the dir45 15(gpio79) as high
			break;
		case GPIO_OUTPUT_STATE_LED_GREEN://index = 4
			phandle->ppinmux_reg->pinmux_1 &= (~(3<<20)); //set the pin1 index 20 & 21 as gpio(0)
			phandle->pgpio_reg->gpio_dir45 &= (~(1<<16)); //set the dir45 16(gpio80) as output 
			phandle->pgpio_reg->gpio_set_data45 = ((1<<16)); //set the dir45 16(gpio80) as high
			break;
		case GPIO_OUTPUT_3G_LED_GREEN:	//index = 5
			phandle->ppinmux_reg->pinmux_4 &= ~( (3<<0) ); //set the pin1 index 0 & 1 as gpio(0)
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<27)); //set the dir45 15(gpio79) as output 
			phandle->pgpio_reg->gpio_set_data01 = ((1<<27)); //set the dir45 15(gpio79) as high
			break;
		case GPIO_OUTPUT_DSP_TVP5150_HI:
			phandle->ppinmux_reg->pinmux_1 = 0x400000;	//set GIO87 as gpio function
			phandle->pgpio_reg->gpio_dir45 &= (~(1<<23)); //set the dir45 87 as output 
			phandle->pgpio_reg->gpio_set_data45 = ((1<<23)); //set the dir45 87 high 
			
			break;
		case GPIO_OUTPUT_DSP_TVP5150_LOW:
			phandle->ppinmux_reg->pinmux_1 |= 0x400000;	//set GIO87 as gpio function
			phandle->pgpio_reg->gpio_dir45 &= (~(1<<23)); //set the dir45 87 as output 
			phandle->pgpio_reg->gpio_clr_data45 = ((1<<23)); //set the dir45 87 low
			break;
			
		case GPIO_OUTPUT_DSP_3G_HI:
			phandle->ppinmux_reg->pinmux_4 &= ~(3<<26);	//set GIO26 as gpio function
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<26)); //set the dir01 26 as output 

			phandle->pgpio_reg->gpio_set_data01 = (1<<26); //set the dir01 26 high 
			break;
		case GPIO_OUTPUT_DSP_3G_LOW:
			phandle->ppinmux_reg->pinmux_4 |= (3<<26);	//set GIO26 as gpio function
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<26)); //set the dir01 26 as output 
			phandle->pgpio_reg->gpio_clr_data01 = ((1<<26)); //set the dir01 26 low
			break;
		case WDQ_GPIO_OUTPUT_RST_LOW_5150:
			phandle->ppinmux_reg->pinmux_4 &= ~(3<<4);	//set GIO28 as gpio function
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<29)); //set the dir01 28 as output 
			phandle->pgpio_reg->gpio_clr_data01 = ((1<<29)); //set the dir01 28 low
			break;
		case WDQ_GPIO_OUTPUT_RST_HIGH_5150:
			phandle->ppinmux_reg->pinmux_4 &= ~(3<<4);	//set GIO28 as gpio function
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<29)); //set the dir01 28 as output 
			phandle->pgpio_reg->gpio_set_data01 = ((1<<29)); //set the dir01 28 low
			break;

		case GPIO_OUTPUT_END:
			break;
		default:
			break;
	}
#endif
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libgpio_clroutput
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/

static succeed_type libgpio_clroutput(LIBGPIO_OBJ *phandle, GPIO_OUTPUT_DEFINE gpio)
{
	if (phandle == NULL)
	{
		SYS_ERROR("phandle_gpio is NULL.\r\n");
		return succeed_type_failed;
	}
#ifndef WDQ_VERSION
	switch(gpio)
	{
		case GPIO_OUTPUT_START:
			break;
		case GPIO_OUTPUT_PWR_LED_RED:		//index = 1
			phandle->ppinmux_reg->pinmux_3 &= (~(1<<25)); //set the pin3 25 as gpio(22)
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<22)); //set the dir01 22 as output 
			phandle->pgpio_reg->gpio_clr_data01 = ((1<<22)); //set the gpio 22 as high 

			break;
		case GPIO_OUTPUT_PWR_LED_GREEN:		//index = 2
			phandle->ppinmux_reg->pinmux_3 &= (~(3<<26)); //set the pin3 26\27 as gpio
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<23)); //set the dir01 23 as output 
			phandle->pgpio_reg->gpio_clr_data01 = ((1<<23)); //set the gpio 23 as high 
		
			break;
		case GPIO_OUTPUT_DSP_PWR_EN:
			phandle->ppinmux_reg->pinmux_1 &= (~(3<<0)); //set the pin1 0-1 as gpio
			phandle->pgpio_reg->gpio_dir23 |= ((1<<(32 - 32))); //set the dir23 0 as input 
			phandle->pgpio_reg->gpio_clr_data23 = (1<<(32 - 32)); //set the gpio32  as high 
			break;
		case GPIO_OUTPUT_STATE_LED_RED:		//index = 3
			phandle->ppinmux_reg->pinmux_1 |= ((1<<22)); //set the pin1 index 22 as gpio(1)
			phandle->pgpio_reg->gpio_dir45 &= (~(1<<15)); //set the dir45 15(gpio79) as output 
			phandle->pgpio_reg->gpio_clr_data45 = ((1<<15)); //set the dir45 15(gpio79) as high
			break;
		case GPIO_OUTPUT_STATE_LED_GREEN:	//index = 4
			phandle->ppinmux_reg->pinmux_1 &= (~(3<<20)); //set the pin1 index 20 & 21 as gpio(0)
			phandle->pgpio_reg->gpio_dir45 &= (~(1<<16)); //set the dir45 15(gpio79) as output 
			phandle->pgpio_reg->gpio_clr_data45 = ((1<<16)); //set the dir45 15(gpio79) as high

			break;
		case GPIO_OUTPUT_3G_LED_GREEN:	//index = 5
			phandle->ppinmux_reg->pinmux_4 &= ~( (1<<gpio_output[gpio]) | (1<<(gpio_output[gpio]+1)) ); //set the pin1 index 20 & 21 as gpio(0)
			phandle->pgpio_reg->gpio_dir01 &= (~(1<<27)); //set the dir45 15(gpio79) as output 
			phandle->pgpio_reg->gpio_clr_data01 = ((1<<27)); //set the dir45 15(gpio79) as high
			break;

		case GPIO_OUTPUT_END:
			break;
		default:
			break;
	}
#endif
	return succeed_type_succeed;
}

static succeed_type libgpio_ioctl(LIBGPIO_OBJ *phandle, GPIO_OUTPUT_DEFINE gpio)
{
	if (phandle == NULL)
	{
		SYS_ERROR("phandle_gpio is NULL.\r\n");
		return succeed_type_succeed;
	}
	//取决于电平高低 有待修改
}

static void libgpio_printf(LIBGPIO_OBJ *phandle)
{
	if (phandle == NULL)
	{
		SYS_ERROR("phandle_gpio is NULL.\r\n");
		return;
	}
	printf("GPIOREG-PHYADDR:0x%x,GPIOREG-LEN:0x%x\r\n",GPIOREG_PHYADDR_BASE, GPIOREG_STRUCT_LEN);
	printf("PINREG-PHYADDR:0x%x,PINREG-LEN:0x%x\r\n",PINMUX_PHYADDR_BASE, PINMUX_STRUCT_LEN);
	printf("GPIOREG-VADDR:0x%x\r\n",(phandle->pgpio_reg));		
	printf("PINREG-VADDR:0x%x\r\n",(phandle->ppinmux_reg));		
	printf("REG-PID:0x%x\r\n",(phandle->pgpio_reg->gpio_pid));		
	printf("REG-DIR01:0x%x\r\n",(phandle->pgpio_reg->gpio_dir01));		
//	printf("REG-OUT-DATA01:0x%x\r\n",(phandle->pgpio_reg->gpio_out_data01));		
//	printf("REG-SET-DATA01:0x%x\r\n",(phandle->pgpio_reg->gpio_set_data01));		
//	printf("REG-CLR-DATA01:0x%x\r\n",(phandle->pgpio_reg->gpio_clr_data01));		
//	printf("REG-IN-DATA01:0x%x\r\n",(phandle->pgpio_reg->gpio_in_data01));		

	printf("PIN-0:0x%x\r\n",(phandle->ppinmux_reg->pinmux_0));		
	printf("PIN-1:0x%x\r\n",(phandle->ppinmux_reg->pinmux_1));		
	//printf("PIN-2:0x%x\r\n",(phandle->ppinmux_reg->pinmux_2));		
	//printf("PIN-3:0x%x\r\n",(phandle->ppinmux_reg->pinmux_3));		
	//printf("PIN-4:0x%x\r\n",(phandle->ppinmux_reg->pinmux_4));		
}

LIBGPIO_OBJ *GPIO_INIT()
{
	return libgpio_init();
}
succeed_type GPIO_DESTROY(LIBGPIO_OBJ *phandle)
{
	return libgpio_destroy(phandle);
}
succeed_type GPIO_SETOUTPUT(LIBGPIO_OBJ *phandle, GPIO_OUTPUT_DEFINE gpio)
{
	return libgpio_setoutput(phandle, gpio);
}
succeed_type GPIO_GETINPUT(LIBGPIO_OBJ *phandle, GPIO_OUTPUT_DEFINE gpio, int *in_data)
{
	return libgpio_getinput(phandle, gpio, in_data);
}
succeed_type GPIO_CLROUTPUT(LIBGPIO_OBJ *phandle, GPIO_OUTPUT_DEFINE gpio)
{
	return libgpio_clroutput(phandle, gpio);
}
succeed_type GPIO_IOCTL(LIBGPIO_OBJ *phandle, GPIO_OUTPUT_DEFINE gpio)
{
	return libgpio_ioctl(phandle, gpio);
}
void GPIO_PRINT(LIBGPIO_OBJ* phandle)
{
	return libgpio_printf(phandle);
}
void I2C_RESET(LIBGPIO_OBJ* phandle)
{
	return i2c_reset(phandle);
}

succeed_type GPIO_STATE_RED(LIBGPIO_OBJ *phandle)
{
	GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_PWR_LED_GREEN);
	return GPIO_CLROUTPUT(phandle, GPIO_OUTPUT_PWR_LED_RED);
}
succeed_type GPIO_STATE_GREEN(LIBGPIO_OBJ *phandle)
{
	GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_PWR_LED_RED);
	return GPIO_CLROUTPUT(phandle, GPIO_OUTPUT_PWR_LED_GREEN);
}

succeed_type GPIO_STATE_GREEN_OFF(LIBGPIO_OBJ *phandle)
{
	GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_PWR_LED_RED);
	return GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_PWR_LED_GREEN);
}

succeed_type GPIO_PWR_RED(LIBGPIO_OBJ *phandle)
{
	GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_STATE_LED_GREEN);
	return GPIO_CLROUTPUT(phandle, GPIO_OUTPUT_STATE_LED_RED);
}
succeed_type GPIO_PWR_GREEN(LIBGPIO_OBJ *phandle)
{
	GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_STATE_LED_RED);
	return GPIO_CLROUTPUT(phandle, GPIO_OUTPUT_STATE_LED_GREEN);
}

succeed_type GPIO_PWR_GREEN_OFF(LIBGPIO_OBJ *phandle)
{
	GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_STATE_LED_RED);
	return GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_STATE_LED_GREEN);
}

succeed_type GPIO_3G_GREEN_ON(LIBGPIO_OBJ *phandle)
{
	return GPIO_CLROUTPUT(phandle, GPIO_OUTPUT_3G_LED_GREEN);
}
succeed_type GPIO_3G_GREEN_OFF(LIBGPIO_OBJ *phandle)
{
	return GPIO_SETOUTPUT(phandle, GPIO_OUTPUT_3G_LED_GREEN);
}
succeed_type GPIO_GET_DSP_KEY_DECT(LIBGPIO_OBJ *phandle, int* in_data)
{
	return GPIO_GETINPUT(phandle, GPIO_INPUT_DSP_KEY_DECT,in_data);
}
succeed_type GPIO_GET_DSP_TRIG(LIBGPIO_OBJ *phandle, int* in_data)
{
	return GPIO_GETINPUT(phandle, GPIO_INPUT_DSP_TRIG,in_data);
}

succeed_type GPIO_5150_SETLOW(LIBGPIO_OBJ *phandle)
{
	return libgpio_setoutput(phandle, WDQ_GPIO_OUTPUT_RST_LOW_5150);
}

succeed_type GPIO_5150_SETHIGH(LIBGPIO_OBJ *phandle)
{
	return libgpio_setoutput(phandle, WDQ_GPIO_OUTPUT_RST_HIGH_5150);
}

