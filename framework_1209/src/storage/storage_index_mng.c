/*******************************************************************************
 * COPYRIGHT
 ********************************************************************************
/*
 * =====================================================================================
 *
 *       Filename:  storage_index_mng.c
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
#include "storage_index_mng.h"

/***************************************************************************/
/* Globel Variables                                                        */
/***************************************************************************/
static stStorageIndex_T  g_astIndex[INDEX_EVENT_NUM_MAX] = {{0}};
static stStorageIndex_T* g_apstIndex[INDEX_EVENT_NUM_MAX] = {NULL};

static pstIdxHandle g_apstHandle[INDEX_EVENT_NUM_MAX] = {NULL};


#define g_pstRedHandle g_apstHandle[INDEX_EVENT_RED]
#define g_pstYellowHandle g_apstHandle[INDEX_EVENT_YELLOW]
#define g_pstSpeedHandle g_apstHandle[INDEX_EVENT_SPEED]
#define g_pstTimeHandle g_apstHandle[INDEX_EVENT_TIME]

#define GetIdxHandl_ByEvent(event) (g_apstHandle[event])
#define GetIdxNum_ByEvent(event) (g_apstIndex[event])

#define Str_nCpy(dst,src)  {strncpy(dst,src,strlen(src)); dst[strlen(src)] = '\0';}

#define Str_Year(year,str) {strncpy(year,str,4);year[4]='\0';}
#define Str_Month(month,str) {strncpy(month,str+4,2);month[2]='\0';}
#define Str_Day(day,str) {strncpy(day,str+4+2,2);day[2]='\0';}
#define Str_Hour(hour,str) {strncpy(hour,str+4+2+2,2);hour[2]='\0';}
#define Str_Min(min,str) {strncpy(min,str+4+2+2+2,2);min[2]='\0';}
#define Str_Sec(sec,str) {strncpy(sec,str+4+2+2+2+2,2);sec[2]='\0';}
#define Str_Usec(usec,str) {strncpy(usec,str+4+2+2+2+2+2,3);usec[3]='\0';}
/*******************************************************************************
 * 函数名称:InitIdxMemAlloc
 * 功    能: 资源分配
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:  存在/不存在 0/1
 *******************************************************************************/
succeed_type InitIdxMemAlloc(void)
{
#define STORAGE_FUNCTION_NAME "InitIdxMemAlloc"
	int iLoop = 0;
	for(iLoop = 0; iLoop < INDEX_EVENT_NUM_MAX; iLoop ++)
	{
		if(g_apstHandle[iLoop] == NULL)
		{
			g_apstHandle[iLoop] = (pstIdxHandle)calloc(STORAGE_IDX_CONTEXT_VOL_MAX, sizeof(stStorageIdxNode_T));
			if( NULL == g_apstHandle[iLoop])
			{
				printf("\nFile:%s, Line:%d,Func:%s:calloc error\n",
						__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
				ExitIdxMemFree();
				return succeed_type_failed;
			}
		}
		storage_index_init(&g_astIndex[iLoop], 1);
		g_apstIndex[iLoop] = storage_index_create(&g_astIndex[iLoop], 1, 0, STORAGE_IDX_CONTEXT_VOL_MAX-1);
		if( NULL == g_apstIndex[iLoop])
		{
			printf("\nFile:%s, Line:%d,Func:%s:storage_index_create error\n",
					__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
			ExitIdxMemFree();
			return succeed_type_failed;
		}
	}
	return succeed_type_succeed;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称:ExitIdxMemFree
 * 功    能: 资源清空
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:  存在/不存在 0/1
 *******************************************************************************/
void ExitIdxMemFree(void)
{
#define STORAGE_FUNCTION_NAME "ExitIdxMemFree"
	int iLoop = 0;
	for(iLoop = 0; iLoop < INDEX_EVENT_NUM_MAX; iLoop ++)
	{
		if(NULL != g_apstHandle[iLoop])
		{
			free(g_apstHandle[iLoop]);
			g_apstHandle[iLoop] = NULL;
		}
		if(NULL != g_apstIndex[iLoop])
		{
			storage_index_destroy(g_apstIndex[iLoop]);
			g_apstHandle[iLoop] = NULL;
		}
	}
#undef STORAGE_FUNCTION_NAME
}

/******************************************************************************
 * 函数名称: GetIdxNumfromTables
 * 功    能: 获取可用的索引号
 * 函数类型:
 * 参    数:
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 * 函数返回:
 * 说    明:
 *******************************************************************************/
succeed_type  GetIdxNumfromTables(enStorageEventType_T stiSeq, unsigned short *pusIndexNum)
{
#define STORAGE_FUNCTION_NAME  "GetIdxNumfromTables"
	unsigned short       usPortTemp = STORAGE_INDEX_NOFREE;
	if( INDEX_EVENT_START == stiSeq)
	{
		printf("\nFile:%s, Line:%d,Func:%s:Input stiSeq is Invalid\n",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	/************************************************************************/
	/* 分配一个号                                                                    */
	/************************************************************************/
	usPortTemp = storage_index_alloc(g_apstIndex[stiSeq]);
	if (STORAGE_INDEX_NOFREE == usPortTemp) /* 没有空闲索引 */
	{
		printf("\nFile:%s, Line:%d,Func:%s:return for there is no vacant index\n",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	*pusIndexNum = usPortTemp;
	return succeed_type_succeed;
#undef STORAGE_FUNCTION_NAME
}

/******************************************************************************
 * 函数名称: UnGetIdxNumfromTables
 * 功    能: 释放指定的索引号
 * 函数类型:
 * 参    数:
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 * 函数返回:
 * 说    明:
 *******************************************************************************/
succeed_type  UnGetIdxNumfromTables(enStorageEventType_T stiSeq, unsigned short *pusIndexNum)
{
#define STORAGE_FUNCTION_NAME "UnGetIdxNumfromTables"
	short        uiRet = 0;
	/************************************************************************/
	/* 释放端口                                                                     */
	/************************************************************************/
	uiRet = storage_index_free(g_apstIndex[stiSeq],*pusIndexNum);
	if (STORAGE_INDEX_ERROR == uiRet) /* 没有空闲索引 */
	{
		printf("\nFile:%s, Line:%d,Func:%s:return for free error\n",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	return succeed_type_succeed;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称:InitIdxFile
 * 功    能: 删除指定文件
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
succeed_type InitIdxFile(void)
{
#define STORAGE_FUNCTION_NAME "InitIdxFile"
	//int iRet = STORAGE_FAIL;
	char cCurPwd[100] = {0};
	GetCurPwdName(cCurPwd);/*保存当前目录 供返回使用*/
	InitIdxMemAlloc();
	CreateIdxDirMulti(INDEX_DIR_NAME_ROOT);
	ChdirInIdxDirMulti(INDEX_DIR_NAME_ROOT);
	if(!ExistIdxDirSingle_if(INDEX_DIR_NAME_DI))
		CreateIdxDirSingle(INDEX_DIR_NAME_DI);
	ChdirInIdxDirSingle(INDEX_DIR_NAME_DI);
	if(!ExistIdxFileSingle_if(INDEX_DIR_NAME_INDEX));
	CreateIdxFileSingle(INDEX_DIR_NAME_INDEX);
	ChdirOutIdxDirSingle();
	/*    if(!ExistIdxDirSingle_if(INDEX_DIR_NAME_RED))
	      CreateIdxDirSingle(INDEX_DIR_NAME_RED);
	      ChdirInIdxDirSingle(INDEX_DIR_NAME_RED);
	      if(!ExistIdxFileSingle_if(INDEX_DIR_NAME_INDEX));
	      CreateIdxFileSingle(INDEX_DIR_NAME_INDEX);
	      ChdirOutIdxDirSingle();
	      if(!ExistIdxDirSingle_if(INDEX_DIR_NAME_YELLOW))
	      CreateIdxDirSingle(INDEX_DIR_NAME_YELLOW);
	      ChdirInIdxDirSingle(INDEX_DIR_NAME_YELLOW);
	      if(!ExistIdxFileSingle_if(INDEX_DIR_NAME_INDEX));
	      CreateIdxFileSingle(INDEX_DIR_NAME_INDEX);
	      ChdirOutIdxDirSingle();
	      if(!ExistIdxDirSingle_if(INDEX_DIR_NAME_SPEED))
	      CreateIdxDirSingle(INDEX_DIR_NAME_SPEED);
	      ChdirInIdxDirSingle(INDEX_DIR_NAME_SPEED);
	      if(!ExistIdxFileSingle_if(INDEX_DIR_NAME_INDEX));
	      CreateIdxFileSingle(INDEX_DIR_NAME_INDEX);
	      ChdirOutIdxDirSingle();
	      if(!ExistIdxDirSingle_if(INDEX_DIR_NAME_TIME))
	      CreateIdxDirSingle(INDEX_DIR_NAME_TIME);
	      ChdirInIdxDirSingle(INDEX_DIR_NAME_TIME);
	      if(!ExistIdxFileSingle_if(INDEX_DIR_NAME_INDEX));
	      CreateIdxFileSingle(INDEX_DIR_NAME_INDEX);
	      ChdirOutIdxDirSingle();
	 */
	ChdirOutIdxDirMulti(INDEX_DIR_NAME_ROOT);
	ChdirInIdxDirMulti(cCurPwd);
	return succeed_type_succeed;
#undef STORAGE_FUNCTION_NAME
}

/**
 **/
void ExitIdxFile(void)
{
#define STORAGE_FUNCTION_NAME "ExitIdxFile"
	ExitIdxMemFree();
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称:DevideIdxName_ByTime
 * 功    能: 删除指定文件
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
void DevideIdxName_ByTime(char* pcMergeFileName, stStorageNameDevide_T* pstDevideFileName)
{
#define STORAGE_FUNCTION_NAME "DevideIdxName_ByTime"
	if(NULL == pcMergeFileName || NULL == pstDevideFileName)
	{
		printf("\nFile:%s, Line:%d,Func:%s:pcMergeFileName|pstDevideFileName is NULL!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return;
	}
	Str_Year(pstDevideFileName->aucYear, pcMergeFileName);
	Str_Month(pstDevideFileName->aucMonth, pcMergeFileName);
	Str_Day(pstDevideFileName->aucDay, pcMergeFileName);
	Str_Hour(pstDevideFileName->aucHour, pcMergeFileName);
	Str_Min(pstDevideFileName->aucMin, pcMergeFileName);
	Str_Sec(pstDevideFileName->aucSec, pcMergeFileName);
	Str_Usec(pstDevideFileName->aucUsec, pcMergeFileName);
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称:MergeIdxName_ByTime
 * 功    能: 合并索引文件名
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
void MergeIdxName_ByTime(char* pcMergeFileName, stStorageNameDevide_T* pstDevideFileName)
{
#define STORAGE_FUNCTION_NAME "MergeIdxName_ByTime"
	if(NULL == pcMergeFileName || NULL == pstDevideFileName)
	{
		printf("\nFile:%s, Line:%d,Func:%s:pcMergeFileName|pstDevideFileName is NULL!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return;
	}
	sprintf(pcMergeFileName,"%4d%2d%2d%2d%2d%2d%3d",
			atoi(pstDevideFileName->aucYear),atoi(pstDevideFileName->aucMonth),atoi(pstDevideFileName->aucDay),
			atoi(pstDevideFileName->aucHour),atoi(pstDevideFileName->aucMin),atoi(pstDevideFileName->aucSec),
			atoi(pstDevideFileName->aucUsec));
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称:GetFileSize_ByStruct
 * 功    能: 分解LocDir
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
long GetFileSize_ByStruct(char* pcFileName)
{
#define STORAGE_FUNCTION_NAME "GetFileSize_ByStruct"
	long iRvSumSize;
	long iPos;
	iPos = GetFileSize_ByByte(pcFileName);
	if(-1 == iPos)
	{
		printf("\nFile:%s, Line:%d,Func:%s:GetFileSize_Byte Failed!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return -1;
	}
	iRvSumSize = iPos / sizeof(stStorageIdxNode_T);
	return iRvSumSize;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称:GetInfoFromIdxLocDir
 * 功    能: 分解LocDir
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
void GetInfoFromIdxLocDir(char const acLocName[], char acFileName[], char acDirName[], enStorageEventType_T* penEventType, \
		enStorageCodeType_T* penCodeType, char acFileNameContext[])
{
#define STORAGE_FUNCTION_NAME "GetInfoFromIdxLocDir"
	int iLoop = 0;
	int iLoopT = 0;
	char* pcDevideDirTemp = NULL;
	char* pcDevideFileTemp = NULL;
	char  acLocNameTemp[STORAGE_IDX_LOC_LENGTH_MAX] = {0};
	char  acFileNameTemp[STORAGE_IDX_NAME_LENGTH_MAX] = {0};
	char* pcDirNameTemp = acDirName;
	char aacDevideLocDir[STORAGE_IDX_DIR_LEVEL_MAX][STORAGE_IDX_NAME_LENGTH_MAX] = {{0}};
	if(NULL == acLocName)
	{
		printf("\nFile:%s, Line:%d,Func:%s:acLocName is NULL!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return;
	}
	Str_nCpy(acLocNameTemp,acLocName);
	pcDevideDirTemp = strtok(acLocNameTemp,"/");
	for( iLoop = 0; iLoop < STORAGE_IDX_DIR_LEVEL_MAX && NULL != pcDevideDirTemp; iLoop++)
	{
		Str_nCpy(aacDevideLocDir[iLoop],pcDevideDirTemp);
		pcDevideDirTemp = strtok(NULL,"/");
	}
	iLoop --;
	if(NULL != acFileName)
	{
		Str_nCpy(acFileName,aacDevideLocDir[iLoop]);
	}
	if(NULL != acFileNameContext)
	{
		Str_nCpy(acFileNameTemp,aacDevideLocDir[iLoop]);
		pcDevideFileTemp = strtok(acFileNameTemp,".");
		Str_nCpy(acFileNameContext,pcDevideFileTemp);
	}
	if(NULL != acDirName)
	{
		if('/' == acLocName[0])
		{
			pcDirNameTemp[0] = '/';
			pcDirNameTemp++;
		}
		for( iLoopT = 0; iLoop < STORAGE_IDX_DIR_LEVEL_MAX && strlen(aacDevideLocDir[iLoopT+1]) != 0; iLoopT++)
		{
			Str_nCpy(pcDirNameTemp,aacDevideLocDir[iLoopT]);
			pcDirNameTemp[strlen(aacDevideLocDir[iLoopT])] = '/';
			pcDirNameTemp += strlen(aacDevideLocDir[iLoopT])+1;
		}
		*pcDirNameTemp = '\0';
	}
	if(NULL != penCodeType)
	{
		if(!strcmp(aacDevideLocDir[iLoop-1],INDEX_DIR_NAME_H264))
			*penCodeType = INDEX_CODE_H264;
		if(!strcmp(aacDevideLocDir[iLoop-1],INDEX_DIR_NAME_MPEG4))
			*penCodeType = INDEX_CODE_MPEG4;
		if(!strcmp(aacDevideLocDir[iLoop-1],INDEX_DIR_NAME_MJPEG))
			*penCodeType = INDEX_CODE_MJPEG;
		else
			*penCodeType = INDEX_CODE_START;
	}
	if(NULL != penEventType)
	{
		if(!strcmp(aacDevideLocDir[iLoop-2],INDEX_DIR_NAME_DI))
			*penEventType = INDEX_EVENT_DI;
		if(!strcmp(aacDevideLocDir[iLoop-2],INDEX_DIR_NAME_RED))
			*penEventType = INDEX_EVENT_RED;
		if(!strcmp(aacDevideLocDir[iLoop-2],INDEX_DIR_NAME_YELLOW))
			*penEventType = INDEX_EVENT_YELLOW;
		if(!strcmp(aacDevideLocDir[iLoop-2],INDEX_DIR_NAME_SPEED))
			*penEventType = INDEX_EVENT_SPEED;
		if(!strcmp(aacDevideLocDir[iLoop-2],INDEX_DIR_NAME_TIME))
			*penEventType = INDEX_EVENT_TIME;
		else
			*penCodeType = INDEX_EVENT_START;
	}
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称: AddRecIntoIdxFile
 * 功    能: 添加一个索引记录
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
succeed_type AddRecIntoIdxFile(STORAGE_PRIVATE_HANDLE *phandle, LIBVSSHM_UINT_HEADER *pvsshm_uint_header)
{
#define STORAGE_FUNCTION_NAME "AddRecIntoIdxFile"
	enStorageEventType_T enStorageEventTypeTemp = INDEX_EVENT_START;
	enStorageCodeType_T enStorageCodeTypeTemp = INDEX_CODE_START;
	unsigned short ucIndexNum = STORAGE_INDEX_ERROR;
	if(NULL == pvsshm_uint_header || NULL == phandle)
	{
		SYS_ERROR("\n\r------------------AddRecIntoIdxFile Get error data------------------------\r\n");
		return succeed_type_failed;
	}
	enStorageEventTypeTemp = INDEX_EVENT_DI; //暂时只做di事件处理
	enStorageCodeTypeTemp = INDEX_CODE_H264; //暂时只做h264格式处理
	if(succeed_type_succeed != GetIdxNumfromTables(enStorageEventTypeTemp, &ucIndexNum))
	{
		SYS_ERROR("\nFile:%s, Line:%d,Func:%s:GetIdxNumfromTables Failed!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	Str_nCpy(GetIdxHandl_ByEvent(enStorageEventTypeTemp)[ucIndexNum].acName, phandle->afile_name);
	Str_nCpy(GetIdxHandl_ByEvent(enStorageEventTypeTemp)[ucIndexNum].acLoc, "/root/data/di/h264/");
	GetIdxHandl_ByEvent(enStorageEventTypeTemp)[ucIndexNum].offset = 0;
	GetIdxHandl_ByEvent(enStorageEventTypeTemp)[ucIndexNum].start_time = pvsshm_uint_header->timestamp;
	GetIdxHandl_ByEvent(enStorageEventTypeTemp)[ucIndexNum].end_time = pvsshm_uint_header->timestamp;
	GetIdxHandl_ByEvent(enStorageEventTypeTemp)[ucIndexNum].acFlags[0] = 1;

	WritenRecIntoIdxFile((stStorageIdxNode_T*) &GetIdxHandl_ByEvent(enStorageEventTypeTemp)[ucIndexNum]);
	return succeed_type_succeed;
#undef  STORAGE_FUNCTION_NAME
}
/*******************************************************************************
 * 函数名称: GetIdxLocName_ByEvent
 * 功    能: 由事件生成索引目录名称
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
succeed_type GetIdxLocName_ByEvent(enStorageEventType_T enStorageEvent, char* pcIdxDirName)
{
#define STORAGE_FUNCTION_NAME "GetIdxLocName_ByEvent"
	if(NULL == pcIdxDirName)
	{
		printf("\nFile:%s, Line:%d,Func:%s:pcIdxDirName is NULL!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	switch(enStorageEvent)
	{
		case  INDEX_EVENT_DI:
			sprintf(pcIdxDirName, "%s/%s/index",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_DI);
			break;
		case  INDEX_EVENT_RED:
			sprintf(pcIdxDirName, "%s/%s/index",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_RED);
			break;
		case  INDEX_EVENT_YELLOW:
			sprintf(pcIdxDirName, "%s/%s/index",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_YELLOW);
			break;
		case  INDEX_EVENT_SPEED:
			sprintf(pcIdxDirName, "%s/%s/index",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_SPEED);
			break;
		case  INDEX_EVENT_TIME:
			sprintf(pcIdxDirName, "%s/%s/index",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_TIME);
			break;
			//case  INDEX_EVENT_INVALID:
		default:
			printf("\nFile:%s, Line:%d,Func:%s:enStorageEvent is INVALID!!",
					__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
	}
	return succeed_type_succeed;
#undef  STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称: GetIdxDirName_ByEvent
 * 功    能: 添加一个索引记录
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
succeed_type GetIdxDirName_ByEvent(enStorageEventType_T enStorageEvent, char* pcIdxDirName)
{
#define STORAGE_FUNCTION_NAME "GetIdxDirName_ByEvent"
	if(NULL == pcIdxDirName)
	{
		printf("\nFile:%s, Line:%d,Func:%s:pcIdxDirName is NULL!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	switch(enStorageEvent)
	{
		case  INDEX_EVENT_DI:
			sprintf(pcIdxDirName, "%s/%s/",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_DI);
			break;
		case  INDEX_EVENT_RED:
			sprintf(pcIdxDirName, "%s/%s/",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_RED);
			break;
		case  INDEX_EVENT_YELLOW:
			sprintf(pcIdxDirName, "%s/%s/",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_YELLOW);
			break;
		case  INDEX_EVENT_SPEED:
			sprintf(pcIdxDirName, "%s/%s/",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_SPEED);
			break;
		case  INDEX_EVENT_TIME:
			sprintf(pcIdxDirName, "%s/%s/",INDEX_DIR_NAME_ROOT,INDEX_DIR_NAME_TIME);
			break;
			//case  INDEX_EVENT_INVALID:
		default:
			printf("\nFile:%s, Line:%d,Func:%s:enStorageEvent is INVALID!!",
					__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
	}
	return succeed_type_succeed;
#undef  STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称: GetDirName_ByEvent
 * 功    能: 添加一个索引记录
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
succeed_type GetDirName_ByEvent(enStorageEventType_T enStorageEvent, char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "GetDirName_ByEvent"
	if(NULL == pcDirName)
	{
		printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	switch(enStorageEvent)
	{
		case  INDEX_EVENT_DI:
			sprintf(pcDirName, "%s",INDEX_DIR_NAME_DI);
			break;
		case  INDEX_EVENT_RED:
			sprintf(pcDirName, "%s",INDEX_DIR_NAME_RED);
			break;
		case  INDEX_EVENT_YELLOW:
			sprintf(pcDirName, "%s",INDEX_DIR_NAME_YELLOW);
			break;
		case  INDEX_EVENT_SPEED:
			sprintf(pcDirName, "%s",INDEX_DIR_NAME_SPEED);
			break;
		case  INDEX_EVENT_TIME:
			sprintf(pcDirName, "%s",INDEX_DIR_NAME_TIME);
			break;
			//case  INDEX_EVENT_INVALID:
		default:
			printf("\nFile:%s, Line:%d,Func:%s:enStorageEvent is INVALID!!",
					__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
	}
	return succeed_type_succeed;
#undef  STORAGE_FUNCTION_NAME
}

/*******************************************************************************
 * 函数名称: WritenRecIntoIdxFile
 * 功    能: 写入一个索引记录
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
succeed_type WritenRecIntoIdxFile(stStorageIdxNode_T* pstStorageIdxNode)
{
#define STORAGE_FUNCTION_NAME "WritenRecIntoIdxFile"
	int iRet = -1;
	FILE *pstFileFd = NULL;
	char acIdxDirNameTemp[STORAGE_IDX_LOC_LENGTH_MAX] = {0};
	if(NULL == pstStorageIdxNode)
	{
		printf("\nFile:%s, Line:%d,Func:%s:pstStorageIdxNode is NULL!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	if(succeed_type_succeed != GetIdxLocName_ByEvent(INDEX_EVENT_DI,acIdxDirNameTemp))
	{
		printf("\nFile:%s, Line:%d,Func:%s:GetIdxDirName_ByEvent is failed!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	if(!ExistIdxFileSingle_if(acIdxDirNameTemp))
		CreateIdxFileSingle(acIdxDirNameTemp);
	pstFileFd = fopen(acIdxDirNameTemp, "awb");
	if(NULL == pstFileFd)
	{
		printf("\nFile:%s, Line:%d,Func:%s:fopen failed!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		return succeed_type_failed;
	}
	if((iRet = fwrite(pstStorageIdxNode, sizeof(stStorageIdxNode_T), 1, pstFileFd)) == -1)
	{
		printf("\nFile:%s, Line:%d,Func:%s:fwrite failed!!",
				__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
		fclose(pstFileFd);
		return succeed_type_failed;
	}
	fclose(pstFileFd);
	return succeed_type_succeed;
#undef  STORAGE_FUNCTION_NAME
}
/*******************************************************************************
 * 函数名称: ReadRecFromIdxFile
 * 功    能: 写入一个索引记录
 * 相关文档:
 * 函数类型: 无
 * 参    数: 无
 * 参数名称            类型                             输入/输出       描述
 * -----------------------------------------------------------------------------
 * 返回值:
 * 说   明:   成功/不成功 0/1
 *******************************************************************************/
/*BOOL ReadRecFromIdxFile( OUT stStorageIdxNode_T* pstStorageIdxNode，
  IN  int iRecNum)
  {
#define STORAGE_FUNCTION_NAME "ReadRecFromIdxFile"
FILE *pstFileFd = NULL;
int iRet = -1;
char acLocNameTemp[STORAGE_IDX_LOC_LENGTH_MAX] = {0};
char acIdxDirNameTemp[STORAGE_IDX_LOC_LENGTH_MAX] = {0};
enStorageCodeType_T enStorageCodeTypeTemp = INDEX_CODE_INVALID;
enStorageEventType_T enStorageEventTypeTemp = INDEX_EVENT_INVALID;
if(NULL == pstStorageIdxNode)
{
printf("\nFile:%s, Line:%d,Func:%s:pstStorageIdxNode is NULL!!",
__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
return False;
}
Str_nCpy(acLocNameTemp, pstStorageIdxNode->acLoc);
GetInfoFromIdxLocDir(acLocNameTemp, NULL, NULL, \
&enStorageEventTypeTemp, &enStorageCodeTypeTemp, NULL);
GetIdxDirName_ByEvent(enStorageEventTypeTemp, acIdxDirNameTemp);
if(!ExistIdxFileSingle_if(acIdxDirNameTemp))
CreateIdxFileSingle(acIdxDirNameTemp);
pstFileFd = fopen(acIdxDirNameTemp, "awb");
if(NULL == pstFileFd)
{
printf("\nFile:%s, Line:%d,Func:%s:fopen failed!!",
__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
return False;
}
if((iRet = fwrite(pstStorageIdxNode, sizeof(stStorageIdxNode_T), 1, pstFileFd)) == -1)
{
printf("\nFile:%s, Line:%d,Func:%s:fwrite failed!!",
__FILE__, __LINE__,STORAGE_FUNCTION_NAME);
fclose(pstFileFd);
return False;
}
fclose(pstFileFd);
return True;
#undef  STORAGE_FUNCTION_NAME
}*/
/********************************* Source Code Start ********************************/

