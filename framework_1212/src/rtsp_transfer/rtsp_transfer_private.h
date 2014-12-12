#ifndef	_RTSP_TRANFER_PRIVATE_H__
#define _RTSP_TRANFER_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "libad.h"
#include "libgbshm.h"
#include "libunix.h"
#include "libvsshm.h"
#include "libasshm.h"
#include "libaudio.h"
#include "libsignal.h"
#include "libsk.h"

#include <stdlib.h>
#include <time.h>
#include "RTP_HEAD.h"
#include "rtp_ctrl_type.h"

#define FALSE 0
#define TRUE  1
#define RTP_PAYLOAD_CTRL 122

#undef SAVE_DATA
#define SAVE_DATA_AUDIO
#undef SAVE_DATA_AUDIO
//#define RTSP_SEND
typedef enum _RTSP_STATE
{
	RTSP_STATE_START,
	RTSP_STATE_IDLE,
	RTSP_STATE_PREVIEW,
	RTSP_STATE_PREVIEW_PAUSE,
	RTSP_STATE_PREVIEW_CALL,
	RTSP_STATE_CALL,
	RTSP_STATE_CALL_PAUSE,
	RTSP_STATE_PREVIEW_PAUSE_CALL,
	RTSP_STATE_PREVIEW_CALL_PAUSE,
	RTSP_STATE_PREVIEW_PAUSE_CALL_PAUSE,
	RTSP_STATE_END
}RTSP_STATE;

/*record cmd info from client_pc*/
typedef struct _INFO_FROM_CLIENTPC
{
	unsigned char IPProtocol;
	char IPStr[15];  /*record for IP：xxx.xxx.xxx.xxx*/
	unsigned short Port;

}INFO_FROM_CLIENTPC;


#define RTSP_TRANFER_POLL_COUNT_MAX 3	

typedef struct _INFO_FROM_AVENC
{
	LIBVSSHM_FRAME_TYPE  frame_type; //码流类型
	LIBVSSHM_I_P_FLAG    i_p_flag;  //I帧，P帧
	long long timestamp;    //帧的时间戳
	unsigned char *pbuf;  //指向数据的buffer
	long buf_len;  //数据长度

}INFO_FROM_AVENC;

typedef  struct _AUDIO_INFO_FROM_AVENC
{ 
	 LIBASSHM_FRAME_TYPE  frame_type;
	  long long timestamp;
	  unsigned  char *pbuf ;
	  long  buf_len ;
}AUDIO_INFO_FROM_AVENC;

typedef struct _RTSP_TRANFER_PRIVATE_HANDLE
{
	LIBGBSHM_OBJ *pgbshm_obj;		//必须的
	LIBVSSHM_OBJ *pvsshm_obj;		//必须的
	LIBASSHM_OBJ *passhm_obj;   //必须的
	LIBAUDIO_OBJ *paudio_obj;   //必须的
	struct pollfd polltimeout[RTSP_TRANFER_POLL_COUNT_MAX];	//必须的
	video_enc_param videoencparam;//视频参数
	int socket_fd;	//TCP or UDP socket
	int audio_socket_fd;	//TCP or UDP socket
    int socket_fd1; //测试用	
	int unix_fd;	//必须的
	int active_fdcount;	//必须的
	int I_frame_sended_flag;  //I帧发送标志
	unsigned short v_sequence_number; //视频序列号
	unsigned short a_sequence_number; //音频序列号
	RTSP_STATE current_state; //状态
	INFO_FROM_AVENC  datainfo; //数据信息
	AUDIO_INFO_FROM_AVENC  audatainfo;
    INFO_FROM_CLIENTPC   cmdinfo; //命令信息
    RTP_HEAD_INFO     rtp_head_info;//rtp头信息
	struct sockaddr_in sockaddr; //地址结构体
	struct sockaddr_in audio_sockaddr; //地址结构体
	struct sockaddr_in sockaddr1; //测试用
	FILE *fp_out;
	FILE *fp_out1;
}RTSP_TRANFER_PRIVATE_HANDLE;

//extern RTSP_TRANFER_PRIVATE_HANDLE global_rtsp_transfer_private_handle;

succeed_type rtsp_tranfer_unix(RTSP_TRANFER_PRIVATE_HANDLE *phandle);
succeed_type rtsp_transfer_socket(RTSP_TRANFER_PRIVATE_HANDLE *phandle);
succeed_type rtsp_transfer_audio_socket(RTSP_TRANFER_PRIVATE_HANDLE *phandle);
unsigned short Recv_Buf_Parse(char *RecvBuf,unsigned short *CmdType);
unsigned char JudgeRecvData(char *RecvBuf,unsigned char *PayLoad);
succeed_type rtsp_tranfser_send_audio(RTSP_TRANFER_PRIVATE_HANDLE *phandle);
succeed_type rtsp_tranfser_send_video(RTSP_TRANFER_PRIVATE_HANDLE *phandle);
succeed_type rtsp_transfer_send_I_frame(RTSP_TRANFER_PRIVATE_HANDLE *phandle);
succeed_type rtsp_transfer_send_P_frame(RTSP_TRANFER_PRIVATE_HANDLE *phandle);
void get_data_info(RTSP_TRANFER_PRIVATE_HANDLE *phandle,LIBVSSHM_UINT_DATA_I *prtsp_transfer_vsshm_unit_buffer);
void get_audiodata_info(RTSP_TRANFER_PRIVATE_HANDLE *phandle,LIBASSHM_UINT_DATA *prtsp_transfer_asshm_unit_buffer);
succeed_type rtsp_transfer_unix_control_net_stop(RTSP_TRANFER_PRIVATE_HANDLE *phandle);
	
#define USE_NEW_AUDIO
#ifdef __cplusplus
}
#endif
#endif
