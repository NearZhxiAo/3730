#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libgbshm.h"

int main(int argc, char **argv)
{
    int i;
    struct gbshm_mutex pmutex;

    LIBGBSHM_OBJ *gbshm_handle = GBSHM_INIT();

    if (gbshm_handle == NULL) {
        SYS_ERROR("libgbshm_init failed.\r\n");
        return -1;
    }
    SYS_INFO("libgbshm_init ok.\r\n");

    SYS_INFO("read all data from gbshm.\r\n");
 /*   for (i = 0; i < SHAREMEM_PHYS_SIZE; i++) {
        printf("%x",*((char*)gbshm_handle->gbshm_mmap + i));
        if ((i % 10000) == 0) {
            printf("\n");
            sleep(1);
            system("clear");
        }
    }*/
 /*   struct gbshm_mutex * my_gbshm_mutex;
//	my_gbshm_mutex=(gbshm_mutex*)malloc(sizeof(gbshm_mutex));
	GBSHM_PARA_GET(gbshm_handle,PARA_CMD_SYSTEM_GBSHM_MUTEX,my_gbshm_mutex,sizeof(struct gbshm_mutex));
	printf("data=%d\n\r",my_gbshm_mutex->data);
	*/
    GBSHM_DESTROY(gbshm_handle);
    return 0;
}
