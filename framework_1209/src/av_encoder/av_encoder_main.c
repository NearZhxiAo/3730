#include "av_encoder_private.h"

AV_ENCODER_PRIVATE_HANDLE global_av_encoder_private_handle;
extern int IsAVencoderSysQuit;

succeed_type av_encoder_exit(AV_ENCODER_PRIVATE_HANDLE *phandle)
{
	int ret;
	if ( NULL == phandle )
	{
		SYS_ERROR("in av_encoder_exit phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	GBSHM_DESTROY(phandle->pgbshm_obj);
	VSSHM_DESTROY(phandle->pvsshm_obj);
	ASSHM_DESTROY(phandle->passhm_obj);

	if ( UNIX_DESTROY(UNIXFILE_SOCKET_AVENC, phandle->unix_fd ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_DESTROY %s failed.\r\n", UNIXFILE_SOCKET_AVENC);
		return succeed_type_failed;
	}

	if(NULL != phandle->cpu_handle)
	{
		CPU_DELETE(phandle->cpu_handle);
	}

	if (phandle->alg_obj != NULL)
	{
		if (ALARM_ALG_DESTROY(phandle->alg_obj) < 0)
		{
			SYS_ERROR("ALG_DESTROY failed.\r\n");
			return succeed_type_failed;
		}
	}
	return succeed_type_succeed;
}

int main(int argc, char **argv)
{
	FILE *fp;
	fp = fopen("out.mp4", "ab+");
	memset(&global_av_encoder_private_handle, 0x00, sizeof(global_av_encoder_private_handle));
	av_encoder_init(&global_av_encoder_private_handle);

	while(0 == IsAVencoderSysQuit)
	{     
		//SYS_INFO("Entering POLL .\r\n");
		if ( poll( &(global_av_encoder_private_handle.polltimeout), global_av_encoder_private_handle.active_fdcount , 40000 ) > 0 )
		{
			struct timeval time_v;
			unsigned int timetemp = 0x00;
			gettimeofday(&time_v, NULL);
			timetemp = time_v.tv_sec * 1000 + time_v.tv_usec / 1000;
			if( 0 == global_av_encoder_private_handle.frame_count )
			{
				global_av_encoder_private_handle.start_timestamp = timetemp;
			}
			else
			{
				if ( (timetemp - global_av_encoder_private_handle.start_timestamp) / 1000 > 10)
				{
					printf("framerates = %d\r\n", global_av_encoder_private_handle.frame_count / ( (timetemp - global_av_encoder_private_handle.start_timestamp)/1000 ));
					global_av_encoder_private_handle.frame_count = 0;
					global_av_encoder_private_handle.start_timestamp = timetemp;
				}
			}

			if ( global_av_encoder_private_handle.polltimeout[0].revents )		//unix event
			{
				av_encoder_unix(&global_av_encoder_private_handle);
			}
			if ( global_av_encoder_private_handle.polltimeout[1].revents )		//capture event
			{
				video_buf_deal(global_av_encoder_private_handle.capture_fd,global_av_encoder_private_handle.resize_fd,fp);
				global_av_encoder_private_handle.frame_count++;
			}


			av_encoder_poll_init(&global_av_encoder_private_handle);
		}
	}
	fclose(fp);
	av_encoder_exit(&global_av_encoder_private_handle);
	return OSA_SOK;
}
