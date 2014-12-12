 /************************************************************************/
//  Copyright (C) Technologies
//  
//  File name  : IdentMain.cpp
//  Description: 识别器主进程的实现
//  Version    : 1.0
//  Author     : 
//  Created    : 
/************************************************************************/
#include "Stdafx.h"
#include "Identifier.h"
#include "Recognizer.h"
#include "IdentClient.h"
#include "IdentCenter.h"
#include "process.h"
#include "SuperLog.h"
#include "CmdConstruct/CmdStruct.h"
#include "PtzControl/PtzControl.h"
#include "PtzControl/PtzControlInterface.h"
#include "ParamMgr.h"
#include "CJpegApi.h"

//连接库
#pragma comment(lib, "AppSV/APPSV.lib")
#pragma comment(lib,"AlgAppLib/Release/AlgAppLib.lib")
#pragma comment(lib, "eWatchSDK/eWatcherSDK.lib")
#pragma comment(lib, "H264_codec/Release/H264_codec.lib")
#pragma comment(lib, "SocketApi/Release/SocketApi.lib")
#pragma comment(lib, "PtzControl/Release/PtzControl.lib")

#define PTZ_PREPOINT_INIT PTZ_PREPOINT_NUM_MAX


//全局数据区
unsigned char pSendBuf[VIDEO_WIDTH * VIDEO_HEIGHT] = {0};//H264编码数据buf区
const CString g_strAlarmTypeDes[] = {_T("模拟"),_T("回型"),_T("禁区"),_T("绊线"),_T("计数"),
							   _T("目标进入"),_T("目标离开"),/*_T("物体遗留"),*/_T("物体消失"),
							   _T("方向报警"),_T("入侵侦测"),_T("人员密度"),_T("打架"),_T("人员聚集"),
							   _T("夜间起床"),_T("跌倒"),_T("徘徊"),_T("物体遗留"),
							   _T("奔跑"),_T("离岗"),_T("警戒线"),_T("声音"),_T("误报")};
//static OutPutApp_Result_Ext g_stOutPutAppResultExt;
typedef struct _SV_OUT_RESULT_T
{
	int type;
	int id;
}SV_OUT_RESULT_T;

static SV_OUT_RESULT_T g_stSVOutResult;
//全局函数区
bool Send485Msgif();
static unsigned int ComputePrePointByAlgandArea(ALG_EXTEND_TYPE type,int AreaNo);//计算出云台预置点
static unsigned int ComputePreTimeByAlgandArea(ALG_EXTEND_TYPE type,int AreaNo);//计算出云台移动到预置点时间
static bool IsSVAlarmExistIf();//判断是否存在报警事件

extern CIdentifierApp theApp;
extern bool g_SimAlarmOne;

Recognizer::Recognizer()
{

}
Recognizer::~Recognizer()
{

}

//包括:
//1.主线程运行的相关环境初始化，即相关变量合法化
//2.开启与中心通信的线程(延后实现)
//3.开启与客户端通信的线程
//4.算法初始化--涉及到调试时修改相关初始化值 需特殊对待 提供IdentALG_Init()和IdentALG_ReInit()两个方法来实现


//AnalogVideoRecognizer 模拟数据识别器类
AnalogVideoRecognizer::AnalogVideoRecognizer()
{
	m_State = STATE_IDLE;
	m_IsReSetAPPSVParam = false;//标志是否重新设置算法参数
	m_IsReSetVideoParam = false;//标志是否重新设置视频参数
	//初始化状态访问关键区
	InitializeCriticalSection(&m_csState);
}

AnalogVideoRecognizer::~AnalogVideoRecognizer()
{
	
}

BOOL AnalogVideoRecognizer::Initilize()
{
	InitDevCamera();
	return TRUE;
}

BOOL AnalogVideoRecognizer::Start()
{
	StartDevCamera();
	return TRUE;
}

BOOL AnalogVideoRecognizer::Run()
{
	return TRUE;
}

BOOL AnalogVideoRecognizer::Debug()
{
	return TRUE;
}

BOOL AnalogVideoRecognizer::Stop()
{
	StopDevCamera();
	return TRUE;
}

// *************************************************************
// 函数名          : AnalogVideoRecognizer::SwitchMode
// 函数功能描述    : 模拟视频切换状态函数
// 函数返回值      : BOOL 成功--true 失败--false
// 函数参数        : State (RUN/DEBUG)切换到的状态 存在情况 idle->run//第一次启动时存在
//					                             run<->debug//客户端来控制                  
// 修改记录        :
// 修改日期--------修改人-------修改说明
// |2012-04-01    |gmaui       | 初始版本                      |
// |              |            |                               |
// *************************************************************
BOOL AnalogVideoRecognizer::SwitchMode(State state)
{
	if(m_State == state)
	{
		return TRUE;
	}
	else
	{

		Stop();
		if (g_VideoResolutionChanged)
		{
			Release();
		}
		EnterCriticalSection(&m_csState);
		m_State = state;
		LeaveCriticalSection(&m_csState);	
		if (g_VideoResolutionChanged)
		{
			Initilize();
		}
		Start();
		g_VideoResolutionChanged = false;
		return TRUE;
	}
}

BOOL AnalogVideoRecognizer::Release()
{
	EteDVR_CloseSDK();
	if(!H264_QUIT(&m_VideoEncode))
	{
		WRITE_LOG(_T("H262编码模块退出!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	}
	return TRUE;
}

BOOL AnalogVideoRecognizer::InitDevCamera()
{
	m_nCardModelSel = EteDVR_CardModel_M878_2v2;
	m_nVideoWidthSel = VIDEO_WIDTH;   
	m_nVideoHeightSel = VIDEO_HEIGHT;
	m_nSrcTypeSel = SrcType_RGB24;
	m_VideoEncode.codec_id = CODEC_ID_H264; /*编码器选择*/
	m_VideoEncode.pix_format = PIX_FMT_RGB24; /*编码数据源选择*/
	//m_VideoEncode.pix_format = PIX_FMT_YUV420P; /*编码数据源选择*/
	m_VideoEncode.type = ENCDEC_TYPE_ENC;
	
	memset(&g_stSVOutResult,0x00,sizeof(g_stSVOutResult));
	//通过对方的设置来决定使用图像的分辨率  --1.cif 2.d1
	switch(g_stGlobal_Variable.globalflash.videopara.analoginfo.vframetype)
	{
	case ANALOG_VFRAME_TYPE_QCIF:
		{
			m_nVideoWidthSel = 192;
			m_nVideoHeightSel = 160;
		}
		break;
	case ANALOG_VFRAME_TYPE_CIF:
		{
			m_nVideoWidthSel = 384;
			m_nVideoHeightSel = 288;
		}
		break;
	case ANALOG_VFRAME_TYPE_D1:
		{
			m_nVideoWidthSel = 768;
			m_nVideoHeightSel = 576;
		}
		break;
	case ANALOG_VFRAME_TYPE_START:
	case ANALOG_VFRAME_TYPE_END:
	default:
		{
			g_stGlobal_Variable.globalflash.videopara.analoginfo.vframetype = ANALOG_VFRAME_TYPE_D1;
			m_nVideoWidthSel = VIDEO_WIDTH;   
			m_nVideoHeightSel = VIDEO_HEIGHT;
		}
		break;
	}
	switch(g_stGlobal_Variable.globalflash.videopara.analoginfo.vbittype)
	{
	case ANALOG_VBIT_TYPE_START:
	case ANALOG_VBIT_TYPE_RGB24:
	case ANALOG_VBIT_TYPE_YV12:
	case ANALOG_VBIT_TYPE_YVY2:
	case ANALOG_VBIT_TYPE_END:
	default:
		{
			//m_nSrcTypeSel = SrcType_YV12;
			m_nSrcTypeSel = SrcType_RGB24;
		}
		break;
	}
	if(!H264_INIT(&m_VideoEncode,m_nVideoWidthSel,m_nVideoHeightSel))
	{
		WRITE_LOG(_T("模拟识别器H264编码初始化成功!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	}
	else
	{
		WRITE_LOG(_T("模拟识别器H264编码初始化失败!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
	}

	//初始化图像采集卡 暂时作为单个设备处理
	m_eWatcherNum = 0;
	WRITE_LOG(_T("模拟识别器采集卡初始化!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	int nRet = EteDVR_InitSDK((int)m_nCardModelSel);
	if(nRet == SUCCEEDED )
	{
		int nDevNum;
		EteDVR_GetDevCount(&nDevNum);
		
		for(int i = 0; i < nDevNum; i++)
		{
			int nDevChNum ;
			EteDVR_GetDevChCount(i,&nDevChNum);
			
			for(int j = 0; j < nDevChNum;j++)
			{

				CString errorInfo;
				nRet = EteDVR_SetResolution(i,j,m_nVideoWidthSel,m_nVideoHeightSel,m_nSrcTypeSel);
				if(nRet != SUCCEEDED)//设置分辨率
				{
					errorInfo.Format(_T("EteDVR_SetResolution 错误dev=%d chm=%d error=%d!\r\n"),i,j,nRet);
					WRITE_LOG(errorInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				}
				// 				if(EteDVR_SetVideoParam(i,j,BRIGHTNESS,g_stGlobal_Variable.globalflash.videopara.bright) != SUCCEEDED)
				// 					WRITE_LOG(_T("EteDVR_SetVideoParam BRIGHTNESS 错误!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				// 				if(EteDVR_SetVideoParam(i,j,HUE,g_stGlobal_Variable.globalflash.videopara.hue) != SUCCEEDED)
				// 					WRITE_LOG(_T("EteDVR_SetResolution HUE 错误!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				// 				if(EteDVR_SetVideoParam(i,j,CONTRAST,g_stGlobal_Variable.globalflash.videopara.contrast) != SUCCEEDED)
				// 					WRITE_LOG(_T("EteDVR_SetResolution CONTRAST 错误!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				// 				if(EteDVR_SetVideoParam(i,j,SATURATION,g_stGlobal_Variable.globalflash.videopara.saturation) != SUCCEEDED)
				// 					WRITE_LOG(_T("EteDVR_SetResolution SATURATION 错误!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				nRet = EteDVR_EnableDeinterlace(i,j,true);
				if(nRet != SUCCEEDED)
				{
					errorInfo.Format(_T("EteDVR_EnableDeinterlace 错误dev=%d chm=%d error=%d !\r\n"),i,j,nRet);
					WRITE_LOG(errorInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				}
				m_eWatcherCapture[m_eWatcherNum].pHandle = this;
				m_eWatcherCapture[m_eWatcherNum].nDevNum = i;
				m_eWatcherCapture[m_eWatcherNum].nChNum = j;
				m_eWatcherCapture[m_eWatcherNum].nPicCnt = 1;
				m_eWatcherCapture[m_eWatcherNum].bAlarm = false;
				m_eWatcherCapture[m_eWatcherNum].iAlarmType = 0;
				m_eWatcherCapture[m_eWatcherNum].dPtzTime = 0;
				m_eWatcherNum++;
			}
		}
	}
	else{
		WRITE_LOG(_T("设备打开失败!!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
		return FALSE;
	}
	WRITE_LOG(_T("模拟识别器初始化完成!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	return TRUE;
}

/*	
向动态库注册回调函数，并且启动回调函数
需要传递的数据是相机的坐标。this的对象 
*/
BOOL AnalogVideoRecognizer::StartDevCamera()
{
	int nRet = 0;
	int nCameraIdx = 0;
	int nDevNum;

// 	EteDVR_GetDevCount(&nDevNum);
// 	for(int i = 0; i < nDevNum ; i++)
// 	{
// 		int nDevChNum ;
// 		EteDVR_GetDevChCount(i,&nDevChNum);
// 		for(int j = 0;j < nDevChNum; j++)
// 		{
			//AUTO:
			//分两种情况 一。运行 CallbackStartRun 二。调试 CallbackStartDebug
			//if(EN_WORKINRUN == GET_WORKMODE())
			switch(m_State)
			{
			case STATE_IDLE:
				{
					EteDVR_RegisterImageCaptueCallback(0, 0, NULL, NULL);
				}
				break;
			case STATE_RUN:
				{
					EteDVR_RegisterImageCaptueCallback(0, 0, CallbackStartRun, &m_eWatcherCapture[nCameraIdx]);
				}
				break;
			case STATE_DEBUG:
				{
					EteDVR_RegisterImageCaptueCallback(0, 0, CallbackStartDebug, &m_eWatcherCapture[nCameraIdx]);
				}
				break;
			default:
				break;
			}
//			nCameraIdx++;
// 		}
// 
// 	}
	return TRUE;
}

BOOL AnalogVideoRecognizer::StopDevCamera()
{
	int m_nDevNum;
	EteDVR_GetDevCount(&m_nDevNum);
	for(int i = 0; i < m_nDevNum ; i++)
	{
		switch (m_State)
		{
		case STATE_IDLE:
		case STATE_RUN:
		case STATE_DEBUG:	
			{
				EteDVR_RegisterImageCaptueCallback(i, 0, NULL, NULL);
				break;
			}
		default:
			break;
		}
	}
	return TRUE;
}


// *************************************************************
// 函数名          : AnalogVideoRecognizer::CallbackStartRun
// 函数功能描述    : 模拟识别器 运行状态时回调注册函数
// 函数返回值      : void
// 函数参数        : pDataBuf     模拟视频采集数据缓冲
//                   nDataLen     模拟视频采集数据总长度
//                   nWidth       模拟视频采集图片宽度
//                   nHeight      模拟视频采集图片高度
//                   nFrameType   模拟视频采集图片帧类型  YUV420P (RGB24)
//                   pContext     模拟视频回调函数注册参数
// 修改记录        :
// 修改日期--------修改人-------修改说明
// |2012-04-01    |gmaui       | 初始版本                      |
// |              |            |                               |
// *************************************************************
void  CALLBACK AnalogVideoRecognizer::CallbackStartRun(PBYTE pDataBuf, int nDataLen, int nWidth, int nHeight,int nFrameType, void *pContext)
{
	//WRITE_LOG(_T("模拟识别器回调运行程序开始!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	EWATCHER_CAPTURE_T *peWatcherCapture = (EWATCHER_CAPTURE_T *)pContext;
	AnalogVideoRecognizer *pThis = (AnalogVideoRecognizer  *)(peWatcherCapture->pHandle);
	bool IsVertReversedRGB24 = false;//记录缓冲中图片是否被上下翻转
	bool IsAppSVParaSetSuccess = false;
	//TODO:
	//只做dev0 ch0 的视频处理
	if(peWatcherCapture->nDevNum == 0 && peWatcherCapture->nChNum == 0)
	{
		//WRITE_LOG(_T("模拟识别器回调运行程序 算法执行!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 		CString cstrinfo;
// 		cstrinfo.Format("当前帧参数 height = %d width = %d datalen = %d\r\n",
// 			nWidth,nWidth,nDataLen);
// 		WRITE_LOG(cstrinfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);

		//算法要求输入RGB是采集卡出来的格式 所以这里不要翻转
//		if(!IsVertReversedRGB24)
//		{
//			pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
//			IsVertReversedRGB24 = true;
//		}
		//pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
		if(!peWatcherCapture->bAlarm)//如果该通道上没有报警
		{
			//AUTO:
			//考虑是否根据时间需要切换算法
			
			SYSTEMTIME stSystemTime;
			GetLocalTime(&stSystemTime);
// 			SYSTEMTIME stSystemTimeDayStart = stSystemTime;//早上8:00
// 			SYSTEMTIME stSystemTimeDayEnd = stSystemTime;  // 晚上18:00
// 			stSystemTimeDayStart.wHour = 8;
// 			stSystemTimeDayStart.wMinute = 0;
// 			stSystemTimeDayStart.wSecond = 0;
// 			stSystemTimeDayStart.wMilliseconds = 0;
// 			stSystemTimeDayEnd.wHour = 18;
// 			stSystemTimeDayEnd.wMinute = 0;
// 			stSystemTimeDayEnd.wSecond = 0;
// 			stSystemTimeDayEnd.wMilliseconds = 0;
// 			
// 			COleDateTime dTime(stSystemTime);
//  			COleDateTime dTimeStart(stSystemTimeDayStart);
//  			COleDateTime dTimeEnd(stSystemTimeDayEnd);
// 			if(dTime >= dTimeStart && dTime <= dTimeEnd)//白天时间段
// 			{
// 				if(g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed != ALG_TIME_DAY)
// 				{
// 					//0-白天 1-晚上
// 					ALG_SETPARAM(&g_stGlobal_Variable.globalflash,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 					g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed = ALG_TIME_DAY;
// 					WRITE_LOG(_T("切换到白天算法参数\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
// 				}
// 			}
// 			else//晚上时间段
// 			{
// // 				if(g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed != ALG_TIME_NIGHT)
// // 				{
// 					//0-白天 1-晚上
// 					ALG_SETPARAM(&g_stGlobal_Variable.globalflash,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// /*					g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed = ALG_TIME_NIGHT;*/
// 					WRITE_LOG(_T("切换到晚上算法参数\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
// /*				}*/
// 			}
// 			COleDateTimeSpan dTimespan =  dTimeEnd - dTimeStart;
// 			int nSecondTime = dTimespan.GetTotalSeconds();
			
			IsAppSVParaSetSuccess = ResetALGParmbyCurrentTimeIf(false);
			if(IsAppSVParaSetSuccess)
			{
				try
				{
					//WRITE_LOG(_T("Processing"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
					ALG_PROCESS(pDataBuf,true);//算法全运行
				}
				catch (...)
				{
					IsAppSVParaSetSuccess = false;
				}
				
			}

// 			//是否有数据请求消息
// 			if (!g_VideoRequestFlag)
// 			{
// 				CString cstrInfo;
// 			    cstrInfo.Format("运行当前时间：%02d-%02d-%02d-%03d!\r\n",
// 					stSystemTime.wHour,stSystemTime.wMinute,stSystemTime.wSecond,stSystemTime.wMilliseconds);
// 				WRITE_LOG(cstrInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 			}
			if(g_VideoRequestFlag)
			{
// 				CString cstrOutInfo;
// 				cstrOutInfo.Format("请求时%02d&%02d&%02d\r\n",
// 					stSystemTime.wHour,
// 					stSystemTime.wMinute,
// 					stSystemTime.wSecond);
// 				WRITE_LOG(cstrOutInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 				static int iCallRate = 0;
				
				unsigned int SendLen = sizeof(pSendBuf);
				if(!IsVertReversedRGB24)//保证反向
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = true;
				}				
				H264_ENC(&pThis->m_VideoEncode, pDataBuf, (unsigned char *)pSendBuf, &SendLen);
				//发送数据1300为一个单元
				//TODO:
				int iSendLenTmp = SendLen;
				H264_ENC_DATA H264EncData;
				int iSendIdx = 1;
				while(iSendLenTmp > 0)
				{
					if(iSendLenTmp > 1300)
					{
						CONSTRUCT_DATA((char *)(pSendBuf + (iSendIdx-1)*1300), 1300, &H264EncData, SendLen, iSendIdx);
						iSendIdx++;
						iSendLenTmp -= 1300;
					}
					else
					{
						CONSTRUCT_DATA((char *)(pSendBuf + (iSendIdx-1)*1300), iSendLenTmp, &H264EncData, SendLen, PACKET_INDEX_LAST);
						iSendLenTmp = 0;
					}
					UDP_SEND(g_stGlobal_Variable.client_udpsocket.socket_it,(unsigned char *)&H264EncData,sizeof(H264EncData),(unsigned char *)g_stGlobal_Variable.client_udpsocket.socket_cip,g_stGlobal_Variable.client_udpsocket.socket_cport);
				}	
// 				CString cstrInfo;
// 			    cstrInfo.Format("运行当前时间：%02d-%02d-%02d-%03d 发送一帧!\r\n",
// 					stSystemTime.wHour,stSystemTime.wMinute,stSystemTime.wSecond,stSystemTime.wMilliseconds);
// 				WRITE_LOG(cstrInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
			}
			//获取处理结果
			//WRITE_LOG(_T("模拟识别器回调运行程序 算法返回!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
			DWORD dCurrentTimeTick = ::GetTickCount();
			if (dCurrentTimeTick - peWatcherCapture->dPtzTime >= ComputePreTimeByAlgandArea((ALG_EXTEND_TYPE)peWatcherCapture->iAlarmType,g_stSVOutResult.id) + 3000)//容忍3秒内建模时间
			{
//				int alg_index = ALG_TYPE_START;
//				for(++alg_index; alg_index < ALG_TYPE_END; alg_index++)
//				{
					if (IsAppSVParaSetSuccess)
					{
						//ALG_GETRESULT(&pThis->m_OutResultData,(ALG_EXTEND_TYPE)alg_index);
						if (IsSVAlarmExistIf())
						//if(0)
						{
							peWatcherCapture->bAlarm = true;
							peWatcherCapture->iAlarmType = g_stSVOutResult.type;
							CString strMoudlePath = Ident_GetModulePath();
							CString strDatePath;
							strDatePath.Format("BaoJingPIC\\%04d-%02d-%02d\\",stSystemTime.wYear,stSystemTime.wMonth,stSystemTime.wDay);
							strMoudlePath += strDatePath;
							Ident_MakeDirExist(strMoudlePath);
							CString strFName;   //bitmap filename
							strFName.Format("%s第%d区BjFullScreen%02d&%02d&%02d-%d.jpg",
								g_strAlarmTypeDes[peWatcherCapture->iAlarmType],
								g_stSVOutResult.id + 1,
								stSystemTime.wHour,
								stSystemTime.wMinute,
								stSystemTime.wSecond,
								peWatcherCapture->nPicCnt++);			
							strFName = strMoudlePath + strFName;
							
							CString strPicText;
							strPicText.Format("%s第%d防区-远景",g_strAlarmTypeDes[peWatcherCapture->iAlarmType],g_stSVOutResult.id + 1);
							pThis->m_cJpegApi.BGRFromRGB(pDataBuf,nWidth,nHeight);
							if(IsVertReversedRGB24)//保证原始
							{
								pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
								IsVertReversedRGB24 = false;
							}
							Ident_WriteTextInPic(theApp.m_pMainWnd->m_hWnd,pDataBuf,nWidth,nHeight,strPicText);
							if(!IsVertReversedRGB24)//保证反向
							{
								pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
								IsVertReversedRGB24 = true;
							}
							if(!pThis->m_cJpegApi.WriteJPEGFile(strFName,pDataBuf,nWidth,nHeight,true,80))
							{
								WRITE_LOG(pThis->m_cJpegApi.GetErrorString(),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
							}
							else
							{
								FileNameNode_T node;
								node.v_name = strFName;
								Send485Msgif();
								EnterCriticalSection(&g_stGlobal_Variable.csFileNameList);
								g_FileNameList.push(node);
								LeaveCriticalSection(&g_stGlobal_Variable.csFileNameList);
							}
							//Output bitmap file to current directory
							//WRITE_LOG(_T("模拟识别器回调运行程序 图片存储!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
							//EteDVR_SaveBuf2Bmp(strFName,nWidth,nHeight,(EteDVR_SrcType)nFrameType,pDataBuf,nDataLen); 
							//切换到第一个报警的放大区
							//做云台动作
							if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
							{
								//计算出防区的预置点
								int PtzPre = ComputePrePointByAlgandArea((ALG_EXTEND_TYPE)peWatcherCapture->iAlarmType,g_stSVOutResult.id);
								if (PtzPre != PTZ_PREPOINT_INIT)
								{
									//这里什么都不做==连续两次跳转到预置点会出现设置界面
									g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzGoToPre(PtzPre);
								}
							}
							peWatcherCapture->dPtzTime = ::GetTickCount();
//							break;
						}/*end if objnum*/
					}

//				}/*end for*/
			}
			if (!peWatcherCapture->bAlarm && (g_SimAlarmOne || g_bAlarmAudioFlag))//如果没有报警 可以模拟一次报警
			{
				WRITE_LOG(_T("模拟报警图片!!\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
				if (g_SimAlarmOne)
				{
					peWatcherCapture->iAlarmType = 0;
					g_SimAlarmOne = false;
				}
				if (g_bAlarmAudioFlag)
				{
					peWatcherCapture->iAlarmType = ALG_AUDIO;
					g_bAlarmAudioFlag = false;
				}
				peWatcherCapture->bAlarm = true;
				CString strMoudlePath = Ident_GetModulePath();
				CString strDatePath;
				strDatePath.Format("BaoJingPIC\\%04d-%02d-%02d\\",stSystemTime.wYear,stSystemTime.wMonth,stSystemTime.wDay);
				strMoudlePath += strDatePath;
				Ident_MakeDirExist(strMoudlePath);
				CString strFName;   //bitmap filename
				strFName.Format("%s第%d区BjFullScreen%02d&%02d&%02d-%d.jpg",
					g_strAlarmTypeDes[peWatcherCapture->iAlarmType],
					g_stSVOutResult.id + 1,
					stSystemTime.wHour,
					stSystemTime.wMinute,
					stSystemTime.wSecond,
					peWatcherCapture->nPicCnt++);			
				strFName = strMoudlePath + strFName;
				
				CString strPicText;
				strPicText.Format("%s第%d防区-远景",g_strAlarmTypeDes[peWatcherCapture->iAlarmType],g_stSVOutResult.id + 1);
				pThis->m_cJpegApi.BGRFromRGB(pDataBuf,nWidth,nHeight);
				if(IsVertReversedRGB24)//保证原始
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = false;
				}
				Ident_WriteTextInPic(theApp.m_pMainWnd->m_hWnd,pDataBuf,nWidth,nHeight,strPicText);
				if(!IsVertReversedRGB24)//保证反向
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = true;
				}
				if(!pThis->m_cJpegApi.WriteJPEGFile(strFName,pDataBuf,nWidth,nHeight,true,80))
				{
					WRITE_LOG(pThis->m_cJpegApi.GetErrorString(),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				}
				else
				{
					FileNameNode_T node;
					node.v_name = strFName;
					Send485Msgif();
					EnterCriticalSection(&g_stGlobal_Variable.csFileNameList);
					g_FileNameList.push(node);
					LeaveCriticalSection(&g_stGlobal_Variable.csFileNameList);
				}
				//Output bitmap file to current directory
				//WRITE_LOG(_T("模拟识别器回调运行程序 图片存储!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				//EteDVR_SaveBuf2Bmp(strFName,nWidth,nHeight,(EteDVR_SrcType)nFrameType,pDataBuf,nDataLen); 
				//切换到第一个报警的放大区
				//做云台动作
				if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
				{
					//计算出防区的预置点
					int PtzPre = ComputePrePointByAlgandArea((ALG_EXTEND_TYPE)peWatcherCapture->iAlarmType,g_stSVOutResult.id);
					g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzGoToPre(PtzPre);
				}
				peWatcherCapture->dPtzTime = ::GetTickCount();
			}/*end if SimAlarmOne*/
		}
		else//发送了报警 特写放大
		{
			//是否有数据请求消息
			if(g_VideoRequestFlag)
			{
				CString cstrOutInfo;
				cstrOutInfo.Format("Dev%d-Ch%d\r\n",peWatcherCapture->nDevNum,peWatcherCapture->nChNum);
				//WRITE_LOG(cstrOutInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				static int iCallRate = 0;
				
				unsigned int SendLen = sizeof(pSendBuf);
				if(!IsVertReversedRGB24)
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = true;
				}
				H264_ENC(&pThis->m_VideoEncode, pDataBuf, (unsigned char *)pSendBuf, &SendLen);
				//发送数据1300为一个单元
				//TODO:
				int iSendLenTmp = SendLen;
				H264_ENC_DATA H264EncData;
				int iSendIdx = 1;
				while(iSendLenTmp > 0)
				{
					if(iSendLenTmp > 1300)
					{
						CONSTRUCT_DATA((char *)(pSendBuf + (iSendIdx-1)*1300), 1300, &H264EncData, SendLen, iSendIdx);
						iSendIdx++;
						iSendLenTmp -= 1300;
					}
					else
					{
						CONSTRUCT_DATA((char *)(pSendBuf + (iSendIdx-1)*1300), iSendLenTmp, &H264EncData, SendLen, PACKET_INDEX_LAST);
						iSendLenTmp = 0;
					}
					UDP_SEND(g_stGlobal_Variable.client_udpsocket.socket_it,(unsigned char *)&H264EncData,sizeof(H264EncData),(unsigned char *)g_stGlobal_Variable.client_udpsocket.socket_cip,g_stGlobal_Variable.client_udpsocket.socket_cport);
				}	
			}
// 			COleDateTime dTimeStart(m_eWatcherCapture[m_eWatcherNum].dPtzTime);
// 			COleDateTime dTimeEnd(stSystemTime);
// 			COleDateTimeSpan dTimespan =  dTimeEnd - dTimeStart;
// 			int nSecondTime = dTimespan.GetTotalSeconds();
			DWORD dCurrentTimeTick = ::GetTickCount();
			if (dCurrentTimeTick - peWatcherCapture->dPtzTime >= ComputePreTimeByAlgandArea((ALG_EXTEND_TYPE)peWatcherCapture->iAlarmType,g_stSVOutResult.id))//等待ptz拉升的时间
			{
				CString strMoudlePath = Ident_GetModulePath();
				CString strDatePath;
				SYSTEMTIME stSystemTime;
				GetLocalTime(&stSystemTime);
				strDatePath.Format("BaoJingPIC\\%04d-%02d-%02d\\",stSystemTime.wYear,stSystemTime.wMonth,stSystemTime.wDay);
				strMoudlePath += strDatePath;
				Ident_MakeDirExist(strMoudlePath);
				CString strFName;   //bitmap filename
				strFName.Format("%s第%d区Bj%02d&%02d&%02d-%d.jpg",
					g_strAlarmTypeDes[peWatcherCapture->iAlarmType],
					g_stSVOutResult.id + 1,
					stSystemTime.wHour,
					stSystemTime.wMinute,
					stSystemTime.wSecond,
					peWatcherCapture->nPicCnt++);			
				strFName = strMoudlePath + strFName;
				
				CString strPicText;
				strPicText.Format("%s第%d防区-近景特写放大",
					g_strAlarmTypeDes[peWatcherCapture->iAlarmType],
					g_stSVOutResult.id + 1);
				pThis->m_cJpegApi.BGRFromRGB(pDataBuf,nWidth,nHeight);
				if(IsVertReversedRGB24)//保证原始
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = false;
				}
				Ident_WriteTextInPic(theApp.m_pMainWnd->m_hWnd,pDataBuf,nWidth,nHeight,strPicText);
				if(!IsVertReversedRGB24)//保证反向
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = true;
				}
				if(!pThis->m_cJpegApi.WriteJPEGFile(strFName,pDataBuf,nWidth,nHeight,true,80))
				{
					WRITE_LOG(pThis->m_cJpegApi.GetErrorString(),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				}
				else
				{
					FileNameNode_T node;
					node.v_name = strFName;
					EnterCriticalSection(&g_stGlobal_Variable.csFileNameList);
					g_FileNameList.push(node);
					LeaveCriticalSection(&g_stGlobal_Variable.csFileNameList);
				}
				//Output bitmap file to current directory
				//WRITE_LOG(_T("模拟识别器回调运行程序 图片存储!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);	
				peWatcherCapture->bAlarm = false;
				peWatcherCapture->dPtzTime = ::GetTickCount();
				//切换到初始位置
				if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
				{
					//计算出防区的预置点
					g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzGoToPre(PTZ_PREPOINT_INIT);
				}
			}			
		}

#if 0
		static int i = 0;
		if(i++ < 2)
		{
			CString strFName;
			strFName.Format("D:\\第%d张图片.jpg",i);
			pThis->m_cJpegApi.BGRFromRGB(pDataBuf,nWidth,nHeight);
			pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
			Ident_WriteTextInPic(theApp.m_pMainWnd->m_hWnd,pDataBuf,nWidth,nHeight,strFName);
			pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
			if(!pThis->m_cJpegApi.WriteJPEGFile(strFName,pDataBuf,nWidth,nHeight,true,80))
				WRITE_LOG(pThis->m_cJpegApi.GetErrorString(),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
		}
#endif
	}

}

// *************************************************************
// 函数名          : AnalogVideoRecognizer::CallbackStartDebug
// 函数功能描述    : 模拟识别器 调试状态时回调注册函数
// 函数返回值      : void
// 函数参数        : pDataBuf     模拟视频采集数据缓冲
//                   nDataLen     模拟视频采集数据总长度
//                   nWidth       模拟视频采集图片宽度
//                   nHeight      模拟视频采集图片高度
//                   nFrameType   模拟视频采集图片帧类型  YUV420P (RGB24)
//                   pContext     模拟视频回调函数注册参数
// 修改记录        :
// 修改日期--------修改人-------修改说明
// |2012-04-01    |gmaui       | 初始版本                      |
// |              |            |                               |
// *************************************************************
void  CALLBACK AnalogVideoRecognizer::CallbackStartDebug(PBYTE pDataBuf, int nDataLen, int nWidth, int nHeight,int nFrameType, void *pContext)
{
	//WRITE_LOG(_T("模拟识别器回调调试程序开始!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	EWATCHER_CAPTURE_T *peWatcherCapture = (EWATCHER_CAPTURE_T *)pContext;
	AnalogVideoRecognizer *pThis = (AnalogVideoRecognizer  *)(peWatcherCapture->pHandle);
	
	if(peWatcherCapture->nDevNum == 0 && peWatcherCapture->nChNum == 0)
	{
// 		SYSTEMTIME stSystemTime;
// 		GetLocalTime(&stSystemTime);
// 		CString cstrInfo;
// 		cstrInfo.Format("调试当前时间：%02d-%02d-%02d-%03d !\r\n",
// 			stSystemTime.wHour,stSystemTime.wMinute,stSystemTime.wSecond,stSystemTime.wMilliseconds);
// 		WRITE_LOG(cstrInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 		if (!g_VideoRequestFlag)
// 		{
// 			CString cstrOutInfo;
// 			cstrOutInfo.Format("调试无请求时%02d&%02d&%02d\r\n",
// 				stSystemTime.wHour,
// 				stSystemTime.wMinute,
// 				stSystemTime.wSecond);
// 			//h264编码一次 将原先25fps 减少到了4-8fps 需要大量优化
// // 			unsigned int SendLen = sizeof(pSendBuf);
// // 			H264_ENC(&pThis->m_VideoEncode, pDataBuf, (unsigned char *)pSendBuf, &SendLen);
// 			WRITE_LOG(cstrOutInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 		}
		if (g_VideoRequestFlag)
		{
// 			CString cstrOutInfo;
// 			cstrOutInfo.Format("调试有请求时%02d&%02d&%02d\r\n",
// 				stSystemTime.wHour,
// 				stSystemTime.wMinute,
// 				stSystemTime.wSecond);
// 			WRITE_LOG(cstrOutInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
//			static int iCallRate = 0;
			//调试时上下翻转一次 翻转一次减少一帧
			pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
			unsigned int SendLen = sizeof(pSendBuf);
			H264_ENC(&pThis->m_VideoEncode, pDataBuf, (unsigned char *)pSendBuf, &SendLen);
			//发送数据1300为一个单元
			//TODO:
			int iSendLenTmp = SendLen;
			H264_ENC_DATA H264EncData;
			int iSendIdx = 1;
			while(iSendLenTmp > 0)
			{
				if(iSendLenTmp > 1300)
				{
					CONSTRUCT_DATA((char *)(pSendBuf + (iSendIdx-1)*1300), 1300, &H264EncData, SendLen, iSendIdx);
					iSendIdx++;
					iSendLenTmp -= 1300;
				}
				else
				{
					CONSTRUCT_DATA((char *)(pSendBuf + (iSendIdx-1)*1300), iSendLenTmp, &H264EncData, SendLen, PACKET_INDEX_LAST);
					iSendLenTmp = 0;
				}
				UDP_SEND(g_stGlobal_Variable.client_udpsocket.socket_it,(unsigned char *)&H264EncData,sizeof(H264EncData),(unsigned char *)g_stGlobal_Variable.client_udpsocket.socket_cip,g_stGlobal_Variable.client_udpsocket.socket_cport);
			}
		}
	
	}
}
//DigitalVideoRecognizer 数字数据识别器类
DigitalVideoRecognizer::DigitalVideoRecognizer()
{
	m_APPSVInterface = NULL;
	m_IsReSetAPPSVParam = FALSE;//标志是否重新设置算法参数
	m_IsReSetVideoParam = FALSE;//标志是否重新设置视频参数
	
}
DigitalVideoRecognizer::~DigitalVideoRecognizer()
{
	
}

BOOL DigitalVideoRecognizer::Initilize()
{
	return TRUE;
}

BOOL DigitalVideoRecognizer::Start()
{
	return TRUE;
}

BOOL DigitalVideoRecognizer::Run()
{
	return TRUE;
}

BOOL DigitalVideoRecognizer::Debug()
{
	return TRUE;
}

BOOL DigitalVideoRecognizer::Stop()
{
	return TRUE;
}

BOOL DigitalVideoRecognizer::SwitchMode(State state)
{
	return TRUE;
}

BOOL DigitalVideoRecognizer::Release()
{
	return TRUE;
}

BOOL DigitalVideoRecognizer::InitNetwork()
{
	m_TcpRecvCmdDataPort = VIDEO_TCP_PORT;
	m_UdpRecvVideoDataPort = VIDEO_UDP_PORT;
	
	INIT_WSASOCK();
	TCP_CREATE((int *)&m_TcpRecvCmdDataSock);
	TCP_BIND(m_TcpRecvCmdDataSock,m_TcpRecvCmdDataPort);
	
	UDP_CREATE((int *)&m_UdpRecvVideoDataSock);
	UDP_BIND(m_UdpRecvVideoDataSock,m_UdpRecvVideoDataPort);
	
	return TRUE;
}

unsigned __stdcall DigitalVideoRecognizer::ThreadForNetworkFunc(void * pParam)
{
	//实现与客户端的连接
	//开启一个扫描线程 针对客户端
	DigitalVideoRecognizer *pThis = (DigitalVideoRecognizer *)pParam;
	INIT_WSASOCK();
	SOCKET NetworkCommSock;
	int SockSvrMax;
	
	listen(pThis->m_TcpRecvCmdDataSock,1);
	
	TIMEVAL TimeVal; 
	TimeVal.tv_sec = 1;
	TimeVal.tv_usec = 0;
	
	FD_SET SockNetworkFdSet;
	
	while(!pThis->m_IsNetworkThreadExit)
	{
		FD_ZERO(&SockNetworkFdSet);
		FD_SET(pThis->m_TcpRecvCmdDataSock,&SockNetworkFdSet);
		if(pThis->m_IsNetworkConnected)
		{
			FD_SET(NetworkCommSock,&SockNetworkFdSet);
			SockSvrMax = (pThis->m_TcpRecvCmdDataSock > NetworkCommSock) ?  (pThis->m_TcpRecvCmdDataSock+1) : (NetworkCommSock+1);
		}
		if (select(SockSvrMax,&SockNetworkFdSet,NULL,NULL,&TimeVal) > 0)
		{
			if (FD_ISSET(pThis->m_TcpRecvCmdDataSock,&SockNetworkFdSet))//接收到了新的链接
			{
				if (pThis->m_IsNetworkConnected)//已经有一个链接
				{
				}
				else
				{
					NetworkCommSock = accept(pThis->m_TcpRecvCmdDataSock,NULL,NULL);
					//开启定时器 ClientConnect.Timer
					pThis->m_IsNetworkConnected = TRUE;
				}
			}
			if (FD_ISSET(NetworkCommSock,&SockNetworkFdSet))//接收到客户端命令
			{
				
			}
		}
	}
	return TRUE;
}

BOOL DigitalVideoRecognizer::StartNetwork()
{
	m_IsNetworkThreadExit = FALSE;
	m_IsNetworkConnected = FALSE;
	
	m_hThreadForNetworkHandle = (HANDLE)_beginthreadex(NULL,0,ThreadForNetworkFunc,this,0,&m_uiThreadForNetworkID);
	return TRUE;
}


//DigitalAlarmRecognizer 数字报警识别器类
DigitalAlarmRecognizer::DigitalAlarmRecognizer()
{
	
}
DigitalAlarmRecognizer::~DigitalAlarmRecognizer()
{
	
}

BOOL DigitalAlarmRecognizer::Initilize()
{
	return TRUE;
}

BOOL DigitalAlarmRecognizer::Start()
{
	return TRUE;
}

BOOL DigitalAlarmRecognizer::Run()
{
	return TRUE;
}

BOOL DigitalAlarmRecognizer::Debug()
{
	return TRUE;
}

BOOL DigitalAlarmRecognizer::Stop()
{
	return TRUE;
}

BOOL DigitalAlarmRecognizer::SwitchMode(State state)
{
	return TRUE;
}

BOOL DigitalAlarmRecognizer::Release()
{
	return TRUE;
}


//SensorAlarmRecognizer 传感器报警识别器类
SensorAlarmRecognizer::SensorAlarmRecognizer()
{
}
SensorAlarmRecognizer::~SensorAlarmRecognizer()
{
	
}

BOOL SensorAlarmRecognizer::Initilize()
{
	return TRUE;
}

BOOL SensorAlarmRecognizer::Start()
{
	return TRUE;
}

BOOL SensorAlarmRecognizer::Run()
{
	return TRUE;
}

BOOL SensorAlarmRecognizer::Debug()
{
	return TRUE;
}

BOOL SensorAlarmRecognizer::Stop()
{
	return TRUE;
}

BOOL SensorAlarmRecognizer::SwitchMode(State state)
{
	return TRUE;
}

BOOL SensorAlarmRecognizer::Release()
{
	return TRUE;
}

//工厂
RecognizerFactory::RecognizerFactory()
{

}

RecognizerFactory::~RecognizerFactory()
{

}

Recognizer * RecognizerFactory::GenerateRecognizer(EN_REGTYPE type)
{
	Recognizer *pRecognizer = NULL;
	switch (type)
	{
	case EN_REGTYPE_ANALOGVIDEO:
		pRecognizer = new AnalogVideoRecognizer();
		break;
	case EN_REGTYPE_DIGITALVIDEO:
		pRecognizer = new DigitalVideoRecognizer();
		break;
	case EN_REGTYPE_DIGITALALARM:
		pRecognizer = new DigitalAlarmRecognizer();
		break;
	case EN_REGTYPE_SENSORALARM:
		pRecognizer = new SensorAlarmRecognizer();
		break;
	default:
		break;
	}
	return pRecognizer;
}

extern bool g_bIsLinkConnected;
/************************************************************************/
/* 从报警名获取相应的报警信息                                                                     */
/************************************************************************/
bool Send485Msgif()
{
	TCP_MSG_T TcpMsg;//客户端与服务器数据交互使用的数据结构
	memset(&TcpMsg,0x00,TCP_MSG_LEN);
	TcpMsg.msg_type = TCP_MSG_TYPE_RS485;
	if (g_bIsLinkConnected && !TCP_SEND(g_stGlobal_Variable.center_tcpsocket.socket_it,(unsigned char *)&TcpMsg,sizeof(TcpMsg)))
	{
		WRITE_LOG(_T("Send485Msgif 向服务器上报485信息成功!!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	}
	else
	{
		WRITE_LOG(_T("Send485Msgif 向服务器上报485信息失败!!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN);
	}
	return true;
}


unsigned int ComputePrePointByAlgandArea(ALG_EXTEND_TYPE type,int AreaNo)
{
	int PrePoint = PTZ_PREPOINT_INIT;
	switch (type)
	{
	case ALG_REC:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algquadpara[AreaNo/4].REC[AreaNo%4].preset;
		}break;//回字形
	case ALG_AREA:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algareapara[AreaNo/4].configunit[AreaNo%4].preset;
		}break;//禁区
	case ALG_LINE:
		{
			PrePoint = g_stGlobal_Variable.globalflash.alglinepara[AreaNo/4].lineunit[AreaNo%4].preset;
		}break;//绊线
	case ALG_LINE_COUNT:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algcountpara[AreaNo/4].countunit[AreaNo%4].preset;
		}break;//计数
	case ALG_OBJ_ENTER:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algenterpara[AreaNo/4].enterunit[AreaNo%4].preset;
		}break;//目标进入
	case ALG_OBJ_OUT:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algoutpara[AreaNo/4].outunit[AreaNo%4].preset;
		}break;//目标离开
//	case ALG_OBJ_STOP:
//		{
//			PrePoint = g_stGlobal_Variable.globalflash.algstoppara[AreaNo/3].stopunit[AreaNo%3].preset;
//		}
//		break;//物体遗留
	case ALG_OBJ_LOST:
		{
			PrePoint = g_stGlobal_Variable.globalflash.alglostpara[AreaNo/3].lostunit[AreaNo%3].preset;
		}break;//物体消失
	case ALG_DIRECTION:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algdirectpara[AreaNo/4].directunit[AreaNo%4].preset;
		}break;//方向报警
	case ALG_DETECTION:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algdetectpara[AreaNo/4].dectunit[AreaNo%4].preset;
		}break;//入侵侦测
	case ALG_PEOPLEDENSE:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algpeopledectpara[AreaNo/4].peopledenseunit[AreaNo%4].preset;
		}break;//人流密度
//	case ALG_FIGHTALARM:
//		{
//
//		}break;//打架检测
	case ALG_FIGHTDETECTION:
		{
		}break;//打架检测
	case ALG_CLUSTERDETECTION:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algclusterdectpara[AreaNo/4].clusterdeteunit[AreaNo%4].preset;
		}break;//聚集检测
	case ALG_ALARMAREA:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algmotionhistorypara[AreaNo/4].motionhistoryunit[AreaNo%4].preset;
		}break;//夜间起床检测
	case ALG_ABNORMALFALL:
		{
		}break;//异常跌倒
	case ALG_HOVERDETECT:
		{
			PrePoint = g_stGlobal_Variable.globalflash.alghoverdectpara[AreaNo/4].hoverdeteUnit[AreaNo%4].preset;
		}break;//徘徊检测
	case ALG_FINDBAG://遗留物检测   modify(add) by hzc 2012-05-20
		{
			PrePoint = g_stGlobal_Variable.globalflash.algfindbagpara[AreaNo/4].findBagUnit[AreaNo%4].preset;
		}break;
	case ALG_RUNNINGOBJ: //物体奔跑检测 modify(add) by hzc 2012-05-20
		{
			PrePoint = g_stGlobal_Variable.globalflash.algrunningpara[AreaNo/4].runningObjUnit[AreaNo%4].preset;
		}break;
	case ALG_PEOPLELEAVE: //离岗检测  modify(add) by hzc 2012-05-20
		{
			//
		}
	case ALG_ALARMLINE: //摸高检测 modify (add) by hzc 2012-05-20
		{
			PrePoint = g_stGlobal_Variable.globalflash.algalarmlinepara[AreaNo/4].alarmLine[AreaNo%4].preset;
		}break;
	case ALG_TYPE_END:
	default:
		{

		}break;
	}
	return PrePoint;
}

unsigned int ComputePreTimeByAlgandArea(ALG_EXTEND_TYPE type,int AreaNo)
{
	int PreTime = 5;
	switch (type)
	{
	case ALG_REC:
		{
			PreTime = g_stGlobal_Variable.globalflash.algquadpara[AreaNo/4].REC[AreaNo%4].pretime;
		}break;//回字形
	case ALG_AREA:
		{
			PreTime = g_stGlobal_Variable.globalflash.algareapara[AreaNo/4].configunit[AreaNo%4].pretime;
		}break;//禁区
	case ALG_LINE:
		{
			PreTime = g_stGlobal_Variable.globalflash.alglinepara[AreaNo/4].lineunit[AreaNo%4].pretime;
		}break;//绊线
	case ALG_LINE_COUNT:
		{
			PreTime = g_stGlobal_Variable.globalflash.algcountpara[AreaNo/4].countunit[AreaNo%4].pretime;
		}break;//计数
	case ALG_OBJ_ENTER:
		{
			PreTime = g_stGlobal_Variable.globalflash.algenterpara[AreaNo/4].enterunit[AreaNo%4].pretime;
		}break;//目标进入
	case ALG_OBJ_OUT:
		{
			PreTime = g_stGlobal_Variable.globalflash.algoutpara[AreaNo/4].outunit[AreaNo%4].pretime;
		}break;//目标离开
// 	case ALG_OBJ_STOP:
// 		{
// 			PreTime = g_stGlobal_Variable.globalflash.algstoppara[AreaNo/3].stopunit[AreaNo%3].pretime;
// 		}
// 		break;//物体遗留
	case ALG_OBJ_LOST:
		{
			PreTime = g_stGlobal_Variable.globalflash.alglostpara[AreaNo/3].lostunit[AreaNo%3].pretime;
		}break;//物体消失
	case ALG_DIRECTION:
		{
			PreTime = g_stGlobal_Variable.globalflash.algdirectpara[AreaNo/4].directunit[AreaNo%4].pretime;
		}break;//方向报警
	case ALG_DETECTION:
		{
			PreTime = g_stGlobal_Variable.globalflash.algdetectpara[AreaNo/4].dectunit[AreaNo%4].pretime;
		}break;//入侵侦测
	case ALG_PEOPLEDENSE:
		{
			PreTime = g_stGlobal_Variable.globalflash.algpeopledectpara[AreaNo/4].peopledenseunit[AreaNo%4].pretime;
		}break;//人流密度
		//	case ALG_FIGHTALARM:
		//		{
		//
		//		}break;//打架检测
	case ALG_FIGHTDETECTION:
		{
		}break;//打架检测
	case ALG_CLUSTERDETECTION:
		{
			PreTime = g_stGlobal_Variable.globalflash.algclusterdectpara[AreaNo/4].clusterdeteunit[AreaNo%4].pretime;
		}break;//聚集检测
	case ALG_ALARMAREA:
		{
			PreTime = g_stGlobal_Variable.globalflash.algmotionhistorypara[AreaNo/4].motionhistoryunit[AreaNo%4].pretime;
		}break;//夜间起床检测
	case ALG_ABNORMALFALL:
		{
		}break;//异常跌倒
	case ALG_HOVERDETECT:
		{
			PreTime = g_stGlobal_Variable.globalflash.alghoverdectpara[AreaNo/4].hoverdeteUnit[AreaNo%4].pretime;
		}break;//徘徊检测
	case ALG_FINDBAG://遗留物检测   modify(add) by hzc 2012-05-20
		{
			PreTime = g_stGlobal_Variable.globalflash.algfindbagpara[AreaNo/4].findBagUnit[AreaNo%4].pretime;
		}break;
	case ALG_RUNNINGOBJ: //物体奔跑检测 modify(add) by hzc 2012-05-20
		{
			PreTime = g_stGlobal_Variable.globalflash.algrunningpara[AreaNo/4].runningObjUnit[AreaNo%4].pretime;
		}break;
	case ALG_PEOPLELEAVE: //离岗检测  modify(add) by hzc 2012-05-20
		{
			//
		}
	case ALG_ALARMLINE: //摸高检测 modify (add) by hzc 2012-05-20
		{
			PreTime = g_stGlobal_Variable.globalflash.algalarmlinepara[AreaNo/4].alarmLine[AreaNo%4].pretime;
		}break;
	case ALG_TYPE_END:
	default:
		{
			
		}break;
	}
	return PreTime * 1000;
}

bool IsSVAlarmExistIf()
{
	int index = ALG_TYPE_START;
	OutPutApp_Result_Ext OutPutAppResultExt;
	memset(&OutPutAppResultExt,0x00,sizeof(OutPutAppResultExt));
	memset(&g_stSVOutResult,0x00,sizeof(g_stSVOutResult));
	for(++index; index <= ALG_DETECTION; index++)
	{
		ALG_GETRESULT(&OutPutAppResultExt,(ALG_EXTEND_TYPE)index);
		if (OutPutAppResultExt.outputwindow._objnum > 0)
		{
			g_stSVOutResult.type = index;
			g_stSVOutResult.id = OutPutAppResultExt.outputwindow._obj_list[0]._alarm;
			return true;
		}
	}
	if ((index == ALG_PEOPLEDENSE) && OutPutAppResultExt.peopledense._dense >= 0.8)//密度大于10
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_FIGHTDETECTION) && OutPutAppResultExt.fightdetection._bFighting)//打架检测
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_CLUSTERDETECTION) && OutPutAppResultExt.clusterdetection._bClustered)//聚集检测
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_ALARMAREA) && OutPutAppResultExt.alarmarea._ALG_AlarmArea_Alarm) //夜间起床
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_ABNORMALFALL) && OutPutAppResultExt.abnormalfall._bFall) //异常跌倒
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_HOVERDETECT) && OutPutAppResultExt.hoverdetect._objnum > 0)//徘徊
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = OutPutAppResultExt.hoverdetect._obj_list[0]._alarm_area;
		return true;
	}
	if((index == ALG_ALARMLINE) && OutPutAppResultExt.alarmline._ALG_AlarmLine_Alarm)//警戒线
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_RUNNINGOBJ) && OutPutAppResultExt.running._objnum > 0)//奔跑
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = OutPutAppResultExt.running._obj_list[0]._alarm_area;
		return true;
	}
	return false;
}