//  *************************************************************
//  Copyright (C) Technologies Co., Ltd.
//  
//  �ļ���       : Global.cpp
//  �ļ�������Ϣ : ȫ�ֲ��������ļ�
//  �汾��Ϣ     : 1.0
//  ����         : gmaui
//  ��������     : 2012-04-01
//  *************************************************************

/************************************************************************/
/* ͷ�ļ�������                                                                     */
/************************************************************************/
#include "StdAfx.h"
#include "Global.h"
#include "SuperLog.h"

/************************************************************************/
/* ȫ�ֱ�������                                                                     */
/************************************************************************/
bool g_bAlarmAudioFlag = false;
bool g_VideoRequestFlag = false;
bool g_VideoResolutionChanged = false;

// *************************************************************
// ������          : ResetALGParmbyCurrentTimeIf
// ������������    : �����㷨����
// ��������ֵ      : bool        
// ��������        : bool    ǿ��Ҫ��      
// �޸ļ�¼        :
// �޸�����--------�޸���-------�޸�˵��
// |2012-04-01    |gmaui       | ��ʼ�汾                      |
// |              |            |                               |
// *************************************************************
bool ResetALGParmbyCurrentTimeIf(bool bForce)
{
	//TODO:
	//�����Ƿ����ʱ����Ҫ�л��㷨
	bool bRet = false;
	SYSTEMTIME stSystemTime;
	GetLocalTime(&stSystemTime);//��ȡ��ǰʱ��
	int iCurrentMinutesOfDay = stSystemTime.wHour * 60 + stSystemTime.wMinute;//���㵱ǰʱ�䴦�ڵ���ķ�����
	//������ǰ���㷨����
	for (int i = 0; i < ALG_PARA_CONF_MAX_NUM; i++)
	{
		//1.�ж��Ƿ�ʹ��--ʹ�ܲ���Ч
		//2.�ж��Ƿ����ض�ʱ�����---��ʱ�������Ч
		//3.�ж��Ƿ�ǰʱ����Ѿ���ʹ��
		if (g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgParaChooseGroup.bAlgEnableFlag
			&& iCurrentMinutesOfDay >= g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgScheduleUint.usAlgRuntimeStart
			&& iCurrentMinutesOfDay <= g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgScheduleUint.usAlgRuntimeEnd
			)
		{
			//ǿ�����ò��� || ����ʱ��䶯���²�������
			if(bForce || (g_stGlobal_Variable.globalflash.algpara.AlgParaConfUsedIndex != i))
			{
				g_stGlobal_Variable.globalflash.algpara.AlgParaConfUsedIndex = i;
				//TODO:���ò���
				try
				{
					ALG_SETPARAM(&g_stGlobal_Variable.globalflash,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmAreaEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmLineEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv,
						&g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv
						);	
					return true;
				}
				catch (...)
				{
					ALG_SETPARAM(&g_stGlobal_Variable.globalflash,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL);
					return false;
				}
			}
			bRet = true;//˵��������������
		}
	}
	return bRet;
}

// *************************************************************
// ������          : Ident_WriteTextInPic
// ������������    : ͼƬ������Ϣ
// ��������ֵ      : bool        
// ��������        : bool    ǿ��Ҫ��      
// �޸ļ�¼        :
// �޸�����--------�޸���-------�޸�˵��
// |2012-04-01    |gmaui       | ��ʼ�汾                      |
// |              |            |                               |
// *************************************************************
bool Ident_WriteTextInPic(HWND hwnd,void *pDataBuf,  int width ,int height, CString strText)
{
	BITMAPINFO	 bitmapInfo;
	int dwImageSize;
	ZeroMemory(&bitmapInfo.bmiHeader,   sizeof(BITMAPINFOHEADER)); 
	bitmapInfo.bmiHeader.biBitCount	=   24; 
	bitmapInfo.bmiHeader.biClrImportant	=   0; 
	bitmapInfo.bmiHeader.biClrUsed	 =   0; 
	bitmapInfo.bmiHeader.biCompression	=   BI_RGB; 
	bitmapInfo.bmiHeader.biHeight	 =   height; 
	bitmapInfo.bmiHeader.biPlanes	 =   1; 
	bitmapInfo.bmiHeader.biSize	 =   sizeof(BITMAPINFOHEADER); 
	bitmapInfo.bmiHeader.biSizeImage	=   height*width*3; 
	bitmapInfo.bmiHeader.biWidth	 =   width; 
	bitmapInfo.bmiHeader.biXPelsPerMeter	=   0; 
	bitmapInfo.bmiHeader.biYPelsPerMeter	=   0; 
	//		bitmapInfo.bmiHeader.biXPelsPerMeter	=   0xEC4; 
	//		bitmapInfo.bmiHeader.biYPelsPerMeter	=   0xEC4; 
	dwImageSize = height*width*3;
	
	CDC *pDC = CDC::FromHandle(GetDC(hwnd));
	CDC *pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);
	
	CString strCurrentTime;
	SYSTEMTIME strSystemTime;
	GetLocalTime(&strSystemTime);
	strCurrentTime.Format("����ʱ��:%04d��%02d��%02d�� %02dʱ%02d��%02d��",
		strSystemTime.wYear,
		strSystemTime.wMonth,
		strSystemTime.wDay,
		strSystemTime.wHour,
		strSystemTime.wMinute,
		strSystemTime.wSecond);
	
	CBitmap *pMemBmp = new CBitmap();//����ͼƬ�Ĵ�С����һ������λͼ
	pMemBmp->CreateCompatibleBitmap(pDC,bitmapInfo.bmiHeader.biWidth,bitmapInfo.bmiHeader.biHeight);
	pMemDC->SelectObject(pMemBmp);
	pMemDC->FillSolidRect(0, 0, bitmapInfo.bmiHeader.biWidth, bitmapInfo.bmiHeader.biHeight, RGB(255, 255, 255));
	//��ͼ������ݻ��Ƶ�����λͼ��
	SetDIBits(pMemDC->GetSafeHdc(),(HBITMAP)pMemBmp->m_hObject,0,bitmapInfo.bmiHeader.biHeight,
		(LPVOID)pDataBuf,(BITMAPINFO*)(&bitmapInfo.bmiHeader), DIB_RGB_COLORS);
	
	if (strText.GetLength() > 0)
	{
		CFont font;
		font.CreateFont(20,0,0,0,100,0,0,0,255,0,0,2,GB2312_CHARSET,"AAAAA");
		CFont *pOldFont = pMemDC->SelectObject(&font);
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = pMemDC->SelectObject(&pen);
		//����͸������ӰЧ��
		
		CRect rcText(0,0,width,40);
		pMemDC->SetBkMode(TRANSPARENT);
		pMemDC->SetTextColor(RGB(0, 0, 255));	
		
		rcText.OffsetRect(2, 2);		
		pMemDC->DrawText(strCurrentTime, rcText, DT_LEFT);
		
		rcText.OffsetRect(2, height-20);		
		pMemDC->DrawText(strText, rcText, DT_LEFT);
		//m_pMemDC->TextOut(11, 11, m_strBmpText);
		//m_pMemDC->SetTextColor(RGB(255, 255, 255));
		//rcText.OffsetRect(-1, -1);
		//m_pMemDC->DrawText(m_strBmpText, rcText, DT_LEFT|DT_WORDBREAK);
		//m_pMemDC->TextOut(10, 10, m_strBmpText);
		
		pMemDC->SelectObject(pOldFont);
		pMemDC->SelectObject(pOldPen);
		//	delete font;
		//	font = NULL;
	}
	GetDIBits(pMemDC->GetSafeHdc(),(HBITMAP)pMemBmp->m_hObject,
		0,bitmapInfo.bmiHeader.biHeight,(LPVOID)pDataBuf,(BITMAPINFO*)&(bitmapInfo.bmiHeader),DIB_RGB_COLORS);
	pDC->DeleteTempMap();
	delete pMemDC;
	delete pMemBmp;
	return true;	
}

bool Ident_MakeDirExist(CString pathname)
{
	if(pathname.Right(1) != "\\")
		pathname += "\\" ;
	int end = pathname.ReverseFind('\\');
	int pt = pathname.Find('\\');
	if (pathname[pt-1] == ':')
		pt = pathname.Find('\\', pt+1);
	CString path;
	while(pt != -1 && pt<=end)
	{
		path = pathname.Left(pt+1);
		
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(path, &fd);
		if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			
		}
		else
		{
			SECURITY_ATTRIBUTES attrib;
			attrib.bInheritHandle = FALSE;
			attrib.lpSecurityDescriptor = NULL;
			attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
			CreateDirectory( path, &attrib);
		}
		FindClose(hFind);
		
		pt = pathname.Find('\\', pt+1);	
	}
	return true;	
}

CString Ident_GetModulePath()
{
	CString strModule_file;
	char Cur_path[MAX_PATH] = "";
	if( !GetModuleFileName(NULL, Cur_path, MAX_PATH ))
	{
		WRITE_LOG(_T("GetModuleFileName failed!!\r\n"),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
	}
	strModule_file.Format("%s",Cur_path);
	strModule_file.Delete(strModule_file.ReverseFind('\\'),strModule_file.GetLength()-strModule_file.ReverseFind('\\'));
	if(strModule_file.Right(1) != "\\")
		strModule_file += "\\";
	return strModule_file;
}
