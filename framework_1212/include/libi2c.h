#ifndef	_LIB_I2C_H 
#define	_LIB_I2C_H

#ifdef __cplusplus
extern "C"{
#endif
#include "parameter.h"
#include "parametertype.hin"

typedef struct _LIBI2C_OBJECT
{
	int fd;
}LIBI2C_OBJ;

LIBI2C_OBJ *I2C_INIT(unsigned int devaddr);
succeed_type I2C_SETREG( LIBI2C_OBJ *phandle, unsigned int regindex, unsigned int regvalue);
succeed_type I2C_GETREG( LIBI2C_OBJ *phandle, unsigned int regindex, unsigned int *regvalue);

#ifdef __cplusplus
extern }
#endif

#endif
