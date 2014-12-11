#ifndef	_LIB_GPMC_H 
#define	_LIB_GPMC_H

#ifdef __cplusplus
extern "C"{
#endif
#include "parameter.h"
#include "parametertype.hin"

typedef struct _LIBGPMC_OBJ
{
    int gpmc_fd;
}LIBGPMC_OBJ;

LIBGPMC_OBJ *GPMC_INIT();
succeed_type GPMC_DESTROY(LIBGPMC_OBJ *phandle);

#ifdef __cplusplus
}
#endif
#endif  
