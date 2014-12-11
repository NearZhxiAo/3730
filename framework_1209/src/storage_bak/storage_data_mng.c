/*******************************************************************************
 * COPYRIGHT
 ********************************************************************************
/*
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
#include "storage_private.h"
#include "storage_file.h"
#include "storage_data_mng.h"

/***************************************************************************/
/* Globel Variables                                                        */
/***************************************************************************/
extern LIBVSSHM_UINT_DATA_I storage_uint_data;
static int filename_changeflag = 0;
 
long long GetDiskfreeSpace(char* pDisk);
/*******************************************************************************
 * º¯ÊýÃû³Æ:InitDATAFile
 * ¹¦    ÄÜ: ´´½¨³õÊ¼»¯Êý¾ÝÎÄ¼þ
 * Ïà¹ØÎÄµµ:
 * º¯ÊýÀàÐÍ: ÎÞ
 * ²Î    Êý: ÎÞ
 * ²ÎÊýÃû³Æ            ÀàÐÍ                             ÊäÈë/Êä³ö       ÃèÊö
 * -----------------------------------------------------------------------------
 * ·µ»ØÖµ:
 * Ëµ   Ã÷:   ³É¹¦/²»³É¹¦ 0/1
 *******************************************************************************/
succeed_type InitDataFile(void)
{
#define STORAGE_FUNCTION_NAME "InitDataFile"
	//int iRet = STORAGE_FAIL;
	char cCurPwd[100] = {0};
	GetCurPwdName(cCurPwd);/*±£´æµ±Ç°Ä¿Â¼ ¹©·µ»ØÊ¹ÓÃ*/
	CreateIdxDirMulti(DATA_DIR_NAME_ROOT);
	ChdirInIdxDirMulti(DATA_DIR_NAME_ROOT);
	// ´´½¨diÎÄ¼þ
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_DI))
		CreateIdxDirSingle(DATA_DIR_NAME_DI);
	ChdirInIdxDirSingle(DATA_DIR_NAME_DI);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_H264))
		CreateIdxDirSingle(DATA_DIR_NAME_H264);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MPEG4))
		CreateIdxDirSingle(DATA_DIR_NAME_MPEG4);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MJPEG))
		CreateIdxDirSingle(DATA_DIR_NAME_MJPEG);
	ChdirOutIdxDirSingle();
	/*   // ´´½¨´³ºìµÆÎÄ¼þ
	     if(!ExistIdxDirSingle_if(DATA_DIR_NAME_RED))
	     CreateIdxDirSingle(DATA_DIR_NAME_RED);
	     ChdirInIdxDirSingle(DATA_DIR_NAME_RED);
	     if(!ExistIdxDirSingle_if(DATA_DIR_NAME_H264))
	     CreateIdxDirSingle(DATA_DIR_NAME_H264);
	     if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MPEG4))
	     CreateIdxDirSingle(DATA_DIR_NAME_MPEG4);
	     if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MJPEG))
	     CreateIdxDirSingle(DATA_DIR_NAME_MJPEG);
	     ChdirOutIdxDirSingle();
	// ´´½¨Ñ¹»ÆÏßÎÄ¼þ
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_YELLOW))
	CreateIdxDirSingle(DATA_DIR_NAME_YELLOW);
	ChdirInIdxDirSingle(DATA_DIR_NAME_YELLOW);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_H264))
	CreateIdxDirSingle(DATA_DIR_NAME_H264);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MPEG4))
	CreateIdxDirSingle(DATA_DIR_NAME_MPEG4);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MJPEG))
	CreateIdxDirSingle(DATA_DIR_NAME_MJPEG);
	ChdirOutIdxDirSingle();
	// ´´½¨³¬ËÙÎÄ¼þ
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_SPEED))
	CreateIdxDirSingle(DATA_DIR_NAME_SPEED);
	ChdirInIdxDirSingle(DATA_DIR_NAME_SPEED);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_H264))
	CreateIdxDirSingle(DATA_DIR_NAME_H264);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MPEG4))
	CreateIdxDirSingle(DATA_DIR_NAME_MPEG4);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MJPEG))
	CreateIdxDirSingle(DATA_DIR_NAME_MJPEG);
	ChdirOutIdxDirSingle();
	// ´´½¨ÅÅ³ÌÎÄ¼þ
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_TIME))
	CreateIdxDirSingle(DATA_DIR_NAME_TIME);
	ChdirInIdxDirSingle(DATA_DIR_NAME_TIME);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_H264))
	CreateIdxDirSingle(DATA_DIR_NAME_H264);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MPEG4))
	CreateIdxDirSingle(DATA_DIR_NAME_MPEG4);
	if(!ExistIdxDirSingle_if(DATA_DIR_NAME_MJPEG))
	CreateIdxDirSingle(DATA_DIR_NAME_MJPEG);
	ChdirOutIdxDirSingle();
	 */
	ChdirOutIdxDirMulti(DATA_DIR_NAME_ROOT);
	//·µ»Øµ½³õÊ¼±£´æµÄÂ·¾¶
	ChdirInIdxDirMulti(cCurPwd);
	return succeed_type_succeed;
#undef STORAGE_FUNCTION_NAME
}



/*******************************************************************************
 * º¯ÊýÃû³Æ: GenerateDataDirName_ByEventAndVideoType
 * ¹¦    ÄÜ: Í¨¹ýÊÂ¼þ¼°ÊÓÆÁ±àÂë¸ñÊ½Éú³ÉÎÄ¼þÃû
 * Ïà¹ØÎÄµµ:
 * º¯ÊýÀàÐÍ: ÎÞ
 * ²Î    Êý: ÎÞ
 * ²ÎÊýÃû³Æ            ÀàÐÍ                             ÊäÈë/Êä³ö       ÃèÊö
 * -----------------------------------------------------------------------------
 * ·µ»ØÖµ:
 * Ëµ   Ã÷:   ³É¹¦/²»³É¹¦ 0/1
 *******************************************************************************/
succeed_type GenerateDataDirName_ByEventAndVideoType(enStorageEventType_T enStorageEvent, int enStorageCodeType, char* pcDataDirName)
{
#define STORAGE_FUNCTION_NAME "GenerateDataDirName_ByEventAndVideoType"
	if(NULL == pcDataDirName)
	{
		printf("\nFile:%s, Line:%d,Func:%s:pcDataDirName is NULL!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	memset(pcDataDirName, 0 ,strlen(pcDataDirName));
	switch(enStorageEvent)
	{
		case  INDEX_EVENT_DI:
			switch(enStorageCodeType)
			{
				case VIDEO_TYPE_H264:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_DI,DATA_DIR_NAME_H264);
					break;
				case VIDEO_TYPE_MJPEG:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_DI,DATA_DIR_NAME_MJPEG);
					break;
				case VIDEO_TYPE_MPEG4:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_DI,DATA_DIR_NAME_MPEG4);
					break;
				default:
					printf("\nFile:%s, Line:%d,Func:%s:storageCodeType Nodefined!!!",
							__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
					break;
			}
			break;
		case  INDEX_EVENT_RED:
			switch(enStorageCodeType)
			{
				case VIDEO_TYPE_H264:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_RED,DATA_DIR_NAME_H264);
					break;
				case VIDEO_TYPE_MJPEG:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_RED,DATA_DIR_NAME_MJPEG);
					break;
				case VIDEO_TYPE_MPEG4:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_RED,DATA_DIR_NAME_MPEG4);
					break;
				default:
					printf("\nFile:%s, Line:%d,Func:%s:storageCodeType Nodefined!!!",
							__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
					break;
			}
			break;
		case  INDEX_EVENT_YELLOW:
			switch(enStorageCodeType)
			{
				case VIDEO_TYPE_H264:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_YELLOW,DATA_DIR_NAME_H264);
					break;
				case VIDEO_TYPE_MJPEG:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_YELLOW,DATA_DIR_NAME_MJPEG);
					break;
				case VIDEO_TYPE_MPEG4:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_YELLOW,DATA_DIR_NAME_MPEG4);
					break;
				default:
					printf("\nFile:%s, Line:%d,Func:%s:storageCodeType Nodefined!!!",
							__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
					break;
			}
			break;
		case  INDEX_EVENT_SPEED:
			switch(enStorageCodeType)
			{
				case VIDEO_TYPE_H264:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_SPEED,DATA_DIR_NAME_H264);
					break;
				case VIDEO_TYPE_MJPEG:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_SPEED,DATA_DIR_NAME_MJPEG);
					break;
				case VIDEO_TYPE_MPEG4:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_SPEED,DATA_DIR_NAME_MPEG4);
					break;
				default:
					printf("\nFile:%s, Line:%d,Func:%s:storageCodeType Nodefined!!!",
							__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
					break;
			}
			break;
		case  INDEX_EVENT_TIME:
			switch(enStorageCodeType)
			{
				case VIDEO_TYPE_H264:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_TIME,DATA_DIR_NAME_H264);
					break;
				case VIDEO_TYPE_MJPEG:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_TIME,DATA_DIR_NAME_MJPEG);
					break;
				case VIDEO_TYPE_MPEG4:
					sprintf(pcDataDirName, "%s/%s/%s/",DATA_DIR_NAME_ROOT,DATA_DIR_NAME_TIME,DATA_DIR_NAME_MPEG4);
					break;
				default:
					printf("\nFile:%s, Line:%d,Func:%s:storageCodeType Nodefined!!!",
							__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
					break;
			}
			break;
			//case  INDEX_EVENT_INVALID:
		default:
			printf("\nFile:%s, Line:%d,Func:%s:enStorageEvent is INVALID!!",
					__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
	}
	return succeed_type_succeed;
#undef  STORAGE_FUNCTION_NAME
}
/*
   Êý¾Ý±£´æº¯Êý
 */


/*******************************************************************************
 * º¯ÊýÃû³Æ: AddRecIntoDataFile
 * ¹¦    ÄÜ: Ìí¼ÓÒ»¸öË÷Òý¼ÇÂ¼
 * Ïà¹ØÎÄµµ:
 * º¯ÊýÀàÐÍ: ÎÞ
 * ²Î    Êý: ÎÞ
 * ²ÎÊýÃû³Æ            ÀàÐÍ                             ÊäÈë/Êä³ö       ÃèÊö
 * -----------------------------------------------------------------------------
 * ·µ»ØÖµ:
 * Ëµ   Ã÷:   ³É¹¦/²»³É¹¦ 0/1
 *******************************************************************************/
static int Count = 0;
succeed_type AddRecIntoDataFile(STORAGE_PRIVATE_HANDLE *phandle, LIBVSSHM_UINT_DATA_I *pvsshm_uint_data)
{
#define STORAGE_FUNCTION_NAME "AddRecIntoDataFile"
	static count  = 0;
	FILE* pFilefd;
	int iRet = -1;
	int fFrameSize = 0;
	char acDataDirName[STORAGE_IDX_DIR_LENGTH_MAX] = {0};
	long long DiskfreeSpace = 0;
//	struct timeval timea;
//	struct timeval timeb;
	if(NULL == phandle->afile_name || NULL == pvsshm_uint_data)
	{
		printf("\nFile:%s, Line:%d,Func:%s:phandle->afile_name||pvsshm_uint_data is INVALID!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	//fFrameSize = pvsshm_uint_data->vsshm_header.data_len + sizeof(pvsshm_uint_data->vsshm_header);
	//SYS_INFO("------------------fFrameSize = %d\r\n", fFrameSize);
#if 0
	{
		unsigned char tempbuf[10240];
		struct sockaddr_in sockaddr; //åœ°å€ç»“æž„ä½“
		memset(tempbuf, 0x00, 10240);
		if (phandle->sockfd > 0)
		{
#if 1
				unsigned int lensend = 10240;
				sockaddr.sin_family = AF_INET;
				sockaddr.sin_port = htons(9999);
				sockaddr.sin_addr.s_addr = inet_addr("192.168.0.234");
				bzero(&(sockaddr.sin_zero),0);

				if (SOCKET_SEND( phandle->sockfd, tempbuf, &lensend, &sockaddr) == succeed_type_succeed)
				{
						SYS_INFO("socketsend retlen = %d.\r\n", lensend);
				}
				else
				{
						SYS_INFO("error socketsend retlen = %d.\r\n", lensend);
				}
#endif
		}
	}
#endif
#if 0
	if((iRet = fwrite(pvsshm_uint_data->vsshm_context, fFrameSize, 1, pFilefd)) <= 0)
	{
		printf("\nFile:%s, Line:%d,Func:%s:fwrite failed!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		fclose(pFilefd);
		return succeed_type_failed;
	}
	fclose(pFilefd);
#endif
#if 1
//	gettimeofday(&timea);
	if(NULL == phandle->store_fd)
	{
	//	long long lltime;
	//	libvsshm_generatetimestamp(&lltime);
	//	SYS_INFO("lltime = %lld\r\n",lltime);
	//	sprintf(phandle->afile_name, "%lld", lltime);

		sprintf(phandle->afile_name, "%lld", pvsshm_uint_data->vsshm_header.timestamp);
		GenerateDataDirName_ByEventAndVideoType(INDEX_EVENT_DI, VIDEO_TYPE_H264, acDataDirName);
		strcat(acDataDirName, phandle->afile_name);
		SYS_WARNING("acDataDirName = %s",acDataDirName);
		phandle->store_fd = fopen(acDataDirName,"ab+");
		filename_changeflag = 1;
		if(NULL == phandle->store_fd)
		{
			filename_changeflag = 0;
			printf("\nFile:%s, Line:%d,Func:%s:fopen %s error!!",
					__FILE__, __LINE__,STORAGE_FUNCTION_NAME,acDataDirName);
			fclose(phandle->store_fd);
			return succeed_type_failed;
		}

	}
	else
	{
		long long lpre_time;
	   	lpre_time = atoll(phandle->afile_name); 
		if(pvsshm_uint_data->vsshm_header.timestamp - lpre_time > phandle->storage_store_duration \
				&& I_P_FLAG_I == pvsshm_uint_data->vsshm_header.i_p_flag)
		{
			fclose(phandle->store_fd);
			sprintf(phandle->afile_name, "%lld", pvsshm_uint_data->vsshm_header.timestamp);
			GenerateDataDirName_ByEventAndVideoType(INDEX_EVENT_DI, VIDEO_TYPE_H264, acDataDirName);
			strcat(acDataDirName, phandle->afile_name);
			SYS_WARNING("acDataDirName = %s",acDataDirName);
			phandle->store_fd = fopen(acDataDirName,"ab+");
			filename_changeflag = 1;
			if(NULL == phandle->store_fd)
			{
				filename_changeflag = 0;
				printf("\nFile:%s, Line:%d,Func:%s:fopen %s error!!",
						__FILE__, __LINE__,STORAGE_FUNCTION_NAME,acDataDirName);
				fclose(phandle->store_fd);
				return succeed_type_failed;
			}
		}
	}
	if(filename_changeflag)
	{
		if(AddRecIntoIdxFile(phandle,&(pvsshm_uint_data->vsshm_header)) != succeed_type_succeed)
		{
			SYS_ERROR("\n\r---------AddRecIntoIdxFile failed---------\r\n");
			return succeed_type_failed;
		}
		filename_changeflag = 0;
	}

	fFrameSize = pvsshm_uint_data->vsshm_header.data_len;
	DiskfreeSpace = GetDiskfreeSpace(STORAGE_DIR_NAME_ROOT);
	if(DiskfreeSpace - fFrameSize >= 0)
	{
		if((iRet = fwrite(pvsshm_uint_data->vsshm_context, 1, fFrameSize, phandle->store_fd)) <= 0)
		{
			printf("\nFile:%s, Line:%d,Func:%s:fwrite failed!!",
					__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
			fclose(phandle->store_fd);
			return succeed_type_failed;
		}
		fflush(phandle->store_fd);
	}
	else
	{
		SYS_WARNING("Sd Fully!! Please replace it with a new one!!\r\n"); 
		return succeed_type_failed;
	}
	//SYS_WARNING("storage fFrameSize = %d , iRet = %d \r\n",fFrameSize ,iRet);
//	fflush(phandle->store_fd);
//	gettimeofday(&timeb);
//	Count ++;
//	if(0 == (Count % 100))
//	{
//		SYS_WARNING("storage[%d] fFrameSize=%d , iRet=%d ,time=%ld \r\n",Count,fFrameSize ,iRet,\
//				(timeb.tv_sec - timea.tv_sec)*1000 + timeb.tv_usec - timea.tv_usec );
//	}
#endif
	return succeed_type_succeed;
#undef STORAGE_FUNCTION_NAME
}

long long GetDiskfreeSpace(char* pDisk)
{
	long long freespace = 0;
	struct statfs disk_statfs;
	if(statfs(pDisk, &disk_statfs) >= 0)
	{
		freespace = (((long long)disk_statfs.f_bsize * (long long)disk_statfs.f_bfree)/(long long)1024);
	}
	return freespace;
}


void pthread_datasave_func(void *arg)
{
	STORAGE_PRIVATE_HANDLE *phandle = (STORAGE_PRIVATE_HANDLE *)arg;
	while( 0 == phandle->IsStorageSysQuit )
	{
		if(1 == phandle->IsStorageData_Available)
		{
		//	AddRecIntoDataFile(phandle,phandle->pstorage_video_data);		
			AddRecIntoDataFile(phandle,&storage_uint_data);		
		}
		pthread_mutex_lock(&(phandle->g_mutex_t));
		phandle->IsStorageData_Available = 0;
		pthread_mutex_unlock(&(phandle->g_mutex_t));
	}
	pthread_exit(NULL);
	SYS_WARNING("pthread_datasave_func exit!!");
}
/********************************Ô´ÎÄ¼þ½áÊø*************************************/
