#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libgbshm.h"

int main(int argc, char **argv)
{
	int gbshm_fd = open("/dev/gbshm", O_RDWR);
	unsigned int gbshm_mmap = 0x00;
	int i = 0;
   	gbshm_mmap = (unsigned int)mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, gbshm_fd, 0x85000000);
	if (gbshm_mmap != 0x00)
	{
			printf("gbshm_mmap = 0x%x.\r\n", gbshm_mmap);
	}
	unsigned char *ptr = gbshm_mmap;
	for (i = 0x10; i < 0x1000; i++)
	{
			if (*(ptr +i) != i%255)
			{
					printf("ptr+ %d = 0x%x.\r\n",i, *(ptr+i));
			}
	}
	munmap(gbshm_mmap, 0x1000);


    return 0;
}
