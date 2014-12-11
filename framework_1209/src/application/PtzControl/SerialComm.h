//////////////////////////////////////////////////////////////////////////  
/// COPYRIGHT NOTICE  
/// Copyright (c) 2009, xidian university����Ȩ������  
/// All rights reserved.  
///   
/// @file    SerialComm.h    
/// @brief   ����ͨ����ͷ�ļ�  
///  
/// ���ļ���ɴ���ͨ���������  
///  
/// @version 1.0     
/// @author  hzc	  
/// @E-mail��zhengchuanhu@gmail.com  
/// @date    2011/11/10
///  
///  �޶�˵����  
//////////////////////////////////////////////////////////////////////////  
#ifndef SERIALCOMM_H_
#define SERIALCOMM_H_

#include <Windows.h>
/*����ͨ����
*����ʵ���˶Դ��ڵĻ�������
*
*/
#define GET_DATA_MSG WM_USER+1

class CSerialComm
{
public:
	CSerialComm(void);
	~CSerialComm(void);
public:
	/** ��ʼ�����ں���
	*  @param:  UINT portNo ���ڱ��,Ĭ��ֵΪ1,��COM1,ע��,������Ҫ����9  
	*  @param:  UINT baud   ������,Ĭ��Ϊ9600  
	*  @param:  char parity �Ƿ������żУ��,'Y'��ʾ��Ҫ��żУ��,'N'��ʾ����Ҫ��żУ��  
	*  @param:  UINT databits ����λ�ĸ���,Ĭ��ֵΪ8������λ  
	*  @param:  UINT stopsbits ֹͣλʹ�ø�ʽ,Ĭ��ֵΪ1  
	*  @param:  DWORD dwCommEvents Ĭ��ΪEV_RXCHAR,��ֻҪ�շ�����һ���ַ�,�����һ���¼�  
	*  @return: bool  ��ʼ���Ƿ�ɹ�  
	*  @note:   ��ʹ�����������ṩ�ĺ���ǰ,���ȵ��ñ��������д��ڵĳ�ʼ��  
	*����������   \n�������ṩ��һЩ���õĴ��ڲ�������,����Ҫ����������ϸ��DCB����,��ʹ�����غ���  
	*           \n������������ʱ���Զ��رմ���,�������ִ�йرմ��� 
	*
	*/
	bool InitSerialPort(UINT portNo = 1, UINT baud = CBR_9600, char parity = 'N', UINT databits = 8, 
				UINT stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);
	/** ���ڳ�ʼ������  
     *  
     *  �������ṩֱ�Ӹ���DCB�������ô��ڲ���  
     *  @param:  UINT portNo  
     *  @param:  const LPDCB & plDCB  
     *  @return: bool  ��ʼ���Ƿ�ɹ�  
     *  @note:   �������ṩ�û��Զ���ش��ڳ�ʼ������  
     *  @see:      
     */ 
	bool InitSerialPort(UINT protNo, const LPDCB & plDCB);
	/** ���������߳�  
     *  
     *  �������߳���ɶԴ������ݵļ���,�������յ������ݴ�ӡ����Ļ���  
     *  @return: bool  �����Ƿ�ɹ�  
     *  @note:   ���߳��Ѿ����ڿ���״̬ʱ,����flase  
     *  @see:      
     */ 
//	bool OpenListenThread();
	 /** �رռ����߳�  
     *  
     *    
     *  @return: bool  �����Ƿ�ɹ�  
     *  @note:   ���ñ�������,�������ڵ��߳̽��ᱻ�ر�  
     *  @see:      
     */ 
	bool WriteData(unsigned char* pData,unsigned int lenght);
	/** ��ȡ���ڻ������е��ֽ���  
     *  
     *    
     *  @return: UINT  �����Ƿ�ɹ�  
     *  @note:   �����ڻ�������������ʱ,����0  
     *  @see:      
     */ 
	UINT GetBytesInSerialCom();
	 /** ��ȡ���ڽ��ջ�������һ���ֽڵ�����  
     *  
     *    
     *  @param:  char & cRecved ��Ŷ�ȡ���ݵ��ַ�����  
     *  @return: bool  ��ȡ�Ƿ�ɹ�  
     *  @note:     
     *  @see:      
     */ 
//	bool ReadChar(char &cRecved);
	/************************************************************************/
	/* �ر�ListenThread()�߳�
	/* @param: 
	/* @note:
	/* @see
	/************************************************************************/
//	bool CloseListenThread();
	/************************************************************************/
	/* �ж϶˿��Ƿ�������
	/* @param: 
	/* @note:
	/* @see
	/************************************************************************/
	bool isPortWorking();
	/** �رմ���  
     *  
     *    
     *  @return: bool  �����Ƿ�ɹ�  
     *  @note:     
     *  @see:      
     */ 
	bool CloseSerialPort();
public:
	/** �򿪴���  
     *  
     *    
     *  @param:  UINT portNo �����豸��  
     *  @return: bool  ���Ƿ�ɹ�  
     *  @note:     
     *  @see:      
     */ 
	bool openPort(UINT portNo);
	/** �رմ���  
     *  
     *    
     *  @return: void  �����Ƿ�ɹ�  
     *  @note:     
     *  @see:      
     */ 
	void closePort();
	/** ���ڼ����߳�  
     *  
     *  �������Դ��ڵ����ݺ���Ϣ  
     *  @param:  void * pParam �̲߳���  
     *  @return: UINT WINAPI �̷߳���ֵ  
     *  @note:     
     *  @see:      
     */ 
//	static UINT WINAPI ListenThread(void *param);
	/* �رմ��ڼ����߳�
	*
	*/
	

private:
	/* ���ھ��*/
	HANDLE m_hSerialComm;
	/* �߳��˳���־����*/
//	static bool s_bExit;
	/*�߳̾��*/
//	volatile HANDLE m_hListenThread;
	/*ͬ�����⣬�ٽ�������*/
	CRITICAL_SECTION m_csCommunicationSync;
	/*���ڴ��ڵ���Ϣ*/
	OVERLAPPED m_osWrite;	
//	OVERLAPPED m_osRead,m_osWrite;	
};
#endif