#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libgbshm.h"

int main(int argc, char **argv)
{
    struct gbshm_mutex pmutex;
	struct gbshm_mutex my_gbshm_mutex;
    LIBGBSHM_OBJ *gbshm_handle = GBSHM_INIT();
	struct video_enc_param localparam;

    if (gbshm_handle == NULL) {
        SYS_ERROR("libgbshm_init failed.\r\n");
        return -1;
    }
    SYS_INFO("libgbshm_init ok.\r\n");

	memset(&localparam, 0x33333, sizeof(localparam));
	if((GBSHM_PARA_SET(gbshm_handle,PARA_CMD_SYSTEM_VIDEO_ENC_PARAM,&localparam,sizeof(struct video_enc_param)))==succeed_type_succeed)
	{
			printf("write success!\n\r");
	}					
	else
	{
			printf("write error\r\n");
	}

	GBSHM_DESTROY(gbshm_handle);
    return 0;
}
