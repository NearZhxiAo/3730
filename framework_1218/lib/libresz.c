/************************************************************************
 * FileName:   libresz.c
 * Description:
 * Date:        2011-04-18
 * Version:     V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libresz.h"

static BufferPool *Malloc(int num, int size )
{
	int i = 0;
	BufferPool *buffer_pool;
	buffer_pool = (BufferPool *)malloc(sizeof(BufferPool));

	if(buffer_pool == NULL)
	{
		printf("buffer_pool failed..\r\n");
	}
	printf("in function malloc..\r\n");
	buffer_pool->Num    = num;
	buffer_pool->buffer = (Buffer *)malloc(sizeof(Buffer)*num);
	if(buffer_pool->buffer == NULL)
	{
		printf("buffer_pool->buffer failed..\r\n");
	}

	printf("after buffer malloc..\r\n");
	for(i = 0; i<num; i++)
	{
		buffer_pool->buffer[i].virtAddr = (unsigned char *)OSA_cmemAlloc(size, 32);
		if(buffer_pool->buffer[i].virtAddr ==NULL)
		{
			printf("cmemalloc error..\r\n\r\n");
		}
		memset(buffer_pool->buffer[i].virtAddr, 0x00, size);
		buffer_pool->buffer[i].physAddr = (unsigned char *)OSA_cmemGetPhysAddr(buffer_pool->buffer[i].virtAddr);
		if(buffer_pool->buffer[i].physAddr ==NULL)
		{
			printf("getphysaddr error..\r\n\r\n");
		} 
	}
	return buffer_pool;
}

static BufferPool *RSZ_MALLOC(int num, int size)
{
	return Malloc(num, size);
}

static succeed_type Free(BufferPool *buffer_pool)
{
	int i = 0;
	int num = 0;
	if(buffer_pool == NULL)
	{
		return succeed_type_failed;
	}
	num = buffer_pool->Num;
	for(i = 0; i<num; i++)
	{
		OSA_cmemFree(buffer_pool->buffer[i].virtAddr);
	}
	free((void *)(buffer_pool->buffer));
	free(buffer_pool);
	return succeed_type_succeed;
}


static succeed_type RSZ_FREE(BufferPool *buffer_pool)
{
	return Free(buffer_pool);
}

/******************************************** 
 *    Function    : libresz_init
 *    Author      : 
 *    Date        : 2011-04-18 
 *    Description : 
 *    Para        : 
 *    Ret         : OSA_SOK       -- succeed
 *                  OSA_EFAIL     -- failed
 * **********************************************/

static succeed_type libresz_init()
{
	int status;
	status = DRV_init();
	if(status != OSA_SOK)
	{
		SYS_ERROR("DRV_init failed.\n");
		return status;
	}
	return status;
}

/******************************************** 
 *    Function    : libresz_rszrun
 *    Author      : 
 *    Date        : 2011-04-18 
 *    Description : 
 *    Para        : 
 *    Ret         : OSA_SOK       -- succeed
 *                  OSA_EFAIL     -- failed
 * **********************************************/

//include the bufinit and rsz configuration

static succeed_type libresz_rszrun( unsigned int inWidth, unsigned int inHeight, unsigned int *outphyaddr,unsigned int outphynum )
{
	int realnum = 0;
	int ret = -1;
	if (ret = rsz_BufInit(inWidth, inHeight, outphyaddr, outphynum, &realnum) != 0)
	{
		printf("\r\n 444555666 rsz_bufinit return = %d..\r\n\r\n", ret);
		printf("rsz_BufInit failed.....\r\n");
		return succeed_type_failed;
	}
	DRV_resz_EnableInt();
	rsz_HwsetParam_oneshotconfig(0); 
	if(rsz_HwsetParam(inWidth, inHeight, outphyaddr[0]) != 0)
	{
		SYS_ERROR("rsz configuration failed.\n");
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libresz_ddrrun
 *    Author      : 
 *    Date        : 2011-04-18 
 *    Description : 
 *    Para        : 
 *    Ret         : OSA_SOK       -- succeed
 *                  OSA_EFAIL     -- failed
 * **********************************************/

static succeed_type libresz_ddrrun(unsigned short Width, unsigned short Height, unsigned char *inPhysAddr, SRC_MODE src_mode )
{
	int status;
	status = rsz_DDRsetParam(Width, Height, inPhysAddr, src_mode);
	if(status != OSA_SOK)
	{
		SYS_ERROR("ddr configuration failed.\n");
		return status;
	}
	return status;
}

/******************************************** 
 *    Function    : libresz_exit
 *    Author      : 
 *    Date        : 2011-04-18 
 *    Description : 
 *    Para        : 
 *    Ret         : OSA_SOK       -- succeed
 *                  OSA_EFAIL     -- failed
 * **********************************************/

static succeed_type libresz_exit()
{
	int status;
	status = DRV_exit();
	if(status != OSA_SOK)
	{
		SYS_ERROR("DRV_init failed.\n");
		return status;
	}

	return status;
}


int libresz_getfd()
{
	int fd;
	fd = DRV_GetRszFd();
	return fd;
}

int RSZ_GETFD()
{
	return libresz_getfd();
}

BufferPool *RSZ_INIT(int num, int size)
{
	BufferPool *p;
	libresz_init();
	p = RSZ_MALLOC(num, size);
	return p;
}

succeed_type RSZ_EXIT(BufferPool *buffer_pool)
{
	libresz_exit();  
	return RSZ_FREE(buffer_pool);
}

succeed_type RSZ_RSZRUN( unsigned int inWidth, unsigned int inHeight, unsigned int *outphyaddr,unsigned int outphynum )
{
	return libresz_rszrun( inWidth, inHeight, outphyaddr,outphynum );
}

succeed_type RSZ_DDRRUN(unsigned short Width, unsigned short Height, unsigned char *inPhysAddr, SRC_MODE src_mode)
{
	return libresz_ddrrun(Width, Height, inPhysAddr, src_mode); 
}


