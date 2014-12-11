#ifndef _LIBVSSHM_H_
#define _LIBVSSHM_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <semaphore.h>

//vsshm 视频帧存储单元大小
#define VSSHM_UINT_LEN_I 100*1024  //I帧
#define VSSHM_UINT_LEN_P 40*1024  //P帧

//vsshm 视频帧存储单元个数  当采样频率为10时 I(num)/P(num)= 1/9  大小为840K<1M
#define VSSHM_UINT_NUM_I 4  //I帧  4*100*1024 = 300K
#define VSSHM_UINT_NUM_P 15  //P帧  15*40*1024 = 600K
#define VSSHM_UINT_NUM VSSHM_UINT_NUM_I+VSSHM_UINT_NUM_P 

//vsshm 信号量的设置2 --暂时提供rtsp和stoarge
#define VSSHM_SEM_MAX 2

//vsshm 初始化控制magic值
#define VSSHM_MAGIC_NUM 0x16888861
#define VSSHM_MAGIC_CLR 0x00000000

//vsshm 存储视频数据的帧格式
typedef enum _LIBVSSHM_FRAME_TYPE 
{
	FRAME_TYPE_VIDEO_START,
	FRAME_TYPE_VIDEO_H264,
	FRAME_TYPE_VIDEO_MP4,
	FRAME_TYPE_VIDEO_JPEG,
	FRAME_TYPE_VIDEO_END,
}LIBVSSHM_FRAME_TYPE;

//vsshm 存储视频数据的i/p标识
typedef enum _LIBVSSHM_I_P_FLAG
{
	I_P_FLAG_START = -1,
	I_P_FLAG_P,
	I_P_FLAG_I,
	I_P_FLAG_END,
}LIBVSSHM_I_P_FLAG;

//vsshm 视频数据存储的头
typedef struct _LIBVSSHM_UINT_HEADER
{
	LIBVSSHM_FRAME_TYPE frame_type;
	LIBVSSHM_I_P_FLAG i_p_flag;
	long long timestamp;
	long data_len;
	int reserved;
}LIBVSSHM_UINT_HEADER;

//vsshm 视频存储I帧的数据
typedef struct _LIBVSSHM_UINT_DATA_I
{	
	LIBVSSHM_UINT_HEADER vsshm_header;
	unsigned char vsshm_context[VSSHM_UINT_LEN_I];
}LIBVSSHM_UINT_DATA_I;

//vsshm 内存I帧单元划分
typedef struct _LIBVSSHM_UINT_STRUCT_I
{
	sem_t vsshm_sem_t;
	LIBVSSHM_UINT_DATA_I vsshm_data;
}LIBVSSHM_UINT_STRUCT_I;

//vsshm 视频存储P帧的数据
typedef struct _LIBVSSHM_UINT_DATA_P
{	
	LIBVSSHM_UINT_HEADER vsshm_header;
	unsigned char vsshm_context[VSSHM_UINT_LEN_P];
}LIBVSSHM_UINT_DATA_P;

//vsshm 内存P帧单元划分
typedef struct _LIBVSSHM_UINT_STRUCT_P
{
	sem_t vsshm_sem_t;
	LIBVSSHM_UINT_DATA_P vsshm_data;
}LIBVSSHM_UINT_STRUCT_P;

//vsshm 操作句柄
typedef struct _LIBVSSHM_OBJ
{
	int vsshm_fd;
	unsigned int vsshm_mmap;
	unsigned int vsshm_phyaddr;
	int vsshm_size;
	int reserved;
}LIBVSSHM_OBJ;

//FUNC init for vsshm
LIBVSSHM_OBJ *VSSHM_INIT();

//IN  phandle VSSHM_INIT returned
//FUNC destroy vsshm
succeed_type VSSHM_DESTROY(LIBVSSHM_OBJ *phandle);

//IN  phandle VSSHM_INIT returned
//IN  vsshm_seq VSSHM_INIT buffer number
//IN  i_p_flag i-0 p-1
//IN  video_type h264
//IN  ptr    put original data ( vedio data/audio data)
//IN  length put original data len
//FUNC PUT the ptr data in the buffer[vsshm_seq]
succeed_type VSSHM_PUT(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq,\
		LIBVSSHM_FRAME_TYPE frame_type, void *ptr, int length);

//IN  phandle VSSHM_INIT return
//OUT pvsshm_seq VSSHM_INIT Available buffer number
//OUT ptr_sem the VSSHM buffer sem_t 0/1/2...
//FUNC get the sem_t and vsshm_seq in the buffer[vsshm_seq]
succeed_type VSSHM_GET_SEQ(LIBVSSHM_OBJ *phandle, LIBVSSHM_I_P_FLAG i_p_flag,\
		unsigned int* pvsshm_seq, int* ptr_sem);

//IN  phandle VSSHM_INIT return
//IN  vsshm_seq VSSHM_INIT buffer number
//OUT ptr_data the VSSHM buffer data 
//FUNC get the data in the buffer[vsshm_seq]
succeed_type VSSHM_GET_DATA(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq, void *ptr_data);

//IN  phandle VSSHM_INIT return
//IN  vsshm_seq VSSHM_INIT buffer number
//FUNC waittry the sem_t in the buffer[vsshm_seq]
succeed_type VSSHM_GET_COMPLETE(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq);

//临时提供为时间戳的获取
extern void libvsshm_generatetimestamp(long long *TimeStamp);

#ifdef __cplusplus
}
#endif

#endif

