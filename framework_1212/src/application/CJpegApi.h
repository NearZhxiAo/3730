//  *************************************************************
//  Copyright (C) Technologies Co., Ltd.
//  
//  文件名      : CJpegApi.h
//  文件描述    : CJpegApi jpeg基本操作类
// 
//  文件版本    : 1.0
//  作者        : gmaui
//  创建日期    : 2012-04-01
//  *************************************************************
//#pragma once
#if !defined(_JPEG_API_)
#define _JPEG_API_

class CJpegApi  
{
public:
	CJpegApi();
	virtual ~CJpegApi();
public:
	
	// 获取错误信息
	CString GetErrorString();
	//读取JPEG文件到缓冲区
	BYTE* ReadJPEGFile(LPCSTR lpstrFileName, UINT *uWidth, UINT *uHeight);
	
	//将位图文件存储为JPEG文件
	BOOL WriteJPEGFile(LPCTSTR lpstrFileName,	// path
					   BYTE *dataBuf,			// RGB buffer
					   UINT width,				// pixels
					   UINT height,				// rows
					   BOOL color,				// TRUE = RGB
												// FALSE = Grayscale
    				   int quality);			// 0 - 100

	////////////////////////////////////////////////////////////////
	// if you have a DWORD aligned buffer, this will copy the
	// RGBs out of it into a new buffer. new width is widthPix * 3 bytes
	// caller is responsible for delete []'ing the buffer
	
	BYTE *ClearDwordAlign(BYTE *inBuf,				// input buf
						UINT widthPix,				// input size
						UINT widthBytes,			// input size
						UINT height);
	BOOL VertFlipBuf(BYTE * inbuf,					// input buf
				   UINT widthBytes,					// input width bytes
				   UINT height);					// height
	BOOL VertReversedRGB24(BYTE * inbuf,					// input buf
				   UINT widthBytes,					// input width bytes
				   UINT height);					// height
	BOOL BGRFromRGB(BYTE *buf,						// input buf
					UINT widthPix,					// width in pixels
					UINT height);					// lines

	//操作错误码信息
	CString m_strJPEGError;
};

#endif // !defined(_JPEG_API_)
