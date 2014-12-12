#ifndef	_LIB_VIDCTL_H 
#define	_LIB_VIDCTL_H

#ifdef __cplusplus
extern "C"{
#endif
#include "parameter.h"
#include "parametertype.hin"

#define VID_DISABLE       2 

typedef struct _LIBVIDCTL_OBJ
{
	int vidctl_fd;
}LIBVIDCTL_OBJ;

typedef enum _enablemode
{
	ENABLE_PAL,
	ENABLE_NTSC
}enablemode;

typedef enum _enableresol
{
	ENABLE_RESOL_D1,
	ENABLE_RESOL_CIF,
	DISABLE_RSZ
}enableresol;

/* GBSHM_INIT
   GBSHM初始化函数，返回为LIBGBSHM_OBJ句柄*/
LIBVIDCTL_OBJ *VIDCTL_INIT();
succeed_type VIDCTL_DESTROY(LIBVIDCTL_OBJ *phandle);
succeed_type VIDCTL_DISABLE(LIBVIDCTL_OBJ *phandle);

succeed_type VIDCTL_ENABLE(LIBVIDCTL_OBJ *phandle, enablemode mode, enableresol resol);

#ifdef __cplusplus
}
#endif
#endif  
