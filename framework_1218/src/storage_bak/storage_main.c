/*******************************************************************************
 * COPYRIGHT
 ********************************************************************************
*
 * =====================================================================================
 *
 *       Filename:  storage_main.c
 *
 *    Description:
 *
 *        Version:
 *        Created:
 *       Revision:
 *       Compiler:
 *
 *         Author:
 *        Company:
 *
 * =====================================================================================
 */
/********************************* Source Code Start ********************************/

/************************************************************************/
/*Includes                                                              */
/************************************************************************/
#include "storage_private.h"

/************************************************************************/
/*Global Variables define                                                     */
/************************************************************************/
STORAGE_PRIVATE_HANDLE global_storage_private_handle; //storage 全局句柄定义

/************************************************************************/
/*Functions declaration                                                      */
/************************************************************************/
void sig_handle_proc(int sig);   //信号处理函数
extern void pthread_datasave_func(void *arg);   //数据存储线程调用函数

/* ===================================================================================== */
/**
 **       Function: main 
 **
 **    Description:
 **
 **          Input:
 **
 **         Output:
 **
 **         Return:
 **
 **  Modify Record:
 **         Author:
 **
**/
/* ===================================================================================== */
static void storage_poll_init(STORAGE_PRIVATE_HANDLE *phandle)
{
	//memset(&(phandle->polltimeout), 0x00, sizeof(struct pollfd) * STORAGE_POLL_COUNT_MAX );
	memset(phandle->polltimeout, 0x00, sizeof(struct pollfd) * STORAGE_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;
	phandle->polltimeout[0].events = POLLIN;

	phandle->active_fdcount = 1;

}

/* ===================================================================================== */
/**
 **       Function: main 
 **
 **    Description:
 **
 **          Input:
 **
 **         Output:
 **
 **         Return:
 **
 **  Modify Record:
 **         Author:
 **
**/
/* ===================================================================================== */
static succeed_type	storage_sd_check_if(STORAGE_PRIVATE_HANDLE *phandle)
{
	int iRet = -1;
	iRet = system("test -b /dev/mmcblk0p1");
	if( 0 != iRet )
	{
		phandle->sdcard_checked_if = 0;
		SYS_WARNING("No SdCard checked!!!\r\n");
		return succeed_type_failed;
	}
	else
	{
		phandle->sdcard_checked_if = 1;
		SYS_WARNING("SdCard checked!!!\r\n");
		return succeed_type_succeed;
	}
	return succeed_type_succeed;
}

/* ===================================================================================== */
/**
 **       Function: main 
 **
 **    Description:
 **
 **          Input:
 **
 **         Output:
 **
 **         Return:
 **
 **  Modify Record:
 **         Author:
 **
**/
/* ===================================================================================== */
static succeed_type storage_init(STORAGE_PRIVATE_HANDLE *phandle)
{
	SIGNAL_STRUCT_T signal;
	if ( NULL == phandle )
	{
		SYS_ERROR("in storage_init phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	//1. 驱动句柄初始化
	phandle->pgbshm_obj = GBSHM_INIT();
	if ( NULL == phandle->pgbshm_obj )
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	phandle->pvsshm_obj = VSSHM_INIT();
	if ( NULL == phandle->pvsshm_obj )
	{
		SYS_ERROR("VSSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	phandle->passhm_obj = ASSHM_INIT();
	if ( NULL == phandle->passhm_obj )
	{
		SYS_ERROR("ASSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	//2. 创建本进程绑定的UNIX文件，返回值为unix_fd
	if ( UNIX_CREATE(UNIXFILE_SOCKET_STORAGE, &(phandle->unix_fd), 0 ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_STORAGE);
		return succeed_type_failed;
	}

	//3. 本进程模块的poll初始化函数
	storage_poll_init(phandle);
	//phandle->pfile_name = NULL;
#if 0
	{
		long long lltime;
		//memcpy(phandle->afile_name, "20110425001100000",sizeof("20110425001100000"));
		libvsshm_generatetimestamp(&lltime);
		SYS_INFO("lltime = %lld\r\n",lltime);
		sprintf(phandle->afile_name, "%lld", lltime);
	}
#endif
	//SOCKET_CREATE( 	LIBSK_NETMODE_UDP,7897, NULL ,   &(phandle->sockfd));
	phandle->store_fd = NULL;

	//信号量初始化操作
	memset(&signal, 0x00, sizeof(signal));
	signal.signo[0] = SIGINT;
	signal.sig_phandle[0] = sig_handle_proc;
	SIGNAL_INIT(&signal, 1);

	if(0 != pthread_mutex_init(&phandle->g_mutex_t ,NULL))
	{
		SYS_ERROR("storage pthread_mutex_init failed\n");
		return succeed_type_failed;
	}
	phandle->IsStorageData_Available = 0;
	phandle->IsStorageSysQuit = 0;
	storage_sd_check_if(phandle);

	storage_setting_param storage_setting;
	Gbshm_paramGet(phandle->pgbshm_obj, PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM,\
			&storage_setting, sizeof(storage_setting));
	phandle->storage_store_duration = ((long long)storage_setting.storage_pre_record_time / 60) * 100;

	SYS_WARNING("Storage_store_duration:%lld \r\n",phandle->storage_store_duration);
	return succeed_type_succeed;
}

/* ===================================================================================== */
/**
 **       Function: main 
 **
 **    Description:
 **
 **          Input:
 **
 **         Output:
 **
 **         Return:
 **
 **  Modify Record:
 **         Author:
 **
**/
/* ===================================================================================== */
static succeed_type storage_exit(STORAGE_PRIVATE_HANDLE *phandle)
{
	if ( NULL == phandle )
	{
		SYS_ERROR("in storage_exit phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	GBSHM_DESTROY(phandle->pgbshm_obj);
	VSSHM_DESTROY(phandle->pvsshm_obj);
	ASSHM_DESTROY(phandle->passhm_obj);
	if ( UNIX_DESTROY(UNIXFILE_SOCKET_STORAGE, phandle->unix_fd ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_DESTROY %s failed.\r\n", UNIXFILE_SOCKET_STORAGE);
		return succeed_type_failed;
	}
	if(NULL !=  phandle->store_fd)
	{
		fclose(phandle->store_fd);
		phandle->store_fd = NULL;
	}
	pthread_mutex_destroy( &(phandle->g_mutex_t) );
	//free(phandle);
	return succeed_type_succeed;
}

/* ===================================================================================== */
/**
 **       Function: main 
 **
 **    Description:
 **
 **          Input:
 **
 **         Output:
 **
 **         Return:
 **
 **  Modify Record:
 **         Author:
 **
**/
/* ===================================================================================== */
void sig_handle_proc(int sig)
{
	SYS_WARNING("Rcv signo=%d\r\n",sig);
	global_storage_private_handle.IsStorageSysQuit = 1;
}

/* ===================================================================================== */
/**
 **       Function: main 
 **
 **    Description:
 **
 **          Input:
 **
 **         Output:
 **
 **         Return:
 **
 **  Modify Record:
 **         Author:
 **
**/
/* ===================================================================================== */
int main(int argc, char **argv)
{
	pthread_t storage_pthread_t;
	memset(&global_storage_private_handle, 0x00, sizeof(global_storage_private_handle));
	storage_init(&global_storage_private_handle);
	printf("sd_checked ret= %d\r\n",global_storage_private_handle.sdcard_checked_if);
	if( 1 == global_storage_private_handle.sdcard_checked_if )
	{
		if(succeed_type_succeed != InitIdxFile())
		{
			SYS_ERROR("\n\r----------------------InitIdxFile failed!!!------------------------\r\n");
			return -1;
		}
		if(succeed_type_succeed != InitDataFile())
		{
			SYS_ERROR("\n\r----------------------InitDataFile failed!!!------------------------\r\n");
			return -1;
		}
	}
#if 1
	if(pthread_create(&storage_pthread_t, NULL, pthread_datasave_func, &global_storage_private_handle) !=0)
	{
		SYS_ERROR("\n\r-----------------pthread_create for data save error-----------------\r\n");
	}
#endif
	
	SYS_WARNING("Enter storage_manager func.\r\n");
	while(0 == global_storage_private_handle.IsStorageSysQuit)
	{
		if ( poll( global_storage_private_handle.polltimeout,\
					global_storage_private_handle.active_fdcount , -1) > 0 )
		{
			//SYS_INFO("POLL event found.\r\n");
			if ( global_storage_private_handle.polltimeout[0].revents )
			{
				//do UNIX recv event.
				storage_unix(&global_storage_private_handle);
			}
			if ( global_storage_private_handle.polltimeout[1].revents )
			{
			}
			storage_poll_init(&global_storage_private_handle);
		}
		//usleep(10);
	}
	ExitIdxFile();
	storage_exit(&global_storage_private_handle);

	pthread_join(storage_pthread_t,NULL);
	SYS_WARNING("QUIT storage_manager func.\r\n");
	return 0;
}

/********************************* Source Code End ********************************/

