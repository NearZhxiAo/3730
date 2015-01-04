#ifndef	_FLASH_PRIVATE_H__
#define _FLASH_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "libad.h"
#include "libgbshm.h"
#include "libunix.h"
#include "libsignal.h"
#include "parameter.h"
//#include "parameter_offset.h"
#include "parameter_offset.default"

#define PARA_FIRST_FILE_NAME 	"paramsave_1.p"
#define PARA_SEC_FILE_NAME	"paramsave_2.p"


#define FILEFLAG_VALID		0x12345678
#define FILEFLAG_INVALID	0xffffffff

#define FILENAME_LENGTH		256

//暂时定义 待从para确定后修改
#define FLASH_MOUNTPOINT_1 "/mnt/nand/"
#define FLASH_MOUNTPOINT_2 "/mnt/nand/"
//#define FLASH_MOUNTPOINT_1 "/var/flash/"
//#define FLASH_MOUNTPOINT_2 "/var/flash/"


#define FLASH_POLL_COUNT_MAX	2

//对flash模块操作标识结构体
typedef enum _FLASH_OPT_FLAG
{
    FLASH_OPT_START,
    FLASH_OPT_SAVE,
    FLASH_OPT_CLEAR,
    FLASH_OPT_END,
}FLASH_OPT_FLAG;

typedef struct _FLASH_PRIVATE_HANDLE
{
    LIBGBSHM_OBJ  *gbshm_handle;		//gbshm句柄
    struct pollfd polltimeout[FLASH_POLL_COUNT_MAX];	//poll加入的描述符集
    int unix_fd;	//unix域套接字
    int active_fdcount;	//活动的套接字数 <= FLASH_POLL_COUNT_MAX
    FLASH_OPT_FLAG opt_flag;   //flash管理的操作标识
    int savesize;   //从flash将参数拷贝到共享缓冲的数据大小
    pthread_mutex_t mutex;   //对opt_flag 操作的互斥量
}FLASH_PRIVATE_HANDLE;//flsh模块的全局句柄

#ifdef __cplusplus
}
#endif
#endif
