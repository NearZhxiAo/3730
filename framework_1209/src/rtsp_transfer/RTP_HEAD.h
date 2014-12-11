#ifndef _RTP_HEAD
#define _RTP_HEAD

#ifdef _cplusplus
extern "C" {
#endif

#define MTU   1452
#define RTP_Msg_Len   	1024
#define DATA_MAX_LEN    1400  //����������ݳ������Ϊ1400 bytes


/*�غ����ͺ궨��*/
#define PAYLOAD_PCMA    8
#define PAYLOAD_H264    98
#define PAYLOAD_MPEG4   100
#define PAYLOAD_G7221   121
#define PAYLOAD_CONTROL 122


/*����Э�����Ͷ���*/
#define UDP 0x01
#define TCP 0x02

typedef struct  RTP_HEAD_INFO
{
    unsigned char  trans_protocol;
    unsigned short   data_len;
    unsigned char  Extension;
    unsigned char  Marker;
    unsigned char  Payload;
    unsigned short seqnumber;
    unsigned int   timestamp;
    unsigned char  SSRC[4];
    unsigned char  AUDIO_SSRC[4];
	unsigned short v_width;
	unsigned short v_height;
    unsigned char  T;
    unsigned char  S;

}RTP_HEAD_INFO;

typedef  struct  _RTP_DATA_HEAD
{
    /*RTPͷ��12�ֽ�*/
    //unsigned char     Version    : 2;  //2bit,�汾
    //unsigned char     Padding    : 1;  //1bit,��϶
    //unsigned char     Extension  : 1;  //1bit,��չλ
    //unsigned char     Ccount     : 4;  //4bit,CSRC ��ʶ��
    unsigned char          VPEC;
    //unsigned char     Marker     : 1;  //1bit,��ʶ
    //unsigned char     Ptype      : 7;  //7bit,ʶ��RTP ��Ч�غɵĸ�ʽ
    unsigned char      Mrk_type;
    unsigned short      Snumber;         //16bit,RTP���ݰ����к�
    unsigned int      Timestamp;         //32bit,ʱ���
    unsigned char       Ssrc[4];         //32bit,ͬ��Դ��ʶ
    //unsigned int       Csrc;           //32bit,����Դ��ʶ

}RTP_DATA_HEAD, *pRTP_DATA_HEAD;

typedef  struct  _RTP_Exten_HEAD
{
    /*RTPͷ��12�ֽ�*/
    //unsigned char     Version    : 2;  //2bit,�汾
    //unsigned char     Padding    : 1;  //1bit,��϶
    //unsigned char     Extension  : 1;  //1bit,��չλ
    //unsigned char     Ccount     : 4;  //4bit,CSRC ��ʶ��
    unsigned char          VPEC;
    //unsigned char     Marker     : 1;  //1bit,��ʶ
    //unsigned char     Ptype      : 7;  //7bit,ʶ��RTP ��Ч�غɵĸ�ʽ
    unsigned char      Mrk_type;
    unsigned short      Snumber;         //16bit,RTP���ݰ����к�
    unsigned int      Timestamp;         //32bit,ʱ���
    unsigned char         Ssrc[4];         //32bit,ͬ��Դ��ʶ
    //unsigned int       Csrc;           //32bit,����Դ��ʶ

    /*��չ���ֶ���*/
    unsigned short    Def_by_profile;    //16bit,ȡֵ0x7376
    unsigned short            Length;    //16bit,ȡֵ3
    //unsigned int    T :1;     //1bit,0��ʾʱ����ѭRTP��׼,Ϊ1��ʾʱ���ĵ�λΪms
    //unsigned int    S :1;     //1bit,��׼�������ܽ����������1,������0
    //unsigned int    Width :14;  //14bit,ͼ����
    //unsigned int    Reserve :2; //2bit,����,��0
    //unsigned int    Height :14; //14bit,ͼ��߶�
    unsigned short             TSWid;
    unsigned short            ResHgt;
    unsigned short    Ext_time_stamp; //16bits
    unsigned short          FTY_code; //16bits,���̴���,ȱʡ0x7376
    unsigned int            CodeType; //32bits,��������,���ն˸��ݴ����������ö�Ӧ�Ľ�����

}RTP_Exten_HEAD, *pRTP_Exten_HEAD;


typedef  struct  _RTP_CTRL_HEAD
{
    /*RTPͷ��12�ֽ�*/
    RTP_DATA_HEAD       rtphead;
    /*��������ֶ��壬20�ֽ�*/
    unsigned short  CtrlCmdType; /*��������*/
    unsigned short   CmdParaLen; /*�����������*/
    unsigned char   CmdPara[16]; /*�������*/

}RTP_CTRL_HEAD, *pRTP_CTRL_HEAD;


char* fillRtpHead(char*databuf, RTP_HEAD_INFO* prtp_head_info, unsigned int* psend_len, unsigned int *offset);
char* fillRtpExtenHead(char*databuf, RTP_HEAD_INFO* prtp_head_info, unsigned int* psend_len, unsigned int *offset);
char* fillRtpCtrlHead(RTP_HEAD_INFO* prtp_head_info, unsigned short rtpCtrltype);

#ifdef _cplusplus
}
#endif


#endif
