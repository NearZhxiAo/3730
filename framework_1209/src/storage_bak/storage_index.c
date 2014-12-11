/*******************************************************************************
 * COPYRIGHT
 ********************************************************************************
/*
 * =====================================================================================
 *
 *       Filename:  storage_index.c
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


/******************************* 包含文件声明 *********************************/
#include "storage_private.h"
#include "storage_index.h"

/******************************* 函数实现 *************************************/

/*******************************************************************************
 * 函数名称: storage_index_init
 * 函数功能: 索引数组的初始化
 * 相关文档:
 * 函数参数:
 * 参数名称:         类型                      输入/输出     描述
 * 返回值:  无

 * 修改日期    版本号   修改人  修改内容（局限在本函数内的缺陷修改需要写在此处）
 * -----------------------------------------------------------------
 *
 *******************************************************************************/
void storage_index_init(stStorageIndex_T *vpstIndexArray,unsigned short vusIndexTablesMax)
{
	unsigned short usLoop = 0;

	/*Initialize all the enteries in the Index Information Table */
	for(usLoop=0; usLoop < vusIndexTablesMax;usLoop++)
	{
		/* Initialize the parameters to default values */
		vpstIndexArray[usLoop].usStart = 0;
		vpstIndexArray[usLoop].usSize  = 0;
		vpstIndexArray[usLoop].usFront = 0;
		vpstIndexArray[usLoop].usRear  = 0;
		vpstIndexArray[usLoop].pArr    = NULL;
		vpstIndexArray[usLoop].pFlag   = NULL;
	}
}

/*******************************************************************************
 * 函数名称: storage_index_create
 * 函数功能: 在一个索引表结点数组中，选取一个结点创建一个索引链表
 * 相关文档:
 * 函数参数:
 * 参数名称:         类型                      输入/输出     描述
 * 返回值:  索引表的头结点指针 （创建链表成功）
 *          NULL   （创建链表失败）

 * 修改日期    版本号   修改人  修改内容（局限在本函数内的缺陷修改需要写在此处）
 * -----------------------------------------------------------------
 *
 *******************************************************************************/
stStorageIndex_T* storage_index_create(stStorageIndex_T *vpstIndexArray,\
		unsigned short vusIndexTablesMax,\
		unsigned short vusStart,\
		unsigned short vusSize)
{
	/* Declarations */
	unsigned short i;
	unsigned short j;
	unsigned short newSize;	/* Modified size to implement
				 * circular queue
				 */
	/*- Check has to be taken on validity of vusStart  -*/

	/*- RETURN INVALID INPUT -*/

	/*- Check has to be taken on validity of vusSize max size can be 65530 others are reserved for err -*/

	/*- RETURN INVALID INPUT -*/


	if(vusSize > 0xFFFA )
		return( NULL ) ;

	/* Size of the required Index array is modified to implement
	 * circular queue
	 */
	newSize=(unsigned short)(vusSize+1);

	for(i=0;i<vusIndexTablesMax;i++)
	{
		/* Check for an free entry in the Index information table */
		if(vpstIndexArray[i].pArr==NULL)
		{
			/* Allocate memory to the index array */
			/* 增加强制类型转换unsigned short * */
			vpstIndexArray[i].pArr=(unsigned short *)malloc(sizeof(unsigned short)*(newSize));

			/*- check for return value -*/
			if(vpstIndexArray[i].pArr==NULL)
				return(NULL);
			memset(vpstIndexArray[i].pArr,0,newSize);

			/*  Allocate memory to the save the busy flag
			 *  note: the memory size is vusSize. add by wangxintai 2003-03-22
			 */
			vpstIndexArray[i].pFlag =(unsigned char *)malloc(sizeof(unsigned char)*(vusSize));

			/*- check for return value -*/
			if(vpstIndexArray[i].pFlag ==NULL)
			{
				/* free the allocated memory for pArr */
				free(vpstIndexArray[i].pArr);
				vpstIndexArray[i].pArr = NULL;
				return NULL;
			}
			/* set all the flag to idle */
			memset(vpstIndexArray[i].pFlag,0,vusSize);



			/* Set the size and the start index value */

			vpstIndexArray[i].usSize=vusSize;
			vpstIndexArray[i].usStart=vusStart;

			/* Enqueue all the array locations to the free list */
			for(j=vusStart;j< vusStart+vusSize;j++)
			{
				vpstIndexArray[i].pArr[j-vusStart]=j;
			}
			/* at this time ,the available queue is full */
			vpstIndexArray[i].usFront =0;
			vpstIndexArray[i].usRear  =vusSize;

			return &vpstIndexArray[i];
		}
	}
	return(NULL);
}

/*******************************************************************************
 * 函数名称: storage_index_enq
 * 函数功能: 在一个结点的索引链表中释放一个索引
 * 相关文档:
 * 函数参数:
 * 参数名称:         类型                      输入/输出     描述
 * vpIndex           stStorageIndex_T*    输入       索引链表的头指针
 * vusIndex          unsigned short                    输入       要释放的索引值
 * 返回值:  0     索引释放成功
 *          -1    索引释放失败（原因可以是输入的索引不对，或链表的头指针错误）

 * 修改日期    版本号   修改人  修改内容（局限在本函数内的缺陷修改需要写在此处）
 * -----------------------------------------------------------------
 *
 *******************************************************************************/
STORAGE_INDEX_STATUS storage_index_enq(stStorageIndex_T *vpIndex,unsigned short vusIndex)
{
	unsigned short usIndexSize;	/* Size of the allocated Index */
	STORAGE_INDEX_STATUS retVal = STORAGE_INDEX_ERROR;		/* return code */

	usIndexSize = (unsigned short)((vpIndex->usSize)+1);

	/* Check for the sanity of the index to be enqueued*/
	if ( 	(vusIndex >= vpIndex->usStart)
			&& (vusIndex < (vpIndex->usStart + vpIndex->usSize)))
	{
		/* Check if the Index array free list is empty ,ie,wheter it have space to save the Index*/
		if((((vpIndex->usRear)+1) % usIndexSize) != vpIndex->usFront)
		{
			/*to check, this index is already is freed or not */

			if(vpIndex->pFlag[vusIndex-vpIndex->usStart]==STORAGE_INDEX_BUSY)
			{
				/* not freed,and the queue have space,then enqueue it to the queue*/
				/* we use the vusIndex to subtract usStart to get the Position */


				/*Append the freed Index to the end of the free list */

				/* to save the  the Index*/
				(vpIndex->pArr[vpIndex->usRear]) = vusIndex;
				/*set the flag to IDLE */
				vpIndex->pFlag[vusIndex-vpIndex->usStart]=STORAGE_INDEX_IDLE;

				vpIndex->usRear=(unsigned short)(((vpIndex->usRear)+1)%usIndexSize);

				retVal = STORAGE_INDEX_SUCCESS;

			}else
			{
				/* the Index is Idle,ie,the Index has existed in this queue */
				/*STORAGE_print("\n already freed %d index ",vusIndex);*/
				retVal= STORAGE_INDEX_SUCCESS;
			}
		}else
		{
			/*#ifdef DEBUG
			  printf("\n  Index Array list is Full,can not enqueue the index \n");
#endif*/
			retVal = STORAGE_INDEX_ERROR;

		}
	}
	else
	{
		/*#ifdef DEBUG
		  printf(" 输入的index不存在，超出设定的数值 \n");
#endif */

	}
	return retVal;
}

/*******************************************************************************
 * 函数名称: storage_index_deq
 * 函数功能: 在一个结点的索引链表中分配一个索引
 * 相关文档: <描述此函数相关的文档名称、版本及在产品设计（或协议）中的位置>
 * 函数参数:
 * 参数名称:         类型                      输入/输出     描述
 * vpIndex           stStorageIndex_T*    输入       索引链表的头指针
 * 返回值:  0xFFFE   无空闲的索引可分配
 *          分配的索引值( 索引值 <=65530)

 * 修改日期    版本号   修改人  修改内容（局限在本函数内的缺陷修改需要写在此处）
 * -----------------------------------------------------------------
 *
 *******************************************************************************/

STORAGE_INDEX_STATUS storage_index_deq(stStorageIndex_T *vpIndex)
{
	unsigned short usIndexSize;	/*Size of the Index Array */
	STORAGE_INDEX_STATUS usRetVal;	/* Return code */

	usIndexSize=(unsigned short)((vpIndex->usSize)+1);

	/* Check if the free list of the index array is Full */
	if((vpIndex->usFront) != (vpIndex->usRear))
	{
		/* got the real Index value */
		usRetVal = vpIndex->pArr[vpIndex->usFront];

		/* set the busy flag */
		vpIndex->pFlag[vpIndex->pArr[vpIndex->usFront]-vpIndex->usStart]=STORAGE_INDEX_BUSY;

		vpIndex->usFront=(unsigned short)(((vpIndex->usFront)+1)%usIndexSize);
	}
	else
	{
		usRetVal = STORAGE_INDEX_NOFREE;
	}
	return (usRetVal);
}


/*******************************************************************************
 * 函数名称: storage_index_alloc
 * 函数功能: 在一个结点的索引链表中分配一个索引
 * 相关文档: <描述此函数相关的文档名称、版本及在产品设计（或协议）中的位置>
 * 函数参数:
 * 参数名称:         类型                      输入/输出     描述
 * vpIndex           stStorageIndex_T*    输入       索引链表的头指针
 * 返回值:  0XFFFF （输入的索引链表的头指针 ==NULL）
 *          0xFFFE   无空闲的索引可分配
 *          分配的索引值( 索引值 <=65530)

 * 修改日期    版本号   修改人  修改内容（局限在本函数内的缺陷修改需要写在此处）
 * -----------------------------------------------------------------
 *
 *******************************************************************************/

unsigned short storage_index_alloc(stStorageIndex_T *vpIndex)
{
	if(vpIndex==NULL)
	{
		/*#ifdef DEBUG
		  printf("Invalid Index Array address");
#endif*/
		return STORAGE_INDEX_NULL;
	}

	else
	{
		return storage_index_deq(vpIndex);
	}
}

/*******************************************************************************
 * 函数名称: storage_index_free
 * 函数功能: 在一个结点的索引链表中释放一个索引
 * 相关文档: <描述此函数相关的文档名称、版本及在产品设计（或协议）中的位置>
 * 函数参数:
 * 参数名称:         类型                      输入/输出     描述
 * vpIndex           stStorageIndex_T*    输入       索引链表的头指针
 * vusIndex          unsigned short                    输入       要释放的索引值
 * 返回值:  0     索引释放成功
 *          -1    索引释放失败（原因可以是输入的索引不对，或链表的头指针错误）

 * 修改日期    版本号   修改人  修改内容（局限在本函数内的缺陷修改需要写在此处）
 * -----------------------------------------------------------------
 *
 *******************************************************************************/
short storage_index_free(stStorageIndex_T *vpIndex,unsigned short vuiIndex)

{
	if(vpIndex == NULL)
	{
		/*#ifdef DEBUG
		  printf("Invalid  Index Array address");
#endif*/
		return STORAGE_INDEX_ERROR;
	}
	else
	{
		return storage_index_enq(vpIndex,vuiIndex);
	}
}


/*******************************************************************************
 * 函数名称: storage_index_destroy
 * 函数功能: 删除一个索引链表
 * 相关文档: <描述此函数相关的文档名称、版本及在产品设计（或协议）中的位置>
 * 函数参数:
 * 参数名称:         类型                      输入/输出     描述
 * vpIndex           stStorageIndex_T*    输入       索引链表的头指针
 * 返回值:  0xFFFF   索引链表的头指针为空
 0        删除一个是索引表成功

 * 修改日期    版本号   修改人  修改内容（局限在本函数内的缺陷修改需要写在此处）
 * -----------------------------------------------------------------
 *
 *******************************************************************************/

unsigned short storage_index_destroy(stStorageIndex_T* vpIndex)
{
	if(vpIndex==NULL)
	{
		/*#ifdef DEBUG
		  printf("Invalid Index Array address");
#endif*/
		return STORAGE_INDEX_NULL;
	}

	free(vpIndex->pArr);
	free(vpIndex->pFlag);
	/*initialising the members in the structure*/
	vpIndex->usStart=0;
	vpIndex->usSize=0;
	vpIndex->usFront=0;
	vpIndex->usRear=0;
	vpIndex->pArr=NULL;
	vpIndex->pFlag = NULL;
	return STORAGE_INDEX_OK;
}
/********************************* Source Code End ********************************/
