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

#include <assert.h>
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
	CreateIdxDirMulti(DATA_DIR_JPEG_NAME_ROOT);   //video dir
	CreateIdxDirMulti(DATA_DIR_VIDEO_NAME_ROOT);  //jpeg dir

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
#if 0
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
#endif
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
		if (GenerateFileName_ByCurTime(acDataDirName, "avi") != 0)
		{
			SYS_ERROR("GenerateFileName_ByCurTime failed\r\n");
			return succeed_type_failed;
		}

		ChdirInIdxDirMulti(DATA_DIR_VIDEO_NAME_ROOT);
//		sprintf(phandle->afile_name, "%lld", pvsshm_uint_data->vsshm_header.timestamp);
		//	GenerateDataDirName_ByEventAndVideoType(INDEX_EVENT_DI, VIDEO_TYPE_H264, acDataDirName);
//		strcat(acDataDirName, phandle->afile_name);
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
	if (phandle->Is_encode_stop == 1)
	{
		fclose(phandle->store_fd);
	}
#if 0
	else
	{
		long long lpre_time;
		lpre_time = atoll(phandle->afile_name); 
		if(pvsshm_uint_data->vsshm_header.timestamp - lpre_time > phandle->storage_store_duration \
				&& I_P_FLAG_I == pvsshm_uint_data->vsshm_header.i_p_flag)
		{
			fclose(phandle->store_fd);
			sprintf(phandle->afile_name, "%lld", pvsshm_uint_data->vsshm_header.timestamp);
			//	GenerateDataDirName_ByEventAndVideoType(INDEX_EVENT_DI, VIDEO_TYPE_H264, acDataDirName);
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
#endif

	if(filename_changeflag)
	{
#if 0
		if(AddRecIntoIdxFile(phandle,&(pvsshm_uint_data->vsshm_header)) != succeed_type_succeed)
		{
			SYS_ERROR("\n\r---------AddRecIntoIdxFile failed---------\r\n");
			return succeed_type_failed;
		}
#endif
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
	//	gettimeofday(&timeb)
	//	Count ++;
	//	if(0 == (Count % 100))
	//	{
	//		SYS_WARNING("storage[%d] fFrameSize=%d , iRet=%d ,time=%ld \r\n",Count,fFrameSize ,iRet,\
	//				(timeb.tv_sec - timea.tv_sec)*1000 + timeb.tv_usec - timea.tv_usec );
	//	}
#endif
	SYS_INFO("storage frame  count is %d...\r\n", count++);
	return succeed_type_succeed;
#undef STORAGE_FUNCTION_NAME
}

long long GetDiskfreeSpace(char* pDisk)
{
	long long freespace = 0;
	struct statfs disk_statfs;
	if(statfs(pDisk, &disk_statfs) >= 0)
	{
		//freespace = (((long long)disk_statfs.f_bsize * (long long)disk_statfs.f_bfree)/(long long)1024);
		freespace = (((long long)disk_statfs.f_bavail * (long long)disk_statfs.f_bsize)>>10);
	}
	return freespace;
}

succeed_type DoStorageJpeg(STORAGE_PRIVATE_HANDLE *phandle)
{
	FILE *fp_jpeg;
	char acDataDirName[STORAGE_IDX_DIR_LENGTH_MAX] = {0};
	char jpeg_file_name[STORAGE_IDX_DIR_LENGTH_MAX] = {0};
	long long DiskfreeSpace = 0;
	int iRet;
	LIBASSHM_UINT_DATA jpeg_uint_data;
	char cCurPwd[100] = {0};
	GetCurPwdName(cCurPwd);/*¿¿¿¿¿¿ ¿¿¿¿¿*/

	//0.check the sd card free space
	//1.get current date and time as the file name
	//2.put the jpg data into the file
	if (GenerateFileName_ByCurTime(acDataDirName, "jpg") != 0)
	{
		SYS_ERROR("GenerateFileName_ByCurTime failed\r\n");
		return succeed_type_failed;
	}
	SYS_INFO("before GET DATA\r\n");

	ChdirInIdxDirMulti(DATA_DIR_JPEG_NAME_ROOT);

	ASSHM_GET_DATA(phandle->passhm_obj, phandle->jpeg_buffer_id, &jpeg_uint_data);

	DiskfreeSpace = GetDiskfreeSpace(STORAGE_DIR_NAME_ROOT);
	if(DiskfreeSpace - jpeg_uint_data.data_len >= 0)
	{
		FILE *fp;
		//sprintf(jpeg_file_name, "/mnt/mmc/jpeg/%s", acDataDirName);
		SYS_INFO("jpg file name is %s\r\n", acDataDirName);
		fp = fopen(acDataDirName, "wb");
		assert(fp != NULL);

		SYS_INFO("before fwrite\r\n");
		SYS_INFO("jpg file data len is  %d\r\n", jpeg_uint_data.data_len);


		if((iRet = fwrite(jpeg_uint_data.asshm_context, 1, jpeg_uint_data.data_len, fp)) <= 0)
		{
			printf("\nFile:%s, Line:%d,Func:%s:fwrite failed!!",
					__FILE__, __LINE__);
			fclose(fp);
			return succeed_type_failed;
		}
		SYS_INFO("after fwrite\r\n");
		fflush(fp);
		fclose(fp);
	}
	else
	{
		SYS_WARNING("Sd Fully!! Please replace it with a new one!!\r\n"); 
		return succeed_type_failed;
	}
	ChdirInIdxDirMulti(cCurPwd);


	return succeed_type_succeed;


}

void pthread_datasave_func(void *arg)
{
	STORAGE_PRIVATE_HANDLE *phandle = (STORAGE_PRIVATE_HANDLE *)arg;
	SYS_INFO("in datasave...\r\n");
	while( 0 == phandle->IsStorageSysQuit )
	{
		if(1 == phandle->IsStorageData_Available)  //video frame data save
		{
			//	AddRecIntoDataFile(phandle,phandle->pstorage_video_data);		
			AddRecIntoDataFile(phandle,&storage_uint_data);		
		}
		if(1 == phandle->IsJpegStore)
		{
			DoStorageJpeg(phandle);
		}
		//	pthread_mutex_lock(&(phandle->g_mutex_t));
		phandle->IsStorageData_Available = 0;
		phandle->IsJpegStore = 0;
		//	pthread_mutex_unlock(&(phandle->g_mutex_t));
	}
	pthread_exit(NULL);
	SYS_WARNING("pthread_datasave_func exit!!");
}
/********************************Ô´ÎÄ¼þ½áÊø*************************************/
