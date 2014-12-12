/************************************************************************
 * Copyright (C), 2011, liangzhang@xidian.edu.cn
 * FileName:   libvsshm.c
 * Description:
 * Date:	2011-03-16
 * Version:	V1.0
 *  *************************************************************************/

#include "system_includes.h"
#include "system_debug.h"
#include "libvsshm.h"
#include "parameter_offset.h"

/*global variables defined*/
//考虑到只有单个进程能对i/p buffer的获取故采用静态变量可以随时跟踪当前使用状况
static unsigned int stCurBufPCount = 0;

//vsshm 固有头部偏移 包含一个全局互斥量、-个magic num、一个当前模块连接数、
//一个i帧序号管理、一个p帧序号管理
//vsshm 固有头部类型定义
typedef enum _VSSHM_OPT
{
	VSSHM_OPT_START,
	VSSHM_OPT_MUTEX,
	VSSHM_OPT_MAGIC,
	VSSHM_OPT_CONN,
	VSSHM_OPT_IFRAME,
	VSSHM_OPT_PFRAME,
	VSSHM_OPT_END,
}VSSHM_OPT;

typedef pthread_mutex_t VSSHM_TYPE_MUTEX_T;
typedef int 			VSSHM_TYPE_MAGIC_T;
typedef int 			VSSHM_TYPE_CONN_T;
typedef int 			VSSHM_TYPE_IFRAME_COUNT_T;
typedef int 			VSSHM_TYPE_PFRAME_COUNT_T;

#define VSSHM_MMAP_START_OFFSET 0x00
#define VSSHM_MMAP_MUTEX_OFFSET VSSHM_MMAP_START_OFFSET 
#define VSSHM_MMAP_MAGIC_OFFSET VSSHM_MMAP_MUTEX_OFFSET + sizeof(VSSHM_TYPE_MUTEX_T)
#define VSSHM_MMAP_CONN_OFFSET VSSHM_MMAP_MAGIC_OFFSET + sizeof(VSSHM_TYPE_MAGIC_T)
#define VSSHM_MMAP_IFRAME_COUNT_OFFSET VSSHM_MMAP_CONN_OFFSET + sizeof(VSSHM_TYPE_CONN_T)
#define VSSHM_MMAP_PFRAME_COUNT_OFFSET VSSHM_MMAP_IFRAME_COUNT_OFFSET \
												+sizeof(VSSHM_TYPE_IFRAME_COUNT_T)
#define VSSHM_MMAP_END_OFFSET VSSHM_MMAP_PFRAME_COUNT_OFFSET + sizeof(VSSHM_TYPE_PFRAME_COUNT_T)

#define VSSHM_IFRAME_COUNT_GET() *(VSSHM_TYPE_IFRAME_COUNT_T*)(phandle->vsshm_mmap\
				+ VSSHM_MMAP_IFRAME_COUNT_OFFSET)

#define VSSHM_PFRAME_COUNT_GET() *(VSSHM_TYPE_PFRAME_COUNT_T*)(phandle->vsshm_mmap\
				+ VSSHM_MMAP_PFRAME_COUNT_OFFSET)

#define VSSHM_IFRAME_COUNT_SET(value) *(VSSHM_TYPE_IFRAME_COUNT_T*)(phandle->vsshm_mmap\
				+ VSSHM_MMAP_IFRAME_COUNT_OFFSET) = value

#define VSSHM_PFRAME_COUNT_SET(value) *(VSSHM_TYPE_PFRAME_COUNT_T*)(phandle->vsshm_mmap\
				+ VSSHM_MMAP_PFRAME_COUNT_OFFSET) = value

//i 帧偏移单位
#define VSSHM_UINT_OFFSET_I	  sizeof(LIBVSSHM_UINT_STRUCT_I)
//i 帧总体偏移量
#define VSSHM_ALL_OFFSET_I	  VSSHM_UINT_OFFSET_I * VSSHM_UINT_NUM_I	
//p 帧偏移单位
#define VSSHM_UINT_OFFSET_P	  sizeof(LIBVSSHM_UINT_STRUCT_P)
//p 帧总体偏移量
#define VSSHM_ALL_OFFSET_P	  VSSHM_UINT_OFFSET_P * VSSHM_UINT_NUM_P	

/******************************************** 
 *    Function    : GenerateTimeStamp
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : void 
 *
 * **********************************************/
#if 1
void libvsshm_generatetimestamp(long long *TimeStamp)
{
	time_t CurTime_t;
	struct tm *pCurTm_t;
	time(&CurTime_t);
	pCurTm_t = localtime(&CurTime_t);
	*TimeStamp = (long long)(\
			((long long)(1900+pCurTm_t->tm_year)*10*10*10*10*10*10*10*10*10*10) \
			+((long long)(1+pCurTm_t->tm_mon)*10*10*10*10*10*10*10*10) \
			+((long long)(pCurTm_t->tm_mday)*10*10*10*10*10*10) \
			+((long long)(pCurTm_t->tm_hour)*10*10*10*10)\
			+((long long)(pCurTm_t->tm_min)*10*10)\
			+((long long)(pCurTm_t->tm_sec))\
			);
}
#endif
#if 0
void libvsshm_generatetimestamp(long long *TimeStamp)
{
	struct timeval time_v;
	gettimeofday(&time_v);
	*TimeStamp = (long long)(\
			((long long)(time_v.tv_sec)*10*10*10*10*10*10) \
			+((long long)(time_v.tv_usec)) \
			);
}
#endif
/******************************************** 
 *    Function    : libvsshm_init
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : not NULL -- succeed
 *                  NULL     -- failed
 * **********************************************/
static LIBVSSHM_OBJ *libvsshm_init()
{
	unsigned int vsshm_addr = 0;

	//分配句柄空间
	LIBVSSHM_OBJ *phandle = malloc(sizeof(LIBVSSHM_OBJ));
	if (NULL == phandle)
	{
		SYS_ERROR("malloc phandle failed.\r\n");
		return NULL;
	}
	//打开vsshm设备
	phandle->vsshm_fd = open("/dev/vsshm", O_RDWR);
	if ( phandle->vsshm_fd <= 0 )
	{
		SYS_ERROR("open fd = %d failed.\r\n",phandle->vsshm_fd);
		free( phandle );
		return NULL;
	}

	SYS_INFO("libvsshm_init. \r\n");

	phandle->vsshm_mmap = 0;
	SYS_INFO("mmap init to 0.\r\n");

	phandle->vsshm_mmap = (unsigned int)mmap(0, VSSHMMEM_PHYS_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, phandle->vsshm_fd, VSSHMMEM_PHYS_ADDR);
	if ( phandle->vsshm_mmap <= 0 )
	{
		close(phandle->vsshm_fd);
		free( phandle );
		phandle = NULL;
		SYS_ERROR("mmap failed.\r\n");
		return NULL;
	}
	phandle->vsshm_phyaddr = VSSHMMEM_PHYS_ADDR;
	phandle->vsshm_size = VSSHMMEM_PHYS_SIZE;

	vsshm_addr = phandle->vsshm_mmap + VSSHM_MMAP_END_OFFSET ;

	VSSHM_TYPE_MUTEX_T* pmutex =  (VSSHM_TYPE_MUTEX_T*)(phandle->vsshm_mmap);
	//init mutex 使用互斥信号量保证同一时间只能有一个进程访问共享内存空间
	//VSSHM_MAGIC_NUM：每个进程都有初始化VSSHM的函数，所以要通过这个参数来判断VSSHM是否已经
	//初始化过，已经初始化的就不需要再初始化，否则会清除共享内存空间的数据
	if(VSSHM_MAGIC_NUM == *(VSSHM_TYPE_MAGIC_T*)(phandle->vsshm_mmap + VSSHM_MMAP_MAGIC_OFFSET))
	{
		int vsshm_conn_num = 0;
		vsshm_conn_num = *(VSSHM_TYPE_CONN_T*)(phandle->vsshm_mmap + VSSHM_MMAP_CONN_OFFSET );
		vsshm_conn_num++;
		pthread_mutex_lock(pmutex);
		*(VSSHM_TYPE_CONN_T*)(phandle->vsshm_mmap + VSSHM_MMAP_CONN_OFFSET) = vsshm_conn_num;
		pthread_mutex_unlock(pmutex);
		SYS_INFO("vsshm not first init!!!vsshm_conn_num= %d\r\n",vsshm_conn_num);
	}
	else //first time invoke
	{ 
		unsigned int iLoop = 0;
		SYS_INFO("vsshm first init!!!\r\n");
		if(0 != pthread_mutex_init( (VSSHM_TYPE_MUTEX_T*)(phandle->vsshm_mmap) ,NULL))
		{
			SYS_ERROR("vsshm mutex init failed\n");
			return NULL;
		}
		for(iLoop = 0; iLoop < VSSHM_UINT_NUM; iLoop++) //iLoop表示i贞和p贞的个数	
		{
			if(iLoop < VSSHM_UINT_NUM_I) 
			{
				//I 帧存储区
				pthread_mutex_lock(pmutex);
				sem_init( (sem_t*)(vsshm_addr+iLoop*VSSHM_UINT_OFFSET_I), 0, 0 );
				pthread_mutex_unlock(pmutex);
			}
			else
			{
				//P 帧存储区
				pthread_mutex_lock(pmutex);
				sem_init( (sem_t*)(vsshm_addr+VSSHM_ALL_OFFSET_I \
							+(iLoop-VSSHM_UINT_NUM_I)*VSSHM_UINT_OFFSET_P), 0, 0 );
				pthread_mutex_unlock(pmutex);
			}
		}
		pthread_mutex_lock(pmutex);
		*(VSSHM_TYPE_MAGIC_T*)(phandle->vsshm_mmap + VSSHM_MMAP_MAGIC_OFFSET) = VSSHM_MAGIC_NUM;
		*(VSSHM_TYPE_CONN_T*)(phandle->vsshm_mmap + VSSHM_MMAP_CONN_OFFSET) = 0x01;
		*(VSSHM_TYPE_IFRAME_COUNT_T*)(phandle->vsshm_mmap + VSSHM_MMAP_IFRAME_COUNT_OFFSET) = 0x00;
		*(VSSHM_TYPE_PFRAME_COUNT_T*)(phandle->vsshm_mmap + VSSHM_MMAP_PFRAME_COUNT_OFFSET) = \
															VSSHM_UINT_NUM_I;
		pthread_mutex_unlock(pmutex);
	}

	// init all the sem when the LIBVSSHM_INIT is firstly invoked

	SYS_INFO("after libvsshm_init. vsshm addr = 0x%x -- phyaddr = 0x%x -- size = 0x%xB\r\n",\
			phandle->vsshm_mmap, phandle->vsshm_phyaddr, phandle->vsshm_size);
	return phandle;
}
/******************************************** 
 *    Function    : libvsshm_destroy
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libvsshm_destroy(LIBVSSHM_OBJ *phandle)
{
	int vsshm_conn_num = 0;
	if (NULL == phandle)
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}

	VSSHM_TYPE_MUTEX_T* pmutex =  (VSSHM_TYPE_MUTEX_T*)(phandle->vsshm_mmap);
	vsshm_conn_num = *(VSSHM_TYPE_CONN_T*)(phandle->vsshm_mmap + VSSHM_MMAP_CONN_OFFSET);
	if(0x01 == vsshm_conn_num) //last invoke
	{
		close(phandle->vsshm_fd);	
		pthread_mutex_lock(pmutex);
		*(VSSHM_TYPE_MAGIC_T*)(phandle->vsshm_mmap + VSSHM_MMAP_MAGIC_OFFSET) = VSSHM_MAGIC_CLR;
		pthread_mutex_unlock(pmutex);
		munmap((void*)phandle->vsshm_mmap, VSSHMMEM_PHYS_SIZE);
	}
	else if(0x01 < vsshm_conn_num)
	{
		vsshm_conn_num--;
		pthread_mutex_lock(pmutex);
		*(VSSHM_TYPE_CONN_T*)(phandle->vsshm_mmap + VSSHM_MMAP_CONN_OFFSET) = vsshm_conn_num;
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
 *    Function    : libvsshm_magic_destroy
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libvsshm_magic_destroy(LIBVSSHM_OBJ *phandle)
{
	int vsshm_conn_num = 0;
	VSSHM_TYPE_MUTEX_T* pmutex =  (VSSHM_TYPE_MUTEX_T*)(phandle->vsshm_mmap);
	if (phandle == NULL)
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	else
	{
		close(phandle->vsshm_fd);	
		if(0 != pthread_mutex_trylock(pmutex))
		{
			pmutex = (VSSHM_TYPE_MUTEX_T*)\
					 pthread_mutex_init( (VSSHM_TYPE_MUTEX_T*)(phandle->vsshm_mmap) ,NULL );
		}
		else
		{
			pthread_mutex_lock(pmutex);
			*(VSSHM_TYPE_MAGIC_T*)(phandle->vsshm_mmap + VSSHM_MMAP_MAGIC_OFFSET) = VSSHM_MAGIC_CLR;
			pthread_mutex_unlock(pmutex);
			munmap((void*)phandle->vsshm_mmap, VSSHMMEM_PHYS_SIZE);
		}
	}
	free(phandle);
	phandle = NULL;
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libvsshm_get_complete
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libvsshm_get_complete(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq)
{
	unsigned int vsshm_offset = 0;
	unsigned int vsshm_addr = 0;
	VSSHM_TYPE_MUTEX_T* pmutex =  (VSSHM_TYPE_MUTEX_T*)(phandle->vsshm_mmap);
	if ( NULL == phandle )
	{
		SYS_ERROR("phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	if(vsshm_seq >= VSSHM_UINT_NUM || vsshm_seq < 0)
	{
		SYS_ERROR("The vsshm_seq is not valid. vsshm_seq = 0x%x \r\n", vsshm_seq);
		return succeed_type_failed;
	}
	if(vsshm_seq < VSSHM_UINT_NUM_I)
	{
		//i帧存储区
		vsshm_offset = vsshm_seq * VSSHM_UINT_OFFSET_I;
		vsshm_addr = (phandle->vsshm_mmap+VSSHM_MMAP_END_OFFSET+vsshm_offset);
		pthread_mutex_lock(pmutex);
		sem_trywait((sem_t*)vsshm_addr);
		pthread_mutex_unlock(pmutex);
	}
	else
	{
		//p帧存储区
		vsshm_offset = (vsshm_seq - VSSHM_UINT_NUM_I)* VSSHM_UINT_OFFSET_P;
		vsshm_addr = (phandle->vsshm_mmap+VSSHM_MMAP_END_OFFSET+VSSHM_ALL_OFFSET_I+vsshm_offset);
		pthread_mutex_lock(pmutex);
		sem_trywait((sem_t*)vsshm_addr);
		pthread_mutex_unlock(pmutex);

	}
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libvsshm_get_sem
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libvsshm_get_seq(LIBVSSHM_OBJ *phandle, LIBVSSHM_I_P_FLAG i_p_flag,\
		unsigned int *pvsshm_seq, int *ptr_sem)
{
	unsigned int vsshm_addr = 0;
	unsigned int vsshm_offset = 0;
	if (NULL == pvsshm_seq || NULL == phandle)
	{
		SYS_ERROR("parameters are not valid. pvsshm_seq = 0x%x phandle = 0x%x\r\n",\
				pvsshm_seq, phandle);
		return succeed_type_failed;
	}
	if(I_P_FLAG_I == i_p_flag)
	{
		//获取I帧可用空间
		*pvsshm_seq = VSSHM_IFRAME_COUNT_GET();
		if(NULL == ptr_sem)
		{}
		else
		{
			vsshm_offset = *pvsshm_seq * VSSHM_UINT_OFFSET_I;
			vsshm_addr =  (phandle->vsshm_mmap + VSSHM_MMAP_END_OFFSET + vsshm_offset);
			sem_getvalue((sem_t *)vsshm_addr, ptr_sem);
		}
	}
	else if(I_P_FLAG_P == i_p_flag)
	{
		//获取P帧可用空间
		*pvsshm_seq = VSSHM_PFRAME_COUNT_GET();
		if(NULL == ptr_sem)
		{}
		else
		{
			vsshm_offset = (*pvsshm_seq - VSSHM_UINT_NUM_I) * VSSHM_UINT_OFFSET_P;
			vsshm_addr =  (phandle->vsshm_mmap + VSSHM_ALL_OFFSET_I + \
					VSSHM_MMAP_END_OFFSET + vsshm_offset);
			sem_getvalue((sem_t *)vsshm_addr, ptr_sem);
		}
	}
	else
	{
		SYS_ERROR("i_p_flag is not valid.i_p_flag = 0x%x \r\n",i_p_flag);
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libvsshm_get_data
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libvsshm_get_data(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq,void *ptr_data)
{
	unsigned int vsshm_addr = 0;
	unsigned int vsshm_offset = 0;
	unsigned int vsshm_data_len = 0;

	if (NULL == ptr_data || NULL == phandle)
	{
		SYS_ERROR("parameters are not valid. ptr_data = 0x%x phandle = 0x%x\r\n",\
				ptr_data, phandle);
		return succeed_type_failed;
	}
	if(vsshm_seq >= VSSHM_UINT_NUM || vsshm_seq < 0)
	{
		SYS_ERROR("The vsshm_seq is not valid. vsshm_seq = 0x%x\r\n", vsshm_seq);
		return succeed_type_failed;
	}
	if(vsshm_seq < VSSHM_UINT_NUM_I)
	{
		//i帧存储区
		vsshm_offset = vsshm_seq * VSSHM_UINT_OFFSET_I + sizeof(sem_t);
		vsshm_addr = (phandle->vsshm_mmap+VSSHM_MMAP_END_OFFSET+vsshm_offset);
		vsshm_data_len = ((LIBVSSHM_UINT_HEADER *)vsshm_addr)->data_len \
						 + sizeof(LIBVSSHM_UINT_HEADER);
		memcpy(ptr_data, (void *)vsshm_addr, vsshm_data_len);
	}
	else
	{
		//p帧存储区
		vsshm_offset = (vsshm_seq - VSSHM_UINT_NUM_I)* VSSHM_UINT_OFFSET_P + sizeof(sem_t);
		vsshm_addr = (phandle->vsshm_mmap+VSSHM_MMAP_END_OFFSET+VSSHM_ALL_OFFSET_I+vsshm_offset);
		vsshm_data_len = ((LIBVSSHM_UINT_HEADER *)vsshm_addr)->data_len \
						 + sizeof(LIBVSSHM_UINT_HEADER);
		memcpy(ptr_data, (void *)vsshm_addr, vsshm_data_len);
	}

	return succeed_type_succeed;
}

/******************************************** 
 *    Function    : libvsshm_put
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : succeed_type_succeed -- succeed
 *                  succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libvsshm_put(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq,\
		LIBVSSHM_FRAME_TYPE frame_type, void *ptr, int length)
{
	unsigned int vsshm_addr = 0;
	unsigned int vsshm_offset = 0;
	LIBVSSHM_UINT_HEADER vsshm_uint_header;

	VSSHM_TYPE_MUTEX_T* pmutex =  (VSSHM_TYPE_MUTEX_T*)(phandle->vsshm_mmap);
	if(NULL == ptr || length <= 0 ||  NULL == phandle)
	{
		SYS_ERROR("parameters are not valid. ptr = 0x%x length = 0x%x phandle = 0x%x\r\n",\
				ptr, length , phandle);
		return succeed_type_failed;
	}
	if(vsshm_seq >= VSSHM_UINT_NUM || vsshm_seq < 0)
	{
		SYS_ERROR("The vsshm_seq is not valid. vsshm_seq = 0x%x \r\n", vsshm_seq);
		return succeed_type_failed;
	}

	if(vsshm_seq < VSSHM_UINT_NUM_I)
	{
		//i帧存储区
		if(VSSHM_IFRAME_COUNT_GET() != vsshm_seq)
		{
			SYS_ERROR("\tThe vsshm_seq is not valid! vsshm_seq = 0x%x\n\
\tYou should invoke the VSSHM_GET_SEQ and use the returned vsshm_seq!!",vsshm_seq);
			return succeed_type_failed;
		}
		else
		{
			//I帧序号 0~5
			VSSHM_IFRAME_COUNT_SET((VSSHM_IFRAME_COUNT_GET() + 1) % VSSHM_UINT_NUM_I);
		}

		if(VSSHM_UINT_LEN_I < length )
		{
			SYS_WARNING("length too long. length = 0x%x \r\n",length);
			length = VSSHM_UINT_LEN_I;
		}
		vsshm_uint_header.frame_type = frame_type;
		vsshm_uint_header.i_p_flag = I_P_FLAG_I;
		vsshm_uint_header.data_len = length;
		libvsshm_generatetimestamp(&vsshm_uint_header.timestamp);
		memset(&vsshm_uint_header.reserved, 0x00, sizeof(vsshm_uint_header.reserved));

		vsshm_offset = vsshm_seq * VSSHM_UINT_OFFSET_I;
		vsshm_addr = (phandle->vsshm_mmap+VSSHM_MMAP_END_OFFSET+vsshm_offset);

		pthread_mutex_lock(pmutex);
		memset((void *)(vsshm_addr),0,sizeof(LIBVSSHM_UINT_STRUCT_I));
		sem_init( (sem_t *)(vsshm_addr+0) ,0 , VSSHM_SEM_MAX);
		memcpy((void *)(vsshm_addr+sizeof(sem_t)),&vsshm_uint_header,sizeof(LIBVSSHM_UINT_HEADER));
		memcpy((void *)(vsshm_addr+sizeof(sem_t)+sizeof(LIBVSSHM_UINT_HEADER)),ptr,length);
		pthread_mutex_unlock(pmutex);

	}
	else
	{
		//p帧存储区
		if(VSSHM_PFRAME_COUNT_GET()!= vsshm_seq)
		{
			SYS_ERROR("\tThe vsshm_seq is not valid! vsshm_seq = 0x%x\n\
\tYou should invoke the VSSHM_GET_SEQ and use the returned vsshm_seq!!",vsshm_seq);
			return succeed_type_failed;
		}
		else
		{
			//P帧序号6~59
			VSSHM_PFRAME_COUNT_SET(((VSSHM_PFRAME_COUNT_GET() + 1)-VSSHM_UINT_NUM_I)\
				% VSSHM_UINT_NUM_P + VSSHM_UINT_NUM_I);
		}

		if(VSSHM_UINT_LEN_P < length )
		{
			SYS_WARNING("length too long.length = 0x%x \r\n",length);
			length = VSSHM_UINT_LEN_P;
		}
		vsshm_uint_header.frame_type = frame_type;
		vsshm_uint_header.i_p_flag = I_P_FLAG_P;
		vsshm_uint_header.data_len = length;
		libvsshm_generatetimestamp(&vsshm_uint_header.timestamp);
		memset(&vsshm_uint_header.reserved, 0x00, sizeof(vsshm_uint_header.reserved));

		vsshm_offset = (vsshm_seq - VSSHM_UINT_NUM_I)* VSSHM_UINT_OFFSET_P;
		vsshm_addr = (phandle->vsshm_mmap+VSSHM_MMAP_END_OFFSET+VSSHM_ALL_OFFSET_I+vsshm_offset);

		pthread_mutex_lock(pmutex);
		memset((void *)(vsshm_addr),0,VSSHM_UINT_OFFSET_P);
		sem_init((sem_t *)(vsshm_addr+0) ,0 , VSSHM_SEM_MAX);
		memcpy((void *)(vsshm_addr+sizeof(sem_t)),&vsshm_uint_header,sizeof(LIBVSSHM_UINT_HEADER));
		memcpy((void *)(vsshm_addr+sizeof(sem_t)+sizeof(LIBVSSHM_UINT_HEADER)),ptr,length);
		pthread_mutex_unlock(pmutex);

	}

	return succeed_type_succeed;
}
/******************************************** 
 *    Function    : libvsshm_getaddr
 *    Author      : 
 *    Date        : 2011-03-15 
 *    Description : 
 *    Para        : 
 *    Ret         : unsigned int  addr
 * **********************************************/
static unsigned int libvsshm_getaddr(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq)
{
	unsigned int vsshm_addr = 0;
	unsigned int vsshm_offset = 0;
	LIBVSSHM_UINT_HEADER vsshm_uint_header;
	if( NULL == phandle)
	{
		SYS_ERROR("phandle = NULL\r\n");
		return succeed_type_failed;
	}
	if(vsshm_seq >= VSSHM_UINT_NUM || vsshm_seq < 0)
	{
		SYS_ERROR("The vsshm_seq is not valid. vsshm_seq = 0x%x \r\n", vsshm_seq);
		return succeed_type_failed;
	}
	if(vsshm_seq < VSSHM_UINT_NUM_I)
	{
		//i帧存储区
		vsshm_offset = vsshm_seq * VSSHM_UINT_OFFSET_I;
		vsshm_addr = (phandle->vsshm_mmap+VSSHM_MMAP_END_OFFSET+vsshm_offset);
	}
	else
	{
		//p帧存储区
		vsshm_offset = (vsshm_seq - VSSHM_UINT_NUM_I)* VSSHM_UINT_OFFSET_P;
		vsshm_addr = (phandle->vsshm_mmap+VSSHM_MMAP_END_OFFSET+VSSHM_ALL_OFFSET_I+vsshm_offset);
	}

	return (vsshm_addr+sizeof(sem_t)+sizeof(LIBVSSHM_UINT_HEADER));
}

LIBVSSHM_OBJ *VSSHM_INIT()
{
	return libvsshm_init();
}
succeed_type VSSHM_DESTROY(LIBVSSHM_OBJ *phandle)
{
	return libvsshm_destroy(phandle);
}

succeed_type VSSHM_PUT(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq,\
		LIBVSSHM_FRAME_TYPE frame_type, void *ptr, int length)
{
	return libvsshm_put(phandle, vsshm_seq, frame_type, ptr, length);
}

succeed_type VSSHM_GET_SEQ(LIBVSSHM_OBJ *phandle, LIBVSSHM_I_P_FLAG i_p_flag, \
		unsigned int *pvsshm_seq, int *ptr_sem)
{
	return libvsshm_get_seq(phandle, i_p_flag, pvsshm_seq, ptr_sem);
}

succeed_type VSSHM_GET_DATA(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq, void *ptr_data)
{
	return libvsshm_get_data(phandle, vsshm_seq, ptr_data);
}

succeed_type VSSHM_GET_COMPLETE(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq)
{
	return libvsshm_get_complete(phandle, vsshm_seq);
}
succeed_type VSSHM_MAGIC_DESTROY(LIBVSSHM_OBJ *phandle)
{
	return libvsshm_magic_destroy(phandle);
}
unsigned int VSSHM_GETADDR(LIBVSSHM_OBJ *phandle, unsigned int vsshm_seq)
{
	return libvsshm_getaddr(phandle, vsshm_seq);
}

/*-------------------------------------Coding End-------------------------------------------*/

