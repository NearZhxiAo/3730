//  *************************************************************
//  Copyright (C) Technologies Co., Ltd.
//  
//  �ļ���      : SuperLog.h
//  �ļ�����    : SuperLog���ص� --1.����Ҫ�Ƚϸ�
//							     --2.�ӿ���� ��дchar*��string��CString��bin
//                               --3.���� ���̣߳�������д
//                               --4.��� ��������������дһ���ļ�
//                               --5.���� д��־�ɷּ��� �ɵ��ص�
//                               --6.ǿ�� ��־�ļ��ж�� �ֻ�д
// 
//  �ļ��汾    : 1.0
//  ����        : gmaui
//  ��������    : 2012-04-01
//  *************************************************************

//#pragma once
#ifndef __SUPERLOG_H__
#define __SUPERLOG_H__

const int MAX_BIN_LOG_INFO_LEN = 30*1024+1;  //��д����־�ļ��Ķ���������������󳤶�
const int MAX_STR_LOG_INFO_LEN = 3*1024+1;   //��д����־�ļ����ַ���������󳤶�
const int MAX_LOG_FILE_LEN     = 5*1024*1024;//��д����־�ļ�����󳤶�
//const int MAX_LOG_FILE_LEN     = 5*1024/**1024*/;
// ��־�ļ�������д���ٸ�
#define   MAX_LOG_FILE_COUNT   3             //��д����־�ļ�������
#define   WRITE_LOG           CSuperLog::WriteLog     //��д����־�� д����ͳһ�ӿ�
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
    // ��־д������״̬
    enum enLogStatus
    {
        ENUM_LOG_INIT, 
        ENUM_LOG_RUN, 
        ENUM_LOG_EXITING, 
        ENUM_LOG_EXITED, 
        ENUM_LOG_INVALID,
    };

    static CRITICAL_SECTION m_csWriteLog;         // д��־���ٽ����
    static int              m_iCurLogFileSeq;     // ��ǰ����д����ļ����к�
    static CString          m_strWriteStrInfo;    // ��д��str
    static int              m_iWriteBinLogLen;    // ��ǰ��д���bin��־�ֽڳ���
    static CStdioFile*      m_pFile;              // �ļ�ָ��
    static int              m_iMaxLogFileLen;     // �����ļ���󳤶�

    static HANDLE           m_hThread;             // �߳̾��
    static unsigned         m_uiThreadID;          // �߳�id
    static enLogStatus      m_enStatus;            // ��ǰlogд������״̬
    static bool             m_bRun;                // ��ǰlogд�����Ƿ����
    static int              m_iLogLevel;           // ��ǰд��־�ļ���
    static HANDLE           m_hMapLogFile;         //ӳ�䵽�����ڴ���
    static LPTSTR           m_psMapAddr;           //ӳ�䵽�����ڴ浽�����ڴ�ĵ�ַ

public:

    // ��־����
    enum enLogInfoLevel
    {
        ENUM_LOG_LEVEL_DEBUG = 0, 
        ENUM_LOG_LEVEL_RUN, 
        ENUM_LOG_LEVEL_ERROR, 
    };
	// ��־����
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

    static enLogStatus OpenLogFile();                              // ��д��־�ļ�
    static int         WriteLogToFile();                           // ����־�ļ��Ļ���д�뵽�ļ���
    static CString&    GetCurTimeStr();                            // ��ȡ��ǰʱ���ַ���
	static CString&	   GetInfoType(enLogInfoType enType);          // ��ȡ��Ӧ��־��Ϣ����
    static int         WriteUnicodeHeadToFile(CFile * pFile);      // ����־�ļ���д��Unicodeͷ��Ϣ    
    static int         GetLogLevelShareMem();                      // ȡ�õ�ǰ����־����
    static int         SetLogLevelShareMem(int iLevel);            // ���õ�ǰ����־����
    static int         OperaterConfig(BOOL bSave);                 // �ѵ�ǰ����д�������ļ���
    static int         CheckLogLevel();                            // �ӹ����ڴ��ж����ò����õ�ǰ����־����

    // ���ز�ͬ������Ϣ��д��־����
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
