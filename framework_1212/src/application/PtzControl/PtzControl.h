#ifndef CPTZ_CONTROL_H
#define CPTZ_CONTROL_H

#include "SerialComm.h"
#include "PtzControlInterface.h"

class CPtzControl:public PtzControlInterface
{
public:
	CPtzControl(void);
	~CPtzControl(void);
	virtual bool PtzInit(unsigned int portNo = 1, unsigned int baud = CBR_2400, char parity = 'N', unsigned int databits = 8, 
		unsigned int stopsbits = 1, unsigned long dwCommEvents = EV_RXCHAR , PTZ_TYPE ptzType = PTZ_TYPE_PELCOD ,
			unsigned short moveSpeed=0x3F , unsigned int logcialNum = 0x01);
	virtual bool PtzSetUp(unsigned short speed);		
	virtual bool PtzSetDown(unsigned short speed);	
	virtual bool PtzSetLeft(unsigned short speed);	
	virtual bool PtzSetRight(unsigned short speed);	
	virtual bool PtzSetZoomIn();			
	virtual bool PtzSetZoomOut();		
	virtual bool PtzSetAutoFocus();		
	virtual bool PtzAutoIRIS();
	virtual bool PtzPreSet(unsigned char pre);	
	virtual bool PtzGoToPre(unsigned char pre);	
	virtual bool PtzDelPre(unsigned char pre);	
	virtual bool PtzClose();

	

private:
	PTZ_TYPE ptzType;
	unsigned short speed; 
	unsigned int logicalNum; 
	CSerialComm *m_serialPort;

private:
	unsigned char GetXOR(unsigned char *buf,int len);
	unsigned char GetMod(unsigned char *buf, int len);
	bool SendData(unsigned short cmd,unsigned short data);

};


#endif
