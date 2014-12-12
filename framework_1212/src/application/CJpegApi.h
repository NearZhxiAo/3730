//  *************************************************************
//  Copyright (C) Technologies Co., Ltd.
//  
//  �ļ���      : CJpegApi.h
//  �ļ�����    : CJpegApi jpeg����������
// 
//  �ļ��汾    : 1.0
//  ����        : gmaui
//  ��������    : 2012-04-01
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
	
	// ��ȡ������Ϣ
	CString GetErrorString();
	//��ȡJPEG�ļ���������
	BYTE* ReadJPEGFile(LPCSTR lpstrFileName, UINT *uWidth, UINT *uHeight);
	
	//��λͼ�ļ��洢ΪJPEG�ļ�
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

	//������������Ϣ
	CString m_strJPEGError;
};

#endif // !defined(_JPEG_API_)
