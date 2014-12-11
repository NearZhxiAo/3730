 /************************************************************************/
//  Copyright (C) Technologies
//  
//  File name  : IdentMain.cpp
//  Description: ʶ���������̵�ʵ��
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

//���ӿ�
#pragma comment(lib, "AppSV/APPSV.lib")
#pragma comment(lib,"AlgAppLib/Release/AlgAppLib.lib")
#pragma comment(lib, "eWatchSDK/eWatcherSDK.lib")
#pragma comment(lib, "H264_codec/Release/H264_codec.lib")
#pragma comment(lib, "SocketApi/Release/SocketApi.lib")
#pragma comment(lib, "PtzControl/Release/PtzControl.lib")

#define PTZ_PREPOINT_INIT PTZ_PREPOINT_NUM_MAX


//ȫ��������
unsigned char pSendBuf[VIDEO_WIDTH * VIDEO_HEIGHT] = {0};//H264��������buf��
const CString g_strAlarmTypeDes[] = {_T("ģ��"),_T("����"),_T("����"),_T("����"),_T("����"),
							   _T("Ŀ�����"),_T("Ŀ���뿪"),/*_T("��������"),*/_T("������ʧ"),
							   _T("���򱨾�"),_T("�������"),_T("��Ա�ܶ�"),_T("���"),_T("��Ա�ۼ�"),
							   _T("ҹ����"),_T("����"),_T("�ǻ�"),_T("��������"),
							   _T("����"),_T("���"),_T("������"),_T("����"),_T("��")};
//static OutPutApp_Result_Ext g_stOutPutAppResultExt;
typedef struct _SV_OUT_RESULT_T
{
	int type;
	int id;
}SV_OUT_RESULT_T;

static SV_OUT_RESULT_T g_stSVOutResult;
//ȫ�ֺ�����
bool Send485Msgif();
static unsigned int ComputePrePointByAlgandArea(ALG_EXTEND_TYPE type,int AreaNo);//�������̨Ԥ�õ�
static unsigned int ComputePreTimeByAlgandArea(ALG_EXTEND_TYPE type,int AreaNo);//�������̨�ƶ���Ԥ�õ�ʱ��
static bool IsSVAlarmExistIf();//�ж��Ƿ���ڱ����¼�

extern CIdentifierApp theApp;
extern bool g_SimAlarmOne;

Recognizer::Recognizer()
{

}
Recognizer::~Recognizer()
{

}

//����:
//1.���߳����е���ػ�����ʼ��������ر����Ϸ���
//2.����������ͨ�ŵ��߳�(�Ӻ�ʵ��)
//3.������ͻ���ͨ�ŵ��߳�
//4.�㷨��ʼ��--�漰������ʱ�޸���س�ʼ��ֵ ������Դ� �ṩIdentALG_Init()��IdentALG_ReInit()����������ʵ��


//AnalogVideoRecognizer ģ������ʶ������
AnalogVideoRecognizer::AnalogVideoRecognizer()
{
	m_State = STATE_IDLE;
	m_IsReSetAPPSVParam = false;//��־�Ƿ����������㷨����
	m_IsReSetVideoParam = false;//��־�Ƿ�����������Ƶ����
	//��ʼ��״̬���ʹؼ���
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
// ������          : AnalogVideoRecognizer::SwitchMode
// ������������    : ģ����Ƶ�л�״̬����
// ��������ֵ      : BOOL �ɹ�--true ʧ��--false
// ��������        : State (RUN/DEBUG)�л�����״̬ ������� idle->run//��һ������ʱ����
//					                             run<->debug//�ͻ���������                  
// �޸ļ�¼        :
// �޸�����--------�޸���-------�޸�˵��
// |2012-04-01    |gmaui       | ��ʼ�汾                      |
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
		WRITE_LOG(_T("H262����ģ���˳�!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	}
	return TRUE;
}

BOOL AnalogVideoRecognizer::InitDevCamera()
{
	m_nCardModelSel = EteDVR_CardModel_M878_2v2;
	m_nVideoWidthSel = VIDEO_WIDTH;   
	m_nVideoHeightSel = VIDEO_HEIGHT;
	m_nSrcTypeSel = SrcType_RGB24;
	m_VideoEncode.codec_id = CODEC_ID_H264; /*������ѡ��*/
	m_VideoEncode.pix_format = PIX_FMT_RGB24; /*��������Դѡ��*/
	//m_VideoEncode.pix_format = PIX_FMT_YUV420P; /*��������Դѡ��*/
	m_VideoEncode.type = ENCDEC_TYPE_ENC;
	
	memset(&g_stSVOutResult,0x00,sizeof(g_stSVOutResult));
	//ͨ���Է�������������ʹ��ͼ��ķֱ���  --1.cif 2.d1
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
		WRITE_LOG(_T("ģ��ʶ����H264�����ʼ���ɹ�!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	}
	else
	{
		WRITE_LOG(_T("ģ��ʶ����H264�����ʼ��ʧ��!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
	}

	//��ʼ��ͼ��ɼ��� ��ʱ��Ϊ�����豸����
	m_eWatcherNum = 0;
	WRITE_LOG(_T("ģ��ʶ�����ɼ�����ʼ��!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
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
				if(nRet != SUCCEEDED)//���÷ֱ���
				{
					errorInfo.Format(_T("EteDVR_SetResolution ����dev=%d chm=%d error=%d!\r\n"),i,j,nRet);
					WRITE_LOG(errorInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				}
				// 				if(EteDVR_SetVideoParam(i,j,BRIGHTNESS,g_stGlobal_Variable.globalflash.videopara.bright) != SUCCEEDED)
				// 					WRITE_LOG(_T("EteDVR_SetVideoParam BRIGHTNESS ����!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				// 				if(EteDVR_SetVideoParam(i,j,HUE,g_stGlobal_Variable.globalflash.videopara.hue) != SUCCEEDED)
				// 					WRITE_LOG(_T("EteDVR_SetResolution HUE ����!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				// 				if(EteDVR_SetVideoParam(i,j,CONTRAST,g_stGlobal_Variable.globalflash.videopara.contrast) != SUCCEEDED)
				// 					WRITE_LOG(_T("EteDVR_SetResolution CONTRAST ����!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				// 				if(EteDVR_SetVideoParam(i,j,SATURATION,g_stGlobal_Variable.globalflash.videopara.saturation) != SUCCEEDED)
				// 					WRITE_LOG(_T("EteDVR_SetResolution SATURATION ����!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				nRet = EteDVR_EnableDeinterlace(i,j,true);
				if(nRet != SUCCEEDED)
				{
					errorInfo.Format(_T("EteDVR_EnableDeinterlace ����dev=%d chm=%d error=%d !\r\n"),i,j,nRet);
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
		WRITE_LOG(_T("�豸��ʧ��!!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN,true);
		return FALSE;
	}
	WRITE_LOG(_T("ģ��ʶ������ʼ�����!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	return TRUE;
}

/*	
��̬��ע��ص����������������ص�����
��Ҫ���ݵ���������������ꡣthis�Ķ��� 
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
			//��������� һ������ CallbackStartRun �������� CallbackStartDebug
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
// ������          : AnalogVideoRecognizer::CallbackStartRun
// ������������    : ģ��ʶ���� ����״̬ʱ�ص�ע�ắ��
// ��������ֵ      : void
// ��������        : pDataBuf     ģ����Ƶ�ɼ����ݻ���
//                   nDataLen     ģ����Ƶ�ɼ������ܳ���
//                   nWidth       ģ����Ƶ�ɼ�ͼƬ���
//                   nHeight      ģ����Ƶ�ɼ�ͼƬ�߶�
//                   nFrameType   ģ����Ƶ�ɼ�ͼƬ֡����  YUV420P (RGB24)
//                   pContext     ģ����Ƶ�ص�����ע�����
// �޸ļ�¼        :
// �޸�����--------�޸���-------�޸�˵��
// |2012-04-01    |gmaui       | ��ʼ�汾                      |
// |              |            |                               |
// *************************************************************
void  CALLBACK AnalogVideoRecognizer::CallbackStartRun(PBYTE pDataBuf, int nDataLen, int nWidth, int nHeight,int nFrameType, void *pContext)
{
	//WRITE_LOG(_T("ģ��ʶ�����ص����г���ʼ!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	EWATCHER_CAPTURE_T *peWatcherCapture = (EWATCHER_CAPTURE_T *)pContext;
	AnalogVideoRecognizer *pThis = (AnalogVideoRecognizer  *)(peWatcherCapture->pHandle);
	bool IsVertReversedRGB24 = false;//��¼������ͼƬ�Ƿ����·�ת
	bool IsAppSVParaSetSuccess = false;
	//TODO:
	//ֻ��dev0 ch0 ����Ƶ����
	if(peWatcherCapture->nDevNum == 0 && peWatcherCapture->nChNum == 0)
	{
		//WRITE_LOG(_T("ģ��ʶ�����ص����г��� �㷨ִ��!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 		CString cstrinfo;
// 		cstrinfo.Format("��ǰ֡���� height = %d width = %d datalen = %d\r\n",
// 			nWidth,nWidth,nDataLen);
// 		WRITE_LOG(cstrinfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);

		//�㷨Ҫ������RGB�ǲɼ��������ĸ�ʽ �������ﲻҪ��ת
//		if(!IsVertReversedRGB24)
//		{
//			pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
//			IsVertReversedRGB24 = true;
//		}
		//pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
		if(!peWatcherCapture->bAlarm)//�����ͨ����û�б���
		{
			//AUTO:
			//�����Ƿ����ʱ����Ҫ�л��㷨
			
			SYSTEMTIME stSystemTime;
			GetLocalTime(&stSystemTime);
// 			SYSTEMTIME stSystemTimeDayStart = stSystemTime;//����8:00
// 			SYSTEMTIME stSystemTimeDayEnd = stSystemTime;  // ����18:00
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
// 			if(dTime >= dTimeStart && dTime <= dTimeEnd)//����ʱ���
// 			{
// 				if(g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed != ALG_TIME_DAY)
// 				{
// 					//0-���� 1-����
// 					ALG_SETPARAM(&g_stGlobal_Variable.globalflash,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 					g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed = ALG_TIME_DAY;
// 					WRITE_LOG(_T("�л��������㷨����\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
// 				}
// 			}
// 			else//����ʱ���
// 			{
// // 				if(g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed != ALG_TIME_NIGHT)
// // 				{
// 					//0-���� 1-����
// 					ALG_SETPARAM(&g_stGlobal_Variable.globalflash,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// /*					g_stGlobal_Variable.globalflash.algpara.AlgTimeUsed = ALG_TIME_NIGHT;*/
// 					WRITE_LOG(_T("�л��������㷨����\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
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
					ALG_PROCESS(pDataBuf,true);//�㷨ȫ����
				}
				catch (...)
				{
					IsAppSVParaSetSuccess = false;
				}
				
			}

// 			//�Ƿ�������������Ϣ
// 			if (!g_VideoRequestFlag)
// 			{
// 				CString cstrInfo;
// 			    cstrInfo.Format("���е�ǰʱ�䣺%02d-%02d-%02d-%03d!\r\n",
// 					stSystemTime.wHour,stSystemTime.wMinute,stSystemTime.wSecond,stSystemTime.wMilliseconds);
// 				WRITE_LOG(cstrInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 			}
			if(g_VideoRequestFlag)
			{
// 				CString cstrOutInfo;
// 				cstrOutInfo.Format("����ʱ%02d&%02d&%02d\r\n",
// 					stSystemTime.wHour,
// 					stSystemTime.wMinute,
// 					stSystemTime.wSecond);
// 				WRITE_LOG(cstrOutInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 				static int iCallRate = 0;
				
				unsigned int SendLen = sizeof(pSendBuf);
				if(!IsVertReversedRGB24)//��֤����
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = true;
				}				
				H264_ENC(&pThis->m_VideoEncode, pDataBuf, (unsigned char *)pSendBuf, &SendLen);
				//��������1300Ϊһ����Ԫ
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
// 			    cstrInfo.Format("���е�ǰʱ�䣺%02d-%02d-%02d-%03d ����һ֡!\r\n",
// 					stSystemTime.wHour,stSystemTime.wMinute,stSystemTime.wSecond,stSystemTime.wMilliseconds);
// 				WRITE_LOG(cstrInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
			}
			//��ȡ������
			//WRITE_LOG(_T("ģ��ʶ�����ص����г��� �㷨����!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
			DWORD dCurrentTimeTick = ::GetTickCount();
			if (dCurrentTimeTick - peWatcherCapture->dPtzTime >= ComputePreTimeByAlgandArea((ALG_EXTEND_TYPE)peWatcherCapture->iAlarmType,g_stSVOutResult.id) + 3000)//����3���ڽ�ģʱ��
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
							strFName.Format("%s��%d��BjFullScreen%02d&%02d&%02d-%d.jpg",
								g_strAlarmTypeDes[peWatcherCapture->iAlarmType],
								g_stSVOutResult.id + 1,
								stSystemTime.wHour,
								stSystemTime.wMinute,
								stSystemTime.wSecond,
								peWatcherCapture->nPicCnt++);			
							strFName = strMoudlePath + strFName;
							
							CString strPicText;
							strPicText.Format("%s��%d����-Զ��",g_strAlarmTypeDes[peWatcherCapture->iAlarmType],g_stSVOutResult.id + 1);
							pThis->m_cJpegApi.BGRFromRGB(pDataBuf,nWidth,nHeight);
							if(IsVertReversedRGB24)//��֤ԭʼ
							{
								pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
								IsVertReversedRGB24 = false;
							}
							Ident_WriteTextInPic(theApp.m_pMainWnd->m_hWnd,pDataBuf,nWidth,nHeight,strPicText);
							if(!IsVertReversedRGB24)//��֤����
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
							//WRITE_LOG(_T("ģ��ʶ�����ص����г��� ͼƬ�洢!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
							//EteDVR_SaveBuf2Bmp(strFName,nWidth,nHeight,(EteDVR_SrcType)nFrameType,pDataBuf,nDataLen); 
							//�л�����һ�������ķŴ���
							//����̨����
							if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
							{
								//�����������Ԥ�õ�
								int PtzPre = ComputePrePointByAlgandArea((ALG_EXTEND_TYPE)peWatcherCapture->iAlarmType,g_stSVOutResult.id);
								if (PtzPre != PTZ_PREPOINT_INIT)
								{
									//����ʲô������==����������ת��Ԥ�õ��������ý���
									g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzGoToPre(PtzPre);
								}
							}
							peWatcherCapture->dPtzTime = ::GetTickCount();
//							break;
						}/*end if objnum*/
					}

//				}/*end for*/
			}
			if (!peWatcherCapture->bAlarm && (g_SimAlarmOne || g_bAlarmAudioFlag))//���û�б��� ����ģ��һ�α���
			{
				WRITE_LOG(_T("ģ�ⱨ��ͼƬ!!\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
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
				strFName.Format("%s��%d��BjFullScreen%02d&%02d&%02d-%d.jpg",
					g_strAlarmTypeDes[peWatcherCapture->iAlarmType],
					g_stSVOutResult.id + 1,
					stSystemTime.wHour,
					stSystemTime.wMinute,
					stSystemTime.wSecond,
					peWatcherCapture->nPicCnt++);			
				strFName = strMoudlePath + strFName;
				
				CString strPicText;
				strPicText.Format("%s��%d����-Զ��",g_strAlarmTypeDes[peWatcherCapture->iAlarmType],g_stSVOutResult.id + 1);
				pThis->m_cJpegApi.BGRFromRGB(pDataBuf,nWidth,nHeight);
				if(IsVertReversedRGB24)//��֤ԭʼ
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = false;
				}
				Ident_WriteTextInPic(theApp.m_pMainWnd->m_hWnd,pDataBuf,nWidth,nHeight,strPicText);
				if(!IsVertReversedRGB24)//��֤����
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
				//WRITE_LOG(_T("ģ��ʶ�����ص����г��� ͼƬ�洢!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
				//EteDVR_SaveBuf2Bmp(strFName,nWidth,nHeight,(EteDVR_SrcType)nFrameType,pDataBuf,nDataLen); 
				//�л�����һ�������ķŴ���
				//����̨����
				if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
				{
					//�����������Ԥ�õ�
					int PtzPre = ComputePrePointByAlgandArea((ALG_EXTEND_TYPE)peWatcherCapture->iAlarmType,g_stSVOutResult.id);
					g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzGoToPre(PtzPre);
				}
				peWatcherCapture->dPtzTime = ::GetTickCount();
			}/*end if SimAlarmOne*/
		}
		else//�����˱��� ��д�Ŵ�
		{
			//�Ƿ�������������Ϣ
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
				//��������1300Ϊһ����Ԫ
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
			if (dCurrentTimeTick - peWatcherCapture->dPtzTime >= ComputePreTimeByAlgandArea((ALG_EXTEND_TYPE)peWatcherCapture->iAlarmType,g_stSVOutResult.id))//�ȴ�ptz������ʱ��
			{
				CString strMoudlePath = Ident_GetModulePath();
				CString strDatePath;
				SYSTEMTIME stSystemTime;
				GetLocalTime(&stSystemTime);
				strDatePath.Format("BaoJingPIC\\%04d-%02d-%02d\\",stSystemTime.wYear,stSystemTime.wMonth,stSystemTime.wDay);
				strMoudlePath += strDatePath;
				Ident_MakeDirExist(strMoudlePath);
				CString strFName;   //bitmap filename
				strFName.Format("%s��%d��Bj%02d&%02d&%02d-%d.jpg",
					g_strAlarmTypeDes[peWatcherCapture->iAlarmType],
					g_stSVOutResult.id + 1,
					stSystemTime.wHour,
					stSystemTime.wMinute,
					stSystemTime.wSecond,
					peWatcherCapture->nPicCnt++);			
				strFName = strMoudlePath + strFName;
				
				CString strPicText;
				strPicText.Format("%s��%d����-������д�Ŵ�",
					g_strAlarmTypeDes[peWatcherCapture->iAlarmType],
					g_stSVOutResult.id + 1);
				pThis->m_cJpegApi.BGRFromRGB(pDataBuf,nWidth,nHeight);
				if(IsVertReversedRGB24)//��֤ԭʼ
				{
					pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
					IsVertReversedRGB24 = false;
				}
				Ident_WriteTextInPic(theApp.m_pMainWnd->m_hWnd,pDataBuf,nWidth,nHeight,strPicText);
				if(!IsVertReversedRGB24)//��֤����
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
				//WRITE_LOG(_T("ģ��ʶ�����ص����г��� ͼƬ�洢!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);	
				peWatcherCapture->bAlarm = false;
				peWatcherCapture->dPtzTime = ::GetTickCount();
				//�л�����ʼλ��
				if (g_stGlobal_Variable.ptz_ctlhandle.ptz_initd && g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface)
				{
					//�����������Ԥ�õ�
					g_stGlobal_Variable.ptz_ctlhandle.ptz_Interface->PtzGoToPre(PTZ_PREPOINT_INIT);
				}
			}			
		}

#if 0
		static int i = 0;
		if(i++ < 2)
		{
			CString strFName;
			strFName.Format("D:\\��%d��ͼƬ.jpg",i);
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
// ������          : AnalogVideoRecognizer::CallbackStartDebug
// ������������    : ģ��ʶ���� ����״̬ʱ�ص�ע�ắ��
// ��������ֵ      : void
// ��������        : pDataBuf     ģ����Ƶ�ɼ����ݻ���
//                   nDataLen     ģ����Ƶ�ɼ������ܳ���
//                   nWidth       ģ����Ƶ�ɼ�ͼƬ���
//                   nHeight      ģ����Ƶ�ɼ�ͼƬ�߶�
//                   nFrameType   ģ����Ƶ�ɼ�ͼƬ֡����  YUV420P (RGB24)
//                   pContext     ģ����Ƶ�ص�����ע�����
// �޸ļ�¼        :
// �޸�����--------�޸���-------�޸�˵��
// |2012-04-01    |gmaui       | ��ʼ�汾                      |
// |              |            |                               |
// *************************************************************
void  CALLBACK AnalogVideoRecognizer::CallbackStartDebug(PBYTE pDataBuf, int nDataLen, int nWidth, int nHeight,int nFrameType, void *pContext)
{
	//WRITE_LOG(_T("ģ��ʶ�����ص����Գ���ʼ!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	EWATCHER_CAPTURE_T *peWatcherCapture = (EWATCHER_CAPTURE_T *)pContext;
	AnalogVideoRecognizer *pThis = (AnalogVideoRecognizer  *)(peWatcherCapture->pHandle);
	
	if(peWatcherCapture->nDevNum == 0 && peWatcherCapture->nChNum == 0)
	{
// 		SYSTEMTIME stSystemTime;
// 		GetLocalTime(&stSystemTime);
// 		CString cstrInfo;
// 		cstrInfo.Format("���Ե�ǰʱ�䣺%02d-%02d-%02d-%03d !\r\n",
// 			stSystemTime.wHour,stSystemTime.wMinute,stSystemTime.wSecond,stSystemTime.wMilliseconds);
// 		WRITE_LOG(cstrInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 		if (!g_VideoRequestFlag)
// 		{
// 			CString cstrOutInfo;
// 			cstrOutInfo.Format("����������ʱ%02d&%02d&%02d\r\n",
// 				stSystemTime.wHour,
// 				stSystemTime.wMinute,
// 				stSystemTime.wSecond);
// 			//h264����һ�� ��ԭ��25fps ���ٵ���4-8fps ��Ҫ�����Ż�
// // 			unsigned int SendLen = sizeof(pSendBuf);
// // 			H264_ENC(&pThis->m_VideoEncode, pDataBuf, (unsigned char *)pSendBuf, &SendLen);
// 			WRITE_LOG(cstrOutInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
// 		}
		if (g_VideoRequestFlag)
		{
// 			CString cstrOutInfo;
// 			cstrOutInfo.Format("����������ʱ%02d&%02d&%02d\r\n",
// 				stSystemTime.wHour,
// 				stSystemTime.wMinute,
// 				stSystemTime.wSecond);
// 			WRITE_LOG(cstrOutInfo,LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
//			static int iCallRate = 0;
			//����ʱ���·�תһ�� ��תһ�μ���һ֡
			pThis->m_cJpegApi.VertReversedRGB24(pDataBuf,nWidth,nHeight);
			unsigned int SendLen = sizeof(pSendBuf);
			H264_ENC(&pThis->m_VideoEncode, pDataBuf, (unsigned char *)pSendBuf, &SendLen);
			//��������1300Ϊһ����Ԫ
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
//DigitalVideoRecognizer ��������ʶ������
DigitalVideoRecognizer::DigitalVideoRecognizer()
{
	m_APPSVInterface = NULL;
	m_IsReSetAPPSVParam = FALSE;//��־�Ƿ����������㷨����
	m_IsReSetVideoParam = FALSE;//��־�Ƿ�����������Ƶ����
	
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
	//ʵ����ͻ��˵�����
	//����һ��ɨ���߳� ��Կͻ���
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
			if (FD_ISSET(pThis->m_TcpRecvCmdDataSock,&SockNetworkFdSet))//���յ����µ�����
			{
				if (pThis->m_IsNetworkConnected)//�Ѿ���һ������
				{
				}
				else
				{
					NetworkCommSock = accept(pThis->m_TcpRecvCmdDataSock,NULL,NULL);
					//������ʱ�� ClientConnect.Timer
					pThis->m_IsNetworkConnected = TRUE;
				}
			}
			if (FD_ISSET(NetworkCommSock,&SockNetworkFdSet))//���յ��ͻ�������
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


//DigitalAlarmRecognizer ���ֱ���ʶ������
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


//SensorAlarmRecognizer ����������ʶ������
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

//����
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
/* �ӱ�������ȡ��Ӧ�ı�����Ϣ                                                                     */
/************************************************************************/
bool Send485Msgif()
{
	TCP_MSG_T TcpMsg;//�ͻ�������������ݽ���ʹ�õ����ݽṹ
	memset(&TcpMsg,0x00,TCP_MSG_LEN);
	TcpMsg.msg_type = TCP_MSG_TYPE_RS485;
	if (g_bIsLinkConnected && !TCP_SEND(g_stGlobal_Variable.center_tcpsocket.socket_it,(unsigned char *)&TcpMsg,sizeof(TcpMsg)))
	{
		WRITE_LOG(_T("Send485Msgif ��������ϱ�485��Ϣ�ɹ�!!\r\n"),LOG_TYPE_INFO,LOG_LEVEL_RUN,true);
	}
	else
	{
		WRITE_LOG(_T("Send485Msgif ��������ϱ�485��Ϣʧ��!!\r\n"),LOG_TYPE_ERROR,LOG_LEVEL_RUN);
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
		}break;//������
	case ALG_AREA:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algareapara[AreaNo/4].configunit[AreaNo%4].preset;
		}break;//����
	case ALG_LINE:
		{
			PrePoint = g_stGlobal_Variable.globalflash.alglinepara[AreaNo/4].lineunit[AreaNo%4].preset;
		}break;//����
	case ALG_LINE_COUNT:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algcountpara[AreaNo/4].countunit[AreaNo%4].preset;
		}break;//����
	case ALG_OBJ_ENTER:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algenterpara[AreaNo/4].enterunit[AreaNo%4].preset;
		}break;//Ŀ�����
	case ALG_OBJ_OUT:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algoutpara[AreaNo/4].outunit[AreaNo%4].preset;
		}break;//Ŀ���뿪
//	case ALG_OBJ_STOP:
//		{
//			PrePoint = g_stGlobal_Variable.globalflash.algstoppara[AreaNo/3].stopunit[AreaNo%3].preset;
//		}
//		break;//��������
	case ALG_OBJ_LOST:
		{
			PrePoint = g_stGlobal_Variable.globalflash.alglostpara[AreaNo/3].lostunit[AreaNo%3].preset;
		}break;//������ʧ
	case ALG_DIRECTION:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algdirectpara[AreaNo/4].directunit[AreaNo%4].preset;
		}break;//���򱨾�
	case ALG_DETECTION:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algdetectpara[AreaNo/4].dectunit[AreaNo%4].preset;
		}break;//�������
	case ALG_PEOPLEDENSE:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algpeopledectpara[AreaNo/4].peopledenseunit[AreaNo%4].preset;
		}break;//�����ܶ�
//	case ALG_FIGHTALARM:
//		{
//
//		}break;//��ܼ��
	case ALG_FIGHTDETECTION:
		{
		}break;//��ܼ��
	case ALG_CLUSTERDETECTION:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algclusterdectpara[AreaNo/4].clusterdeteunit[AreaNo%4].preset;
		}break;//�ۼ����
	case ALG_ALARMAREA:
		{
			PrePoint = g_stGlobal_Variable.globalflash.algmotionhistorypara[AreaNo/4].motionhistoryunit[AreaNo%4].preset;
		}break;//ҹ���𴲼��
	case ALG_ABNORMALFALL:
		{
		}break;//�쳣����
	case ALG_HOVERDETECT:
		{
			PrePoint = g_stGlobal_Variable.globalflash.alghoverdectpara[AreaNo/4].hoverdeteUnit[AreaNo%4].preset;
		}break;//�ǻ����
	case ALG_FINDBAG://��������   modify(add) by hzc 2012-05-20
		{
			PrePoint = g_stGlobal_Variable.globalflash.algfindbagpara[AreaNo/4].findBagUnit[AreaNo%4].preset;
		}break;
	case ALG_RUNNINGOBJ: //���屼�ܼ�� modify(add) by hzc 2012-05-20
		{
			PrePoint = g_stGlobal_Variable.globalflash.algrunningpara[AreaNo/4].runningObjUnit[AreaNo%4].preset;
		}break;
	case ALG_PEOPLELEAVE: //��ڼ��  modify(add) by hzc 2012-05-20
		{
			//
		}
	case ALG_ALARMLINE: //���߼�� modify (add) by hzc 2012-05-20
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
		}break;//������
	case ALG_AREA:
		{
			PreTime = g_stGlobal_Variable.globalflash.algareapara[AreaNo/4].configunit[AreaNo%4].pretime;
		}break;//����
	case ALG_LINE:
		{
			PreTime = g_stGlobal_Variable.globalflash.alglinepara[AreaNo/4].lineunit[AreaNo%4].pretime;
		}break;//����
	case ALG_LINE_COUNT:
		{
			PreTime = g_stGlobal_Variable.globalflash.algcountpara[AreaNo/4].countunit[AreaNo%4].pretime;
		}break;//����
	case ALG_OBJ_ENTER:
		{
			PreTime = g_stGlobal_Variable.globalflash.algenterpara[AreaNo/4].enterunit[AreaNo%4].pretime;
		}break;//Ŀ�����
	case ALG_OBJ_OUT:
		{
			PreTime = g_stGlobal_Variable.globalflash.algoutpara[AreaNo/4].outunit[AreaNo%4].pretime;
		}break;//Ŀ���뿪
// 	case ALG_OBJ_STOP:
// 		{
// 			PreTime = g_stGlobal_Variable.globalflash.algstoppara[AreaNo/3].stopunit[AreaNo%3].pretime;
// 		}
// 		break;//��������
	case ALG_OBJ_LOST:
		{
			PreTime = g_stGlobal_Variable.globalflash.alglostpara[AreaNo/3].lostunit[AreaNo%3].pretime;
		}break;//������ʧ
	case ALG_DIRECTION:
		{
			PreTime = g_stGlobal_Variable.globalflash.algdirectpara[AreaNo/4].directunit[AreaNo%4].pretime;
		}break;//���򱨾�
	case ALG_DETECTION:
		{
			PreTime = g_stGlobal_Variable.globalflash.algdetectpara[AreaNo/4].dectunit[AreaNo%4].pretime;
		}break;//�������
	case ALG_PEOPLEDENSE:
		{
			PreTime = g_stGlobal_Variable.globalflash.algpeopledectpara[AreaNo/4].peopledenseunit[AreaNo%4].pretime;
		}break;//�����ܶ�
		//	case ALG_FIGHTALARM:
		//		{
		//
		//		}break;//��ܼ��
	case ALG_FIGHTDETECTION:
		{
		}break;//��ܼ��
	case ALG_CLUSTERDETECTION:
		{
			PreTime = g_stGlobal_Variable.globalflash.algclusterdectpara[AreaNo/4].clusterdeteunit[AreaNo%4].pretime;
		}break;//�ۼ����
	case ALG_ALARMAREA:
		{
			PreTime = g_stGlobal_Variable.globalflash.algmotionhistorypara[AreaNo/4].motionhistoryunit[AreaNo%4].pretime;
		}break;//ҹ���𴲼��
	case ALG_ABNORMALFALL:
		{
		}break;//�쳣����
	case ALG_HOVERDETECT:
		{
			PreTime = g_stGlobal_Variable.globalflash.alghoverdectpara[AreaNo/4].hoverdeteUnit[AreaNo%4].pretime;
		}break;//�ǻ����
	case ALG_FINDBAG://��������   modify(add) by hzc 2012-05-20
		{
			PreTime = g_stGlobal_Variable.globalflash.algfindbagpara[AreaNo/4].findBagUnit[AreaNo%4].pretime;
		}break;
	case ALG_RUNNINGOBJ: //���屼�ܼ�� modify(add) by hzc 2012-05-20
		{
			PreTime = g_stGlobal_Variable.globalflash.algrunningpara[AreaNo/4].runningObjUnit[AreaNo%4].pretime;
		}break;
	case ALG_PEOPLELEAVE: //��ڼ��  modify(add) by hzc 2012-05-20
		{
			//
		}
	case ALG_ALARMLINE: //���߼�� modify (add) by hzc 2012-05-20
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
	if ((index == ALG_PEOPLEDENSE) && OutPutAppResultExt.peopledense._dense >= 0.8)//�ܶȴ���10
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_FIGHTDETECTION) && OutPutAppResultExt.fightdetection._bFighting)//��ܼ��
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_CLUSTERDETECTION) && OutPutAppResultExt.clusterdetection._bClustered)//�ۼ����
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_ALARMAREA) && OutPutAppResultExt.alarmarea._ALG_AlarmArea_Alarm) //ҹ����
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_ABNORMALFALL) && OutPutAppResultExt.abnormalfall._bFall) //�쳣����
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_HOVERDETECT) && OutPutAppResultExt.hoverdetect._objnum > 0)//�ǻ�
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = OutPutAppResultExt.hoverdetect._obj_list[0]._alarm_area;
		return true;
	}
	if((index == ALG_ALARMLINE) && OutPutAppResultExt.alarmline._ALG_AlarmLine_Alarm)//������
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = 0;
		return true;
	}
	if((index == ALG_RUNNINGOBJ) && OutPutAppResultExt.running._objnum > 0)//����
	{
		g_stSVOutResult.type = index;
		g_stSVOutResult.id = OutPutAppResultExt.running._obj_list[0]._alarm_area;
		return true;
	}
	return false;
}