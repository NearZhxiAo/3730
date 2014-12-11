/************************************************************************
 * Copyright (C), 2011, liangzhang@xidian.edu.cn
 * FileName:   libasshm.c
 * Description:
 * Date:	2011-03-16
 * Version:	V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libasshm.h"
#include "parameter_offset.h"


/******************************************** 
 *    Function    : GenerateTimeStamp
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : void 
 *
 * **********************************************/
void libasshm_generatetimestamp(long long *TimeStamp)
{
      time_t CurTime_t;
      struct tm *pCurTm_t;
      time(&CurTime_t);
      pCurTm_t = localtime(&CurTime_t);
      *TimeStamp = (long long)( ((long long)(1970+pCurTm_t->tm_year)*10*10*10*10*10*10*10*10*10*10*10) \
				  +((long long)(1+pCurTm_t->tm_mon)*10*10*10*10*10*10*10*10*10) \
				  +((long long)(pCurTm_t->tm_mday)*10*10*10*10*10*10*10) \
				  +((long long)(pCurTm_t->tm_hour)*10*10*10*10*10) +((long long)(pCurTm_t->tm_min)*10*10*10)\
				  +((long long)(pCurTm_t->tm_sec)) );
}


/******************************************** 
 *    Function    : libasshm_init
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static LIBASSHM_OBJ *libasshm_init()
{
	unsigned int iLoop = 0;
	unsigned int mmap_offset = 0;
	unsigned int asshm_offset = 0;
	unsigned int asshm_addr = 0;
	int asshm_conn_num = 0;

	LIBASSHM_OBJ *phandle = malloc(sizeof(LIBASSHM_OBJ));
	if (phandle == NULL)
	{
		SYS_ERROR("malloc phandle failed.\r\n");
		return NULL;
	}
	phandle->asshm_fd = open("/dev/asshm", O_RDWR);
	if ( phandle->asshm_fd <= 0 )
	{
		SYS_ERROR("open fd=%d failed.\r\n",phandle->asshm_fd);
		free( phandle );
		return NULL;
	}

	SYS_INFO("libasshm_init. \r\n");

	phandle->asshm_mmap = 0;
	SYS_INFO("mmap init to 0.\r\n");

	phandle->asshm_mmap = (unsigned int)mmap(0, ASSHMMEM_PHYS_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, phandle->asshm_fd, ASSHMMEM_PHYS_ADDR);
	phandle->asshm_phyaddr = ASSHMMEM_PHYS_ADDR;
	if ( phandle->asshm_mmap <= 0 )
	{
		free( phandle );
		SYS_ERROR("mmap failed.\r\n");
		return NULL;
	}
	SYS_INFO("after libasshm_init. asshm addr = 0x%x \r\n",phandle->asshm_mmap);

	asshm_offset = sizeof(LIBASSHM_UINT_STRUCT);
	mmap_offset = sizeof(pthread_mutex_t) + 2 * sizeof(int);
	asshm_addr = phandle->asshm_mmap + mmap_offset;
	phandle->asshm_size = (ASSHMMEM_PHYS_SIZE-mmap_offset) / sizeof(LIBASSHM_UINT_STRUCT);

	pthread_mutex_t * pmutex =  (pthread_mutex_t *)(phandle->asshm_mmap);
	//init mutex
	if(ASSHM_MAGIC_NUM == *(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t)))
	{
			asshm_conn_num = *(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t) + sizeof(int));
			asshm_conn_num++;
			pthread_mutex_lock(pmutex);
			*(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t) + sizeof(int)) = asshm_conn_num;
			pthread_mutex_unlock(pmutex);
			SYS_INFO("asshm not first init!!!asshm_conn_num= %d\r\n",asshm_conn_num);
	}
	else //first time invoke
	{ 
		SYS_INFO("asshm first init!!!\r\n");
		if(0 != pthread_mutex_init( (pthread_mutex_t *)(phandle->asshm_mmap) ,NULL))
		{
			SYS_ERROR("asshm mutex init failed\n");
			return NULL;
		}
		for(iLoop = 0; iLoop < phandle->asshm_size; iLoop++)	
		{
			pthread_mutex_lock(pmutex);
			sem_init( (sem_t*)(asshm_addr+iLoop*asshm_offset), 0, 0 );
			pthread_mutex_unlock(pmutex);
		}
		pthread_mutex_lock(pmutex);
		*(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t)) = ASSHM_MAGIC_NUM;
		*(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t) + sizeof(int)) = 0x01;
		pthread_mutex_unlock(pmutex);
	}

	// init all the sem when the LIBASSHM_INIT is firstly invoked

	SYS_INFO("after libasshm_init. asshm size = 0x%x \r\n",phandle->asshm_size );
	return phandle;
}
/******************************************** 
 *    Function    : libasshm_destroy
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libasshm_destroy(LIBASSHM_OBJ *phandle)
{
	int asshm_conn_num = 0;
	if (phandle == NULL)
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}

	pthread_mutex_t * pmutex =  (pthread_mutex_t *)(phandle->asshm_mmap);
	asshm_conn_num = *(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t) + sizeof(int));
	if(0x01 == asshm_conn_num) //last invoke
	{
		close(phandle->asshm_fd);	
		pthread_mutex_lock(pmutex);
		*(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t)) = ASSHM_MAGIC_CLR;
		pthread_mutex_unlock(pmutex);
		munmap((void*)phandle->asshm_mmap, ASSHMMEM_PHYS_SIZE);
	}
	else if(0x01 < asshm_conn_num)
	{
		asshm_conn_num--;
		pthread_mutex_lock(pmutex);
		*(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t) + sizeof(int)) = asshm_conn_num;
		pthread_mutex_unlock(pmutex);
	}
	else
	{
		SYS_INFO("destroy do nothing");
		return succeed_type_succeed;
	}
	free(phandle);
 	phandle = NULL;
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libasshm_magic_destroy
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libasshm_magic_destroy(LIBASSHM_OBJ *phandle)
{
	int asshm_conn_num = 0;
	if (phandle == NULL)
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}

	pthread_mutex_t * pmutex =  (pthread_mutex_t *)(phandle->asshm_mmap);
	{
		close(phandle->asshm_fd);	
		pthread_mutex_lock(pmutex);
		*(int*)(phandle->asshm_mmap + sizeof(pthread_mutex_t)) = ASSHM_MAGIC_CLR;
		pthread_mutex_unlock(pmutex);
		munmap((void*)phandle->asshm_mmap, ASSHMMEM_PHYS_SIZE);
	}
	free(phandle);
 	phandle = NULL;
	return succeed_type_succeed;
}


static succeed_type libasshm_get_complete(LIBASSHM_OBJ *phandle, int asshm_seq)
{
	unsigned int mmap_offset = 0;
	unsigned int asshm_offset = 0;
	if ( phandle == NULL )
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	if(asshm_seq > phandle->asshm_size)
	{
		SYS_ERROR("The asshm_seq is not valid. \r\n");
		return succeed_type_failed;
	}
	mmap_offset = sizeof(pthread_mutex_t) + 2 * sizeof(int);
	asshm_offset = (unsigned int )asshm_seq * sizeof(LIBASSHM_UINT_STRUCT);
	sem_trywait((sem_t*)(phandle->asshm_mmap+mmap_offset+asshm_offset));
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libasshm_get_sem
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libasshm_get_sem(LIBASSHM_OBJ *phandle, int asshm_seq, int *ptr_sem)
{
	unsigned int mmap_offset = 0;
	unsigned int asshm_addr = 0;
	unsigned int asshm_offset = 0;
	if (ptr_sem == NULL || phandle == NULL)
	{
		SYS_ERROR("phandle is NULL or other parameter is not valid. \r\n");
		return succeed_type_failed;
	}
	if((unsigned int)asshm_seq > phandle->asshm_size)
	{
		SYS_ERROR("The asshm_seq is not valid. \r\n");
		return succeed_type_failed;
	}

	mmap_offset = sizeof(pthread_mutex_t) + 2 * sizeof(int);
	asshm_offset = (unsigned int)(asshm_seq) * sizeof(LIBASSHM_UINT_STRUCT);
	asshm_addr =  (phandle->asshm_mmap + mmap_offset + asshm_offset);

	if(0 != sem_getvalue((sem_t *)asshm_addr, ptr_sem))	
	{
		SYS_ERROR("\n\r Sem_getvalue failed .\r\n");
		return succeed_type_succeed;
	}	
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libasshm_get_data
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libasshm_get_data(LIBASSHM_OBJ *phandle, int asshm_seq, void *ptr_data)
{
	unsigned int mmap_offset = 0;
	unsigned int asshm_addr = 0;
	unsigned int asshm_offset = 0;
	unsigned int asshm_len = 0;
	unsigned int asshm_data_len = 0;

	if (ptr_data == NULL || phandle == NULL)
	{
		SYS_ERROR("phandle is NULL or other parameter is not valid. \r\n");
		return succeed_type_failed;
	}
	if(asshm_seq > phandle->asshm_size)
	{
		SYS_ERROR("The asshm_seq is not valid. \r\n");
		return succeed_type_failed;
	}

	mmap_offset = sizeof(pthread_mutex_t) + 2 * sizeof(int);
	asshm_len = sizeof(LIBASSHM_UINT_STRUCT);
	asshm_data_len = sizeof(LIBASSHM_UINT_DATA);
	asshm_offset = (unsigned int)(asshm_seq) * asshm_len + sizeof(sem_t);
	asshm_addr =  (phandle->asshm_mmap + mmap_offset + asshm_offset);

	memcpy(ptr_data, (void *)asshm_addr, asshm_data_len);

	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libasshm_put
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libasshm_put(LIBASSHM_OBJ *phandle,int asshm_seq, LIBASSHM_FRAME_TYPE frame_type, void *ptr, int length)
{
	unsigned int mmap_offset = 0;
	unsigned int asshm_addr = 0;
	unsigned int asshm_offset = 0;
	LIBASSHM_UINT_HEADER asshm_uint_header;

	if(ptr == NULL || length == 0 || phandle == NULL)
	{
		SYS_ERROR("phandle is NULL or other parameter is not valid. \r\n");
		return succeed_type_failed;
	}
	if(sizeof(LIBASSHM_UINT_DATA) <= length )
	{
		SYS_INFO("length too long. \r\n");
		length = sizeof(LIBASSHM_UINT_DATA);
	}

	if((unsigned int)asshm_seq > phandle->asshm_size)
	{
		SYS_ERROR("The asshm_seq is not valid. \r\n");
		return succeed_type_failed;
	}
	pthread_mutex_t * pmutex =  (pthread_mutex_t *)(phandle->asshm_mmap);

	asshm_uint_header.frame_type = frame_type;
	asshm_uint_header.data_len = length;
	libasshm_generatetimestamp(&asshm_uint_header.timestamp);
	memset(&asshm_uint_header.reserved, 0x00, sizeof(asshm_uint_header.reserved));

	mmap_offset = sizeof(pthread_mutex_t) + 2 * sizeof(int);
    asshm_offset = (unsigned int)(asshm_seq) * sizeof(LIBASSHM_UINT_STRUCT);
	asshm_addr = (phandle->asshm_mmap + mmap_offset + asshm_offset);


	pthread_mutex_lock(pmutex);
	memset((void *)(asshm_addr),0,sizeof(LIBASSHM_UINT_STRUCT));
	sem_init( (sem_t *)(asshm_addr+0) ,0 , ASSHM_SEM_MAX);
	memcpy((void *)(asshm_addr+sizeof(sem_t)), &asshm_uint_header, sizeof(LIBASSHM_UINT_HEADER));
	memcpy((void *)(asshm_addr+sizeof(sem_t)+sizeof(LIBASSHM_UINT_HEADER)), ptr,length);
	pthread_mutex_unlock(pmutex);

	return succeed_type_succeed;
}
static unsigned int libasshm_getaddr(LIBASSHM_OBJ *phandle,int asshm_seq)
{
	unsigned int mmap_offset = 0;
	unsigned int asshm_addr = 0;
	unsigned int asshm_offset = 0;
	LIBASSHM_UINT_HEADER asshm_uint_header;
	mmap_offset = sizeof(pthread_mutex_t) + 2 * sizeof(int);
    asshm_offset = (unsigned int)(asshm_seq) * sizeof(LIBASSHM_UINT_STRUCT);
	asshm_addr = (phandle->asshm_mmap + mmap_offset + asshm_offset);

    
	return (asshm_addr+sizeof(sem_t)+sizeof(LIBASSHM_UINT_HEADER));

}

LIBASSHM_OBJ *ASSHM_INIT()
{
	return libasshm_init();
}
succeed_type ASSHM_DESTROY(LIBASSHM_OBJ *phandle)
{
	return libasshm_destroy(phandle);
}

succeed_type ASSHM_PUT(LIBASSHM_OBJ *phandle, int asshm_seq, LIBASSHM_FRAME_TYPE frame_type, void *ptr, int length)
{
	return libasshm_put(phandle, asshm_seq, frame_type, ptr, length);
}

succeed_type ASSHM_GET_SEM(LIBASSHM_OBJ *phandle, int asshm_seq, int *ptr_sem)
{
	return libasshm_get_sem(phandle, asshm_seq, ptr_sem);
}

succeed_type ASSHM_GET_DATA(LIBASSHM_OBJ *phandle, int asshm_seq, void *ptr_data)
{
	return libasshm_get_data(phandle, asshm_seq, ptr_data);
}

succeed_type ASSHM_GET_COMPLETE(LIBASSHM_OBJ *phandle, int asshm_seq)
{
	return libasshm_get_complete(phandle, asshm_seq);
}
succeed_type ASSHM_MAGIC_DESTROY(LIBASSHM_OBJ *phandle)
{
	return libasshm_magic_destroy(phandle);
}
unsigned int ASSHM_GETADDR(LIBASSHM_OBJ *phandle, int asshm_seq)
{
	return libasshm_getaddr(phandle, asshm_seq);
}
