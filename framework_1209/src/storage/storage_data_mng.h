/*******************************************************************************
* COPYRIGHT
********************************************************************************
/*
  * =====================================================================================
  *
  *       Filename:  storage_data_common.h
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
/******************************** Header Code Start ******************************/

/******************************** ͷ�ļ�������ͷ ******************************/

#ifndef _STORAGE_COMMON_H_
#define _STORAGE_COMMON_H_

/************************************************************************/
/* ����ͷ�ļ�                                                                     */
/************************************************************************/
#include "storage_common.h"

/************************************************************************/
/* ��������������                                                                     */
/************************************************************************/

succeed_type InitDataFile(void);
succeed_type GenerateDataDirName_ByEventAndVideoType(enStorageEventType_T , enStorageCodeType_T , char* );
//succeed_type AddRecIntoDataFile(const char* pcStoreFileName, char* pcavdata, int fFrameSize);
succeed_type AddRecIntoDataFile(STORAGE_PRIVATE_HANDLE *phandle, LIBVSSHM_UINT_DATA_I *pvsshm_uint_data);

#endif/*_STORAGE_STORE_INDEX_MNG_H_*/

/******************************** Header Code End ******************************/
