#include "system_debug.h"
#include "system_includes.h"
#include "davincifb_r61505w.h"

#include "liblcd.h"

static LIBGPIO_HANDLER *libspi_gpio_init()
{
	int spi_gpio_fd;
	int var = 0;
	int ret;
	LIBGPIO_HANDLER *pgpio_handle = malloc(sizeof(LIBGPIO_HANDLER));
	if(pgpio_handle == NULL)
	{
		printf("libspi_gpio_init MALLOC mem failed \n");
		return NULL;
	}
	pgpio_handle->gpio_fd = open("/dev/R61505W", O_RDWR);
	if(pgpio_handle->gpio_fd < 0)
	{
		printf("zhoujian@@@open libspi_gpio_init failed \n");
		free(pgpio_handle);
		return NULL;
	}
	ret = ioctl(pgpio_handle->gpio_fd,SPI_GPIO_IOC_INIT,&var);
	if(ret < 0)
	{
		printf("zhoujian@@@init libspi_gpio_init failed \n");
		free(pgpio_handle);
		return NULL;
	}
	return pgpio_handle;
}
static succeed_type libspi_gpio_release(LIBGPIO_HANDLER * phandle)
{
	int ret;

	if(phandle == NULL)
	{
		return succeed_type_failed;
	}
	
	ret = close(phandle->gpio_fd);	
	if(ret < 0)
	{
		printf("zhoujian@@@release libspi_gpio_release failed \n");
		free(phandle);
		return succeed_type_failed;
	}
	free(phandle);
	return succeed_type_succeed;
}
static succeed_type libspi_gpio_set(LIBGPIO_HANDLER * phandle, int mode)
{
	int ret;
	if(phandle == NULL)
	{
		printf("zhoujian@@@open libspi_gpio_sleep failed \n");
		return succeed_type_failed;
	}
	ret = ioctl(phandle->gpio_fd,SPI_GPIO_IOC_SET,mode);
	if(ret < 0)
	{
		printf("zhoujian@@@sleep libspi_gpio_sleep failed \n");
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}

LIBGPIO_HANDLER *SPI_GPIO_INIT()
{
	return libspi_gpio_init();
}
succeed_type SPI_GPIO_RELEASE(LIBGPIO_HANDLER * phandle)
{
	return libspi_gpio_release(phandle);
}
succeed_type SPI_GPIO_SET(LIBGPIO_HANDLER * phandle,int mode)
{
	return libspi_gpio_set(phandle, mode);
}

