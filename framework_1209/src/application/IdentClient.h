#ifndef _IDENTCLIENT_H_
#define _IDENTCLIENT_H_

#ifdef _cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include "SocketApi.h"
#include "CmdStruct.h"
#include "CmdDef.h"
#include "Ptz.h"
#include "ParamMgr.h"
#include "SystemConf.h"
#include "system_debug.h"
#include "MediaConf.h"
#include "Global.h"
#include <string>
    using namespace std;
//#include "PtzControl/PtzControl.h"


typedef struct _IDENT_CLIENT_CONN_STRUCT_
{
    int Sock;
    unsigned int   Timer;
}IDENT_CLIENT_CONN_STRUCT;

//class Recognizer;

class IdentClient{
public:
	IdentClient();
	~IdentClient();

private:
		
	IDENT_CLIENT_CONN_STRUCT m_IdentClientConn;

	static bool m_IsSvrConnected;
	static bool m_IsSvrThreadExit;

	
    pthread_t m_hThreadForScanHandle;

	pthread_t m_hThreadForCommHandle;


	pthread_t m_hThreadForSendPicHandle;

	CMD_TRANSFER m_cmdTransfer;
public:
	
	static void*  ThreadForSendPicFunc(void * pParam);//模拟发送图片线程
	static void*  ThreadForScanFunc(void * pParam);
	static void*  ThreadForCommFunc(void * pParam);
	//static void CALLBACK TimerOutProc(HWND hWnd,UINT nMsg,UINT nTimerid,unsigned long dwTime);

	bool Initilize();
	bool CreateAllThreads();
	bool CommandProc(unsigned char *buf, unsigned int cmd);
	bool SendParaPtz();
	bool SendParaVideo();
	bool SendParaAlg();
	bool SendParaAlgConf(int num);
	bool SendParaAlgQuad(int num);
	bool SendParaAlgArea(int num);
	bool SendParaAlgLine(int num);
	bool SendParaAlgCount(int num);
	bool SendParaAlgEnter(int num);
	bool SendParaAlgOut(int num);
	//bool SendParaAlgStop(int num);
	bool SendParaAlgLost(int num);
	bool SendParaAlgDirect(int num);
	bool SendParaAlgDetect(int num);
	bool SendParaAlgPeopleDense(int num);
	bool SendParaAlgClusterDete(int num);
	bool SendParaAlgMotionHistory(int num);
	bool SendParaAlgHoverDete(int num);
	bool SendParaAlgFindBag(int num);
	bool SendParaAlgAlarmLine(int num);
	bool SendParaAlgRunning(int num);

	bool SendParaSysConfig();
	bool SendParaAlarmYM();
	bool SendParaPicContents(unsigned char *fname);
	bool SendParaSysStorage();
	bool SendParaAlarmDay();
	bool SendParaAlarName();
 
	bool SendParaSetAck(int num,bool flag);
	bool SendParaMsg();
protected:
private:
	bool       m_tachReg;

};
	
#ifdef _cplusplus
}
#endif



#endif /*#ifndef _IDENTCLIENT_H_*/
