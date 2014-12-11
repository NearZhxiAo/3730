#ifndef MEDIACONF_H_INCLUDED
#define MEDIACONF_H_INCLUDED
#include "TypeDefs.h"
#ifdef _cplusplus
extern "C" {
#endif

/*��Ƶ������ز���*/
/////////////////////////////////////////////
//������������������ͼ�IP
typedef enum _DIGITAL_DVRDVS
{
    DIGITAL_DVRDVS_START,
    DIGITAL_DVRDVS_HK,				//�����������Ϊ����DVR����IPCAM
    DIGITAL_DVRDVS_DAHUA,			//�����������Ϊ��DVR����IPCAM
	//...
    DIGITAL_DVRDVS_END
}DIGITAL_DVRDVS;

//�������ֱ����豸������
typedef enum _DIGITAL_ALARM
{
    DIGITAL_ALARM_START,
    DIGITAL_ALARM_IQ,				//���ֱ�������Ϊ�Ĵ�����IQ
    DIGITAL_ALARM_ZHIANBANG,		//���ֱ�������Ϊ�ǰ���
    DIGITAL_ALARM_END			
}DIGITAL_ALARM;

//�����豸����ϸ��Ϣ
typedef struct _DIGITAL_INFO
{
	DIGITAL_DVRDVS digital_dvrdvs;	//DVR����IPCAM������	
	DIGITAL_ALARM  digital_alarm;	//���ֱ����豸����	
	UCHAR  ipaddr[32];		//��Ӧ��IP��ַ
}DIGITAL_INFO;

//��ƵԴ
typedef enum _VIDEO_SRC
{
    VIDEO_SRC_START,
    VIDEO_SRC_ANALOG,          		//ģ����Ƶ����
    VIDEO_SRC_DIGITAL_DVRDVS,  		//������Ƶ���룬�������������Ƶ
    VIDEO_SRC_DIGITAL_ALARM,  		//�������룬������Ǳ�����Ϣ
    VIDEO_SRC_SENSOR_ALARM,    		//�ⲿ��sensor���뱨�����������뱨��
    VIDEO_SRC_END
}VIDEO_SRC;

//�����ʽ
typedef enum _VIDEOENC_MODE
{
    VIDEO_ENC_START,
    VIDEO_ENC_JPEG,           	//�����ʽΪJPEG
    VIDEO_ENC_H264,           	//�����ʽΪH264
    VIDEO_ENC_MPEG4,			//�����ʽΪMPEG4
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
	//�ɼ�ԭ��ʽ RGB24 YV12 YVY2--��ɼ�������
	ANALOG_VBIT_TYPE vbittype;
	ANALOG_VFRAME_TYPE vframetype;
}ANALOG_INFO;//ģ����Ƶ������Ϣ

/*��Ƶ���ýṹ�嶨��*/
typedef struct _VIDEO_PARA
{
    VIDEO_SRC     src;			//��ƵԴ
    VIDEOENC_MODE mod;			//�����ʽ
	ANALOG_INFO   analoginfo;   //ģ����Ƶ��Ӧ��Ϣ
    DIGITAL_INFO  digitalinfo;	//������Ƶ��Ӧ��Ϣ
    UINT  		  bright;       //����
    UINT          contrast;     //�Աȶ�
    UINT          saturation;   //���Ͷ�
    UINT          hue;          //ɫ��
}VIDEO_PARA;


//��Ƶ��ʼ��ֹͣ��������
typedef enum _VIDEO_ACTION
{
    VIDEO_START,
    VIDEO_REQUEST,  			//��Ƶ����
    VIDEO_STOP,   				//��Ƶֹͣ
    VIDEO_END
}VIDEO_ACTION;

//��Ƶ��ʼ��ֹͣ��������
typedef enum _VIDEO_ATTRSET
{
    VIDEO_ATTRSET_START,
	VIDEO_ATTRSET_BRIGHT,  			//��Ƶ����
	VIDEO_ATTRSET_CONTRAST,   		//��Ƶֹͣ
	VIDEO_ATTRSET_SATURATION,
	VIDEO_ATTRSET_HUE,
	VIDEO_ATTRSET_END,
}VIDEO_ATTRSET;

typedef struct _VIDEO_ATTRSET_STRUCT
{
	VIDEO_ATTRSET video_attrset;
	UINT          video_attrvalue;
}VIDEO_ATTRSET_STRUCT;


//����ͼƬĿ¼��ȡ(�ꡢ����Ч��)
#define MAXYEARARRAY	2
typedef struct _DIR_CONTENTS
{
    //����ͷ��Ϣ����
    UINT              camid;	//����ͷID��Ϣ�������ĸ�����ͷ
    UINT              year[MAXYEARARRAY];	//�ӵ�bit��ʼ��2000�꣬0Ϊ��Ч��1Ϊ��Ч��32λ��ʾ32��
    USHORT            mon[MAXYEARARRAY*32];	//�ӵ�bit��ʼ��1�£�0Ϊ��Ч��1Ϊ��Ч
}DIR_CONTENTS;

//����Ϊ��λ����������Ч���ڻ�ȡ
typedef struct _DIR_DAY
{
    UINT  day;       //ÿλΪ��Ԫ��1Ϊ��Ч
}DIR_DAY;

//�������ջ�ȡ�������ڵı���ͼƬ����
typedef struct  DIR_PICNAME
{
    UINT  totalcount;  //�ܹ����е��ļ���Ŀ
    UINT  currentstart; //�˰���ʼ��������Ŀ
    UCHAR picname[16][64]; //ÿ�����16���ļ���
}DIR_PICNAME;

//����Ŀ¼��Ϣ���ꡢ����Ϣ��
typedef struct _ALARM_YM_PARA
{
    DIR_CONTENTS dircontents;
}ALARM_YM_PARA;

//����ͼƬ
#define PIC_LENGTH_PERPACKET	1300
typedef  struct  _PIC_CONTENTS
{
    UINT  totallen;//ͼƬ��������
    UINT  curr_index; //���һ��indexΪ0xFFFFFFFF
    UINT  packet_len;//������
    UCHAR packet_content[PIC_LENGTH_PERPACKET]; //����ʵ������
}PIC_CONTENTS;

//����Ŀ¼��Ϣ��ָ���������������Ϣ��
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

//ĳ���ڵı���ͼƬ����
typedef struct _ALARM_NAME_PARA
{
    DIR_PICNAME picname;
}ALARM_NAME_PARA;

//��Ƶ�����Ӧ����Ƶֹͣ��Ӧ��keepalive��Ӧ
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
