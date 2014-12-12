/*******************************************************************************
* COPYRIGHT
********************************************************************************
/*
  * =====================================================================================
  *
  *       Filename:  storage_index_mng.h
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
/******************************** Include Code Start ******************************/

#ifndef _STORAGE_INDEX_MNG_H_
#define _STORAGE_INDEX_MNG_H_

/************************************************************************/
/* 包含头文件                                                                     */
/************************************************************************/
#include "storage_private.h"
#include "storage_common.h"
#include "storage_index.h"
#include "storage_file.h"

/*typedef unsigned char BOOL;*/


typedef struct _storage_name_devide_t
{
    unsigned char aucYear[5];
    unsigned char aucMonth[3];
    unsigned char aucDay[3];
    unsigned char aucHour[3];
    unsigned char aucMin[3];
    unsigned char aucSec[3];
    unsigned char aucUsec[4];
}stStorageNameDevide_T;

typedef struct _storage_idx_node_t
{
    unsigned char acName[STORAGE_IDX_NAME_CONTEXT_LENGTH_MAX];
    unsigned char acLoc[STORAGE_IDX_LOC_LENGTH_MAX];
    unsigned int offset;
    long long start_time;
    long long end_time;
    unsigned char acFlags[STORAGE_IDX_FLAG_LENGTH_MAX];
}stStorageIdxNode_T;
typedef stStorageIdxNode_T* pstIdxHandle;

/************************************************************************/
/*Global Variables                                                      */
/************************************************************************/
//static  pstIdxHandle g_apstHandle[INDEX_EVENT_NUM_MAX];

/************************************************************************/
/* 函数声明及定义                                                                     */
/************************************************************************/
void DevideIdxName_ByTime(char* pcMergeFileName, stStorageNameDevide_T* pstDevideFileName);
void MergeIdxName_ByTime(char* pcMergeFileName,stStorageNameDevide_T* pstDevideFileName);
succeed_type GetIdxNumfromTables(enStorageEventType_T stiSeq, unsigned short *pusIndexNum);
succeed_type UnGetIdxNumfromTables(enStorageEventType_T stiSeq, unsigned short *pusIndexNum);

succeed_type InitIdxMemAlloc(void);
succeed_type InitIdxFile(void);
void ExitIdxMemFree(void);
void ExitIdxFile(void);

void GetInfoFromIdxLocDir(char const acLocName[], char acFileName[], char acDirName[],\
	    enStorageEventType_T* penEventType, enStorageCodeType_T* penCodeType, char acFileNameContext[]);
//succeed_type AddRecIntoIdxFile(char* const pcLocName);
succeed_type WritenRecIntoIdxFile(stStorageIdxNode_T* pstStorageIdxNode);
succeed_type AddRecIntoIdxFile(STORAGE_PRIVATE_HANDLE *phandle, LIBVSSHM_UINT_HEADER *pvsshm_uint_header);
long GetFileSize_ByStruct(char* pcFileName);

succeed_type GetIdxDirName_ByEvent(enStorageEventType_T enStorageEvent, char* pcIdxDirName);
succeed_type GetIdxLocName_ByEvent(enStorageEventType_T enStorageEvent, char* pcIdxDirName);
succeed_type GetDirName_ByEvent(enStorageEventType_T enStorageEvent, char* pcDirName);

#endif/*_STORAGE_STORE_INDEX_MNG_H_*/

/******************************** Include Code End ******************************/
