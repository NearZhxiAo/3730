#ifndef MEDIACONF_H_INCLUDED
#define MEDIACONF_H_INCLUDED
#include "TypeDefs.h"
#ifdef _cplusplus
extern "C" {
#endif

/*视频配置相关参数*/
/////////////////////////////////////////////
//定义数字摄像机的类型及IP
typedef enum _DIGITAL_DVRDVS
{
    DIGITAL_DVRDVS_START,
    DIGITAL_DVRDVS_HK,				//数字相机类型为海康DVR或者IPCAM
    DIGITAL_DVRDVS_DAHUA,			//数字相机类型为大华DVR或者IPCAM
	//...
    DIGITAL_DVRDVS_END
}DIGITAL_DVRDVS;

//定义数字报警设备的类型
typedef enum _DIGITAL_ALARM
{
    DIGITAL_ALARM_START,
    DIGITAL_ALARM_IQ,				//数字报警类型为澳大利亚IQ
    DIGITAL_ALARM_ZHIANBANG,		//数字报警类型为智安邦
    DIGITAL_ALARM_END			
}DIGITAL_ALARM;

//数字设备的详细信息
typedef struct _DIGITAL_INFO
{
	DIGITAL_DVRDVS digital_dvrdvs;	//DVR或者IPCAM的类型	
	DIGITAL_ALARM  digital_alarm;	//数字报警设备类型	
	UCHAR  ipaddr[32];		//对应的IP地址
}DIGITAL_INFO;

//视频源
typedef enum _VIDEO_SRC
{
    VIDEO_SRC_START,
    VIDEO_SRC_ANALOG,          		//模拟视频输入
    VIDEO_SRC_DIGITAL_DVRDVS,  		//数字视频输入，输入的是数字视频
    VIDEO_SRC_DIGITAL_ALARM,  		//数字输入，输入的是报警信息
    VIDEO_SRC_SENSOR_ALARM,    		//外部的sensor输入报警传感器输入报警
    VIDEO_SRC_END
}VIDEO_SRC;

//编码格式
typedef enum _VIDEOENC_MODE
{
    VIDEO_ENC_START,
    VIDEO_ENC_JPEG,           	//编码格式为JPEG
    VIDEO_ENC_H264,           	//编码格式为H264
    VIDEO_ENC_MPEG4,			//编码格式为MPEG4
    VIDEO_ENC_END
}VIDEOENC_MODE;

#define QCIF_WIDTH 192
#define QCIF_HEIGHT 160
#define CIF_WIDTH 384
#define CIF_HEIGHT 288
#define D1_WIDTH 768
#define D1_HEIGHT 576

typedef enum _ANALOG_VFRAME_TYPE
{
	ANALOG_VFRAME_TYPE_START,
// 	ANALOG_VFRAME_TYPE_NTSC_QCIF,//168*128
// 	ANALOG_VFRAME_TYPE_NTSC_CIF,//320*240
// 	ANALOG_VFRAME_TYPE_NTSC_D1,//640*480
// 	ANALOG_VFRAME_TYPE_PAL_QCIF,//192*160
// 	ANALOG_VFRAME_TYPE_PAL_CIF,//384*288
// 	ANALOG_VFRAME_TYPE_PAL_D1,//768*576
	ANALOG_VFRAME_TYPE_QCIF,//192*160
	ANALOG_VFRAME_TYPE_CIF,//384*288
	ANALOG_VFRAME_TYPE_D1,//768*576
	ANALOG_VFRAME_TYPE_END,
}ANALOG_VFRAME_TYPE;

typedef enum _ANALOG_VBIT_TYPE
{
	ANALOG_VBIT_TYPE_START,
	ANALOG_VBIT_TYPE_RGB24,
	ANALOG_VBIT_TYPE_YV12,
	ANALOG_VBIT_TYPE_YVY2,
	ANALOG_VBIT_TYPE_END,
}ANALOG_VBIT_TYPE;

typedef struct _ANALOG_INFO
{
	//采集原格式 RGB24 YV12 YVY2--与采集卡关联
	ANALOG_VBIT_TYPE vbittype;
	ANALOG_VFRAME_TYPE vframetype;
}ANALOG_INFO;//模拟视频数据信息

/*视频配置结构体定义*/
typedef struct _VIDEO_PARA
{
    VIDEO_SRC     src;			//视频源
    VIDEOENC_MODE mod;			//编码格式
	ANALOG_INFO   analoginfo;   //模拟视频对应信息
    DIGITAL_INFO  digitalinfo;	//数字视频对应信息
    UINT  		  bright;       //亮度
    UINT          contrast;     //对比度
    UINT          saturation;   //饱和度
    UINT          hue;          //色度
}VIDEO_PARA;


//视频开始、停止动作定义
typedef enum _VIDEO_ACTION
{
    VIDEO_START,
    VIDEO_REQUEST,  			//视频请求
    VIDEO_STOP,   				//视频停止
    VIDEO_END
}VIDEO_ACTION;

//视频开始、停止动作定义
typedef enum _VIDEO_ATTRSET
{
    VIDEO_ATTRSET_START,
	VIDEO_ATTRSET_BRIGHT,  			//视频请求
	VIDEO_ATTRSET_CONTRAST,   		//视频停止
	VIDEO_ATTRSET_SATURATION,
	VIDEO_ATTRSET_HUE,
	VIDEO_ATTRSET_END,
}VIDEO_ATTRSET;

typedef struct _VIDEO_ATTRSET_STRUCT
{
	VIDEO_ATTRSET video_attrset;
	UINT          video_attrvalue;
}VIDEO_ATTRSET_STRUCT;


//报警图片目录获取(年、月有效性)
#define MAXYEARARRAY	2
typedef struct _DIR_CONTENTS
{
    //摄像头信息描述
    UINT              camid;	//摄像头ID信息，描述哪个摄像头
    UINT              year[MAXYEARARRAY];	//从低bit开始，2000年，0为无效，1为有效，32位表示32年
    USHORT            mon[MAXYEARARRAY*32];	//从低bit开始，1月，0为无效，1为有效
}DIR_CONTENTS;

//以月为单位进行月内有效日期获取
typedef struct _DIR_DAY
{
    UINT  day;       //每位为单元，1为有效
}DIR_DAY;

//以年月日获取到该日内的报警图片名称
typedef struct  DIR_PICNAME
{
    UINT  totalcount;  //总共具有的文件数目
    UINT  currentstart; //此包开始的索引数目
    UCHAR picname[16][64]; //每包最大传16个文件名
}DIR_PICNAME;

//报警目录信息（年、月消息）
typedef struct _ALARM_YM_PARA
{
    DIR_CONTENTS dircontents;
}ALARM_YM_PARA;

//报警图片
#define PIC_LENGTH_PERPACKET	1300
typedef  struct  _PIC_CONTENTS
{
    UINT  totallen;//图片整个长度
    UINT  curr_index; //最后一个index为0xFFFFFFFF
    UINT  packet_len;//包长度
    UCHAR packet_content[PIC_LENGTH_PERPACKET]; //包里实际数据
}PIC_CONTENTS;

//报警目录信息（指定年月里面的日消息）
typedef struct  _DIR_YMD
{
    UINT year;
    UINT mon;
    UINT day;
}DIR_YMD;

typedef struct _ALARM_DAY_PARA
{
    DIR_DAY day;
}ALARM_DAY_PARA;

//某日内的报警图片名称
typedef struct _ALARM_NAME_PARA
{
    DIR_PICNAME picname;
}ALARM_NAME_PARA;

//视频请求回应，视频停止回应和keepalive回应
typedef enum _ACK_
{
    ACK_START,
    ACK_OK,
    ACK_FAILED,
    ACK_END
}ACK;

typedef struct _VIDEO_REQUEST_STRUCT
{
	char cip[32];
	int  iport;
}VIDEO_REQUEST_STRUCT;



#define VIDEO_WIDTH 768

#define VIDEO_HEIGHT 576

#ifdef _cplusplus
}
#endif



#endif // MEDIACONF_H_INCLUDED
