/************************************************************************
 * Copyright (C), 2011, liangzhang@xidian.edu.cn
 * FileName:   liblog.c
 * Description:
 * Date:	2011-03-16
 * Version:	V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "liblog.h"
#include "parameter_offset.h"

/**
	LOG opt define	
**/
typedef enum _LOG_OPT
{
	LOG_OPT_START,
	LOG_OPT_MAGIC,
	LOG_OPT_CONN,
	LOG_OPT_COUNT,
	LOG_OPT_END,
}LOG_OPT;

/******************************************** 
 *    Function    : liblog_value_getbyopt
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static succeed_type liblog_value_getbyopt(LIBLOG_OBJ *phandle, LOG_OPT opt, int *ptr_value)
{
	succeed_type iret = succeed_type_failed;
	unsigned int value_offset = 0;
	if ( NULL == phandle || NULL == ptr_value)
	{
		SYS_ERROR("phandle || ptr_value is NULL.\r\n");
		return succeed_type_failed;
	}
	switch(opt)
	{
		case LOG_OPT_START:
			SYS_ERROR("InValid opt\r\n");
			break;
		case LOG_OPT_MAGIC:
			value_offset = sizeof(pthread_mutex_t);
			*ptr_value = *(int *)(phandle->log_mmap + value_offset);
			iret = succeed_type_succeed;
			break;
		case LOG_OPT_CONN:
			value_offset = sizeof(pthread_mutex_t) + sizeof(int);
			*ptr_value = *(int *)(phandle->log_mmap + value_offset);
			iret = succeed_type_succeed;
			break;
		case LOG_OPT_COUNT:
			value_offset = sizeof(pthread_mutex_t) + 2 * sizeof(int);
			*ptr_value = *(int *)(phandle->log_mmap + value_offset);
			iret = succeed_type_succeed;
			break;
		case LOG_OPT_END:
		default:
			SYS_ERROR("InValid opt\r\n");
			break;
	}
	return iret;
}
/******************************************** 
 *    Function    : liblog_value_setbyopt
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static succeed_type liblog_value_setbyopt(LIBLOG_OBJ *phandle, LOG_OPT opt, int *ptr_value)
{
	succeed_type iret = succeed_type_failed;
	unsigned int value_offset = 0;
	if ( NULL == phandle || NULL == ptr_value)
	{
		SYS_ERROR("phandle || ptr_value is NULL.\r\n");
		return succeed_type_failed;
	}
	pthread_mutex_t * pmutex =  (pthread_mutex_t *)(phandle->log_mmap);
	switch(opt)
	{
		case LOG_OPT_START:
			SYS_ERROR("InValid opt\r\n");
			break;
		case LOG_OPT_MAGIC:
			value_offset = sizeof(pthread_mutex_t);
			pthread_mutex_lock(pmutex);
			*(int *)(phandle->log_mmap + value_offset) = *ptr_value;
			pthread_mutex_unlock(pmutex);
			iret = succeed_type_succeed;
			break;
		case LOG_OPT_CONN:
			value_offset = sizeof(pthread_mutex_t) + sizeof(int);
			pthread_mutex_lock(pmutex);
			*(int *)(phandle->log_mmap + value_offset) = *ptr_value;
			pthread_mutex_unlock(pmutex);
			iret = succeed_type_succeed;
			break;
		case LOG_OPT_COUNT:
			value_offset = sizeof(pthread_mutex_t) + 2 * sizeof(int);
			pthread_mutex_lock(pmutex);
			*(int *)(phandle->log_mmap + value_offset) = *ptr_value;
			pthread_mutex_unlock(pmutex);
			iret = succeed_type_succeed;
			break;
		case LOG_OPT_END:
		default:
			SYS_ERROR("InValid opt\r\n");
			break;
	}
	return iret;
}
/******************************************** 
 *    Function    : liblog_init
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static LIBLOG_OBJ *liblog_init()
{
	unsigned int mmap_offset = 0;
	int log_magic_num = LOG_MAGIC_NUM;
	int log_conn_num = 0x01;
	int log_count_num = 0x00;
	LIBLOG_OBJ *phandle = malloc(sizeof(LIBLOG_OBJ));
	if (phandle == NULL)
	{
		SYS_ERROR("malloc phandle failed.\r\n");
		return NULL;
	}
	phandle->log_fd = open("/dev/log", O_RDWR);
	if ( phandle->log_fd <= 0 )
	{
		SYS_ERROR("open fd=%d failed.\r\n",phandle->log_fd);
		free( phandle );
		return NULL;
	}

	SYS_INFO("liblog_init.\r\n");

	phandle->log_mmap = 0;
	SYS_INFO("mmap init to 0.\r\n");

	phandle->log_mmap = (unsigned int)mmap(0, SHAREMEM_PHYS_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, phandle->log_fd, LOGMEM_PHYS_ADDR);
	SYS_INFO("after liblog_init. log addr 0x%x \r\n",phandle->log_mmap );
	if ( phandle->log_mmap <= 0 )
	{
		free( phandle );
		SYS_ERROR("mmap failed.\r\n");
		return NULL;
	}
	//init mutex
	mmap_offset = sizeof(pthread_mutex_t) + 3 * sizeof(int); //mutex magic conn count
	if( LOG_MAGIC_NUM == *(int *)(phandle->log_mmap + sizeof(pthread_mutex_t)))
	{
		log_conn_num++;
		liblog_value_setbyopt(phandle, LOG_OPT_CONN, &log_conn_num);
	}
	else //fisrt time used
	{
		if(0 != pthread_mutex_init( (pthread_mutex_t *)(phandle->log_mmap) ,NULL))
		{
			SYS_ERROR("log mutex init failed\n");
			return NULL;
		}
		//*(int *)(phandle->log_mmap + sizeof(pthread_mutex_t)) =	LOG_MAGIC_NUM;
		//*(int *)(phandle->log_mmap + sizeof(pthread_mutex_t) + sizeof(int)) = 0x00;
		liblog_value_setbyopt(phandle, LOG_OPT_MAGIC, &log_magic_num);
		liblog_value_setbyopt(phandle, LOG_OPT_CONN, &log_conn_num);
		liblog_value_setbyopt(phandle, LOG_OPT_COUNT, &log_count_num);
	}

	phandle->log_numsize = (LOGMEM_PHYS_SIZE - mmap_offset)/sizeof(LIBLOG_UINT_STRUCT);

	SYS_INFO("after liblog_init. log size 0x%x \r\n",phandle->log_numsize );
	return phandle;
}
/******************************************** 
 *    Function    : liblog_destroy
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type liblog_destroy(LIBLOG_OBJ *phandle)
{
	int log_magic_num = LOG_MAGIC_CLR;
	int log_conn_num = 0x00;
	if (phandle == NULL)
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	liblog_value_getbyopt(phandle, LOG_OPT_CONN, &log_conn_num);
	SYS_INFO("liblog_destroy log_conn_num = %d", log_conn_num);
	if(0x01 == log_conn_num)//the last invoke
	{
		close(phandle->log_fd);
		pthread_mutex_destroy( (pthread_mutex_t *)(phandle->log_mmap) );
		liblog_value_setbyopt(phandle, LOG_OPT_MAGIC, &log_magic_num);
		munmap((void*)(phandle->log_mmap), SHAREMEM_PHYS_SIZE);
	}
	else if(0x01 < log_conn_num)
	{
		log_conn_num--;
		liblog_value_setbyopt(phandle, LOG_OPT_CONN, &log_conn_num);
	}	
	else
	{
		SYS_INFO("destory do nothing!!!\r\n");
		return succeed_type_succeed;
	} 
	free(phandle);
 	phandle = NULL;
	return succeed_type_succeed;
}

static succeed_type liblog_getvirtaddr(LIBLOG_OBJ *phandle)
{
	if ( phandle == NULL )
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	return phandle->log_mmap;
}

/******************************************** 
 *    Function    : liblog_read
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type liblog_read(LIBLOG_OBJ *phandle, int log_seq, void *ptr, int length)
{
	int log_addr = 0;
	int log_offset = 0;
	int map_offset = 0;
	int log_count_num = 0x00;
	if (ptr == NULL || length == 0 || phandle == NULL)
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	if(succeed_type_succeed != liblog_value_getbyopt(phandle, LOG_OPT_COUNT, &log_count_num))
	{	
		SYS_ERROR("liblog_value_getbyopt failed.\r\n");
		return succeed_type_failed;
	}
	if(log_count_num < phandle->log_numsize && log_seq >= log_count_num)
	{
		SYS_ERROR("The LogRecord no written yet!!");
		return succeed_type_failed;
	}
	map_offset = sizeof(pthread_mutex_t) + 3 * sizeof(int);
	log_offset = (log_seq % phandle->log_numsize) * sizeof(LIBLOG_UINT_STRUCT);
	log_addr =  (phandle->log_mmap + map_offset + log_offset);
	memcpy(ptr, (void *)log_addr, length);

	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : liblog_write
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type liblog_write(LIBLOG_OBJ *phandle, void *ptr, int length)
{
	int log_addr = 0;
	int log_offset = 0;
	int map_offset = 0;
	int log_count_num = 0x00;
	pthread_mutex_t * pmutex =  (pthread_mutex_t *)(phandle->log_mmap);

	if(ptr == NULL || length == 0||phandle == NULL)
	{
		SYS_ERROR("phandle is NULL or other parameter is not valid. \r\n");
		return succeed_type_failed;
	}
	if(succeed_type_succeed != liblog_value_getbyopt(phandle, LOG_OPT_COUNT, &log_count_num))
	{	
		SYS_ERROR("liblog_value_getbyopt failed.\r\n");
		return succeed_type_failed;
	}
	map_offset = sizeof(pthread_mutex_t) + 3 * sizeof(int);
    log_offset = (log_count_num % phandle->log_numsize) * sizeof(LIBLOG_UINT_STRUCT);
	log_addr = (phandle->log_mmap + map_offset + log_offset);

	pthread_mutex_lock(pmutex);
	memcpy((void *)log_addr, ptr, length);
	pthread_mutex_unlock(pmutex);
	log_count_num++; 
	liblog_value_setbyopt(phandle, LOG_OPT_COUNT, &log_count_num);
	return succeed_type_succeed;

}

LIBLOG_OBJ *LOG_INIT()
{
	return liblog_init();
}
succeed_type LOG_DESTROY(LIBLOG_OBJ *phandle)
{
	return liblog_destroy(phandle);
}

succeed_type LOG_READ(LIBLOG_OBJ *phandle, int log_seq, void *ptr, int length)
{
	return liblog_read(phandle, log_seq, ptr, length);
}

succeed_type LOG_WRITE(LIBLOG_OBJ *phandle, void *ptr, int length)
{
	return liblog_write(phandle, ptr, length);
}

