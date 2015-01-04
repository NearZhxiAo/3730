#ifndef _RTP_HEAD
#define _RTP_HEAD

#ifdef _cplusplus
extern "C" {
#endif

#define MTU   1452
#define RTP_Msg_Len   	1024
#define DATA_MAX_LEN    1400  //定义包的数据长度最大为1400 bytes


/*载荷类型宏定义*/
#define PAYLOAD_PCMA    8
#define PAYLOAD_H264    98
#define PAYLOAD_MPEG4   100
#define PAYLOAD_G7221   121
#define PAYLOAD_CONTROL 122


/*传输协议类型定义*/
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
    /*RTP头，12字节*/
    //unsigned char     Version    : 2;  //2bit,版本
    //unsigned char     Padding    : 1;  //1bit,间隙
    //unsigned char     Extension  : 1;  //1bit,扩展位
    //unsigned char     Ccount     : 4;  //4bit,CSRC 标识符
    unsigned char          VPEC;
    //unsigned char     Marker     : 1;  //1bit,标识
    //unsigned char     Ptype      : 7;  //7bit,识别RTP 有效载荷的格式
    unsigned char      Mrk_type;
    unsigned short      Snumber;         //16bit,RTP数据包序列号
    unsigned int      Timestamp;         //32bit,时间戳
    unsigned char       Ssrc[4];         //32bit,同步源标识
    //unsigned int       Csrc;           //32bit,分信源标识

}RTP_DATA_HEAD, *pRTP_DATA_HEAD;

typedef  struct  _RTP_Exten_HEAD
{
    /*RTP头，12字节*/
    //unsigned char     Version    : 2;  //2bit,版本
    //unsigned char     Padding    : 1;  //1bit,间隙
    //unsigned char     Extension  : 1;  //1bit,扩展位
    //unsigned char     Ccount     : 4;  //4bit,CSRC 标识符
    unsigned char          VPEC;
    //unsigned char     Marker     : 1;  //1bit,标识
    //unsigned char     Ptype      : 7;  //7bit,识别RTP 有效载荷的格式
    unsigned char      Mrk_type;
    unsigned short      Snumber;         //16bit,RTP数据包序列号
    unsigned int      Timestamp;         //32bit,时间戳
    unsigned char         Ssrc[4];         //32bit,同步源标识
    //unsigned int       Csrc;           //32bit,分信源标识

    /*扩展部分定义*/
    unsigned short    Def_by_profile;    //16bit,取值0x7376
    unsigned short            Length;    //16bit,取值3
    //unsigned int    T :1;     //1bit,0表示时戳遵循RTP标准,为1表示时戳的单位为ms
    //unsigned int    S :1;     //1bit,标准解码器能解码的码流置1,否则置0
    //unsigned int    Width :14;  //14bit,图像宽度
    //unsigned int    Reserve :2; //2bit,保留,填0
    //unsigned int    Height :14; //14bit,图像高度
    unsigned short             TSWid;
    unsigned short            ResHgt;
    unsigned short    Ext_time_stamp; //16bits
    unsigned short          FTY_code; //16bits,厂商代码,缺省0x7376
    unsigned int            CodeType; //32bits,编码类型,接收端根据此类型来调用对应的解码器

}RTP_Exten_HEAD, *pRTP_Exten_HEAD;


typedef  struct  _RTP_CTRL_HEAD
{
    /*RTP头，12字节*/
    RTP_DATA_HEAD       rtphead;
    /*控制命令部分定义，20字节*/
    unsigned short  CtrlCmdType; /*命令类型*/
    unsigned short   CmdParaLen; /*命令参数长度*/
    unsigned char   CmdPara[16]; /*命令参数*/

}RTP_CTRL_HEAD, *pRTP_CTRL_HEAD;


char* fillRtpHead(char*databuf, RTP_HEAD_INFO* prtp_head_info, unsigned int* psend_len, unsigned int *offset);
char* fillRtpExtenHead(char*databuf, RTP_HEAD_INFO* prtp_head_info, unsigned int* psend_len, unsigned int *offset);
char* fillRtpCtrlHead(RTP_HEAD_INFO* prtp_head_info, unsigned short rtpCtrltype);

#ifdef _cplusplus
}
#endif


#endif
