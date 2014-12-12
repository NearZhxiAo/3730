/*******************************************************************************
* COPYRIGHT
********************************************************************************
/*
  * =====================================================================================
  *
  *       Filename:  storage_private.h
  *
  *    Description:
  *
  *        Version:
  *        Created:
  *       Revision:
  *       Compiler:
  *
  *         Author:
  *        Company:
  *
  * =====================================================================================
*/
/******************************** Include Code Start ******************************/
#ifndef	_STORAGE_PRIVATE_H__
#define _STORAGE_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "sys/vfs.h"
#include "libad.h"
#include "libgbshm.h"
#include "libunix.h"
#include "libvsshm.h"
#include "libasshm.h"
#include "libsignal.h"
#include "libsk.h"
#include "storage_common.h"

#define STORAGE_POLL_COUNT_MAX	2  //storage poll事件的最大值

typedef struct _STORAGE_PRIVATE_HANDLE
{
    LIBGBSHM_OBJ *pgbshm_obj;		//gbshm 对象句柄
    LIBVSSHM_OBJ *pvsshm_obj;		//vsshm 对象句柄
    LIBASSHM_OBJ *passhm_obj;		//vsshm 对象句柄
    struct pollfd polltimeout[STORAGE_POLL_COUNT_MAX];	//poll 描述符集
    int unix_fd;	//unix域套接字描述符
    int active_fdcount;	//poll 描述符集的活动数
    alert_event_type enalert_event_type; //告警事件类型
	//int sockfd; // 
    unsigned char afile_name[50]; //存储的文件名
	FILE *store_fd;               //存储文件fd
	int sdcard_checked_if;        //sd是否检测到的标志 1：有 0：无
	pthread_mutex_t g_mutex_t;    //全局互斥信号量
	int IsStorageSysQuit;         //storage 程序推出标志符
	int IsStorageData_Available;  //存储数据是否可用标志
	LIBVSSHM_UINT_DATA_I* pstorage_video_data; //实际视频存储数据首地址
	LIBASSHM_UINT_DATA* pstorage_audio_data; //实际音频数据存储首地址
	long long storage_store_duration;     //存储周期长度
}STORAGE_PRIVATE_HANDLE;  //storage 全局句柄类型定义

extern STORAGE_PRIVATE_HANDLE global_storage_private_handle;  //storage 全局句柄定义 外部声明
//succeed_type storage_unix(STORAGE_PRIVATE_HANDLE *phandle);

#ifdef __cplusplus
}
#endif
#endif

/******************************** Include Code End ******************************/
