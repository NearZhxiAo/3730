#include "SerialComm.h"
#include <iostream>
#include <process.h>
#include <iostream>
#include <winbase.h>
/** 线程退出标志 */ 
//bool CSerialComm::s_bExit = false;
/** 当串口无数据时,sleep至下次查询间隔的时间,单位:秒 */   
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
	/*临时变量，将制定参数转化为字符串形式，构造DCB结构*/
	char szDCBparam[50];
	sprintf(szDCBparam,"COM1: baud=%d parity=%c data=%d stop=%d",baud,parity,databits,stopsbits);

	/*打开指定串口，该函数内部已经有临界区保护 上面将不要加保护*/
	if(!openPort(portNo))
	{
		MessageBox(NULL,"PtzControlLib_InitSerialPort_openPort","错误信息",MB_OK);
		return false;
	}
	/*进入临界区*/
	EnterCriticalSection(&m_csCommunicationSync);
	
	SetEvent(m_osWrite.hEvent);
	/*是否有错误发生*/
	BOOL bIsSuccess = TRUE;
	/*设置输入输出缓存区大小，如果不设置，则系统会设置默认值*/
	bIsSuccess = SetupComm(m_hSerialComm,1024,1024);
	if (bIsSuccess == FALSE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL,"PtzControlLib_InitSerialPort_SetupComm" , "错误信息" ,MB_OK);
		return false;
	}
	
	/*设置串口超时时间，设为0，表示不使用超时限制*/
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
		MessageBox(NULL,"PtzControlLib_InitSerialPort_SeCommTimeouts" , "错误信息" ,MB_OK);
		return false;
	}
	
	DCB dcb;

	//将ANSI字符串转化为UNICODE字符串
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
	wchar_t *pwText = new wchar_t[dwNum];
	MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum);
	/*获取当前串口配置参数，并且构造串口DCB参数*/

	bIsSuccess = GetCommState(m_hSerialComm,&dcb)&&BuildCommDCB(szDCBparam,&dcb);
	
	if(bIsSuccess == FALSE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL, "PtzControlLib_InitSerialPort_GetCommState" , "错误信息" , MB_OK);
		return false;
	}
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	/*释放内存空间*/
	delete[] pwText;

	bIsSuccess = SetCommState(m_hSerialComm,&dcb);
	if(bIsSuccess == FALSE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL, "PtzControlLib_InitSerialPort_SetCommState" , "错误信息" , MB_OK);
		return false;
	}
	/*清空串口缓存区*/
	PurgeComm(m_hSerialComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	
	/*离开临界区*/
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
	
}

bool CSerialComm::InitSerialPort(UINT protNo, const LPDCB & plDCB)
{
	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
	if (!openPort(protNo))
	{
		return false;
	}
	/*进入临界段*/
	EnterCriticalSection(&m_csCommunicationSync);

	/*配置串口参数*/
	if(!SetCommState(m_hSerialComm, plDCB))
	{	
		LeaveCriticalSection(&m_csCommunicationSync);
		MessageBox(NULL, "PtzControlLib_InitSerialPort_SetCommState" , "错误信息" , MB_OK);
		return false;
	}
	/*清空串口缓存区*/
	PurgeComm(m_hSerialComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	/*离开临界区*/
	LeaveCriticalSection(&m_csCommunicationSync);
	
	return true;

}

void CSerialComm::closePort()
{
	/*如果有串口被打开，关闭它*/
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
	/*进入临界区*/
	EnterCriticalSection(&m_csCommunicationSync);

	/*把串口的编号转化成设备名*/
	char szComPort[128];
	sprintf(szComPort,"COM%d",portNo);
	/*打开指定的串口*/
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
	/*临界区保护*/
	EnterCriticalSection(&m_csCommunicationSync);

	/*向缓冲区中写入指定的数据*/
	bResult = WriteFile(m_hSerialComm,pData,lenght,&BytesToSend,&m_osWrite);
	if(!bResult)
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osWrite.hEvent,1000);
			/*清空串口缓冲区*/
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
	/*进入临界区*/
	EnterCriticalSection(&m_csCommunicationSync);
	closePort();
	/*出临界区*/
	LeaveCriticalSection(&m_csCommunicationSync);
	return true;
}