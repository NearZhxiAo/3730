#ifndef _IDENTCENTER_H_
#define _IDENTCENTER_H_

#ifdef _cplusplus
extern "C" {
#endif

//#include "Recognizer.h"
#include "SocketApi.h"


#define MAGIC_DEV2CENT_START		0x12345678
#define MAGIC_DEV2CENT_KEEPALIVE	0x11223344
#define MAGIC_DEV2CENT_END			0x87654321

#define MAGIC_CENT2DEV_START		0x87654321
#define MAGIC_CENT2DEV_KEEPALIVE	0x55667788
#define MAGIC_CENT2DEV_END			0x12345678


#define MAGIC_ALARM_START			0x9ABCDEF0
#define MAGIC_ALARM_END				0x0FEDCBA9
#define MAGIC_PIC_START				0x99AABBCC
#define MAGIC_PIC_END				0xDDEEFF00

#define PACKAGE_END					0xFFFFFFFF

#define CHAR_LEN					0x30
typedef struct TCP_MSG_TO_CENTER_
{
	unsigned char dev_id[CHAR_LEN];
	unsigned char dev_name[CHAR_LEN];
	unsigned char dev_ip[CHAR_LEN];
}TCPMSG_TO_CENTER;

#define TCPMSG_TO_CENTER_LEN sizeof(TCPMSG_TO_CENTER)

typedef enum _ALARM_CONNTYPE
{
	ALARM_CONN_UDP = 1,
	ALARM_CONN_TCP,
}ALARM_CONNTYPE;

typedef enum _DEV2CENTMSGTYPE
{
	MSG_ALARM = 1,
	MSG_PICTURE
}DEV2CENTMSGTYPE;

typedef enum _ALARMTYPE
{
	ALARM_WINDOW,
	ALARM_OTHER
}ALARMTYPE;

typedef struct TCP_MSG_ACK_
{
	unsigned char	alarm_ipaddr[CHAR_LEN]; 
	unsigned int	alarm_port;
	ALARM_CONNTYPE	alarm_conntype; 
	//SYSTEMTIME		alarm_systime;
}TCPMSG_ACK;
#define TCPMSG_ACK_LEN sizeof(TCPMSG_ACK)

typedef struct _dev_to_center_keepalive
{
	unsigned int magic_start;
	unsigned int keepalivedata;
	unsigned int magic_end;
}DEV2CENT_KEEPALIVE;
#define DEV2CENT_KEEPALIVE_LEN sizeof(DEV2CENT_KEEPALIVE)

typedef struct _center_to_dev_keepalive
{
	unsigned int magic_start;
	unsigned int keepalivedata;
	unsigned int magic_end;
}CENT2DEV_KEEPALIVE;
#define CENT2DEV_KEEPALIVE_LEN sizeof(CENT2DEV_KEEPALIVE)

typedef struct _dev_to_center_alarmmsg
{
	unsigned int	magic_start;
	unsigned char	dev_id[CHAR_LEN];
	DEV2CENTMSGTYPE msg_type; 
	unsigned int	alarm_index; 
	unsigned char	alarm_area[CHAR_LEN]; 
	ALARMTYPE		alarmtype; 
	unsigned char	alarm_time[CHAR_LEN];
	unsigned char	alarm_picname[CHAR_LEN];
	unsigned int	magic_end;
}DEV2CENT_ALARMMSG;
#define DEV2CENT_ALARMMSG_LEN sizeof(DEV2CENT_ALARMMSG)

typedef struct _dev_to_center_pic
{
	unsigned int	magic_start; 
	unsigned int	alarm_index;
	unsigned int	package_index;
	unsigned char	alarm_picname[CHAR_LEN];
	unsigned int	msg_len;
	unsigned char	msg_contents[1200];
	unsigned int	magic_end;
}DEV2CENT_PIC;
#define DEV2CENT_PIC_LEN sizeof(DEV2CENT_PIC)

typedef enum _tcp_msg_type_en
{
	TCP_MSG_TYPE_START,
	TCP_MSG_TYPE_CONNECT,
	TCP_MSG_TYPE_CONNECT_ACK,
	TCP_MSG_TYPE_KEEPALIVE,
	TCP_MSG_TYPE_KEEPALIVE_ACK,
	TCP_MSG_TYPE_DISCONNECT,
	TCP_MSG_TYPE_DISCONNECT_ACK,
	TCP_MSG_TYPE_REFUSE,
	TCP_MSG_TYPE_SERVER_QUIT,
	TCP_MSG_TYPE_RECONNECT,
	TCP_MSG_TYPE_RS485,
	TCP_MSG_TYPE_RS485_ACK,
	TCP_MSG_TYPE_END,
}TCP_MSG_TYPE_EN;

typedef struct _tcp_msg_t
{
	TCP_MSG_TYPE_EN msg_type;
	unsigned char msg_buf[1024];
}TCP_MSG_T;
#define TCP_MSG_LEN sizeof(TCP_MSG_T)

typedef enum _udp_msg_type_en
{
	UDP_MSG_TYPE_START,
	UDP_MSG_TYPE_ALARM,
	UDP_MSG_TYPE_ALARM_ACK,
	UDP_MSG_TYPE_PIC,
	UDP_MSG_TYPE_PIC_ACK,
	UDP_MSG_TYPE_END,
}UDP_MSG_TYPE_EN;

typedef struct _udp_msg_t
{
	UDP_MSG_TYPE_EN msg_type;
	unsigned char msg_buf[1400];
}UDP_MSG_T;
#define UDP_MSG_LEN sizeof(UDP_MSG_T)


class IdentCenter{
public:
	IdentCenter();
	~IdentCenter();
public:
	static unsigned  ThreadForConnectServer(void * pParam);
	static unsigned  ThreadForCommFunc(void * pParam);
	static unsigned  ThreadForNewFileSend(void * pParam);
	//static void CALLBACK TimerOutProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime); 
	//bool Attach(Recognizer * pReg,HWND pHwnd);
	//bool Detach();
	bool ConnectServer();
	bool Initilize();
	bool OnTimerOut();
	bool GetDev2CenMsgByName(DEV2CENT_PIC &dev2cent_pic,DEV2CENT_ALARMMSG &dev2cent_alarmmsg,const char fname[]);
private:
	static bool m_bIsExit;
	
	pthread_t m_hThread4ConnectServer;

	pthread_t m_hThread4CommFunc;

	pthread_t m_hThread4NewFileSend;

	//CString m_cstrErrorInfo;
};


#ifdef _cplusplus
}
#endif



#endif /*#ifndef _IDENTCENTER_H_*/
