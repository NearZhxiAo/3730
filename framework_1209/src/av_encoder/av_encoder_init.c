#include "av_encoder_private.h"

void sig_handle_proc(int sig);
int IsAVencoderSysQuit = 0;

void av_encoder_poll_init(AV_ENCODER_PRIVATE_HANDLE *phandle)
{
	memset(phandle->polltimeout, 0x00, sizeof(struct pollfd) * AV_ENCODER_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;			//PTZ, BOA, ....... 
	phandle->polltimeout[0].events = POLLIN;

	phandle->polltimeout[1].fd = phandle->capture_fd;
	phandle->polltimeout[1].events = POLLIN ;
	

	phandle->active_fdcount = 2;
}

succeed_type av_encoder_privatehandle_init(AV_ENCODER_PRIVATE_HANDLE *phandle)
{

	int i;
	Uint16 alignWidth = 0;
	Uint16 alignHeight = 0;
	Uint16 framerate = 0;
	Uint32 size = 0;

	phandle->bnum = 3;
	Uint32 outphyaddr[phandle->bnum];

	GBSHM_PARA_GET(phandle->pgbshm_obj,PARA_CMD_SYSTEM_VIDEO_ENC_PARAM,&(phandle->videoencparam), sizeof(video_enc_param)); 

	printf("input mode 0x%x, VIDEO_INPUT_NTSC = %d, VIDEO_INPUT_PAL = %d.\r\n",phandle->videoencparam.video_input_mode, VIDEO_INPUT_NTSC, VIDEO_INPUT_PAL);
	printf("resolution mode 0x%x\r\n", phandle->videoencparam.video_enc_resolution);
	if(phandle->videoencparam.video_input_mode == VIDEO_INPUT_PAL)
	{
		if(phandle->videoencparam.video_enc_resolution == VIDEO_ENC_D1)
		{
			printf("FILE: %s, LINE: %d.\r\n", __FILE__, __LINE__);
			phandle->width = 720;
			phandle->height = 480;
		}
		else if(phandle->videoencparam.video_enc_resolution == VIDEO_ENC_CIF)
		{
			printf("FILE: %s, LINE: %d.\r\n", __FILE__, __LINE__);
			phandle->width = 360;
			phandle->height = 240;
		}
		else
		{
			printf("FILE: %s, LINE: %d.\r\n", __FILE__, __LINE__);
			phandle->width = 720;
			phandle->height = 480;
		}
	}
	else if(phandle->videoencparam.video_input_mode == VIDEO_INPUT_NTSC)
	{
		if(phandle->videoencparam.video_enc_resolution == VIDEO_ENC_D1)
		{
			printf("FILE: %s, LINE: %d.\r\n", __FILE__, __LINE__);
			phandle->width = 720;
			phandle->height = 480;
		}
		else if(phandle->videoencparam.video_enc_resolution == VIDEO_ENC_CIF)
		{
			printf("FILE: %s, LINE: %d.\r\n", __FILE__, __LINE__);
			phandle->width = 360;
			phandle->height = 240;
		}
		else
		{
			printf("FILE: %s, LINE: %d.\r\n", __FILE__, __LINE__);
			phandle->width = 720;
			phandle->height = 480;
		}
	}
	else
	{
		printf("FILE: %s, LINE: %d. phandle->videoencparam.video_enc_mode = %d, VIDEO_INPUT_MODE = %d \r\n", __FILE__, __LINE__, phandle->videoencparam.video_enc_mode , VIDEO_INPUT_NTSC);
		phandle->width = 720;
		phandle->height = 480;
	}
	
	printf("width ============== %d height == %d..............\r\n", phandle->width, phandle->height);



   //added 2012-1-8  zsx
   
	alignWidth = OSA_align(phandle->width, 32);
	alignHeight = phandle->height;
	size = alignWidth*alignHeight*2;
    phandle->inVirtAddr = malloc(size);
	phandle->inPhysAddr = malloc(sizeof(phandle->inVirtAddr));
	if(phandle->inVirtAddr == NULL || phandle->inPhysAddr == NULL) 
	{
		SYS_ERROR("malloc failed\n");  
		return succeed_type_failed;
	}
	//TODO
	//add H264/MJPEG algorithm handle create interface
	//

/******************************************************************************/
#if 0 
	alignWidth = OSA_align(phandle->width, 32);
	alignHeight = phandle->height;
	size = alignWidth*alignHeight*2;
	
	CMEM_init();
	printf("alloc size = 0x%x.\r\n", size);
	
	phandle->inVirtAddr = (Uint8 *)OSA_cmemAlloc(size, 32); 
	phandle->inPhysAddr = (Uint8 *)OSA_cmemGetPhysAddr(phandle->inVirtAddr);
	if(phandle->inVirtAddr == NULL || phandle->inPhysAddr == NULL) 
	{
		SYS_ERROR("OSA_cmemAlloc()\n");  
		return succeed_type_failed;
	}
	memset(phandle->inVirtAddr, 0x0, size); 
	if(phandle->videoencparam.video_input_mode == VIDEO_INPUT_PAL)
	{
		switch(phandle->videoencparam.video_enc_framerate)
		{
			case VIDEO_ENC_30FPS:
			case VIDEO_ENC_25FPS:
				framerate = 25;
				break;
			case VIDEO_ENC_15FPS:
				framerate = 15;
				break;
			case VIDEO_ENC_7FPS:
				framerate = 7;
				break;
			default:
				framerate = 25;
				break;
		}
	}
	else if(phandle->videoencparam.video_enc_mode == VIDEO_INPUT_NTSC)
	{
		switch(phandle->videoencparam.video_enc_framerate)
		{
			case VIDEO_ENC_30FPS:
				framerate = 30;
				break;
			case VIDEO_ENC_25FPS:
				framerate = 25;
				break;
			case VIDEO_ENC_15FPS:
				framerate = 15;
				break;
			case VIDEO_ENC_7FPS:
				framerate = 7;
				break;
			default:
				framerate = 30;
				break;
		}
	}
	else
	{
		framerate = 25;
	}

	SYS_WARNING("22222222222, keyframinterval = %d...\r\n\r\n", phandle->videoencparam.keyFrameInterval);
	if ( phandle->videoencparam.keyFrameInterval == 0 )		
	{
		phandle->videoencparam.keyFrameInterval = 10;
	}

	if ( phandle->videoencparam.video_enc_bitrate == 0 )		
	{
		phandle->videoencparam.video_enc_bitrate = 1536000;
	}

#if 0  //modified at 2011-10-25 zm
	phandle->pvideoalg_handle = ALG_CREATE(phandle->width,phandle->height,alignWidth,alignHeight,\
			phandle->videoencparam.video_enc_bitrate,framerate*1000,\
			phandle->videoencparam.keyFrameInterval);
#endif

	phandle->pvideoalg_handle_jpeg = ALG_CREATE_JPEG(phandle->width,phandle->height,alignWidth,alignHeight,\
			phandle->videoencparam.video_enc_bitrate,framerate*1000,\
			phandle->videoencparam.keyFrameInterval);

	printf("\r\n\r\n -------width:%d,height:%d,alignWidth:%d,alignHeight:%d,bitrate:%d,\
framerate:%d,FrameInterval:%d.\r\n\r\n",phandle->width,phandle->height,alignWidth,\
			alignHeight,phandle->videoencparam.video_enc_bitrate,framerate,\
			phandle->videoencparam.keyFrameInterval);


#endif
	return succeed_type_succeed;
}


succeed_type av_encoder_init(AV_ENCODER_PRIVATE_HANDLE *phandle)
{
	phandle->current_state = AVENC_IDLE;
	SIGNAL_STRUCT_T signal;
	if ( NULL == phandle )
	{
		SYS_ERROR("in av_encoder_init phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	SYS_INFO("before GBSHM_INIT()");

	phandle->pgbshm_obj = GBSHM_INIT();
	if ( NULL == phandle->pgbshm_obj )
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	phandle->pvsshm_obj = VSSHM_INIT();
	if ( NULL == phandle->pvsshm_obj )
	{
		SYS_ERROR("VSSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	phandle->passhm_obj = ASSHM_INIT();
	if ( NULL == phandle->passhm_obj )
	{
		SYS_ERROR("ASSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	SYS_WARNING("\r\n\r\n .....before alg_init.....\r\n\r\n");
/*****************************************************************************/
	if(av_encoder_privatehandle_init(phandle) == succeed_type_failed)
	{
		SYS_ERROR("av_encoder_privatehandle_init failed.\r\n");
		return succeed_type_failed;
	}

	if ( UNIX_CREATE(UNIXFILE_SOCKET_AVENC, &(phandle->unix_fd), 0 ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_AVENC);
		return succeed_type_failed;
	}

/*********************************************************************/
	//added 2012-1-6 zsx
	//CaptureAttr attr = Cap_Attrs_DEFAULT;	
	CaptureAttr attr ;			//2014-12-02
	phandle->pcapture_obj = CAPTURE_INIT(&attr);
	if (phandle->pcapture_obj == NULL)
	{
		SYS_ERROR("capture init failed.\r\n");
		return succeed_type_failed;
	}
	phandle->capture_fd= phandle->pcapture_obj->capture_fd;
	

	av_encoder_poll_init(phandle);


	//set bright,colorsaturation,hue and contrast
	GBSHM_PARA_GET(phandle->pgbshm_obj,PARA_CMD_SYSTEM_AD_SETTING_PARAM,&(phandle->videoadsettingparam), sizeof(ad_setting_param)); 

	memset(&signal, 0x00, sizeof(signal));
	signal.signo[0] = SIGINT;
	signal.sig_phandle[0] = sig_handle_proc;
	SIGNAL_INIT(&signal, 1);
    SYS_INFO("avencoder_init success!");
	return succeed_type_succeed;
}

void sig_handle_proc(int sig)
{
	SYS_WARNING("AVencode Rcv SigNo = %d!\r\n",sig);
	IsAVencoderSysQuit = 1;
}
