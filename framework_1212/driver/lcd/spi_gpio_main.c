#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "Liblcd_ctrl.h"
int main(void)
{
	LIBGPIO_HANDLER * phld;
	succeed_type result;
	phld = Liblcd_init();
	if(phld == NULL)
	{
		printf("Liblcd_init failed\n");
		return -1;
	}
	result = Liblcd_set(phld, 0);
	if(result == type_failed)
		printf("Liblcd_set 0 failed\n");
	result = Liblcd_set(phld, 1);
	if(result == type_failed)
		printf("Liblcd_set 1 failed\n");
	result = Liblcd_destroy(phld);
	if(result == type_failed)
		printf("Liblcd_destroy failed\n");
}

