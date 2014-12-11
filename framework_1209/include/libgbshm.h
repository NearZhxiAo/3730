#ifndef	_LIB_GBSHM_H 
#define	_LIB_GBSHM_H

#ifdef __cplusplus
extern "C"{
#endif
#include "parameter.h"
#include "parametertype.hin"

typedef struct _LIBGBSHM_OBJ
{
    int gbshm_fd;
    unsigned int gbshm_mmap;
    int countnum;
    int reserved;
}LIBGBSHM_OBJ;

typedef struct LIBGBSHM_OBJ * Gbshm_Handle;
/* GBSHM_INIT
 GBSHM初始化函数，返回为LIBGBSHM_OBJ句柄*/
LIBGBSHM_OBJ *GBSHM_INIT();
succeed_type GBSHM_DESTROY(LIBGBSHM_OBJ *phandle);

/* GBSHM_PARA_GET
  获取参数对应的参数值
  ptrhandle_gbshm，句柄
  cmd，参数命令号，对应于parameter.def文件中的参数定义:PARA_CMD_SYSTEM_XXX
  ptr，存放获取到的参数值的指针位置
  length，长度信息
 */
succeed_type GBSHM_PARA_GET(LIBGBSHM_OBJ *ptrhandle_gbshm, int cmd, void * ptr, int length);

/* GBSHM_PARA_SET
  获取参数对应的参数值
  ptrhandle_gbshm，句柄
  cmd，参数命令号，对应于parameter.def文件中的参数定义:PARA_CMD_SYSTEM_XXX
  ptr, 待设置的参数对应的值的指针
  length，长度信息
 */
succeed_type GBSHM_PARA_SET(LIBGBSHM_OBJ *ptrhandle_gbshm, int cmd, void * ptr, int length);

#ifdef __cplusplus
}
#endif
#endif  
