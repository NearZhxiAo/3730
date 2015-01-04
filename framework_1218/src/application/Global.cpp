//  *************************************************************
//  Copyright (C) Technologies Co., Ltd.
//  
//  文件名       : Global.cpp
//  文件描述信息 : 全局操作定义文件
//  版本信息     : 1.0
//  作者         : gmaui
//  创建日期     : 2012-04-01
//  *************************************************************

/************************************************************************/
/* 头文件定义区                                                                     */
/************************************************************************/
#include "StdAfx.h"
#include "Global.h"
#include "SuperLog.h"

/************************************************************************/
/* 全局变量区域                                                                     */
/************************************************************************/
bool g_bAlarmAudioFlag = false;
bool g_VideoRequestFlag = false;
bool g_VideoResolutionChanged = false;

// *************************************************************
// 函数名          : ResetALGParmbyCurrentTimeIf
// 函数功能描述    : 重置算法参数
// 函数返回值      : bool        
// 函数参数        : bool    强制要求      
// 修改记录        :
// 修改日期--------修改人-------修改说明
// |2012-04-01    |gmaui       | 初始版本                      |
// |              |            |                               |
// *************************************************************
bool ResetALGParmbyCurrentTimeIf(bool bForce)
{
	//TODO:
	//考虑是否根据时间需要切换算法
	bool bRet = false;
	SYSTEMTIME stSystemTime;
	GetLocalTime(&stSystemTime);//获取当前时间
	int iCurrentMinutesOfDay = stSystemTime.wHour * 60 + stSystemTime.wMinute;//计算当前时间处于当天的分钟数
	//更换当前的算法参数
	for (int i = 0; i < ALG_PARA_CONF_MAX_NUM; i++)
	{
		//1.判断是否使能--使能才有效
		//2.判断是否在特定时间段内---该时间段内有效
		//3.判断是否当前时间段已经再使用
		if (g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgParaChooseGroup.bAlgEnableFlag
			&& iCurrentMinutesOfDay >= g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgScheduleUint.usAlgRuntimeStart
			&& iCurrentMinutesOfDay <= g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[i].AlgScheduleUint.usAlgRuntimeEnd
			)
		{
			//强制配置参数 || 由于时间变动导致参数调整
			if(bForce || (g_stGlobal_Variable.globalflash.algpara.AlgParaConfUsedIndex != i))
			{
				g_stGlobal_Variable.globalflash.algpara.AlgParaConfUsedIndex = i;
				//TODO:设置参数
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
			bRet = true;//说明参数设置正常
		}
	}
	return bRet;
}

// *************************************************************
// 函数名          : Ident_WriteTextInPic
// 函数功能描述    : 图片叠加信息
// 函数返回值      : bool        
// 函数参数        : bool    强制要求      
// 修改记录        :
// 修改日期--------修改人-------修改说明
// |2012-04-01    |gmaui       | 初始版本                      |
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
	strCurrentTime.Format("报警时间:%04d年%02d月%02d日 %02d时%02d分%02d秒",
		strSystemTime.wYear,
		strSystemTime.wMonth,
		strSystemTime.wDay,
		strSystemTime.wHour,
		strSystemTime.wMinute,
		strSystemTime.wSecond);
	
	CBitmap *pMemBmp = new CBitmap();//根据图片的大小创建一个兼容位图
	pMemBmp->CreateCompatibleBitmap(pDC,bitmapInfo.bmiHeader.biWidth,bitmapInfo.bmiHeader.biHeight);
	pMemDC->SelectObject(pMemBmp);
	pMemDC->FillSolidRect(0, 0, bitmapInfo.bmiHeader.biWidth, bitmapInfo.bmiHeader.biHeight, RGB(255, 255, 255));
	//把图像的数据绘制到兼容位图上
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
		//制造透明和重影效果
		
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
