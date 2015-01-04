#include "system_debug.h"
#include "system_includes.h"
#include "libi2c.h"
LIBI2C_OBJ *I2C_INIT(unsigned int devaddr)
{
	LIBI2C_OBJ *phandle = malloc(sizeof(LIBI2C_OBJ));

	if ( phandle == NULL )
	{
		printf("alloc failed.\r\n");
	}
	else
	{
		if ( DRV_i2cOpen( phandle, devaddr) < 0  )
		{
			free(phandle);
			return NULL;
		}
	}
	return phandle;
}

succeed_type I2C_DESTROY(LIBI2C_OBJ *phandle)
{

	if ( phandle == NULL )
	{
		return succeed_type_failed;
	}
	else
	{
		if ( DRV_i2cClose( phandle) < 0  )
		{
			return succeed_type_failed;
		}
	}
	free(phandle);
	return succeed_type_succeed;;
}


succeed_type I2C_SETREG( LIBI2C_OBJ *phandle, unsigned int regindex, unsigned int regvalue)
{
	if ( phandle == NULL )
	{
		return succeed_type_failed;
	}
	if ( DRV_i2cWrite8( phandle, &regindex, &regvalue, 1) < 0 )	
	{
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}
succeed_type I2C_SETREG16( LIBI2C_OBJ *phandle, unsigned int regindex, unsigned int regvalue)
{
	if ( phandle == NULL )
	{
		return succeed_type_failed;
	}
	if ( DRV_i2cWrite16( phandle, &regindex, &regvalue, 1) < 0 )	
	{
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}


succeed_type I2C_GETREG( LIBI2C_OBJ *phandle, unsigned int regindex, unsigned int *regvalue)
{
	if ( phandle == NULL )
	{
		return succeed_type_failed;
	}
	if ( DRV_i2cRead8( phandle, &regindex, regvalue, 1) < 0 )	
	{
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}

succeed_type I2C_GETREG16( LIBI2C_OBJ *phandle, unsigned int regindex, unsigned int *regvalue)
{
	if ( phandle == NULL )
	{
		return succeed_type_failed;
	}
	if ( DRV_i2cRead16( phandle, &regindex, regvalue, 2) < 0 )	
	{
	
		return succeed_type_failed;
	}
	 
	return succeed_type_succeed;
}
