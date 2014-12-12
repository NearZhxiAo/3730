//  *************************************************************
//  Copyright (C) Technologies Co., Ltd.
//  
//  文件名      : SuperLog.h
//  文件描述    : SuperLog类特点 --1.性能要比较高
//							     --2.接口灵活 能写char*，string，CString，bin
//                               --3.智能 有线程，能主动写
//                               --4.灵活 可配置三到四秒写一次文件
//                               --5.合理 写日志可分级别 可调控的
//                               --6.强大 日志文件有多个 轮换写
// 
//  文件版本    : 1.0
//  作者        : gmaui
//  创建日期    : 2012-04-01
//  *************************************************************

//#pragma once
#ifndef __SUPERLOG_H__
#define __SUPERLOG_H__

const int MAX_BIN_LOG_INFO_LEN = 30*1024+1;  //待写入日志文件的二进制码流缓冲最大长度
const int MAX_STR_LOG_INFO_LEN = 3*1024+1;   //待写入日志文件的字符串缓冲最大长度
const int MAX_LOG_FILE_LEN     = 5*1024*1024;//待写入日志文件的最大长度
//const int MAX_LOG_FILE_LEN     = 5*1024/**1024*/;
// 日志文件最多可以写多少个
#define   MAX_LOG_FILE_COUNT   3             //待写入日志文件最大个数
#define   WRITE_LOG           CSuperLog::WriteLog     //待写入日志类 写操作统一接口
#define   LOG_LEVEL_DEBUG     CSuperLog::ENUM_LOG_LEVEL_DEBUG
#define   LOG_LEVEL_RUN       CSuperLog::ENUM_LOG_LEVEL_RUN
#define   LOG_LEVEL_ERROR     CSuperLog::ENUM_LOG_LEVEL_ERROR

#define   LOG_TYPE_INFO      CSuperLog::ENUM_LOG_TYPE_INFO 
#define   LOG_TYPE_WARNING	 CSuperLog::ENUM_LOG_TYPE_WARNING
#define   LOG_TYPE_ERROR	 CSuperLog::ENUM_LOG_TYPE_ERROR
#define   LOG_TYPE_FATAL	 CSuperLog::ENUM_LOG_TYPE_FATAL
#define   LOG_TYPE_END		 CSuperLog::ENUM_LOG_TYPE_END

#define   WELCOME_LOG_INFO    _T("\r\nLog Version 1.0\r\n")

class CSuperLog
{
private:
    // 日志写入器的状态
    enum enLogStatus
    {
        ENUM_LOG_INIT, 
        ENUM_LOG_RUN, 
        ENUM_LOG_EXITING, 
        ENUM_LOG_EXITED, 
        ENUM_LOG_INVALID,
    };

    static CRITICAL_SECTION m_csWriteLog;         // 写日志的临界变量
    static int              m_iCurLogFileSeq;     // 当前正在写入的文件序列号
    static CString          m_strWriteStrInfo;    // 待写入str
    static int              m_iWriteBinLogLen;    // 当前待写入的bin日志字节长度
    static CStdioFile*      m_pFile;              // 文件指针
    static int              m_iMaxLogFileLen;     // 单个文件最大长度

    static HANDLE           m_hThread;             // 线程句柄
    static unsigned         m_uiThreadID;          // 线程id
    static enLogStatus      m_enStatus;            // 当前log写入器的状态
    static bool             m_bRun;                // 当前log写入器是否结束
    static int              m_iLogLevel;           // 当前写日志的级别
    static HANDLE           m_hMapLogFile;         //映射到共享内存中
    static LPTSTR           m_psMapAddr;           //映射到共享内存到程序内存的地址

public:

    // 日志级别
    enum enLogInfoLevel
    {
        ENUM_LOG_LEVEL_DEBUG = 0, 
        ENUM_LOG_LEVEL_RUN, 
        ENUM_LOG_LEVEL_ERROR, 
    };
	// 日志类型
	enum enLogInfoType
    {
        ENUM_LOG_TYPE_INFO = 0, 
		ENUM_LOG_TYPE_WARNING, 
		ENUM_LOG_TYPE_ERROR, 
		ENUM_LOG_TYPE_FATAL,
		ENUM_LOG_TYPE_END,
    }; 
    CSuperLog(void);
    ~CSuperLog(void);

    static unsigned __stdcall LogProcStart( void* pParam );

    static enLogStatus OpenLogFile();                              // 打开写日志文件
    static int         WriteLogToFile();                           // 将日志文件的缓冲写入到文件中
    static CString&    GetCurTimeStr();                            // 获取当前时间字符串
	static CString&	   GetInfoType(enLogInfoType enType);          // 获取相应日志信息类型
    static int         WriteUnicodeHeadToFile(CFile * pFile);      // 向日志文件中写入Unicode头信息    
    static int         GetLogLevelShareMem();                      // 取得当前的日志级别
    static int         SetLogLevelShareMem(int iLevel);            // 设置当前的日志级别
    static int         OperaterConfig(BOOL bSave);                 // 把当前级别写到配置文件中
    static int         CheckLogLevel();                            // 从共享内存中读配置并设置当前的日志级别

    // 重载不同类型消息的写日志函数
#ifdef _DEBUG
    static int WriteLog(CString &strLog, enLogInfoType enType = ENUM_LOG_TYPE_INFO,enLogInfoLevel enLevel = ENUM_LOG_LEVEL_DEBUG, bool bForce = false);
    static int WriteLog(TCHAR* pstrLog,  enLogInfoType enType = ENUM_LOG_TYPE_INFO,enLogInfoLevel enLevel = ENUM_LOG_LEVEL_DEBUG, bool bForce = false);
    static int WriteLog(char* pszLog, int nLen, enLogInfoType enType = ENUM_LOG_TYPE_INFO, enLogInfoLevel enLevel = ENUM_LOG_LEVEL_DEBUG, bool bForce = false);
#else
    static int WriteLog(CString &strLog, enLogInfoType enType = ENUM_LOG_TYPE_INFO, enLogInfoLevel enLevel = ENUM_LOG_LEVEL_RUN, bool bForce = false);
    static int WriteLog(TCHAR* pstrLog,  enLogInfoType enType = ENUM_LOG_TYPE_INFO, enLogInfoLevel enLevel = ENUM_LOG_LEVEL_RUN, bool bForce = false);
    static int WriteLog(char* pszLog, int nLen, enLogInfoType enType = ENUM_LOG_TYPE_INFO, enLogInfoLevel enLevel = ENUM_LOG_LEVEL_RUN, bool bForce = false);
#endif
};

#endif
