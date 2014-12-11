#ifndef	_LIB_ENC_H
#define	_LIB_ENC_H

#ifdef __cplusplus
extern "C"{
#endif

#define OSA_SOK   0
#define OSA_EFAIL -1

#define ALG_VID_ENC_ROI_MAX_ROI    5

#define DRV_DATA_FORMAT_YUV420     2
#define DRV_DATA_FORMAT_YUV422     1
#define FALSE                      0
#define TRUE                       1

#define ALG_VID_CODEC_H264         0
#define ALG_VID_CODEC_MPEG4        1 
#define ALG_VID_CODEC_MJPEG        2 
#define ALG_VID_ENC_MODE_STANDARD  0

typedef unsigned char Uint8;
typedef unsigned int  Uint32;


typedef struct
{
	short MVx;
	short MVy;
	int   SAD;
}ALG_MotionDetectMbMvInfo;


typedef enum {
	ALG_VID_ENC_ROI_TYPE_FACE               = 0,
	ALG_VID_ENC_ROI_TYPE_BACKGROUND         = 1,
	ALG_VID_ENC_ROI_TYPE_FOREGROUND         = 2,
	ALG_VID_ENC_ROI_TYPE_DEFAUL             = 3,
	ALG_VID_ENC_ROI_TYPE_PRIVACYMASK        = 4
}ALG_VidEncROIType;



typedef struct {
	Uint32 topLeft_x;
	Uint32 topLeft_y;
	Uint32 bottomRight_x;
	Uint32 bottomRight_y;
	ALG_VidEncROIType roiType;
	Uint32 roiPriority;
}ALG_VidEncROI;


typedef struct {
	unsigned short numROI;
	ALG_VidEncROI roiInfo[ALG_VID_ENC_ROI_MAX_ROI];
}ALG_VidEncROIPrm;


typedef struct H264_param_
{
	unsigned int profileIdc;		// 66 = baseline, 77 = main, 100 = high profile; 100
	unsigned int levelIdc;		// level Idc, e.g, 20 = level 2.0	       ; 30
	unsigned int entropyMode;		// Entropy Coding mode, 0 = CAVLC, 1 = CABAC   ; 1	
	unsigned int transform8x8FlagIntraFrame;	// 0 = Disable, 1 = Enable	       ; 1
	unsigned int transform8x8FlagInterFrame;	// 0 = Disable, 1 = Enable             ; 1
	unsigned int aspectRatioX;		// Aspect Ratio Width Scale, 1                 ; 1
	unsigned int aspectRatioY;		// Aspect Ratio Width Scale, 1                 ; 1
	unsigned int pixelRange;		// 1 = Enable , 0 = Disable		       ; 1	
	unsigned int timeScale;		// Timer Resolution for Picture Timing	 60    ;300 
	unsigned int numUnitsInTicks;	// 1, Number of Timer units per Tick	       ; 1	
	unsigned int enableVUIparams;	// 1, Enable VUI parameters; 0, Disable VUI parameters ; 1
	unsigned int disableHDVICPeveryFrame;//1, On 2, Off 					       ; 0
	unsigned int meAlgo;		// ME search algorithm, 0, normal, 1, low power; 0
	unsigned int unrestrictedMV;	// 1. Enable, 0, Disable		       ; 1
	unsigned int seqScalingFlag;	// 0, disable, 1, auto, 2, low, 3, moderate, 4, high   ; 1	
	unsigned int encQuality;		// 0, standard quality, 1, high quality	       ; 1
	unsigned int sliceMode;		// 3 					       ; 3	
}H264_PARAM;


typedef struct {

	unsigned short inStartX;
	unsigned short inStartY;

	unsigned char *inAddr;
	unsigned char *outAddr;

	ALG_MotionDetectMbMvInfo *mbMvInfo;

	Uint32 outDataMaxSize;
	Uint32 forceKeyFrame;
	ALG_VidEncROIPrm roiPrm;

} ALG_VidEncRunPrm;

typedef struct {

	Uint8 *inAddr;
	Uint32 inDataSize;

	Uint8 *outAddr;
	Uint32 inputBufId;

	Uint32 outOffsetH;
	Uint32 outOffsetV;

} ALG_VidDecRunPrm;


typedef struct {

	Uint32 bytesGenerated;
	Uint32 isKeyFrame;
	unsigned short numMB;

} ALG_VidEncRunStatus;


typedef struct {

	Uint32 bytesUsed;
	Uint32 isKeyFrame;

	unsigned short frameWidth;
	unsigned short frameHeight;

	unsigned int outputBufId;
	unsigned int freeBufId;

	unsigned int outStartX;
	unsigned int outStartY;
	unsigned int outOffsetH;
	unsigned int outOffsetV;

} ALG_VidDecRunStatus;

typedef struct {

	unsigned short codec;
	unsigned short dataFormat;
	unsigned short width;
	unsigned short height;
	unsigned short offsetH;
	unsigned short offsetV;
	int    bitrate;
	unsigned short mode;
	unsigned short keyFrameInterval;
	Uint32 mbMvOutEnable;     
	unsigned short qValue; // only used for JPG
	unsigned short rateControlType;
	unsigned short vnfEnable;
	unsigned short framerate;
	unsigned short enableROI;
} ALG_VidEncCreate;

typedef struct {

	unsigned short codec;
	unsigned short dataFormat;
	unsigned short maxWidth;
	unsigned short maxHeight;
} ALG_VidDecCreate;

typedef struct _LIBALG_OBJ
{
	void * algEncHndl;

	void * algEncHndl_mpeg4;
}LIBALG_OBJ;


typedef enum _VIDEO_TYPE
{
	VIDEO_START,
	VIDEO_H264,
	VIDEO_MPEG4,
	VIDEO_MJPEG,
	VIDEO_END,

}VIDEO_TYPE;

succeed_type ALG_INIT();

succeed_type ALG_EXIT();

void *ALG_CREATE_H264(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval);

void *ALG_CREATE_MPEG4(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval);

void *ALG_CREATE_H264DEC(unsigned short width,unsigned short height);

void *ALG_CREATE_MPEG4DEC(unsigned short width,unsigned short height);

int ALG_RUN(void *algEncHndl, unsigned char *inaddr , unsigned char *outaddr,ALG_VidEncRunStatus *pencStatus, VIDEO_TYPE type);

int ALG_RUN_DEC(void *algDecHndl, unsigned char *inaddr , unsigned char *outaddr,ALG_VidDecRunStatus *pdecStatus, unsigned short width, unsigned short height, VIDEO_TYPE type, int len);

succeed_type ALG_DELETE(void *algEncHndl);

succeed_type ALG_DELETEDEC(void *algDecHndl);

succeed_type ALG_BITRATE(void *algEncHndl, int bitrate);

succeed_type ALG_FRAMERATE(void *algEncHndl, int framerate);

succeed_type ALG_KEYFRAMEINTERVAL(void *algEncHndl, int keyframeinterval);

#ifdef __cplusplus
}
#endif
#endif  
