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
* 函数名称: ExistIdxDirSingle_if
* 功    能: 判断指定目录是否存在
* 相关文档:
* 函数类型: 无
* 参    数: 无
* -----------------------------------------------------------------------------
* 返回值:
*******************************************************************************/
int ExistIdxDirSingle_if(char *pcDirName)
{
#define STORAGE_FUNCTION_NAME "ExistIdxDirSingle_if"
    struct stat stStatBuf;
    int iRet = -1;
    /*入参判断*/
    if(NULL == pcDirName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcDirName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return False;
    }
    /*读取打开的文件信息*/
    iRet = stat(pcDirName, &stStatBuf);
    if(0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:stat error or %s isn't exist!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcDirName);
        return False;
    }
    /*判断指定文件是否是目录*/
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
* 函数名称: GetCurPwdName
* 功    能: 获取当前的工作目录
* 相关文档:
* 函数类型: 无
* 参    数: 无
* -----------------------------------------------------------------------------
* 返回值:
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
* 函数名称: CreateIdxDirSingle
* 功    能: 创建指定名目录
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:  成功/不成功 0/1
*******************************************************************************/
void CreateIdxDirSingle(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "CreateIdxDirSingle"
    int iRet = STORAGE_FAIL;
/*
    INT8 aucCmd[STORAGE_CMD_LENGTH];
    memset(aucCmd,0,sizeof(aucCmd));
*/
   /*入参判断*/
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
* 函数名称: CreateIdxDirMulti
* 功    能: 创建指定名目录
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:  成功/不成功 0/1
*******************************************************************************/
void CreateIdxDirMulti(char* pcDirName)
{
#define STORAGE_FUNCTION_NAME "CreateIdxDirMulti"
    int iRet = STORAGE_FAIL;
    char aucCmd[STORAGE_CMD_LENGTH_MAX];
    memset(aucCmd,0,sizeof(aucCmd));
    /*入参判断*/
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
* 函数名称: DeleteIdxDirSingle
* 功    能: 删除指定名目录
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称: DeleteIdxDirMulti
* 功    能: 删除指定名目录
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称: ChdirInIdxDirSingle
* 功    能: 近一级
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称: ChdirInIdxDirMulti
* 功    能: 进多级
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称: ChdirOutIdxDirSingle
* 功    能: 退一级
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称: ChdirOutIdxDirMulti
* 功    能: 退多级
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称:DevideIdxLocDir_By
* 功    能: 分解LocDir
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称: ExistIdxFileSingle_if
* 功    能: 判断指定文件是否存在
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
*******************************************************************************/
int ExistIdxFileSingle_if(char* pcFileName)
{
#define STORAGE_FUNCTION_NAME "ExistIdxFileSingle_if"
    struct stat stStatBuf;
    int iRet = STORAGE_FAIL;
    /*入参判断*/
    if(NULL == pcFileName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcFileName is NULL!!\n",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return False;
    }
    /*读取打开的文件信息*/
    iRet = stat(pcFileName, &stStatBuf);
    if( 0 != iRet)
    {
        printf("\nFile:%s, Line:%d,Func:%s:stat error or %s isn't exist!!\n",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME,pcFileName);
        return False;
    }
    /*判断指定文件是否是文件*/
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
* 函数名称: CreateIdxFileSingle
* 功    能: 创建指定文件
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
*******************************************************************************/
void CreateIdxFileSingle(char* pcFileName)
{
#define STORAGE_FUNCTION_NAME "CreateIdxFileSingle"
    FILE* pstRvFd = NULL;
    /*入参判断*/
    if(NULL == pcFileName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcFileName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    /*读取打开的文件信息*/
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
* 函数名称: DeleteIdxFileSingle
* 功    能: 删除指定文件
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
*******************************************************************************/
void DeleteIdxFileSingle(char* pcFileName)
{
#define STORAGE_FUNCTION_NAME "DeleteIdxFileSingle"
    int iRet = STORAGE_FAIL;
    /*入参判断*/
    if(NULL == pcFileName)
    {
        printf("\nFile:%s, Line:%d,Func:%s:pcFileName is NULL!!",
            __FILE__, __LINE__,STORAGE_FUNCTION_NAME);
        return ;
    }
    /*读取打开的文件信息*/
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
* 函数名称:GetFileSize_ByByte
* 功    能: 获取文件的大小
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称:GenerateFileName_ByCurTime
* 功    能: 根据当前时间生成文件名
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称:GenerateTarFileName_ByCurTime
* 功    能: 根据当前时间生成tar文件名
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称:GenerateLocName_ByCurTimeAndCurPwd
* 功    能: 根据当前时间生成文件名
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称:GenerateLocName_ByCurTimeAndThePwd
* 功    能: 根据当前时间生成文件名
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
* 函数名称:GenerateLocName_ByTheTimeAndThePwd
* 功    能: 根据当前时间生成文件名
* 相关文档:
* 函数类型: 无
* 参    数: 无
* 参数名称            类型                             输入/输出       描述
* -----------------------------------------------------------------------------
* 返回值:
* 说   明:   成功/不成功 0/1
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
