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


/******************************* �����ļ����� *********************************/
#include "storage_private.h"
#include "storage_index.h"

/******************************* ����ʵ�� *************************************/

/*******************************************************************************
 * ��������: storage_index_init
 * ��������: ��������ĳ�ʼ��
 * ����ĵ�:
 * ��������:
 * ��������:         ����                      ����/���     ����
 * ����ֵ:  ��

 * �޸�����    �汾��   �޸���  �޸����ݣ������ڱ������ڵ�ȱ���޸���Ҫд�ڴ˴���
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
 * ��������: storage_index_create
 * ��������: ��һ���������������У�ѡȡһ����㴴��һ����������
 * ����ĵ�:
 * ��������:
 * ��������:         ����                      ����/���     ����
 * ����ֵ:  �������ͷ���ָ�� ����������ɹ���
 *          NULL   ����������ʧ�ܣ�

 * �޸�����    �汾��   �޸���  �޸����ݣ������ڱ������ڵ�ȱ���޸���Ҫд�ڴ˴���
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
			/* ����ǿ������ת��unsigned short * */
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
 * ��������: storage_index_enq
 * ��������: ��һ�����������������ͷ�һ������
 * ����ĵ�:
 * ��������:
 * ��������:         ����                      ����/���     ����
 * vpIndex           stStorageIndex_T*    ����       ���������ͷָ��
 * vusIndex          unsigned short                    ����       Ҫ�ͷŵ�����ֵ
 * ����ֵ:  0     �����ͷųɹ�
 *          -1    �����ͷ�ʧ�ܣ�ԭ�������������������ԣ��������ͷָ�����

 * �޸�����    �汾��   �޸���  �޸����ݣ������ڱ������ڵ�ȱ���޸���Ҫд�ڴ˴���
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
		  printf(" �����index�����ڣ������趨����ֵ \n");
#endif */

	}
	return retVal;
}

/*******************************************************************************
 * ��������: storage_index_deq
 * ��������: ��һ���������������з���һ������
 * ����ĵ�: <�����˺�����ص��ĵ����ơ��汾���ڲ�Ʒ��ƣ���Э�飩�е�λ��>
 * ��������:
 * ��������:         ����                      ����/���     ����
 * vpIndex           stStorageIndex_T*    ����       ���������ͷָ��
 * ����ֵ:  0xFFFE   �޿��е������ɷ���
 *          ���������ֵ( ����ֵ <=65530)

 * �޸�����    �汾��   �޸���  �޸����ݣ������ڱ������ڵ�ȱ���޸���Ҫд�ڴ˴���
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
 * ��������: storage_index_alloc
 * ��������: ��һ���������������з���һ������
 * ����ĵ�: <�����˺�����ص��ĵ����ơ��汾���ڲ�Ʒ��ƣ���Э�飩�е�λ��>
 * ��������:
 * ��������:         ����                      ����/���     ����
 * vpIndex           stStorageIndex_T*    ����       ���������ͷָ��
 * ����ֵ:  0XFFFF ����������������ͷָ�� ==NULL��
 *          0xFFFE   �޿��е������ɷ���
 *          ���������ֵ( ����ֵ <=65530)

 * �޸�����    �汾��   �޸���  �޸����ݣ������ڱ������ڵ�ȱ���޸���Ҫд�ڴ˴���
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
 * ��������: storage_index_free
 * ��������: ��һ�����������������ͷ�һ������
 * ����ĵ�: <�����˺�����ص��ĵ����ơ��汾���ڲ�Ʒ��ƣ���Э�飩�е�λ��>
 * ��������:
 * ��������:         ����                      ����/���     ����
 * vpIndex           stStorageIndex_T*    ����       ���������ͷָ��
 * vusIndex          unsigned short                    ����       Ҫ�ͷŵ�����ֵ
 * ����ֵ:  0     �����ͷųɹ�
 *          -1    �����ͷ�ʧ�ܣ�ԭ�������������������ԣ��������ͷָ�����

 * �޸�����    �汾��   �޸���  �޸����ݣ������ڱ������ڵ�ȱ���޸���Ҫд�ڴ˴���
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
 * ��������: storage_index_destroy
 * ��������: ɾ��һ����������
 * ����ĵ�: <�����˺�����ص��ĵ����ơ��汾���ڲ�Ʒ��ƣ���Э�飩�е�λ��>
 * ��������:
 * ��������:         ����                      ����/���     ����
 * vpIndex           stStorageIndex_T*    ����       ���������ͷָ��
 * ����ֵ:  0xFFFF   ���������ͷָ��Ϊ��
 0        ɾ��һ����������ɹ�

 * �޸�����    �汾��   �޸���  �޸����ݣ������ڱ������ڵ�ȱ���޸���Ҫд�ڴ˴���
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
