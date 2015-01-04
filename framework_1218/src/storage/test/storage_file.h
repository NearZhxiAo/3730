/*******************************************************************************
* COPYRIGHT
********************************************************************************
/*
  * =====================================================================================
  *
  *       Filename:  storage_file.h
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
/******************************** Souurce Code Start ******************************/

#ifndef _STORAGE_FILE_H_
#define _STORAGE_FILE_H_

/************************************************************************/
/* 包含头文件                                                                     */
/************************************************************************/
#ifdef _cplusplus
extern "C"{
#endif
#include "storage_common.h"
#include "storage_file.h"


#define STORAGE_CMD_LENGTH_MAX 50
#define STORAGE_IDX_NAME_CONTEXT_LENGTH_MAX 20
#define STORAGE_IDX_NAME_FOMAT_LENGTH_MAX 8
#define STORAGE_IDX_NAME_LENGTH_MAX 32
#define STORAGE_IDX_DIR_LENGTH_MAX 80
#define STORAGE_IDX_LOC_LENGTH_MAX 100
#define STORAGE_IDX_FLAG_LENGTH_MAX 4
#define STORAGE_IDX_DIR_LEVEL_MAX 10
#define STORAGE_IDX_CONTEXT_VOL_MAX 1000



/************************************************************************/
/*Global Variables                                                      */
/************************************************************************/

/************************************************************************/
/* 函数声明及定义                                                                     */
/************************************************************************/
void DevideIdxLocDir_By(	char const acMergeLocDir[],
				char aacDevideLocDir[][STORAGE_IDX_NAME_LENGTH_MAX]);

int ExistIdxFileSingle_if(  char* pcFileName);
void CreateIdxFileSingle(  char* pcFileName);
void DeleteIdxFileSingle(  char* pcFileName);
int ExistIdxDirSingle_if(  char *pcDirName);
void CreateIdxDirSingle(  char* pcDirName);
void CreateIdxDirMulti(  char* pcDirName);
void DeleteIdxDirSingle(  char* pcDirName);
void DeleteIdxDirMulti(  char* pcDirName);
void ChdirInIdxDirSingle(  char* pcDirName);
void ChdirInIdxDirMulti(  char* pcDirName);
void ChdirOutIdxDirSingle(void);
void ChdirOutIdxDirMulti(  char* pcDirName);
void GetCurPwdName( char *pcDirName);

long GetFileSize_ByByte( char* pcFileName);
int GenerateFileName_ByCurTime( char *pstFileName,  char *pstFileType);
int GenerateTarFileName_ByCurTime(  char *strFileName);
int GenerateLocName_ByCurTimeAndCurPwd( char *pcLocName, char *pstFileType);
int GenerateLocName_ByCurTimeAndThePwd( char *pcLocName, char *pstFileType, char *pstDirName);
#ifdef _cplusplus
}
#endif

#endif/*_STORAGE_STORE_FILE_H_*/

/******************************** Souurce Code End ******************************/
