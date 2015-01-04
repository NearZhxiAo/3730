/*******************************************************************************
* COPYRIGHT
********************************************************************************
/*
  * =====================================================================================
  *
  *       Filename:  storage_index.h
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

#ifndef _STORAGE_INDEX_H_
#define _STORAGE_INDEX_H_

#ifdef _cplusplus
extern "C"{
#endif

/************************************************************************/
/* 包含头文件                                                                     */
/************************************************************************/
//#include "storage_store_type.h"

/************************************************************************/
/* 数据结构定义                                                                     */
/************************************************************************/

/* Index tables that can be supported */
#define STORAGE_INDEX_NULL            0xFFFF /*Invalid index*/
#define STORAGE_INDEX_INVALID_INPUT   0xFFFB /*for invalid input*/
#define STORAGE_INDEX_TABLES_FULL     0xFFFC /*for tables full*/
#define STORAGE_INDEX_MEM_FAILED      0xFFFD /*if MEM allocation fails*/
#define STORAGE_INDEX_NOFREE          0xFFFE /*if no index is free*/


#define STORAGE_INDEX_IDLE    0  /* the Data in this Position is available   */
#define STORAGE_INDEX_BUSY    1  /* the Data in this Position is unavailable */

typedef short STORAGE_INDEX_STATUS;

#define STORAGE_INDEX_ERROR            (-1)
#define STORAGE_INDEX_OK                 0
#define STORAGE_INDEX_SUCCESS            0
#define STORAGE_INDEX_FAILURE          (-1)

/*#define storage_print  printf*/

typedef struct _storage_index_t
{
    unsigned short usStart;      /* Starting value */
    unsigned short usSize;       /* Size of the index table */
    unsigned short usFront;      /* Available Front Position */
    unsigned short usRear;       /* Available Rear Position */
    unsigned short *pArr;        /* To store the starting address of user create Index array */
    unsigned char  *pFlag;       /* To save the busy or idle flag of the Position, 0:idle,1:busy */
}stStorageIndex_T;

/************************************************************************/
/* 函数声明及定义                                                                     */
/************************************************************************/
/* 索引表结点数组初始化 */
extern void storage_index_init(stStorageIndex_T *vpstIndexArray,unsigned short vusIndexTablesMax);
/* 在一个索引表结点数组中，选取一个结点创建一个索引链表 */
extern stStorageIndex_T * storage_index_create(stStorageIndex_T *vpstIndexArray,\
                                           unsigned short vusIndexTablesMax,\
                                           unsigned short vusStart,\
                                           unsigned short vusSize);
/* 在一个结点的索引链表中分配一个索引 */
extern unsigned short storage_index_alloc(stStorageIndex_T *vpIndex);
/* 在一个结点的索引链表中释放一个索引 */
extern short storage_index_free(stStorageIndex_T *vpIndex,unsigned short vuiIndex);
/* 将一个结点对应的索引链表删除 */
extern unsigned short storage_index_destroy(stStorageIndex_T* vpIndex);

#ifdef _cplusplus
}
#endif

#endif/*_STORAGE_STORE_INDEX_H_*/

/******************************** 头文件结束 **********************************/
/**
  Sample
  /code
**/
/**
#include <stdio.h>
#include "storage_store_index.h"

#define _DEBUG

#ifdef _DEBUG
#define DBG_Info(fmt, args...) fprintf(stderr, "Info: " fmt, ##args)
#else
#define DBG_Info(fmt, args...)
#endif
#define DBG_Err(fmt, args...) fprintf(stderr, "Err: " fmt, ##args)

static   stEncIndex_T                gstIndex;
static   stEncIndex_T                *gpstIndex;

int main(int argc,char **argv)
{
	unsigned short ucRet = -1;
	unsigned short ucLoop = 0;
	storage_index_init(&gstIndex, 20);
	gpstIndex = storage_index_create(&gstIndex, 1, 1, 10);
	(NULL == gpstIndex) ? DBG_Err("\r\nCreate Index failed\n") : DBG_Info("\r\nCreate Index success\n");
	for(ucLoop = 0; ucLoop < 20; ucLoop++)
	{
		ucRet = storage_index_alloc(gpstIndex);
		DBG_Info("\r\nAlloc[%d] = %d\n", ucLoop, ucRet);
		ucRet = storage_index_free(gpstIndex, ucRet);
		DBG_Info("\r\nFree[%d] = %d\n", ucLoop, ucRet);
	}
	storage_index_destroy(gpstIndex);
	return 0;
}
**/
/**
  \endcode
**/
/******************************** Include Code End ******************************/

