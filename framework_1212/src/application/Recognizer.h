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

//工厂设计模式 
typedef int State;//识别器状态
typedef enum _EN_REGTYPE_ //识别器类型
{
	EN_REGTYPE_START,
	EN_REGTYPE_ANALOGVIDEO,  //模拟视频
	EN_REGTYPE_DIGITALVIDEO, //数字视频
	EN_REGTYPE_DIGITALALARM, //数字报警
	EN_REGTYPE_SENSORALARM,  //传感器报警
	EN_REGTYPE_END,
}EN_REGTYPE;

static const int STATE_IDLE = 0; //空闲状态
static const int STATE_RUN = 1;  //运行状态
static const int STATE_DEBUG = 2;//调试状态

//基于接口设计的工厂模式
//考虑到的接口设计
//1.识别器初始化
//2.识别器模式切换
//3.识别器释放
//4.
class Recognizer
{
public:
	virtual ~Recognizer();
	Recognizer();
public:

public:
	virtual BOOL Initilize() = 0;//识别器初始化工作
	virtual BOOL Start() = 0;//识别器开始工作
	virtual BOOL Run() = 0;//识别器运行状态
	virtual BOOL Debug() = 0;//识别器调试状态
	virtual BOOL Stop() = 0;//识别器停止工作
	virtual BOOL SwitchMode(State state) = 0;//识别器工作切换
	virtual BOOL Release() = 0;//识别器资源释放
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
	//APPSVInterface的指针
	OutPutAPP_XIAN_WindowExt m_OutResultData;

	BOOL m_IsReSetAPPSVParam;//标志是否重新设置算法参数
	BOOL m_IsReSetVideoParam;//标志是否重新设置视频参数
	
	int m_nVideoHeightSel;//video height of resolution
	int m_nVideoWidthSel;    //video width of resolution
	EteDVR_SrcType		m_nSrcTypeSel; //RGB24 Format
	EteDVR_CardModel	m_nCardModelSel; //Card Model Select
	
	int m_eWatcherNum;//记录所有视频源通道总数
	EWATCHER_CAPTURE_T m_eWatcherCapture[EWATCHER_MAX_CH_CNT];//各通道相关信息
	//视频数据结构
	Video_Encoder m_VideoEncode;

	//视频请求标志
	bool m_IsVideoPreview;

	//Jpeg 图片处理对象
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
	//APPSVInterface的指针
	InParamAPP_XIAN_Window m_InputParam;
	OutPutAPP_XIAN_Window m_OutResultData;
	APPSVInterface * m_APPSVInterface;
	BOOL m_IsReSetAPPSVParam;//标志是否重新设置算法参数
	BOOL m_IsReSetVideoParam;//标志是否重新设置视频参数
	
	//视频数据结构
	Video_Encoder m_VideoEncode;
	
	//网络视频数据结构
	SOCKET m_UdpRecvVideoDataSock; //接收数据的套接字
	UINT m_UdpRecvVideoDataPort; //接收数据的端口
	SOCKET m_TcpRecvCmdDataSock;   //接收命令的套接字
	UINT m_TcpRecvCmdDataPort; //接收数据的端口
	
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

//识别器工厂一次只能创建生成单一的识别器
class RecognizerFactory
{
public:
	~RecognizerFactory();
	RecognizerFactory();
	static Recognizer *GenerateRecognizer(const EN_REGTYPE type);
};

#endif /*#ifndef _IDENTMAIN_H_*/