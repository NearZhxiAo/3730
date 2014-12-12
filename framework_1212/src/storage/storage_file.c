/*******************************************************************************
* COPYRIGHT
********************************************************************************
/*
  * =====================================================================================
  *
  *       Filename:  storage_file.c
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

/*******************************************************************************
* ��������: ExistIdxDirSingle_if
* ��    ��: �ж�ָ��Ŀ¼�Ƿ����
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* -----------------------------------------------------------------------------
* ����ֵ:
*******************************************************************************/
int ExistIdxDirSingle_if(char *pcDirName)
{
#define STORAGE_FUNCTION_NAME "ExistIdxDirSingle_if"
    struct stat stStatBuf;
    int iRet = -1;
    /*����ж�*/
    if(NULL == pcDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return False;
    }
    /*��ȡ�򿪵��ļ���Ϣ*/
    iRet = stat(pcDirName, &stStatBuf);
    if(0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:stat error or %s isn't exist!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcDirName);
        return False;
    }
    /*�ж�ָ���ļ��Ƿ���Ŀ¼*/
    if(!(S_IFDIR & stStatBuf.st_mode))
    {
        printf("\nFile:%s, Line:%d,Func:%s:%s isn't dir!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcDirName);
        return False;
    }
    return True;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: GetCurPwdName
* ��    ��: ��ȡ��ǰ�Ĺ���Ŀ¼
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* -----------------------------------------------------------------------------
* ����ֵ:
*******************************************************************************/
void GetCurPwdName(char *pcDirName)
{
#define STORAGE_FUNCTION_NAME "GetCurPwdName"
    char acMergeDirName[STORAGE_IDX_DIR_LENGTH_MAX] = {0};
    if (NULL == pcDirName)
    {
      printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
        __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
      return ;
    }
    getcwd(acMergeDirName, STORAGE_IDX_DIR_LENGTH_MAX);
    //getcurdir(0, acMergeDirName);
    strncpy(pcDirName, acMergeDirName, strlen(acMergeDirName));
    if('/' != pcDirName[strlen(acMergeDirName) - 1])
    {
        pcDirName[strlen(acMergeDirName)] = '/';
        pcDirName[strlen(acMergeDirName)+1] = '\0';
    }
    else
    {
        pcDirName[strlen(acMergeDirName)] = '\0';
    }
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: CreateIdxDirSingle
* ��    ��: ����ָ����Ŀ¼
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:  �ɹ�/���ɹ� 0/1
*******************************************************************************/
void CreateIdxDirSingle(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "CreateIdxDirSingle"
    int iRet = STORAGE_FAIL;
/*
    INT8 aucCmd[STORAGE_CMD_LENGTH];
    memset(aucCmd,0,sizeof(aucCmd));
*/
   /*����ж�*/
   if (NULL == pcDirName)
    {
      printf("\nFile:%s, Line:%d,Func:%s:%s is NULL!!",
        __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcDirName);
      return ;
    }
    iRet = mkdir(pcDirName,0666);
/*
    sprintf(aucCmd,"mkdir %s",pcDirName);
    iRet = system(aucCmd);
*/
    if ( 0 != iRet)
    {
      printf("\nFile:%s, Line:%d,Func:%s: %s Mkdir error!!",
        __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcDirName);
      return ;
    }
#undef  STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: CreateIdxDirMulti
* ��    ��: ����ָ����Ŀ¼
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:  �ɹ�/���ɹ� 0/1
*******************************************************************************/
void CreateIdxDirMulti(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "CreateIdxDirMulti"
    int iRet = STORAGE_FAIL;
    char aucCmd[STORAGE_CMD_LENGTH_MAX];
    memset(aucCmd,0,sizeof(aucCmd));
    /*����ж�*/
    if (NULL == pcDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:%s is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcDirName);
        return ;
    }
/*
      iRet = mkdir(pcDirName,0666);
*/
    sprintf(aucCmd,"mkdir -p %s",pcDirName);
    iRet = system(aucCmd);
    if ( 0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s: %s Mkdir error!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcDirName);
        return ;
    }
#undef  STORAGE_FUNCTION_NAME
}


/*******************************************************************************
* ��������: DeleteIdxDirSingle
* ��    ��: ɾ��ָ����Ŀ¼
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void DeleteIdxDirSingle(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "DeleteIdxDirSingle"
    int iRet = STORAGE_FAIL;
/*
    INT8 aucCmd[STORAGE_CMD_LENGTH];
    memset(aucCmd,0,sizeof(aucCmd));
*/
    if (NULL == pcDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    iRet = rmdir(pcDirName);
/*
    sprintf(aucCmd,"rmdir %s",pcDirName);
    iRet = system(aucCmd);
*/
    if ( 0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:rmdir error!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
#undef  STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: DeleteIdxDirMulti
* ��    ��: ɾ��ָ����Ŀ¼
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void DeleteIdxDirMulti(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "DeleteIdxDirMulti"
    int iRet = STORAGE_FAIL;
    char aucCmd[STORAGE_CMD_LENGTH_MAX];
    memset(aucCmd,0,sizeof(aucCmd));
    if (NULL == pcDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
/*
    iRet = rmdir(pcDirName);
*/
    sprintf(aucCmd,"rm -fr %s",pcDirName);
    iRet = system(aucCmd);
    if ( 0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:rmdir error!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
#undef  STORAGE_FUNCTION_NAME
}
/*******************************************************************************
* ��������: ChdirInIdxDirSingle
* ��    ��: ��һ��
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void ChdirInIdxDirSingle(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "ChdirInIdxDirSingle"
    int iRet = STORAGE_FAIL;
/*
    INT8 aucCmd[STORAGE_CMD_LENGTH];
    memset(aucCmd,0,sizeof(aucCmd));
*/
    if (NULL == pcDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    iRet = chdir(pcDirName);
/*
    sprintf(aucCmd,"cd %s",pcDirName);
    iRet = system(aucCmd);
*/
    if ( 0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:cd error!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
#undef  STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: ChdirInIdxDirMulti
* ��    ��: ���༶
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void ChdirInIdxDirMulti(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "ChdirInIdxDirMulti"
    int iLoop = 0;
    char acMergeDirName[STORAGE_IDX_LOC_LENGTH_MAX];
    char aacDevideDirName[STORAGE_IDX_DIR_LEVEL_MAX][STORAGE_IDX_NAME_LENGTH_MAX] = {{0}};
    /*
    char aucCmd[STORAGE_CMD_LENGTH_MAX];
    memset(aucCmd,0,sizeof(aucCmd));
    */
    if (NULL == pcDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    if(STORAGE_IDX_LOC_LENGTH_MAX < strlen(pcDirName))
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is too long!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return;
    }
    strncpy(acMergeDirName, pcDirName,STORAGE_IDX_LOC_LENGTH_MAX);
    DevideIdxLocDir_By(acMergeDirName,aacDevideDirName);
    if('/' == pcDirName[0])
        ChdirInIdxDirSingle("/");
    for(iLoop = 0; iLoop < STORAGE_IDX_DIR_LEVEL_MAX && strlen(aacDevideDirName[iLoop]) != 0;iLoop++)
    {
        ChdirInIdxDirSingle(aacDevideDirName[iLoop]);
    }
    /*
    sprintf(aucCmd,"cd %s",pcDirName);
    iRet = system(aucCmd);
    */
#undef  STORAGE_FUNCTION_NAME
}


/*******************************************************************************
* ��������: ChdirOutIdxDirSingle
* ��    ��: ��һ��
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void ChdirOutIdxDirSingle(void)
{
#define STORAGE_FUNCTION_NAME "ChdirOutIdxDirSingle"
    int iRet = STORAGE_FAIL;
/*
    INT8 aucCmd[STORAGE_CMD_LENGTH];
    memset(aucCmd,0,sizeof(aucCmd));
*/
    iRet = chdir("..");
/*
    sprintf(aucCmd,"cd %s",pcDirName);
    iRet = system(aucCmd);
*/
    if ( 0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:cd error!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
#undef  STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: ChdirOutIdxDirMulti
* ��    ��: �˶༶
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void ChdirOutIdxDirMulti(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "ChdirOutIdxDirMulti"
    int iLoop = 0;
    char acMergeDirName[STORAGE_IDX_LOC_LENGTH_MAX];
    char aacDevideDirName[STORAGE_IDX_DIR_LEVEL_MAX][STORAGE_IDX_NAME_LENGTH_MAX] = {{0}};
    /*
    char aucCmd[STORAGE_CMD_LENGTH_MAX];
    memset(aucCmd,0,sizeof(aucCmd));
    */
    if (NULL == pcDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    if(STORAGE_IDX_LOC_LENGTH_MAX < strlen(pcDirName))
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is too long!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return;
    }
    strncpy(acMergeDirName, pcDirName,STORAGE_IDX_LOC_LENGTH_MAX);
    DevideIdxLocDir_By(acMergeDirName,aacDevideDirName);
    for(iLoop = 0; iLoop < STORAGE_IDX_DIR_LEVEL_MAX && strlen(aacDevideDirName[iLoop]) != 0;iLoop++)
    {
        ChdirOutIdxDirSingle();
    }
    if('/' == pcDirName[0])
        ChdirOutIdxDirSingle();
    /*
    sprintf(aucCmd,"cd %s",pcDirName);
    iRet = system(aucCmd);
    */
#undef  STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������:DevideIdxLocDir_By
* ��    ��: �ֽ�LocDir
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void DevideIdxLocDir_By(char const acMergeLocDir[],char aacDevideLocDir[][STORAGE_IDX_NAME_LENGTH_MAX])
{
#define STORAGE_FUNCTION_NAME "DevideIdxLocDir_By"
    int iLoop = 0;
    char* pcDevideDirTemp = NULL;
    char acMergeLocDirTemp[STORAGE_IDX_LOC_LENGTH_MAX] = {0};
    if(NULL == acMergeLocDir || NULL == aacDevideLocDir)
    {
        printf("\nFile:%s, Line:%d,Func:%s:acMergeLocDir|aacDevideLocDir is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return;
    }
    strncpy(acMergeLocDirTemp, acMergeLocDir, STORAGE_IDX_LOC_LENGTH_MAX);
    pcDevideDirTemp = strtok(acMergeLocDirTemp,"/");
    for( iLoop = 0; iLoop < STORAGE_IDX_DIR_LEVEL_MAX && NULL != pcDevideDirTemp; iLoop++)
    {
        strncpy(aacDevideLocDir[iLoop],pcDevideDirTemp,strlen(pcDevideDirTemp));
        aacDevideLocDir[iLoop][strlen(pcDevideDirTemp)] = '\0';
        pcDevideDirTemp = strtok(NULL,"/");
    }
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: ExistIdxFileSingle_if
* ��    ��: �ж�ָ���ļ��Ƿ����
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
int ExistIdxFileSingle_if(char* pcFileName)
{
#define STORAGE_FUNCTION_NAME "ExistIdxFileSingle_if"
    struct stat stStatBuf;
    int iRet = STORAGE_FAIL;
    /*����ж�*/
    if(NULL == pcFileName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcFileName is NULL!!\n",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return False;
    }
    /*��ȡ�򿪵��ļ���Ϣ*/
    iRet = stat(pcFileName, &stStatBuf);
    if( 0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:stat error or %s isn't exist!!\n",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcFileName);
        return False;
    }
    /*�ж�ָ���ļ��Ƿ����ļ�*/
    if(!(S_IFREG & stStatBuf.st_mode))
    {
        printf("\nFile:%s, Line:%d,Func:%s:%s isn't general file!!\n",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcFileName);
        return False;
    }
    return True;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: CreateIdxFileSingle
* ��    ��: ����ָ���ļ�
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void CreateIdxFileSingle(char* pcFileName)
{
#define STORAGE_FUNCTION_NAME "CreateIdxFileSingle"
    FILE* pstRvFd = NULL;
    /*����ж�*/
    if(NULL == pcFileName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcFileName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    /*��ȡ�򿪵��ļ���Ϣ*/
    pstRvFd = fopen(pcFileName, "awb+");
    if(NULL == pstRvFd)
    {
        printf("\nFile:%s, Line:%d,Func:%s:fopen error",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    fclose(pstRvFd);
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������: DeleteIdxFileSingle
* ��    ��: ɾ��ָ���ļ�
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
void DeleteIdxFileSingle(char* pcFileName)
{
#define STORAGE_FUNCTION_NAME "DeleteIdxFileSingle"
    int iRet = STORAGE_FAIL;
    /*����ж�*/
    if(NULL == pcFileName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcFileName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    /*��ȡ�򿪵��ļ���Ϣ*/
    iRet = remove(pcFileName);
    if(-1 == iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:remove error",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������:GetFileSize_ByByte
* ��    ��: ��ȡ�ļ��Ĵ�С
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
long GetFileSize_ByByte(char* pcFileName)
{
#define STORAGE_FUNCTION_NAME "GetFileSize_ByByte"
    long iRvPos;
    FILE* pstFileFd = NULL;
    pstFileFd = fopen(pcFileName,"r");
    if(NULL == pcFileName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcFileName is NULL!!",
                  __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
	      return -1;
    }
    fseek(pstFileFd, 0, SEEK_END);
    iRvPos = ftell(pstFileFd);
    fclose(pstFileFd);
    return iRvPos;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������:GenerateFileName_ByCurTime
* ��    ��: ���ݵ�ǰʱ�������ļ���
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
int GenerateFileName_ByCurTime(char *pstFileName,char *pstFileType)
{
#define STORAGE_FUNCTION_NAME "GenerateFileName_ByCurTime"
    time_t tCurrentTime;
    struct tm *tmnow;
    struct timeval tv;
    if(NULL == pstFileName || NULL == pstFileType)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pstFileName||pstFileName is NULL!!",
                  __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return -1;
    }
    time(&tCurrentTime);
    gettimeofday(&tv,NULL);
    tmnow = localtime(&tCurrentTime);
    sprintf(pstFileName, "%04d%02d%02d%02d%02d%02d%03ld.%s",
        tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday,
        tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec, tv.tv_usec / 1000, pstFileType);
    return 0;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������:GenerateTarFileName_ByCurTime
* ��    ��: ���ݵ�ǰʱ������tar�ļ���
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
int GenerateTarFileName_ByCurTime(char *strFileName)
{
#define STORAGE_FUNCTION_NAME "GenerateTarFileName_ByCurTime"
    time_t tCurrentTime;
    struct tm *tmnow;
    struct timeval tv;
    if(NULL == strFileName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:strFileName is NULL!!",
                  __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return -1;
    }
    time(&tCurrentTime);
    gettimeofday(&tv,NULL);
    tmnow = localtime(&tCurrentTime);
    sprintf(strFileName, "%04d%02d%02d%02d%02d%02d%03ld.tar",
        tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday,
        tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec, tv.tv_usec / 1000);
    return 0;
#undef STORAGE_FUNCTION_NAME
}
/*******************************************************************************
* ��������:GenerateLocName_ByCurTimeAndCurPwd
* ��    ��: ���ݵ�ǰʱ�������ļ���
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
int GenerateLocName_ByCurTimeAndCurPwd(char *pcLocName,char *pstFileType)
{
#define STORAGE_FUNCTION_NAME "GenerateLocName_ByCurTimeAndCurPwd"
    char acLocNameTemp[STORAGE_IDX_LOC_LENGTH_MAX] = {0};
    char acFileNameTemp[STORAGE_IDX_NAME_LENGTH_MAX] = {0};
    char acDirNameTemp[STORAGE_IDX_DIR_LENGTH_MAX] = {0};
    if(NULL == pcLocName ||NULL == pstFileType)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcLocName||pstFileType is NULL!!",
                  __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return -1;
    }
    GetCurPwdName(acDirNameTemp);
    GenerateFileName_ByCurTime(acFileNameTemp,pstFileType);
    strcat(acLocNameTemp, acDirNameTemp);
    strcat(acLocNameTemp, acFileNameTemp);
    strncpy(pcLocName, acLocNameTemp, STORAGE_IDX_LOC_LENGTH_MAX);
    return 0;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������:GenerateLocName_ByCurTimeAndThePwd
* ��    ��: ���ݵ�ǰʱ�������ļ���
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
int GenerateLocName_ByCurTimeAndThePwd(char *pcLocName,char *pstFileType,char *pstDirName)
{
#define STORAGE_FUNCTION_NAME "GenerateLocName_ByCurTimeAndThePwd"
    char acLocNameTemp[STORAGE_IDX_LOC_LENGTH_MAX] = {0};
    char acFileNameTemp[STORAGE_IDX_NAME_LENGTH_MAX] = {0};
    //char acDirNameTemp[STORAGE_IDX_DIR_LENGTH_MAX] = {0};

    if(NULL == pcLocName ||NULL == pstFileType || NULL == pstDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcLocName||pstFileType||pstDirName is NULL!!",
                  __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return -1;
    }
    //GetCurPwdName(acDirNameTemp);
    GenerateFileName_ByCurTime(acFileNameTemp,pstFileType);
    strcat(acLocNameTemp, pstDirName);
    strcat(acLocNameTemp, "/");
    strcat(acLocNameTemp, acFileNameTemp);
    strncpy(pcLocName, acLocNameTemp, STORAGE_IDX_LOC_LENGTH_MAX);
    return 0;
#undef STORAGE_FUNCTION_NAME
}

/*******************************************************************************
* ��������:GenerateLocName_ByTheTimeAndThePwd
* ��    ��: ���ݵ�ǰʱ�������ļ���
* ����ĵ�:
* ��������: ��
* ��    ��: ��
* ��������            ����                             ����/���       ����
* -----------------------------------------------------------------------------
* ����ֵ:
* ˵   ��:   �ɹ�/���ɹ� 0/1
*******************************************************************************/
int GenerateLocName_ByTheTimeAndThePwd(char *pcLocName,char *pstFileName,char *pstDirName)
{
#define STORAGE_FUNCTION_NAME "GenerateLocName_ByCurTimeAndThePwd"
    char acLocNameTemp[STORAGE_IDX_LOC_LENGTH_MAX] = {0};
    //char acFileNameTemp[STORAGE_IDX_NAME_LENGTH_MAX] = {0};
    //char acDirNameTemp[STORAGE_IDX_DIR_LENGTH_MAX] = {0};

    if(NULL == pcLocName ||NULL == pstFileName || NULL == pstDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcLocName||pstFileName||pstDirName is NULL!!",
                  __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return -1;
    }
    //GetCurPwdName(acDirNameTemp);
    //GenerateFileName_ByCurTime(acFileNameTemp,pstFileType);
    strcat(acLocNameTemp, pstDirName);
    strcat(acLocNameTemp, "/");
    strcat(acLocNameTemp, pstFileName);
    strncpy(pcLocName, acLocNameTemp, STORAGE_IDX_LOC_LENGTH_MAX);
    return 0;
#undef STORAGE_FUNCTION_NAME
}
/********************************* Source Code End ********************************/
