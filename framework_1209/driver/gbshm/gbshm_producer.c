#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libgbshm.h"

int main(int argc, char **argv)
{
    struct gbshm_mutex pmutex;
    LIBGBSHM_OBJ *gbshm_handle = GBSHM_INIT();

    if (gbshm_handle == NULL) {
        SYS_ERROR("libgbshm_init failed.\r\n");
        return -1;
    }
    SYS_INFO("libgbshm_init ok.\r\n");
/*    struct gbshm_mutex* my_gbshm_mutex;
	my_gbshm_mutex=(struct gbshm_mutex*)malloc(sizeof(struct gbshm_mutex));
	my_gbshm_mutex->data=13111;
	GBSHM_PARA_SET(gbshm_handle,PARA_CMD_SYSTEM_GBSHM_MUTEX,my_gbshm_mutex,sizeof(struct gbshm_mutex));

//    memset(gbshm_handle->gbshm_mmap, 0xab, SHAREMEM_PHYS_SIZE);

    printf("all gbshm mem write value 0xab, total size is 0x%x.\r\n", SHAREMEM_PHYS_SIZE);
*/
//    while(1);
	GBSHM_DESTROY(gbshm_handle);
    return 0;
}
