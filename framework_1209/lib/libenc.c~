/************************************************************************
 * FileName:   libalg.c
 * Description:
 * Date:        2011-04-19
 * Version:     V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libenc.h"

/******************************************** 
 *    Function    : libalg_init
 *    Author      : 
 *    Date        : 2011-04-19 
 *    Description : 
 *    Para        : 
 *    Ret         : OSA_SOK       -- succeed
 *                  OSA_EFAIL     -- failed
 * **********************************************/

static succeed_type libalg_init()
{
	int status;

	status = ALG_sysInit();

	if(status != OSA_SOK)
	{
		SYS_ERROR("ALG_sysInit error!!\r\n");
		return status;
	}
	return status;
}

/******************************************** 
 *    Function    : libalg_exit
 *    Author      : 
 *    Date        : 2011-04-19 
 *    Description : 
 *    Para        : 
 *    Ret         : OSA_SOK       -- succeed
 *                  OSA_EFAIL     -- failed
 * **********************************************/

static succeed_type libalg_exit()
{
	int status;
	status = ALG_sysExit();

	if(status != OSA_SOK)
	{
		SYS_ERROR("ALG_sysInit error!!\r\n");
		return status;
	}
	return status;
}


/******************************************** 
 *    Function    : libalg_create
 *    Author      : 
 *    Date        : 2011-04-19 
 *    Description : 
 *    Para        : 
 *    Ret         : 
 * **********************************************/

static void *libalg_create_h264(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval)

{
	void *algEncHndl;
	ALG_VidEncCreate createPrm;
	memset(&createPrm, 0x00, sizeof(createPrm));

	createPrm.enableROI                  = FALSE;
	createPrm.mbMvOutEnable              = FALSE;
	createPrm.dataFormat                 = DRV_DATA_FORMAT_YUV420;
	//createPrm.dataFormat                 = DRV_DATA_FORMAT_YUV422;
	//createPrm.dataFormat                 = 0;
	createPrm.vnfEnable                  = FALSE;

	createPrm.codec                      = ALG_VID_CODEC_H264;
	createPrm.width                      = width;
	createPrm.height                     = height;
	createPrm.offsetH                    = offsetH;
	createPrm.offsetV                    = offsetV;
	createPrm.bitrate                    = bitrate;
	//createPrm.mode                       = ALG_VID_ENC_MODE_STANDARD;
	createPrm.mode                       = 1;	//2011-08-18 
	createPrm.keyFrameInterval           = keyFrameInterval;
	//createPrm.qValue                     = 50880;
	createPrm.qValue                     = 75;	//2011-08-18
	// createPrm.rateControlType            = 1;
	createPrm.rateControlType            = 2;
	createPrm.framerate                  = framerate;

	//algEncHndl = ALG_vidEncCreate(&createPrm);
	algEncHndl = ALG_vidEncCreate_islab(&createPrm);

	if(algEncHndl == NULL)
	{
		SYS_ERROR("vidEncCreate fail~~\r\n");
		return NULL;
	}

	return algEncHndl; 

}


//added in 20110907 set params
static void *libalg_create_h264_param(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval)

{
	void *algEncHndl;
	ALG_VidEncCreate createPrm;
	H264_PARAM Prm;
	memset(&createPrm, 0x00, sizeof(createPrm));

	createPrm.enableROI                  = TRUE;
	createPrm.mbMvOutEnable              = TRUE;
	createPrm.dataFormat                 = DRV_DATA_FORMAT_YUV420;
	createPrm.vnfEnable                  = TRUE;

	createPrm.codec                      = ALG_VID_CODEC_H264;
	createPrm.width                      = width;
	createPrm.height                     = height;
	createPrm.offsetH                    = offsetH;
	createPrm.offsetV                    = offsetV;
	createPrm.bitrate                    = bitrate;
	//createPrm.mode                       = ALG_VID_ENC_MODE_STANDARD;
	createPrm.mode                       = 1;	//2011-08-18 
	createPrm.keyFrameInterval           = keyFrameInterval;
//	createPrm.qValue                     = 50880;
	createPrm.qValue                     = 75;	//2011-08-18
//	createPrm.rateControlType            = 1;
	createPrm.rateControlType            = 2;
	createPrm.framerate                  = framerate;


	Prm.profileIdc                   = 100;  //66 77 
	//Prm.levelIdc                     = 31;
	Prm.levelIdc                     = 30;
	//Prm.entropyMode                  = 0;    //0 CAVLC 1 CABAC
	Prm.entropyMode                  = 1;    //0 CAVLC 1 CABAC
	Prm.transform8x8FlagIntraFrame   = 1;    // I frame : 0 disable 1 enable

	Prm.transform8x8FlagInterFrame   = 1;    // P frame : 0 disable 1 enable
//	Prm.transform8x8FlagInterFrame   = 0;    // P frame : 0 disable 1 enable
	Prm.aspectRatioX                 = 1;
	Prm.aspectRatioY                 = 1;
	Prm.pixelRange                   = 1;    //0 Restricted range 1 full range (luma chroma pixel value)
	Prm.timeScale                    = 50;   //300
	Prm.numUnitsInTicks              = 1; 
	Prm.enableVUIparams              = 1;   //0 disable 1 enable 
	Prm.disableHDVICPeveryFrame      = 0; 
	Prm.meAlgo                       = 0;   //0 Low Power Search 1 normal search
	Prm.unrestrictedMV               = 1;  //0 disable 1 enable 
//	Prm.seqScalingFlag               = 1;  //0 disable 1 auto 2 low 3 moderate 4 high
	//Prm.seqScalingFlag               = 0;  //0 disable 1 auto 2 low 3 moderate 4 high
	//Prm.seqScalingFlag               = 4;  //0 disable 1 auto 2 low 3 moderate 4 high
	Prm.seqScalingFlag               = 4;  //0 disable 1 auto 2 low 3 moderate 4 high
	Prm.encQuality                   = 1;  //0 standard 1 high quality, must 1
	//Prm.encQuality                   = 0;  //0 standard 1 high quality
	Prm.sliceMode                    = 3;

	algEncHndl = ALG_vidEncCreate_islab_param(&createPrm, &Prm);

	if(algEncHndl == NULL)
	{
		SYS_ERROR("vidEncCreate fail~~\r\n");
		return NULL;
	}

	return algEncHndl; 

}

static void *libalg_create_mpeg4(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval)
{
	void *algEncHndl;
	ALG_VidEncCreate createPrm;
	memset(&createPrm, 0x00, sizeof(createPrm));

	createPrm.enableROI                  = FALSE;
	createPrm.mbMvOutEnable              = FALSE;
	createPrm.dataFormat                 = DRV_DATA_FORMAT_YUV420;
	//createPrm.dataFormat                 = DRV_DATA_FORMAT_YUV422;
	//createPrm.dataFormat                 = 0;
	createPrm.vnfEnable                  = FALSE;

	createPrm.codec                      = ALG_VID_CODEC_MPEG4;
	createPrm.width                      = width;
	createPrm.height                     = height;
	createPrm.offsetH                    = offsetH;
	createPrm.offsetV                    = offsetV;
	createPrm.bitrate                    = bitrate;
	createPrm.mode                       = ALG_VID_ENC_MODE_STANDARD;
	//createPrm.mode                       = 1;	//2011-08-18 
	createPrm.keyFrameInterval           = keyFrameInterval;
	//createPrm.qValue                     = 50880;
	createPrm.qValue                     = 75;	//2011-08-18
	// createPrm.rateControlType            = 1;
	createPrm.rateControlType            = 2;
	createPrm.framerate                  = framerate;

	algEncHndl = ALG_vidEncCreate(&createPrm);
	//algEncHndl = ALG_vidEncCreate_islab(&createPrm);

	if(algEncHndl == NULL)
	{
		SYS_ERROR("vidEncCreate fail~~\r\n");
		return NULL;
	}
	return algEncHndl; 
}

static void *libalg_create_mjpeg(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval)

{
	void *algEncHndl;
	ALG_VidEncCreate createPrm;
	memset(&createPrm, 0x00, sizeof(createPrm));

	createPrm.enableROI                  = FALSE;
	createPrm.mbMvOutEnable              = FALSE;
	createPrm.dataFormat                 = DRV_DATA_FORMAT_YUV420;
	createPrm.vnfEnable                  = FALSE;

	createPrm.codec                      = ALG_VID_CODEC_MJPEG;
	createPrm.width                      = width;
	createPrm.height                     = height;
	createPrm.offsetH                    = offsetH;
	createPrm.offsetV                    = offsetV;
	createPrm.bitrate                    = bitrate;
	createPrm.mode                       = ALG_VID_ENC_MODE_STANDARD;
	createPrm.keyFrameInterval           = keyFrameInterval;
	createPrm.qValue                     = 75;	//2011-08-18
	createPrm.rateControlType            = 2;
	createPrm.framerate                  = framerate;

	algEncHndl = ALG_vidEncCreate(&createPrm);
	//algEncHndl = ALG_vidEncCreate_islab(&createPrm);

	if(algEncHndl == NULL)
	{
		SYS_ERROR("vidEncCreate fail~~\r\n");
		return NULL;
	}

	return algEncHndl; 

}

static void *libalg_create_h264dec(unsigned short width,unsigned short height)
{
	void *algDecHndl;
	ALG_VidDecCreate createPrm;
	memset(&createPrm, 0x00, sizeof(createPrm));

	createPrm.dataFormat                 = DRV_DATA_FORMAT_YUV420;
	createPrm.codec                      = ALG_VID_CODEC_H264;
	createPrm.maxWidth                   = width;
	createPrm.maxHeight                  = height;

	algDecHndl = ALG_vidDecCreate(&createPrm);

	if(algDecHndl == NULL)
	{
		SYS_ERROR("vidDecCreate fail~~\r\n");
		return NULL;
	}

	return algDecHndl; 
}


static void *libalg_create_mpeg4dec(unsigned short width,unsigned short height)
{
	void *algDecHndl;
	ALG_VidDecCreate createPrm;
	memset(&createPrm, 0x00, sizeof(createPrm));

	createPrm.dataFormat                 = DRV_DATA_FORMAT_YUV420;
	createPrm.codec                      = ALG_VID_CODEC_MPEG4;
	createPrm.maxWidth                   = width;
	createPrm.maxHeight                  = height;

	algDecHndl = ALG_vidDecCreate(&createPrm);

	if(algDecHndl == NULL)
	{
		SYS_ERROR("vidDecCreate fail~~\r\n");
		return NULL;
	}
	return algDecHndl; 
}

/******************************************** 
 *    Function    : libalg_delete
 *    Author      : 
 *    Date        : 2011-04-19 
 *    Description : 
 *    Para        : 
 *    Ret         : OSA_SOK       -- succeed
 *                  OSA_EFAIL     -- failed
 * **********************************************/

static succeed_type libalg_delete(void *algEncHndl)
{
	int status;
	status = ALG_vidEncDelete(algEncHndl);
	if(status != OSA_SOK)
	{
		SYS_ERROR("vidEncDelete failed.\n");
		return status;
	}
	return status;
}


static succeed_type libalg_deletedec(void *algDecHndl)
{
	int status;
	status = ALG_vidDecDelete(algDecHndl);
	if(status != OSA_SOK)
	{
		SYS_ERROR("vidEncDelete failed.\n");
		return status;
	}
	return status;
}
/******************************************** 
 *    Function    : libalg_run
 *    Author      : 
 *    Date        : 2011-04-19 
 *    Description : 
 *    Para        : 
 *    Ret         : 
 * **********************************************/

static int framecount = 0;

static int libalg_run(void *algEncHndl, unsigned char *inaddr , unsigned char *outaddr,ALG_VidEncRunStatus *pencStatus, VIDEO_TYPE type) 
{
	int status;
	ALG_VidEncRunPrm encPrm;

	memset(&encPrm, 0x00, sizeof(encPrm));
	memset(pencStatus, 0x00, sizeof(ALG_VidEncRunStatus));

	encPrm.inAddr                    = inaddr;
	encPrm.inStartX                  = 0;
	encPrm.inStartY                  = 0;	//
	encPrm.outAddr                   = outaddr;
	encPrm.mbMvInfo                  = NULL;
	encPrm.outDataMaxSize            = 4000000;
	encPrm.forceKeyFrame             = FALSE;
	encPrm.roiPrm.numROI             = 0;

	framecount ++;

	status = ALG_vidEncRun(algEncHndl, &encPrm, pencStatus);
	if(status != OSA_SOK)
	{       
		SYS_ERROR("vidEncRun failed.\n");
		return -1;
	}
	if(framecount < 200)
	{
//		ALG_vidEncSet_QP_islab(algEncHndl, 20, 40, 20, 40);   //Imin Imax Pmin Pmax  better
		ALG_vidEncSet_QP_islab(algEncHndl, 30, 35, 30, 35);   //Imin Imax Pmin Pmax   best
//		ALG_vidEncSet_QP_islab(algEncHndl, 20, 30, 20, 30);   //Imin Imax Pmin Pmax   best
//		ALG_vidEncSet_QP_islab(algEncHndl, 30, 40, 20, 40);   //Imin Imax Pmin Pmax  not good for QCIF
//		ALG_vidEncSet_QP_islab(algEncHndl, 0, 51, 20, 40);   //Imin Imax Pmin Pmax
	}
	//	printf("ALG_vidEncSet_QP succeed ..\r\n");

#if 0
	if(type == VIDEO_MJPEG)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/jpeg.jpg", "wb+");
		fwrite(encPrm.outAddr, pencStatus->bytesGenerated, 1, fp);
		fclose(fp);
	}
#endif

#if 0
	if(type == VIDEO_H264)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/h264.mp4", "ab+");
//		fwrite(&pencStatus->isKeyFrame,sizeof(int), 1,fp);
//		fwrite(&pencStatus->bytesGenerated,sizeof(int), 1,fp);
		fwrite(encPrm.outAddr, pencStatus->bytesGenerated, 1, fp);
		fclose(fp);

	}
	if(type == VIDEO_MPEG4)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/mpeg4.mp4", "ab+");
		fwrite(encPrm.outAddr, pencStatus->bytesGenerated, 1, fp);
		fclose(fp);
	}

	if(type == VIDEO_MJPEG)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/jpeg.jpg", "ab+");
		fwrite(encPrm.outAddr, pencStatus->bytesGenerated, 1, fp);
		fclose(fp);
	}
	#endif
	return status;
}

#if 1
static int libalg_run_dec_h264(void *algDecHndl, unsigned char *inaddr , unsigned char *outaddr, ALG_VidDecRunStatus *pdecStatus, unsigned short width, unsigned short height, VIDEO_TYPE type, int len, int bufId) 
{
	int status;
	ALG_VidDecRunPrm decPrm;

	memset(&decPrm, 0x00, sizeof(decPrm));
	memset(pdecStatus, 0x00, sizeof(ALG_VidDecRunStatus));

	decPrm.inAddr                    = inaddr;
	decPrm.outAddr                   = outaddr[bufId];
	decPrm.inputBufId                = bufId;
	decPrm.inDataSize                = len;

	if(type == VIDEO_MPEG4 || type == VIDEO_MJPEG)
	{
		decPrm.outOffsetH            = width;
		decPrm.outOffsetV            = height;
	}

	if(type == VIDEO_H264)
	{
		decPrm.outOffsetH            = width;
		decPrm.outOffsetV            = height + 48;
	}

	status = ALG_vidDecRun(algDecHndl, &decPrm, pdecStatus);
	if(status != OSA_SOK)
	{       
		SYS_ERROR("vidDecRun failed.\n");
		return -1;
	}
	printf("after decrun ...\r\n\r\n");
#if 0
	if(type == VIDEO_MPEG4)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/mpeg4.dat", "ab+");
		fwrite(decPrm.outAddr, 720*480*3/2, 1, fp);
		//		fwrite(decPrm.outAddr, width*height*3/2, 1, fp);
		fclose(fp);
	}

	if(type == VIDEO_H264)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/h264.dat", "ab+");
		fwrite(decPrm.outAddr, 720*480*3/2, 1, fp);
		fclose(fp);
	}

	if(type == VIDEO_MJPEG)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/jpeg.dat", "ab+");
		fwrite(decPrm.outAddr, 720*480*3/2, 1, fp);
		fclose(fp);
	}	
#endif
	bufId ^= 1;
	return status;
}
#endif



static int libalg_run_dec(void *algDecHndl, unsigned char *inaddr , unsigned char *outaddr, ALG_VidDecRunStatus *pdecStatus, unsigned short width, unsigned short height, VIDEO_TYPE type, int len) 
{
	int status;
	ALG_VidDecRunPrm decPrm;

	memset(&decPrm, 0x00, sizeof(decPrm));
	memset(pdecStatus, 0x00, sizeof(ALG_VidDecRunStatus));

	decPrm.inAddr                    = inaddr;
	decPrm.outAddr                   = outaddr;
	decPrm.inputBufId                = 0;
	decPrm.inDataSize                = len;

	if(type == VIDEO_MPEG4)
	{
		decPrm.outOffsetH            = width;
		decPrm.outOffsetV            = height;
	}

	if(type == VIDEO_H264)
	{
		decPrm.outOffsetH            = width;
		decPrm.outOffsetV            = height + 48;
	}

#if 1
	printf("...inaddr = 0x%x...\r\n\r\n", decPrm.inAddr);
	printf("...outaddr = 0x%x...\r\n\r\n", decPrm.outAddr);
	printf("...inputbufid = %d...\r\n\r\n", decPrm.inputBufId);
	printf("...indatasize = %d...\r\n\r\n", decPrm.inDataSize);
	printf("...outoffseth = %d...\r\n\r\n", decPrm.outOffsetH);
	printf("...outoffsetv = %d...\r\n\r\n", decPrm.outOffsetV);
#endif

	status = ALG_vidDecRun(algDecHndl, &decPrm, pdecStatus);
	if(status != OSA_SOK)
	{       
		SYS_ERROR("vidDecRun failed.\n");
		return -1;
	}
	printf("after decrun ...\r\n\r\n");

	if(type == VIDEO_MPEG4)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/mpeg4.dat", "ab+");
		fwrite(decPrm.outAddr, 720*480*3/2, 1, fp);
		fclose(fp);
	}

	if(type == VIDEO_H264)
	{
		FILE *fp = NULL;
		fp = fopen("/opt/install/bin/h264.dat", "ab+");
		fwrite(decPrm.outAddr, 720*480*3/2, 1, fp);
		fclose(fp);
	}
	return status;
}

static succeed_type libalg_setbitrate(void *algEncHndl, int bitrate)
{
	int status;
	status = ALG_vidEncSetBitrate(algEncHndl, bitrate);
	if(status != OSA_SOK)
	{
		SYS_ERROR("vidEncSetBitrate failed.\n");
		return status;
	}
	return status;
}

static succeed_type libalg_setframerate(void *algEncHndl, int framerate)
{
	int status;
	
	status = ALG_vidEncSetFramerate(algEncHndl, framerate);
	if(status != OSA_SOK)
	{
		SYS_ERROR("vidEncSetBitrate failed.\n");
		return status;
	}
	printf("-------status = %d  ---set framerate = %d-----------\r\n\r\n", status, framerate);
	return status;
}

static succeed_type libalg_setkeyframeinterval(void *algEncHndl, int keyframeinterval)
{
	int status;
	status = ALG_vidEncSetKeyFrameInterval(algEncHndl, keyframeinterval);
	if(status != OSA_SOK)
	{
		SYS_ERROR("vidEncSetBitrate failed.\n");
		return status;
	}
	return status;
}

succeed_type ALG_BITRATE(void *algEncHndl, int bitrate)
{
	return libalg_setbitrate(algEncHndl, bitrate);
}

succeed_type ALG_FRAMERATE(void *algEncHndl, int framerate)
{
	return libalg_setframerate(algEncHndl, framerate);
}

succeed_type ALG_KEYFRAMEINTERVAL(void *algEncHndl, int keyframeinterval)
{
	return libalg_setkeyframeinterval(algEncHndl, keyframeinterval);
}


succeed_type ALG_INIT()
{
	return libalg_init();
}

succeed_type ALG_EXIT()
{
	return libalg_exit();
}

void *ALG_CREATE_H264(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval)
{
	return libalg_create_h264(width,height,offsetH,offsetV,bitrate,framerate,keyFrameInterval);
}

// added in 20110907 set param 
void *ALG_CREATE_H264_PARAM(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval)
{
	return libalg_create_h264_param(width,height,offsetH,offsetV,bitrate,framerate,keyFrameInterval);
}


void *ALG_CREATE_MPEG4(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval)
{
	return libalg_create_mpeg4(width,height,offsetH,offsetV,bitrate,framerate,keyFrameInterval);
}

void *ALG_CREATE_MJPEG(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval)
{
	return libalg_create_mjpeg(width, height, offsetH, offsetV, bitrate, framerate, keyFrameInterval);
}

void *ALG_CREATE_H264DEC(unsigned short width,unsigned short height)
{
	return libalg_create_h264dec(width,height);
}

void *ALG_CREATE_MPEG4DEC(unsigned short width,unsigned short height)
{
	return libalg_create_mpeg4dec(width,height);
}

succeed_type ALG_DELETE(void *algEncHndl)
{
	return libalg_delete(algEncHndl);
}

succeed_type ALG_DELETEDEC(void *algDecHndl)
{
	return libalg_deletedec(algDecHndl);
}

int ALG_RUN(void *algEncHndl, unsigned char *inaddr , unsigned char *outaddr,ALG_VidEncRunStatus *pencStatus, VIDEO_TYPE type)
{
	return libalg_run(algEncHndl, inaddr , outaddr, pencStatus, type);
}

int ALG_RUN_DEC(void *algDecHndl, unsigned char *inaddr , unsigned char *outaddr,ALG_VidDecRunStatus *pdecStatus, unsigned short width, unsigned short height, VIDEO_TYPE type, int len)
{
	return libalg_run_dec(algDecHndl, inaddr , outaddr, pdecStatus, width, height, type, len);
}

#if 1
int ALG_RUN_DECH264(void *algDecHndl, unsigned char *inaddr , unsigned char *outaddr,ALG_VidDecRunStatus *pdecStatus, unsigned short width, unsigned short height, VIDEO_TYPE type, int len, int bufId)
{
	return libalg_run_dec_h264(algDecHndl, inaddr , outaddr, pdecStatus, width, height, type, len, bufId);
}
#endif

