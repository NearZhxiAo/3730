#ifndef _RECOGNIZER_H_
#define _RECOGNIZER_H_

#include "WorkMode.h"
#include "AppSV/APPInterface.h"
#include "eWatchSDK/eWatcherSDK.h"
#include "eWatcherStruct.h"
#include "H264_codec/H264_codec.h"
#include "PtzControl/PtzControl.h"
#include "PtzControl/PtzControlInterface.h"
#include "CJpegApi.h"

#define VIDEO_UDP_PORT 0x8100
#define VIDEO_TCP_PORT 0x8101

//�������ģʽ 
typedef int State;//ʶ����״̬
typedef enum _EN_REGTYPE_ //ʶ��������
{
	EN_REGTYPE_START,
	EN_REGTYPE_ANALOGVIDEO,  //ģ����Ƶ
	EN_REGTYPE_DIGITALVIDEO, //������Ƶ
	EN_REGTYPE_DIGITALALARM, //���ֱ���
	EN_REGTYPE_SENSORALARM,  //����������
	EN_REGTYPE_END,
}EN_REGTYPE;

static const int STATE_IDLE = 0; //����״̬
static const int STATE_RUN = 1;  //����״̬
static const int STATE_DEBUG = 2;//����״̬

//���ڽӿ���ƵĹ���ģʽ
//���ǵ��Ľӿ����
//1.ʶ������ʼ��
//2.ʶ����ģʽ�л�
//3.ʶ�����ͷ�
//4.
class Recognizer
{
public:
	virtual ~Recognizer();
	Recognizer();
public:

public:
	virtual BOOL Initilize() = 0;//ʶ������ʼ������
	virtual BOOL Start() = 0;//ʶ������ʼ����
	virtual BOOL Run() = 0;//ʶ��������״̬
	virtual BOOL Debug() = 0;//ʶ��������״̬
	virtual BOOL Stop() = 0;//ʶ����ֹͣ����
	virtual BOOL SwitchMode(State state) = 0;//ʶ���������л�
	virtual BOOL Release() = 0;//ʶ������Դ�ͷ�
	virtual State GetMode() = 0;
public:
	State m_State;
	CRITICAL_SECTION m_csState;
};

class AnalogVideoRecognizer:public Recognizer
{
public:
	~AnalogVideoRecognizer();
	AnalogVideoRecognizer();
private:
	BOOL InitDevCamera();
	BOOL StartDevCamera();
	BOOL StopDevCamera();
	static void  CALLBACK CallbackStartRun(PBYTE pDataBuf, int nDataLen, int nWidth, int nHeight,int nFrameType, void *pContext);
	static void  CALLBACK CallbackStartDebug(PBYTE pDataBuf, int nDataLen, int nWidth, int nHeight,int nFrameType, void *pContext);

public:
	BOOL Initilize();
	BOOL Start();
	BOOL Run();
	BOOL Debug();
	BOOL Stop();
	BOOL SwitchMode(State state);
	State GetMode(){return m_State;}
	BOOL Release();
private:
	//APPSVInterface��ָ��
	OutPutAPP_XIAN_WindowExt m_OutResultData;

	BOOL m_IsReSetAPPSVParam;//��־�Ƿ����������㷨����
	BOOL m_IsReSetVideoParam;//��־�Ƿ�����������Ƶ����
	
	int m_nVideoHeightSel;//video height of resolution
	int m_nVideoWidthSel;    //video width of resolution
	EteDVR_SrcType		m_nSrcTypeSel; //RGB24 Format
	EteDVR_CardModel	m_nCardModelSel; //Card Model Select
	
	int m_eWatcherNum;//��¼������ƵԴͨ������
	EWATCHER_CAPTURE_T m_eWatcherCapture[EWATCHER_MAX_CH_CNT];//��ͨ�������Ϣ
	//��Ƶ���ݽṹ
	Video_Encoder m_VideoEncode;

	//��Ƶ�����־
	bool m_IsVideoPreview;

	//Jpeg ͼƬ�������
	CJpegApi m_cJpegApi;
};

class DigitalVideoRecognizer:public Recognizer
{
public:
	~DigitalVideoRecognizer();
	DigitalVideoRecognizer();

public:
	BOOL InitNetwork();
	BOOL StartNetwork();
	static unsigned __stdcall ThreadForNetworkFunc(void * pParam);

public:
	BOOL Initilize();
	BOOL Start();
	BOOL Run();
	BOOL Debug();
	BOOL Stop();
	BOOL SwitchMode(State state);
	State GetMode(){return m_State;}
	BOOL Release();
private:
	//APPSVInterface��ָ��
	InParamAPP_XIAN_Window m_InputParam;
	OutPutAPP_XIAN_Window m_OutResultData;
	APPSVInterface * m_APPSVInterface;
	BOOL m_IsReSetAPPSVParam;//��־�Ƿ����������㷨����
	BOOL m_IsReSetVideoParam;//��־�Ƿ�����������Ƶ����
	
	//��Ƶ���ݽṹ
	Video_Encoder m_VideoEncode;
	
	//������Ƶ���ݽṹ
	SOCKET m_UdpRecvVideoDataSock; //�������ݵ��׽���
	UINT m_UdpRecvVideoDataPort; //�������ݵĶ˿�
	SOCKET m_TcpRecvCmdDataSock;   //����������׽���
	UINT m_TcpRecvCmdDataPort; //�������ݵĶ˿�
	
	HANDLE m_hThreadForNetworkHandle;
	UINT m_uiThreadForNetworkID;
	BOOL m_IsNetworkThreadExit;
	BOOL m_IsNetworkConnected;
};

class DigitalAlarmRecognizer:public Recognizer
{
public:
	~DigitalAlarmRecognizer();
	DigitalAlarmRecognizer();
public:
	BOOL Initilize();
	BOOL Start();
	BOOL Run();
	BOOL Debug();
	BOOL Stop();
	BOOL SwitchMode(State state);
	State GetMode(){return m_State;}
	BOOL Release();
};

class SensorAlarmRecognizer:public Recognizer
{
public:
	~SensorAlarmRecognizer();
	SensorAlarmRecognizer();
public:
	BOOL Initilize();
	BOOL Start();

	BOOL Run();
	BOOL Debug();
	BOOL Stop();
	BOOL SwitchMode(State state);
	State GetMode(){return m_State;}
	BOOL Release();
};

//ʶ��������һ��ֻ�ܴ������ɵ�һ��ʶ����
class RecognizerFactory
{
public:
	~RecognizerFactory();
	RecognizerFactory();
	static Recognizer *GenerateRecognizer(const EN_REGTYPE type);
};

#endif /*#ifndef _IDENTMAIN_H_*/