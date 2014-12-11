#include <linux/ioctl.h>

/* ioctls definition */
#define SPI_GPIO_IOC_BASE   	'S'
#define SPI_GPIO_IOC_INIT     	_IOW(SPI_GPIO_IOC_BASE, 1, int)
#define SPI_GPIO_IOC_SET   	_IOW(SPI_GPIO_IOC_BASE, 2, int)
#define SPI_GPIO_IOC_MAXNR  2
/* End of ioctls */

