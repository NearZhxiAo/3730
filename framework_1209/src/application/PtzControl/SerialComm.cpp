#include "SerialComm.h"
#include <iostream>
#include <process.h>
#include <iostream>
#include <winbase.h>
/** �߳��˳���־ */ 
//bool CSerialComm::s_bExit = false;
/** ������������ʱ,sleep���´β�ѯ�����ʱ��,��λ:�� */   
const UINT SLEEP_TIME_INTERVAL = 5;  

CSerialComm::CSerialComm(void)
{
	m_hSerialComm = INVALID_HANDLE_VALUE;

	memset(&m_osWrite,0x00,sizeof(OVERLAPPED));
	m_osWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	InitializeCriticalSection(&m_csCommunicationSync);
}

bool CSerialComm::InitSerialPort(UINT portNo , UINT baud , char parity , UINT databits , 
								 UINT stopsbits , DWORD dwCommEvents )
{
	/*��ʱ���������ƶ�����ת��Ϊ�ַ�����ʽ������DCB�ṹ*/
	char szDCBparam[50];
	sprintf(szDCBparam,"COM1: baud=%d parity=%c data=%d stop=%d",baud,parity,databits,stopsbits);

	/*��ָ�����ڣ��ú����ڲ��Ѿ����ٽ������� ���潫��Ҫ�ӱ���*/
	if(!openPort(portNo))
	{
		MessageBox(NULL,"PtzControlLib_InitSerialPort_openPort","������Ϣ",MB_OK);
		return false;
	}
	/*�����ٽ���*/
	EnterCriticalSection(&m_csCommunicationSync);
	
	SetEvent(m_osWrite.hEvent);
	/*�Ƿ��д�����*/
	BOOL bIsSuccess = TRUE;
	/*�������������������С����������ã���ϵͳ������Ĭ��ֵ*/
	bIsSuccess = SetupComm(m_hSerialComm,1024,1024);
	if (bIsSuccess == FALSE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL,"PtzControlLib_InitSerialPort_SetupComm" , "������Ϣ" ,MB_OK);
		return false;
	}
	
	/*���ô��ڳ�ʱʱ�䣬��Ϊ0����ʾ��ʹ�ó�ʱ����*/
	COMMTIMEOUTS commTimeOuts;
	commTimeOuts.ReadIntervalTimeout = 0;
	commTimeOuts.ReadTotalTimeoutConstant = 0;
	commTimeOuts.ReadTotalTimeoutMultiplier = 0;
	commTimeOuts.WriteTotalTimeoutConstant = 0;
	commTimeOuts.WriteTotalTimeoutMultiplier = 0;
	bIsSuccess = SetCommTimeouts(m_hSerialComm, &commTimeOuts);
	if (bIsSuccess == FALSE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL,"PtzControlLib_InitSerialPort_SeCommTimeouts" , "������Ϣ" ,MB_OK);
		return false;
	}
	
	DCB dcb;

	//��ANSI�ַ���ת��ΪUNICODE�ַ���
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
	wchar_t *pwText = new wchar_t[dwNum];
	MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum);
	/*��ȡ��ǰ�������ò��������ҹ��촮��DCB����*/

	bIsSuccess = GetCommState(m_hSerialComm,&dcb)&&BuildCommDCB(szDCBparam,&dcb);
	
	if(bIsSuccess == FALSE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL, "PtzControlLib_InitSerialPort_GetCommState" , "������Ϣ" , MB_OK);
		return false;
	}
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	/*�ͷ��ڴ�ռ�*/
	delete[] pwText;

	bIsSuccess = SetCommState(m_hSerialComm,&dcb);
	if(bIsSuccess == FALSE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL, "PtzControlLib_InitSerialPort_SetCommState" , "������Ϣ" , MB_OK);
		return false;
	}
	/*��մ��ڻ�����*/
	PurgeComm(m_hSerialComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	
	/*�뿪�ٽ���*/
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
	
}

bool CSerialComm::InitSerialPort(UINT protNo, const LPDCB & plDCB)
{
	/** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */
	if (!openPort(protNo))
	{
		return false;
	}
	/*�����ٽ��*/
	EnterCriticalSection(&m_csCommunicationSync);

	/*���ô��ڲ���*/
	if(!SetCommState(m_hSerialComm, plDCB))
	{	
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL, "PtzControlLib_InitSerialPort_SetCommState" , "������Ϣ" , MB_OK);
		return false;
	}
	/*��մ��ڻ�����*/
	PurgeComm(m_hSerialComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	/*�뿪�ٽ���*/
	LeaveCriticalSection(&m_csCommunicationSync);
	
	return true;

}

void CSerialComm::closePort()
{
	/*����д��ڱ��򿪣��ر���*/
	EnterCriticalSection(&m_csCommunicationSync);
	if(m_hSerialComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSerialComm);
		m_hSerialComm = INVALID_HANDLE_VALUE;
	}
	LeaveCriticalSection(&m_csCommunicationSync);
}

bool CSerialComm::openPort(UINT portNo)
{
	/*�����ٽ���*/
	EnterCriticalSection(&m_csCommunicationSync);

	/*�Ѵ��ڵı��ת�����豸��*/
	char szComPort[128];
	sprintf(szComPort,"COM%d",portNo);
	/*��ָ���Ĵ���*/
// 	m_hSerialComm = CreateFile(szComPort,
// 				 GENERIC_WRITE,
// 				0,
// 				NULL,
// 				OPEN_EXISTING,
// 				FILE_FLAG_OVERLAPPED,NULL);
	m_hSerialComm = CreateFile(szComPort,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);

	if(m_hSerialComm == INVALID_HANDLE_VALUE)
	{
		DWORD word = GetLastError();
		LeaveCriticalSection(&m_csCommunicationSync);
		
		return false;
	}

	LeaveCriticalSection(&m_csCommunicationSync);
	return true;

}


bool CSerialComm::WriteData(unsigned char* pData,unsigned int lenght)
{	
	BOOL bResult = TRUE;
	DWORD BytesToSend = 0;
	if(m_hSerialComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	/*�ٽ�������*/
	EnterCriticalSection(&m_csCommunicationSync);

	/*�򻺳�����д��ָ��������*/
	bResult = WriteFile(m_hSerialComm,pData,lenght,&BytesToSend,&m_osWrite);
	if(!bResult)
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osWrite.hEvent,1000);
			/*��մ��ڻ�����*/
			PurgeComm(m_hSerialComm,PURGE_RXCLEAR | PURGE_RXABORT);
			LeaveCriticalSection(&m_csCommunicationSync);
			return true;
		}
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}
	LeaveCriticalSection(&m_csCommunicationSync);
	return true;
}

CSerialComm::~CSerialComm(void)
{
	if (m_hSerialComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSerialComm);
		m_hSerialComm = INVALID_HANDLE_VALUE;
	} 
}


bool CSerialComm::isPortWorking()
{
	if(m_hSerialComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}else{
		return true;
	}

}

bool CSerialComm::CloseSerialPort()
{
	/*�����ٽ���*/
	EnterCriticalSection(&m_csCommunicationSync);
	closePort();
	/*���ٽ���*/
	LeaveCriticalSection(&m_csCommunicationSync);
	return true;
}