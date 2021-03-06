
#ifndef __CLIENT_INTER_COMM_H__
#define __CLIENT_INTER_COMM_H__
#ifdef __cplusplus
extern "C"{
#endif
//DTP Type
#define CMD_DTP_HEADER_T 						0x0
#define CMD_DTP_HEART_BEAT_REQ 				0x120
#define CMD_DTP_HEART_BEAT_RES 				0x121
#define CMD_DTP_QUERY_RECORDSTATUS_REQ 	0x164
#define CMD_DTP_QUERY_RECORDSTATUS_RES 	0x165
#define CMD_DTP_PU_LOGIN_STEP1_REQ 			0x1000
#define CMD_DTP_PU_LOGIN_STEP1_RES 			0x1001
#define CMD_DTP_PU_LOGIN_STEP2_REQ			0x1002
#define CMD_DTP_PU_LOGIN_STEP2_RES 			0x1003
#define CMD_DTP_PU_LOGOUT_REQ 				0x1004
#define CMD_DTP_PU_LOGOUT_RES 				0x1005
#define CMD_DTP_PREVIEW_START_REQ 			0x1100
#define CMD_DTP_PREVIEW_START_RES 			0x1101
#define CMD_DTP_PREVIEW_STOP_REQ 			0x1102
#define CMD_DTP_PREVIEW_STOP_RES 			0x1103

//摄像机控制请求
#define CMD_DTP_CONTROL_PTZ_REQ                 	0x1200   

//摄像机控制响应
#define CMD_DTP_CONTROL_PTZ_RES                 	0x1201

//添加摄像机预置点信息请求 
#define CMD_DTP_CONTROL_ADDCPS_REQ               0x1202

//添加摄像机预置点信息响应
#define CMD_DTP_CONTROL_ADDCPS_RES                0x1203

//删除摄像机预置点信息请求
#define CMD_DTP_CONTROL_DELCPS_REQ                0x1204

//删除摄像机预置点信息响应
#define CMD_DTP_CONTROL_DELCPS_RES                 0x1205

//摄像机预置点定位请求
#define CMD_DTP_CONTROL_GOTOCPS_REQ             0x1206

//摄像机预置点定位响应                       
#define CMD_DTP_CONTROL_GOTOCPS_RES             0x1207

//清空摄像机预置点信息请求
#define CMD_DTP_CONTROL_CLRCPS_REQ                0x1208    

//清空摄像机预置点信息请求
#define CMD_DTP_CONTROL_CLRCPS_RES                0x1209    

//建立双向语音对讲请求
#define CMD_DTP_CALL_START_REQ                    	0x1300

//建立双向语音对讲响应
#define CMD_DTP_CALL_START_RES                    	0x1301

//挂断双向语音对讲请求
#define CMD_DTP_CALL_STOP_REQ                     	0x1302

//挂断双向语音对讲响应
#define CMD_DTP_CALL_STOP_RES                     	0x1303
//PE type
#define CMD_PE_HEADER 			0x0
#define CMD_PE_STRING64 		0x10
#define CMD_PE_STRING254 		0x11
#define CMD_PE_CHAR16 			0x12
#define CMD_PE_UCHAR 			0x15
#define CMD_PE_UUDATA 			0x16
#define CMD_PE_CU_ID 			0x1a
#define CMD_PE_CHANNEL_INFO 		0x1f
#define CMD_PE_RECORDSTART_REQ 		0x21
#define CMD_PE_RECORD_STATUS 		0x25
#define CMD_PE_LONG 			0x26
#define CMD_PE_COMPRESS 		0x2e
#define CMD_PE_FD_LOGIN_INFO 		0x1000
#define CMD_PE_FD_ID 			0x1001
#define CMD_PE_CHANNEL_ID	 	0x1002
#define CMD_PE_PTZ_CMD 			0x100a
#define CMD_PE_CTRL_CMD		 	0x100b
#define CMD_PE_TIME 			0x100c
#define CMD_PE_TIME_FILE 		0x100e
#define CMD_PE_VIDEO_CONFIG 		0x100f
#define CMD_PE_VIDEO_REGION 		0x1010
#define CMD_PE_VIDEO_MOTION 		0x1011
#define CMD_PE_CAMERA_CONFIG 		0x1012
#define CMD_PE_CAMERA_PRESET_CONFIG  	0x1013
#define CMD_PE_NETADDRESS 		0x1014
#define CMD_PE_NET_SCHEDULE_ADDRESS 	0x1016
#define CMD_PE_SYSTEM_CONFIG 		0x1017
#define CMD_PE_POINT_MAGNIGY_PARAM 	0x1019
#define CMD_PE_FD_VERSION 		0x101a
#define CMD_PE_CAPTURE 			0x101b
#define CMD_PE_PTZLOCK_INFO 		0x101d
#define CMD_PE_CHANNEL_STATUS 		0x101f
#define CMD_PE_PICTURE 			0x1022
#define CMD_PE_RESULT 			0x1200
#define CMD_PE_VERSION 			0x1201
#define CMD_PE_VIDEOIN_CAPACITY 	0x1202
#define CMD_PE_ALARM_CONTENT_HEADER 	0x2000
#define CMD_PE_ALARM_CONTENT_TEXT 	0x2001
#define CMD_PE_ALARM_CONTENT_PICTURE 	0x2002
#define CMD_PE_ALARM_CONTENT_PICTURE_RECT 0x2005
#define CMD_PE_RECORD_PLAN 		0x2006
#define PE_TIME                                   0x100c

//others
//消息头
#define  CMD_PH_HEAD_FLAG                                  0x4454   

//版本号
#define  CMD_MESSAGE_VERSION_FLAG                  0x0001

/*根据与廉工交流结果，启动媒体预览通道命令中的CHANNEL_ID中的CHANNEL_TYPE为
1表示启动视频预览，为3表示启动音频。停止也相同*/
#define  CMD_VIEW_FLAG                           1             //  视频标志     
#define  CMD_AUDIO_FLAG                          3               // 音频标志

#define  CMD_NET_WORK_STOP                            0x3000

//type definition
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int  uint32;
typedef  int sint32;

typedef struct _PE{
	ushort petype;
	uint32 pelength;
}PE;

typedef struct _PE_CHANNEL_ID
{
    ushort channel_type;
    ushort channel_num;
}PE_CHANNEL_ID;	//閫氶亾ID瀹氫箟

//瑙嗛璋冨害鍦板潃瀹氫箟
typedef struct _PE_NET_SCHEDULE_ADDRESS
{
    uchar protocol;
    uchar trans_direction;
    uchar trans_protocol;
    uchar active_flag;
    ushort serverport;
    uchar ipaddr[64];	//鏄惁鍙互锛�
    //ushort port;
    ushort reserved[2];
}PE_NET_SCHEDULE_ADDRESS;

//璇︾粏閫氶亾鍙傛暟淇℃伅
typedef struct _PE_CHANNEL_INFO
{
	uchar chan_direction;
	uchar chan_qos;
	uchar chan_codetype;
	uchar chan_videosize;
	ushort chan_framerate;
	ushort chan_iframeinter;
	uint32 audio_framelen;
	uint32 chan_maxbit;
}PE_CHANNEL_INFO;


typedef struct _PE_NETADDRESS
{
    uchar ip[64];	//鏄惁鍙互琛ㄧずIP鍦板潃锛熷叧閿槸鍜孭C浜や簰鐨勫ぇ灏忚涓�鑷达紝鍏跺彲浠ヨВ鏋�
    ushort port;
    uchar net_transtype;
    uchar reserved;
}PE_NETADDRESS;

typedef unsigned char PE_UCHAR;
typedef struct _PE_RECORDSTART_REQ
{
    uint32 starttime;
    uint32 stoptime;
    uint32 recordmode;
}PE_RECORDSTART_REQ;
typedef int PE_LONG;

typedef struct _PE_TIME_FILE
{
    uchar usemode;
    uchar status;
    uint32 unlocktime;
    uint32 starttime;
    uint32 stoptime;
    uchar device_id[18];	//鏄惁杩欐牱琛ㄧずstring鍦ㄨ繖閲�
    PE_CHANNEL_ID channel_id;
    uchar storage_area[254];
    uchar filename[254];
    uchar playback_url[254];
    uchar download_url[254];
}PE_TIME_FILE;

typedef struct _PE_CTRL_CMD
{
    uchar control_type;
    int timeoffset;
}PE_CTRL_CMD;
/*typedef struct _PE_VIDEO_CONFIG
{
    ushort chann_type;
    ushort chann_id;
    ushort videoformat;
    ushort videodisplay;
    uchar videoqos;
    uchar framerate;
    ushort keyframe;
    uint32 bitrate;

}PE_VIDEO_CONFIG;
*/
typedef struct _PE_FD_VERSION{
	ushort hardwareversion;
	uint32 softwareversion;
}PE_FD_VERSION;

typedef struct _PE_VERSION{
	uchar majorversion;
	uchar miniversion;
	ushort editversion;
	ushort reviseversion;
}PE_VERSION;

typedef struct _PE_FD_LOGIN_INFO{
	uchar puid[18];
	char passwd[16];
	char seed[16];
	ushort netstat;
}PE_FD_LOGIN_INFO;

typedef struct _QOS_CAPACITY{
	uchar qos;
	uchar maxframe;
	uchar miniframe;
	uchar reserved;
	uint32 bitmap;
	uint32 maxbitmap;
	uint32 minibitmap;
	ushort maxbminterval;
	ushort minibminterval;
	uint32 reserved_1;
	uint32 reserved_2;
	uint32 reserved_3;
}QOS_CAPACITY;

typedef struct _PE_VIDEOIN_CAPACITY{
	PE_CHANNEL_ID channelid;
	QOS_CAPACITY qoscapacity;
}PE_VIDEOIN_CAPACITY;

typedef struct _PE_PTZLOCK_INFO{
	uchar lockemail[65];
	uint32 locklevel;
	uint32 utc;
}PE_PTZLOCK_INFO;

typedef struct _PE_PTZ_CMD{
	PE_CHANNEL_ID channelid;
	uchar cmd;
	uchar cmdpararm;
}PE_PTZ_CMD;

typedef struct _PE_SYSTEM_CONFIG{
	uchar serverip[65];
	ushort port;
	uchar puid[19];
	uchar puname[65];
}PE_SYSTEM_CONFIG;

typedef struct _PE_VIDEO_CONFIG{
	ushort channeltype;
	ushort channelid;
	ushort encodeformat;
	ushort showformat;
	uchar prior;
	uchar frame;
	ushort frameinterval;
	uint32 coderate;
}PE_VIDEO_CONFIG;


typedef struct _PE_HEADER
{
    ushort flag;
    ushort version;
    ushort zipflag;
    ushort msgtype;
    ushort penum;
    ushort reserved;
    uint32 msglen;
    uint32 seq;
    unsigned char srcdomain[6];
    unsigned char srcid[18];
    unsigned char destdomain[6];
    unsigned char destid[18];
}PE_HEADER;

typedef enum _DTP_MSG_DEF
{
    DTP_HEADER_T = 0x00,
    DTP_HEART_BEAT_REQUEST = 0x120,
    DTP_HEART_BEAT_RESPONSE,
    DTP_QUERY_RECORDSTATUS_REQ = 0x164,
    DTP_MSG_END
}DTP_MSG_DEF;

typedef enum _PE_CMD_DEF
{
    PE_HEADER_CMD = 0x00,
    PE_STRING64_CMD = 0x10,
    PE_STRING254_CMD,
    PE_CHAR16_CMD,
    PE_CMD_END
}PE_CMD_DEF;

typedef struct _DTP_HEART_BEAT_REQ{
	ushort channeltype;
	ushort channelid;
	uchar islocalrecording;
	uchar ispreviewing;
	uchar isvcr;
	uchar isdownloading;
	ushort videocodeformat;
	ushort videoshowformat;
	uint32 totalstorageunit;
	uint32 sparestorageunit;
	uint32 sendingcode;
	uchar islostvideo;
	uchar isterminal;
	uchar terminalexception;
}DTP_HEART_BEAT_REQ;

typedef struct _DTP_HEART_BEAT_RES{
	uint32 time;
}DTP_HEART_BEAT_RES;

typedef struct _DTP_PU_LOGIN_STEP1_REQ{
	uchar puid[18];
	PE_FD_VERSION puversion;
	ushort pucompress;
	PE_NETADDRESS puip;
}DTP_PU_LOGIN_STEP1_REQ;

typedef struct _DTP_PU_LOGIN_STEP1_RES{
	int result;
	//PE_ADDRESS puip;
	char seed[16];
	PE_VERSION peversion;
	ushort pucompress;
}DTP_PU_LOGIN_STEP1_RES;

typedef struct _DTP_PU_LOGIN_STEP2_REQ{
	PE_FD_LOGIN_INFO logininfo;
	PE_VIDEOIN_CAPACITY pvc;
	uchar url[255];
}DTP_PU_LOGIN_STEP2_REQ;

typedef struct _DTP_PU_LOGIN_STEP2_RES{
	int result;
	uint32 time;
}DTP_PU_LOGIN_STEP2_RES;

typedef struct _DTP_PU_LOGOUT_REQ{
	uchar puid[18];
}DTP_PU_LOGOUT_REQ;

typedef struct _DTP_PU_LOGOUT_RES{
	int result;
}DTP_PU_LOGOUT_RES;

typedef struct _DTP_PREVIEW_START_REQ{
	PE_CHANNEL_ID channelid;
	char guid[16];
	PE_NET_SCHEDULE_ADDRESS videoip;
	PE_CHANNEL_INFO channelinfo;
}DTP_PREVIEW_START_REQ;

typedef struct _DTP_PREVIEW_START_RES{
	int result;
	PE_NETADDRESS udpip;
	uchar opt[255];
	PE_CHANNEL_INFO channelinfo;
}DTP_PREVIEW_START_RES;

typedef struct _DTP_PREVIEW_STOP_REQ{
	PE_CHANNEL_ID channelid;
	char guid[16];
}DTP_PREVIEW_STOP_REQ;

typedef struct _DTP_PREVIEW_STOP_RES{
	int result;
}DTP_PREVIEW_STOP_RES;


typedef struct _DTP_CONTROL_PTZ_REQ{
	PE_PTZ_CMD ptzcmd;
	PE_PTZLOCK_INFO lockinfo;
}DTP_CONTROL_PTZ_REQ;

typedef struct _DTP_CONTROL_PTZ_RES{
	int result;
	PE_PTZLOCK_INFO lockinfo;
}DTP_CONTROL_PTZ_RES;

typedef struct _DTP_CALL_START_REQ{
	PE_CHANNEL_ID channelid;
	PE_NETADDRESS audioip;
}DTP_CALL_START_REQ;

typedef struct _DTP_CALL_START_RES{
	int result;
	PE_NETADDRESS netip;
}DTP_CALL_START_RES;

typedef struct _DTP_CALL_STOP_REQ{
	PE_CHANNEL_ID channelid;
}DTP_CALL_STOP_REQ;

typedef struct _DTP_CALL_STOP_RES{
	int result;
}DTP_CALL_STOP_RES;

typedef struct _DTP_CONFIG_SYSINFO_REQ{
	PE_SYSTEM_CONFIG systemconfig;
}DTP_CONFIG_SYSINFO_REQ;

typedef struct _DTP_CONFIG_SYSINFO_RES{
	int result;
}DTP_CONFIG_SYSINFO_RES;

typedef struct _DTP_CONFIG_VPARAM_REQ{
	PE_VIDEO_CONFIG videoparam;
}DTP_CONFIG_VPARAM_REQ;

typedef struct _DTP_CONFIG_VPARAM_RES{
 	int result;
}DTP_CONFIG_VPARAM_RES;
#ifdef __cplusplus
}
#endif

#endif
