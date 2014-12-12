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

    Uint32 bytesGenerated;
    Uint32 isKeyFrame;
    unsigned short numMB;

} ALG_VidEncRunStatus;



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



typedef struct _LIBALG_OBJ
{
    void * algEncHndl;

}LIBALG_OBJ;


succeed_type ALG_INIT();
	
succeed_type ALG_EXIT();

void *ALG_CREATE(unsigned short width,unsigned short height,unsigned short offsetH,unsigned short offsetV,int bitrate,unsigned short framerate,unsigned short keyFrameInterval);

int ALG_RUN(void *algEncHndl, unsigned char *inaddr , unsigned char *outaddr,ALG_VidEncRunStatus *pencStatus);

succeed_type ALG_DELETE(void *algEncHndl);

succeed_type ALG_BITRATE(void *algEncHndl, int bitrate);

succeed_type ALG_FRAMERATE(void *algEncHndl, int framerate);

succeed_type ALG_KEYFRAMEINTERVAL(void *algEncHndl, int keyframeinterval);

#ifdef __cplusplus
}
#endif
#endif  
