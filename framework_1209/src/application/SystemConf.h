#ifndef SYSTEMCONF_H_INCLUDED
#define SYSTEMCONF_H_INCLUDED
#include "TypeDefs.h"
#include "CmdDef.h"
//#include "AppSV/APPInterface.h"

#ifdef _cplusplus
extern "C" {
#endif


/*系统配置相关参数*/



/*算法参数*/
// typedef enum _ALG_TYPE
// {
// 	ALG_TYPE_START,
// 	ALG_TYPE_LINE,
// 	ALG_TYPE_WINDOW,
// 	ALG_TYPE_END,
// }ALG_TYPE;

typedef enum _ALG_TIME_USED
{
	ALG_TIME_START,
	ALG_TIME_DAY,
	ALG_TIME_NIGHT,
	ALG_TIME_END,
}ALG_TIME_USED;

/**********************************************************************************************************************/
/*算法外部的配置参数*/
typedef struct _ALG_XIANWINDOW_PARAM_ENV{
	int imgw;
	int imgh;
	int iniframe;
	int grayThresh;
	float light_area_ratio;
	int disthresh;
	int max_tracksize;
	int lostcount;	

	int minfind;
	int minmove;
	float track_pos_diff_thresh;
	float track_rect_diff_thresh;
	int cont_minw;
	int cont_maxw;
	int cont_minh;
	int cont_maxh;
	float cont_area_rate;
	int rect_merge_flag;
	int rect_minw;
	int rect_minh;
	float rect_area_rate;
}ALG_XIANWINDOW_PARAM_ENV;//回型

typedef struct _ALG_AREALINE_PARAM_ENV{
	int imgw;
	int imgh;
	int iniframe;
	int thresh;


	int disthresh;
	int max_tracksize;
	int lostcount;	

	int minfind;
	int minmove;
	float track_pos_diff_thresh;
	float track_rect_diff_thresh;

	int cont_minw;
	int cont_maxw;
	int cont_minh;
	int cont_maxh;
	float cont_area_rate;

	int rect_merge_flag;
	int rect_minw;
	int rect_minh;
	float rect_area_rate;
}ALG_AREALINE_PARAM_ENV;//禁区伴线

typedef struct _ALG_PEOPLEDENSE_ENV{
	int		_imgw;
	int		_imgh;
	int		_nLg;
	int		_nLm;
	int		_nNm;
	int		_nNs;
	int		_nSPNF;
	float	_fSigma0;
	float	_fSigma1;
	int		_nT0;
	int		_nOccHighestWarningThr;
	bool	_bVanishPt;
}ALG_PEOPLEDENSE_ENV;//人员密度

//modify by hzc 2012-05-20
//typedef struct _ALG_FIGHEALARM_ENV{
//	int _imgw;
//	int _imgh;
//	int _thresh;
//	int _iniframe;
//	int _grayThresh;
//	int	_binaryGray;
//	int _ChangeThresh;
//
//	int _cont_minw;
//	int _cont_maxw;
//	int _cont_minh;
//	int _cont_maxh;
//	float _cont_area_rate;
//}ALG_FIGHTALARM_ENV;//打架

typedef struct _ALG_FIGHTDETECTION_ENV{
	int _IsMovingCountThresh;
	int _UniDirectionCountThresh;
	float _UnidirectionRatio;
	int _ImageWidth;
	int _ImageHeight;
	float _HypotenuseMin;
	float _HypotenuseMax;
	float _IsMovingThresh;
	float _ROIBlockThresh;
	int _ROIBlockCountMax;//感兴趣区域宽度最大占的格数
	int _ROIBlockCountMin;//感兴趣区域宽度最小占的格数
}ALG_FIGHTDETECTION_ENV;//打架

typedef struct _ALG_CLUSTERDETECTION_ENV{
	int _clustermovingCountThresh;
	int _ImageWidth;
	int _ImageHeight;
	float _HypotenuseMin;
	float _HypotenuseMax;
	float _ROIBlockThresh;
	int _ROIBlockCountMinWidth;
	int _ROIBlockCountMinHeight;
}ALG_CLUSTERDETECTION_ENV;//人员聚集


//modify by hzc 2012-05-20
//typedef struct _ALG_MOTIONHISTORY_ENV{
//	int _imgw;
//	int _imgh;
//	int _FrameDiffNum;
//	float _MHI_Duration;
//	float _Ratio;
//	int _Diff_Threshold;
//}ALG_MOTIONHISTORY_ENV;//夜间起床

//modify(add) by hzc 2012-05-20
typedef struct _ALG_ALARMAREA_ENV{
	int _Imgw;
	int _Imgh;

	float _MHI_Duration;
	float _RatioDetect;
	float _RatioNoisze;
	int _Diff_Threshold;
}ALG_ALARMAREA_ENV; //警戒区域（夜间起床）

//modify(add) by hzc 2012-05-20
typedef struct _AlG_ALRAMLINE_ENV{
	int _Imgw;
	int _Imgh;

	float _MHI_Duration; 
	float _ObjWidth;
	float _RatioNoisze;
	int _Diff_Threshold;

}AlG_ALRAMLINE_ENV;//警戒线 （触碰警戒线）

//modify(modify) by hzc 2012-05-2
typedef struct _ALG_ABNORMALFALL_ENV{
	int _IsMovingCountThresh;
	int _UniDirectionCountThresh;
	float _UnidirectionRatio;
	int _ImageWidth;
	int _ImageHeight;
	float _HypotenuseMin;
	float _HypotenuseMax;
	float _IsMovingThresh;
	float _ROIBlockThresh;
	int _ROIBlockCountMax;//感兴趣区域宽度最大占的格数
	int _ROIBlockCountMin;//感兴趣区域宽度最小占的格数
}ALG_ABNORMALFALL_ENV;//跌倒

typedef struct _ALG_HOVERDETECTION_ENV{
	int _imgw;
	int _imgh;
	int _thresh;
	int _iniframe;
	int _timeThresh;
	int _disthresh;//徘徊帧数
	int _max_tracksize;
	int _lostcount;	

	int _minfind;
	int _minmove;
	float _track_pos_diff_thresh;
	float _track_rect_diff_thresh;

	int _cont_minw;
	int _cont_maxw;
	int _cont_minh;
	int _cont_maxh;		
	float _cont_area_rate;
}ALG_HOVERDETECTION_ENV;//徘徊

//modify(add) by hzc 2012-05-20
typedef struct _ALG_RUNNINGDETECT_ENV{
	int _imgw;
	int _imgh;
	int _thresh;
	int _iniframe;

	int _disthresh;
	int _max_tracksize;
	int _lostcount;	

	int _minfind;
	int _minmove;
	float _track_pos_diff_thresh;
	float _track_rect_diff_thresh;

	int _cont_minw;
	int _cont_maxw;
	int _cont_minh;
	int _cont_maxh;
	float _cont_area_rate;

	int _rect_merge_flag;
	int _rect_minw;
	int _rect_minh;
	float _rect_area_rate;
}ALG_RUNNINGDETECT_ENV; //奔跑检测

//modify(add)  by hzc 2012-05-20
typedef struct _ALG_FINDBAG_ENV{
	int _imgw;
	int _imgh;
	int _thresh;
	int _iniframe;
	int _cont_minw;
	int _cont_maxw;
	int _cont_minh;
	int _cont_maxh;
}ALG_FINDBAG_ENV; //遗留物检测

typedef struct _ALG_PEOPLE_LEAVE_ENV
{
	int Reserved;
}ALG_PEOPLE_LEAVE_ENV;
/**********************************************************************************************************************/

#define ALG_TIME_USED_MAX 2
#define ALG_PARA_MAX_NUM 6//支持6种算法
#define ALG_PARA_CONF_MAX_NUM 5//支持5个算法参数配置

typedef struct _ALG_PARA_CHOOSE_GROUP//各个算法使能配置标志位
{
	unsigned int bAlgAlarmAreaLine:1;//禁区伴线
	unsigned int bAlgAlarmArea:1;//警戒线(夜间起床)
	unsigned int bAlgAlarmLine:1;//警戒线(摸高)
	unsigned int bAlgWindowThief:1;//翻窗检测
	unsigned int bAlgLeaveOverBag:1;//遗留物
	unsigned int bAlgPeopleFight:1;//打架
	unsigned int bAlgPeopleCluster:1;//聚集
	unsigned int bAlgPeopleDense:1;//密度
	unsigned int bAlgPeopleRunning:1;//奔跑
	unsigned int bAlgPeopleLeave:1;//离岗检测
	unsigned int bAlgPeopleFall:1;//跌倒
	unsigned int bAlgPeopleHover:1;//徘徊
	unsigned int bAlgReserved:(32-14);
	unsigned int bAlgParaGeneratedWay:1;//算法配置的方法 0 -- 自动生成 1 -- 用户配置
	unsigned int bAlgEnableFlag:1;//此段配置算法总使能位置
}ALG_PARA_CHOOSE_GROUP;

typedef struct _ALG_CONF_RUNTIME
{
	unsigned short usAlgRuntimeStart;
	unsigned short usAlgRuntimeEnd;
}ALG_CONF_RUNTIME;

typedef enum _ALG_APP_BRIGHT
{
	ALG_APP_BRIGHT_START,
	ALG_APP_BRIGHT_0,
	ALG_APP_BRIGHT_1,
	ALG_APP_BRIGHT_2,
	ALG_APP_BRIGHT_3,
	ALG_APP_BRIGHT_END,
}ALG_APP_BRIGHT;

typedef enum _ALG_APP_GRAY
{
	ALG_APP_GRAY_START,
	ALG_APP_GRAY_0,
	ALG_APP_GRAY_1,
	ALG_APP_GRAY_2,
	ALG_APP_GRAY_END,
}ALG_APP_GRAY;

typedef enum _ALG_APP_SENSE
{
	ALG_APP_SENSE_START,
	ALG_APP_SENSE_0,
	ALG_APP_SENSE_1,
	ALG_APP_SENSE_2,
	ALG_APP_SENSE_3,
	ALG_APP_SENSE_4,
	ALG_APP_SENSE_END,
}ALG_APP_SENSE;

typedef struct _ALG_CONF_APP_UINT
{
	int iImgHeightVal;
	int iImgWithdVal;
	int iObjHeightVal;
	int iObjWithdVal;
	ALG_APP_BRIGHT AlgAppBright;
	ALG_APP_GRAY   AlgAppGray;
	ALG_APP_SENSE  AlgAppSense;	
}ALG_CONF_APP_UINT;

typedef union _ALG_CONF_APP_RESULT
{	
	ALG_AREALINE_PARAM_ENV   AlgAreaLineParamEnv;//禁区伴线
	ALG_ALARMAREA_ENV        AlgAlarmAreaEnv;//警戒线(夜间起床)
	AlG_ALRAMLINE_ENV        AlgAlarmLineEnv;//警戒线(摸高)
	ALG_XIANWINDOW_PARAM_ENV AlgXianWindowParamEnv;//翻窗检测
	ALG_FINDBAG_ENV          AlgFindBagEnv;//遗留物
	ALG_FIGHTDETECTION_ENV	 AlgFightDetectionParamEnv;//打架
	ALG_CLUSTERDETECTION_ENV AlgClusterDetectionParamEnv;//聚集
	ALG_PEOPLEDENSE_ENV		 AlgPeoPleDense2ParamEnv;//密度
	ALG_RUNNINGDETECT_ENV	 AlgRunningDetectEnv;//奔跑
	ALG_PEOPLE_LEAVE_ENV     AlgPeopleLeaveEnv;//离岗检测
	ALG_ABNORMALFALL_ENV     AlgAbnormalFallEnv;//跌倒
	ALG_HOVERDETECTION_ENV   AlgHoverDetectionEnv;//徘徊
}ALG_CONF_APP_RESULT;


// typedef union _ALG_CONF_APP_RESULT
// {
// 	InParamAPP_AlarmAreaLine   AlgAreaLineParamEnv;
// 	InParamAPP_XIAN_Window AlgXianWindowParamEnv;	
// 	InParamAPP_FightDetection AlgFightDetectionParamEnv;
// 	InParamAPP_MotionHistory AlgMotionHistoryParamEnv;
// 	InParamAPP_ClusterDetection AlgClusterDetectionParamEnv;
// 	InParamAPP_AbnormalFall AlgAbnormalFallParamEnv;
// 	InParamAPP_HoverDetect AlgHoverDetectParamEnv;
// 	InParamAPP_PeopleDense_2 AlgPeoPleDense2ParamEnv;
// }ALG_CONF_APP_RESULT;

typedef struct _ALG_CONF_APP_AUTO_PARA
{
	int AlgParaConfUsedIndex;
	int AlgParaIndex;
	ALG_CONF_APP_UINT AlgConfAppUint;
	ALG_CONF_APP_RESULT AlgConfAppResult;
}ALG_CONF_APP_AUTO_PARA;

typedef struct _ALG_CONF_SV_PARA
{
	ALG_AREALINE_PARAM_ENV   AlgAreaLineParamEnv;//禁区伴线
	ALG_ALARMAREA_ENV        AlgAlarmAreaEnv;//警戒线(夜间起床)
	AlG_ALRAMLINE_ENV        AlgAlarmLineEnv;//警戒线(摸高)
	ALG_XIANWINDOW_PARAM_ENV AlgXianWindowParamEnv;//翻窗检测
	ALG_FINDBAG_ENV          AlgFindBagEnv;//遗留物
	ALG_FIGHTDETECTION_ENV	 AlgFightDetectionParamEnv;//打架
	ALG_CLUSTERDETECTION_ENV AlgClusterDetectionParamEnv;//聚集
	ALG_PEOPLEDENSE_ENV		 AlgPeoPleDense2ParamEnv;//密度
	ALG_RUNNINGDETECT_ENV	 AlgRunningDetectEnv;//奔跑
	ALG_PEOPLE_LEAVE_ENV     AlgPeopleLeaveEnv;//离岗检测
	ALG_ABNORMALFALL_ENV     AlgAbnormalFallEnv;//跌倒
	ALG_HOVERDETECTION_ENV   AlgHoverDetectionEnv;//徘徊
}ALG_CONF_SV_PARA;

typedef struct _ALG_PARA_CONF_UINT
{
	ALG_PARA_CHOOSE_GROUP AlgParaChooseGroup;//算法使能标志位
	ALG_CONF_RUNTIME      AlgScheduleUint;   //算法配置有效时间 和防区配置方法一致
	ALG_CONF_SV_PARA      AlgConfSvPara;     //算法用户配置参数
}ALG_PARA_CONF_UINT;

typedef  struct _ALG_PARA
{
	//数据结构定义,for example:
 	int AlgParaConfUsedIndex;//此段算法是否正在被使用 0..1..2..3..4 其余无效 一个时间段只能使用其中一个（上层应用来保证）
	ALG_PARA_CONF_UINT	AlgParaConfUint[ALG_PARA_CONF_MAX_NUM];
// 	ALG_TYPE AlgType;
// 	ALG_TIME_USED AlgTimeUsed;
// 	bool AlgAffectedPicFlag;
// 
// 	ALG_XIANWINDOW_PARAM_ENV AlgXianWindowParamEnv[ALG_TIME_USED_MAX];
// 	ALG_AREALINE_PARAM_ENV AlgAreaLineParamEnv[ALG_TIME_USED_MAX];

//	struct InParamAPP_XIAN_Window InWindParam;
//	struct InParamAPP_AlarmAeraLine InLineParam;
}ALG_PARA;

#define CHAR_LEN					0x30
//识别器配置(见当前识别器和中心服务器交互或者中心服务器数据库设计)
typedef struct _CAM_CONFIG
{
   //数据结构定义,for example:
	unsigned char dev_id[CHAR_LEN];//识别器ID 年月日时分秒毫秒+3位随机数
	unsigned char dev_name[CHAR_LEN]; //识别器名称字段
}CAM_CONFIG;

//识别器状态
typedef enum _CAM_STATE
{
   CAM_STATE_START,
   CAM_STATE_WORKING,
   CAM_STATE_DEBUG,
   CAM_STATE_END
}CAM_STATE;

//识别器网络参数
typedef  struct  cam_ipaddr
{
   UCHAR ipaddr[32];
   UCHAR ipmask[32];
   UCHAR ipgw[32];
   UCHAR macaddr[48];
   UINT  port_video;  //客户端连接服务器的视频图像端口
}CAM_IPCONFIG; //25UINT

//中心网络配置参数
typedef  struct  center_ipaddr
{
   UCHAR ipaddr[32];
   UCHAR ipmask[32];
   UCHAR ipgw[32];     //木有MAC地址
   UCHAR macaddr[48];
   UINT  center_port;
}CENTER_IPCONFIG; //25UINT


typedef enum _drive_type
{
	DRIVE_TYPE_START = 0,
	DRIVE_TYPE_UNKNOWN = 0,//未知的磁盘类型
	DRIVE_TYPE_NO_ROOT_DIR,//说明lpRootPathName是无效的
	DRIVE_TYPE_REMOVABLE,//可移动磁盘
	DRIVE_TYPE_FIXED,//固定磁盘
	DRIVE_TYPE_REMOTE,//网络磁盘
	DRIVE_TYPE_CDROM,//光驱
	DRIVE_TYPE_RAMDISK,// 为RAM
	DRIVE_TYPE_END,
}DRIVE_TYPE;

typedef enum _drive_fs_type
{
	DRIVE_FS_TYPE_START,
	DRIVE_FS_TYPE_UNKNOWN,
	DRIVE_FS_TYPE_NTFS,
	DRIVE_FS_TYPE_FAT32,
	DRVIE_FS_TYPE_END,
}DRVIE_FS_TYPE;

 //识别器存储相关参数
typedef  struct  disk_desc
{
   DRIVE_TYPE drive_type; //磁盘类型
   DRVIE_FS_TYPE drive_fstype;//磁盘文件系统
   UCHAR disk_name[32];  //盘符名称
   UINT  realsize;    //M为单位
   UINT  emptysize;   //M为单位
}DISK_DESC; //10UINT

typedef enum _system_type
{
	SYSTEM_TYPE_START,
	SYSTEM_TYPE_WINDOWS,
	SYSTEM_TYPE_LINUX,
	SYSTEM_TYPE_END,
}SYSTEM_TYPE;

typedef  struct  _storage_parameter
{
   SYSTEM_TYPE     systemtype;  //系统类型
   UINT      diskcount;         //盘符数目
   DISK_DESC diskdesc[10];      //盘符描述
}STORAGE_PARA; //102UINT

//识别器存储位置参数
typedef  struct  cam_storage
{
	UCHAR storage_position[256];
	UINT  percent_alarm;     //报警百分比，低于这个值时系统报警
	UINT  autodelete_flag;    //将近percent_alarm/2的时候按照时间进行删除，每次删除percent_alarm/2
	UINT  delete_period;   //定时检查磁盘容量、执行删除时间，以分钟为单位，最小值为10
}CAM_STORAGE; //18 UINT

//系统版本等信息
typedef  struct  _system_version
{
   UCHAR  product_num[64];
   UCHAR  soft_version[64];
   UCHAR  hard_version[64];
}SYS_VERSION;



//系统配置
typedef  struct _SYS_CONFIG_PARA
{
    CAM_CONFIG      camconfig;
    CAM_STATE       camstate;
    CAM_IPCONFIG    camipconfig;
    CENTER_IPCONFIG centeripconfig;
}SYS_CONFIG_PARA;


//系统存储参数
typedef  struct  _SYS_STORAGE_PARA
{
    STORAGE_PARA storageconfig;
    CAM_STORAGE  camstorage;
}SYS_STORAGE_PARA;

#ifdef _cplusplus
}
#endif

#endif // SYSTEMCONF_H_INCLUDED
