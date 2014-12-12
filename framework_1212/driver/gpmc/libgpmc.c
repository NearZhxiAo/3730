/************************************************************************
 * Copyright (C), 2011, liangzhang@xidian.edu.cn
 * FileName:   libgbshm.c
 * Description:
 * Date:	2011-03-16
 * Version:	V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libgpmc.h"

/******************************************** 
 *    Function    : Gbshm_init
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static LIBGPMC_OBJ* libgpmc_init()
{
    int i = 0;
    LIBGPMC_OBJ *phandle_gpmc = malloc(sizeof(LIBGPMC_OBJ));
    if (phandle_gpmc == NULL)
    {
        SYS_ERROR("malloc phandle failed.\r\n");
        return NULL;
    }

    phandle_gpmc->gpmc_fd = open("/dev/gpmc", O_RDWR);
    if ( phandle_gpmc->gpmc_fd <= 0 )
    {
        SYS_ERROR("open fd failed.\r\n");
        free( phandle_gpmc );
        return NULL;
    }

    return phandle_gpmc;
}


/******************************************** 
 *    Function    : libgbshm_destroy
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libgpmc_destroy(LIBGPMC_OBJ *phandle)
{
    if (phandle == NULL)
    {
        SYS_ERROR("phandle_gbshm is NULL.\r\n");
        return succeed_type_failed;
    }
    free(phandle);
    return succeed_type_succeed;
}

LIBGPMC_OBJ *GPMC_INIT()
{
    return libgpmc_init();
}
succeed_type GPMC_DESTROY(LIBGPMC_OBJ *phandle)
{
    return libgpmc_destroy(phandle);
}


