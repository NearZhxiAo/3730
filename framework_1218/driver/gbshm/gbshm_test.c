#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libgbshm.h"

pthread_mutex_t globalmutex ;
int main(int argc, char **argv)
{
	int gbshm_fd = open("/dev/gbshm", O_RDWR);
	unsigned int gbshm_mmap = 0x00;
	unsigned int index = 0;
	unsigned char *ptr = NULL;
	pthread_mutex_t *ptrmutex = NULL;

    gbshm_mmap = (unsigned int)mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, gbshm_fd, 0x85000000);
	if (gbshm_mmap != 0x00)
	{
			printf("gbshm_mmap = 0x%x.\r\n", gbshm_mmap);
	}
	printf("globalmutex addr = 0x%x.\r\n", &globalmutex);
	//ptrmutex = (pthread_mutex_t *)gbshm_mmap;
	ptrmutex = (pthread_mutex_t *)&globalmutex;
	ptrmutex = (pthread_mutex_t *) (gbshm_mmap + 0x10);
	if (pthread_mutex_init(ptrmutex, NULL ) != 0)
	{
			SYS_ERROR("gbshm mutex init failed\n");
			return NULL;
	}
//	pthread_mutex_lock(ptrmutex);
//	pthread_mutex_unlock(ptrmutex);

#if 0
	//init mutex
    if (0 != pthread_mutex_init((pthread_mutex_t *)( gbshm_mmap), NULL)) 
	{
        SYS_ERROR("gbshm mutex init failed\n");
        return NULL;
    }
	
    pthread_mutex_lock(gbshm_mmap);
#endif
	memset(gbshm_mmap, 0x00, 0x1000);
	ptr = (unsigned char *)gbshm_mmap;
	for ( index = 0x10; index < 0x1000; index++)
	{
		*(ptr+index) = index%255;
	}

   // pthread_mutex_unlock(gbshm_mmap);
	while(1);
	munmap(gbshm_mmap, 0x1000);


    return 0;
}
