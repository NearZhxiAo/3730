
#include "PtzControl.h"
#include <stdio.h>
#include <string.h>

CPtzControl::CPtzControl(void)
{
	m_serialPort = NULL;
}

CPtzControl::~CPtzControl(void)
{
	if (m_serialPort)
	{
		delete m_serialPort;
		m_serialPort = NULL;
	}
	
}

bool CPtzControl::PtzInit(UINT portNo , UINT baud , char parity , UINT databits , 
						  UINT stopsbits , DWORD dwCommEvents , PTZ_TYPE ptzType  ,
						  WORD moveSpeed , UINT logcialNum_out )
{

	if(m_serialPort != NULL )
	{
		if(m_serialPort->isPortWorking())
		{
			m_serialPort->closePort();
		}
		delete m_serialPort;
		m_serialPort = NULL;
	}
/*
	if(m_serialPort == NULL)
	{
		m_serialPort = new CSerialComm();
	}
*/	
	m_serialPort = new CSerialComm();
	m_serialPort->InitSerialPort(portNo, baud, parity, databits, stopsbits, dwCommEvents);
	if((m_serialPort ==NULL) || !(m_serialPort->isPortWorking()))
	{
	//	MessageBox(NULL,"���ڴ�ʧ��","������Ϣ",MB_OK);
		return false;
	}else{
	//	MessageBox(NULL,"���ڴ򿪳ɹ�","������Ϣ",MB_OK);
	}

	this->ptzType = ptzType;
	this->logicalNum = logcialNum_out;
	this->speed = moveSpeed;
	return true;

}
bool CPtzControl::PtzSetUp(WORD moveSpeed)
{
//	MessageBox(NULL,"��̨����","��̨����",MB_OK);
	this->speed = moveSpeed;
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x08,this->speed);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x08,this->speed);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}
bool CPtzControl::PtzSetDown(WORD moveSpeed)
{
	this->speed = moveSpeed;
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x10,this->speed);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x10,this->speed);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}



bool CPtzControl::PtzSetLeft(WORD moveSpeed)
{
	this->speed = moveSpeed;
	
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x04,speed<<8);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x04,speed<<8);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}

bool CPtzControl::PtzSetRight(WORD moveSpeed)
{
	this->speed = moveSpeed;
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x02,speed<<8);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x02,speed<<8);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}

bool CPtzControl::PtzAutoIRIS()
{
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}

bool CPtzControl::PtzPreSet(BYTE pre)
{
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x03,pre);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x03,pre);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}
bool CPtzControl::PtzGoToPre(BYTE pre)
{
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x07,pre);
			//Sleep(100);
			//SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x07,pre);
			//Sleep(100);
			//SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}
bool CPtzControl::PtzDelPre(BYTE pre)
{
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x05,pre);
			//Sleep(100);
			//SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x05,pre);
			//Sleep(100);
			//SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}
bool CPtzControl::PtzSetAutoFocus()
{
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			break;
		case PTZ_TYPE_PELCOP:
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}
//����
bool CPtzControl::PtzSetZoomIn()
{
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x20,0x00);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x40,0x00);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}

bool CPtzControl::PtzSetZoomOut()
{
	if((m_serialPort != NULL) && (m_serialPort->isPortWorking()))
	{
		switch (ptzType)
		{
		case PTZ_TYPE_PELCOD:
			SendData(0x40,0x00);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		case PTZ_TYPE_PELCOP:
			SendData(0x20,0x00);
			Sleep(100);
			SendData(0x00,0x00);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}


BYTE CPtzControl ::GetMod(unsigned char *buf,int len)
{
	WORD	modval=0;
	BYTE	retval=0;
	for(int i=0;i<len;i++) {
		modval += buf[i];
	}
	retval = (BYTE) (modval % 0x100);
	return retval;
}

BYTE CPtzControl::GetXOR(unsigned char *buf,int len)
{
	unsigned char retval = buf[0];
	for(int i=1; i<len; i++)
	{
		retval ^= buf[i]; 
	}
	return retval;
}

bool CPtzControl::SendData(WORD cmd,WORD data)
{
	if(m_serialPort == NULL)
	{
		return false;
	}

	BYTE m_sendBuff[128];
	memset(m_sendBuff , 0x00 , sizeof(m_sendBuff));
	
	m_sendBuff[1] = this->logicalNum; //�豸�߼�λ
	//MessageBox("%d",m_sendBuff[1]);
	m_sendBuff[2] = ((cmd>>8))&0xFF;   //����λ
	m_sendBuff[3] = cmd&0xFF;    //����λ
	m_sendBuff[4] = (data>>8)&0xFF;  //ˮƽ�ٶ�
	m_sendBuff[5] = data&0xFF;  //��ֱ�ٶ�
	if(ptzType == PTZ_TYPE_PELCOD)
	{	
		m_sendBuff[0] = 0xFF; //ͬ���ֽ�
		m_sendBuff[6]= GetMod(&m_sendBuff[1] , 5);  //У��λ
		m_serialPort->WriteData(m_sendBuff , 7);
	}
	if(ptzType == PTZ_TYPE_PELCOP)
	{
		m_sendBuff[0] = 0xA0; //ͬ���ֽ�
		m_sendBuff[6] = 0xAF;  //ֹͣλ
		m_sendBuff[7] = GetXOR(&(m_sendBuff[1]),5);
		m_serialPort->WriteData(m_sendBuff, 8);
 		
	}
		
	return true;
	
}

bool CPtzControl::PtzClose()
{
	if(m_serialPort!= NULL )
	{
		m_serialPort->CloseSerialPort();
		delete m_serialPort;
		m_serialPort = NULL;
		return true;
	}
	return false;
}