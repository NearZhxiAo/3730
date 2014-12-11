/************************************************************************
 * Copyright (C), 2011, liangzhang@xidian.edu.cn
 * FileName:   libgbshm.c
 * Description:
 * Date:	2011-03-16
 * Version:	V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libgbshm.h"
#include "parameter_offset.h"

/******************************************** 
 *    Function    : Gbshm_init
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/

static LIBGBSHM_OBJ* libgbshm_init()
{
    int i = 0;
	//struct gbshm_mutex *plocalmutex = NULL;
    LIBGBSHM_OBJ *phandle_gbshm = malloc(sizeof(LIBGBSHM_OBJ));
    if (phandle_gbshm == NULL)
    {
        SYS_ERROR("malloc phandle failed.\r\n");
        return NULL;
    }
    phandle_gbshm->gbshm_fd = open("/dev/gbshm", O_RDWR);
    if ( phandle_gbshm->gbshm_fd <= 0 )
    {
        SYS_ERROR("open fd failed.\r\n");
        free( phandle_gbshm );
        return NULL;
    }

    SYS_INFO("libgbshm_init. size = 0x%x\r\n", SHAREMEM_PHYS_SIZE);

    phandle_gbshm->gbshm_mmap = 0;

    phandle_gbshm->gbshm_mmap = (unsigned int)mmap(0, SHAREMEM_PHYS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, phandle_gbshm->gbshm_fd, SHAREMEM_PHYS_ADDR );
//    phandle_gbshm->gbshm_mmap = (unsigned int)mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, phandle_gbshm->gbshm_fd, 0x88000000);
    SYS_INFO("after libgbshm_init. shm addr 0x%x \r\n",phandle_gbshm->gbshm_mmap );

    if ( phandle_gbshm->gbshm_mmap <= 0 )
    {
        free( phandle_gbshm );
        SYS_ERROR("mmap failed.\r\n");
        return NULL;
    }
    phandle_gbshm->countnum++ ;
    //init mutex
//	plocalmutex = phandle_gbshm->gbshm_mmap;
#if 0
    //if (0 != pthread_mutex_init((pthread_mutex_t *)(phandle_gbshm->gbshm_mmap), NULL)) 
    if (0 != pthread_mutex_init(&(plocalmutex->mutex), NULL)) 
	{
        SYS_ERROR("gbshm mutex init failed\n");
        return NULL;
    }
#endif
    return phandle_gbshm;
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
static succeed_type libgbshm_destroy(LIBGBSHM_OBJ *phandle)
{
    if (phandle == NULL)
    {
        SYS_ERROR("phandle_gbshm is NULL.\r\n");
        return succeed_type_failed;
    }
    free(phandle);
    return succeed_type_succeed;
}


/******************************************** 
 *    Function    : Gbshm_paramGet
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libgbshm_para_get(LIBGBSHM_OBJ *phandle_gbshm, int offset, void * ptr, int length)
{
    int addr = 0;
    if (ptr ==NULL || length == 0 || phandle_gbshm == NULL)
    {
        SYS_ERROR("phandle_gbshm is NULL.\r\n");
        return succeed_type_failed;
    }
    addr =  (phandle_gbshm->gbshm_mmap + offset);

    memcpy(ptr, (void *)addr, length);

    return succeed_type_succeed;
}


/******************************************** 
 *    Function    : Gbshm_paramSet
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libgbshm_para_set(LIBGBSHM_OBJ *phandle_gbshm, int offset, void * ptr, int length)
{
    int addr = 0;
	//struct gbshm_mutex *plocalmutex = NULL;
	printf("enter libgbshm_para_set()\n\r");
   // plocalmutex =  (struct gbshm_mutex *)(phandle_gbshm->gbshm_mmap);
	printf("pthread_mutex_t * pmutex init success.\r\n");
    if(ptr == NULL || length == 0||phandle_gbshm == NULL)
    {
        SYS_ERROR("phandle_gbshm is NULL or other parameter is not valid. \r\n");
        return succeed_type_failed;
    }
    addr = phandle_gbshm->gbshm_mmap + offset;
   // pthread_mutex_lock(&(plocalmutex->mutex));
	memcpy(addr, ptr, length);
   // pthread_mutex_unlock(&(plocalmutex->mutex));

    return succeed_type_succeed;
}

LIBGBSHM_OBJ *GBSHM_INIT()
{
    return libgbshm_init();
}
succeed_type GBSHM_DESTROY(LIBGBSHM_OBJ *phandle)
{
    return libgbshm_destroy(phandle);
}


succeed_type GBSHM_PARA_GET(LIBGBSHM_OBJ *phandle_gbshm, int cmd, void * ptr, int length)
{
    return libgbshm_para_get(phandle_gbshm, parameter_offset(cmd), ptr, length);
}

succeed_type GBSHM_PARA_SET(LIBGBSHM_OBJ *phandle_gbshm, int cmd, void * ptr, int length)
{
    return libgbshm_para_set(phandle_gbshm, parameter_offset(cmd), ptr, length);
}

succeed_type GBSHM_PARA_FLASH_DODEFAULT_SET(LIBGBSHM_OBJ *phandle_gbshm, int cmd, void * ptr, int length)
{
    return libgbshm_para_set(phandle_gbshm, cmd, ptr, length);
}

