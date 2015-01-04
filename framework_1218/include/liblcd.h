#ifndef	_LIB_LCD_H 
#define	_LIB_LCD_H

#include "system_debug.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _LIBGPIO_HANDLER
{
	int gpio_fd;
	int reserved;
}LIBGPIO_HANDLER;
LIBGPIO_HANDLER * SPI_GPIO_INIT();

succeed_type SPI_GPIO_RELEASE(LIBGPIO_HANDLER *p);




#ifdef __cplusplus
}
#endif
#endif  

