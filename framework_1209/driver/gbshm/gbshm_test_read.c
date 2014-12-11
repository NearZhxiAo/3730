#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libgbshm.h"

int main(int argc, char **argv)
{

		struct video_enc_param localparam;
		LIBGBSHM_OBJ *gbshm_handle = GBSHM_INIT();

		if (gbshm_handle == NULL) {
				SYS_ERROR("libgbshm_init failed.\r\n");
				return -1;
		}
		SYS_INFO("libgbshm_init ok.\r\n");

		SYS_INFO("read all data from gbshm.\r\n");
		memset(&localparam, 0x00, sizeof(localparam));
		int i;
		if((i=(int)GBSHM_PARA_GET(gbshm_handle,PARA_CMD_SYSTEM_VIDEO_ENC_PARAM,&localparam,sizeof(struct video_enc_param)))==succeed_type_succeed)
		{
				SYS_ERROR("get libgbshm_init success.\r\n");
		}	
		else
		{
				printf("read error.\r\n");
		}
		printf("frame=0x%x\r\n",localparam.video_enc_bitrate);
		GBSHM_DESTROY(gbshm_handle);
		return 0;
}
