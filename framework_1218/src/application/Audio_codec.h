#if !defined(AFX_AUDIOPLAYREC_H__A2C89DE8_176E_43ED_AD32_7CF7BD1F667D__INCLUDED_)
#define AFX_AUDIOPLAYREC_H__A2C89DE8_176E_43ED_AD32_7CF7BD1F667D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AudioPlayRec.h : header file
//			
//////////////////////////////////////////////////////////////////////

/****************************************************************************************************************
		投稿说明：
****************************************************************************************************************/
#define NOTE_AudioPlayRec "lib_audio.lib"
//#include "windows.h"
#include "afxwin.h"
#include <mmsystem.h>
#include "Audio_base.h"

typedef void (*FUNC_CallbackNotify) ( UINT uWaveMsg, WPARAM wParam );
//gmaui
typedef bool (*FUNC_CallbackAlarm)(LPWAVEHDR lpWaveHdr);
/////////////////////////////////////////////////////////////////////////////
// CAudio_Codec window

EXPORT_CLASS class  CAudio_Codec : public CWnd
{
// Construction
public:
	typedef enum _enum_status
	{
		ENUM_STATUS_INVALID,		// 无效的状态
		ENUM_STATUS_READY,			// 准备就绪
		ENUM_STATUS_STARTING,	    // 正在采集
		ENUM_STATUS_PLAYING,		// 正在播放
		ENUM_STATUS_DETECTING,		// 正在检测
		ENUM_STATUS_RECORDING,		// 正在录音
		ENUM_STATUS_STOPING,		// 正在停止
		ENUM_STATUS_NUM,
	} ENUM_STATUS;
	typedef enum _enum_channel
	{
		ENUM_CHANNEL_MONO,			// 单声道
		ENUM_CHANNEL_STEREO,		// 立体声
		ENUM_CHANNEL_LEFT,			// 左声道
		ENUM_CHANNEL_RIGHT,			// 右声道
		ENUM_CHANNEL_NUM,
	} ENUM_CHANNEL;

	enum
	{
		TIMER_EVENT_STOPREC = 1,
		TIMER_EVENT_STOPPLAY,
		TIMER_EVENT_STOPDETECT,
		TIMER_EVENT_NUM,
	};

	CAudio_Codec();
	virtual ~CAudio_Codec();
public:
	CString GetWaveOutName(UINT nIndex);
	UINT GetWaveOutCount();
	CString GetWaveInName(UINT nIndex);
	UINT GetWaveInCount();
	BOOL Initialize (
		HWND hwndParent=NULL,
		FUNC_CallbackAlarm Proc_CallbackAlarm=NULL,
		ENUM_CHANNEL eChannel=ENUM_CHANNEL_MONO,/*声道*/
		DWORD nSamplesPerSec=11025*2,/*样本数*/
		WORD wBitsPerSample=16,/*样本基本单元字节*/
		UINT uDeviceID=WAVE_MAPPER,
		DWORD dwBufferSize=1024,
		LPRECT lpRect=NULL,
		FUNC_CallbackNotify Proc_CallbackNotify=NULL,
		WPARAM wParam=NULL
	);
	inline ENUM_STATUS GetStatus () { return m_eStatus; }

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudio_Codec)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBkColor(COLORREF clr);
	BOOL Start();
	BOOL Play ( LPCTSTR lpszWaveFileName );
	void Stop();
	BOOL Record (LPCTSTR lpszAudioFileName=NULL );
	BOOL SetCallBackAlarmFunc(FUNC_CallbackAlarm pfunc);
	// Generated message map functions
protected:
	LRESULT OnMM_WIM_DATA ( WPARAM wParam, LPARAM lParam );
	LRESULT OnMM_WOM_DONE ( WPARAM wParam, LPARAM lParam );
	//{{AFX_MSG(CAudio_Codec)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void DrwaWaveShort ( CClientDC &dc, DWORD dwDrawBytes, SHORT *pShortData );
	void DrwaWaveChar ( CClientDC &dc, DWORD dwDrawBytes, BYTE *pCharData );
	void ResetMp3EncodeVar();
	BOOL WaveBufferMp3Encode ( char *szWavData, int nWavSize );
	void EndEncodeMp3 ();
	BOOL PrepareEncodeMp3(LPCTSTR lpszMp3FileName);
	int  PickupMonoData ( IN WORD wBitsPerSample, char *szOrgData, int nOrgSize, BOOL bLeft );
	void DrawWave ( WAVEHDR *pHdr );
	void StopPlay();
	void StopDetect();
	void StopRec();
	void FreeBuffer ();
	BOOL AllocateBuffer ( DWORD dwBufferSize );
	BOOL AddOutputBufferToQueue ( int nIndex, int nSize );
	BOOL ReadSoundDataFromFile ( LPVOID data, int &size );
	BOOL OpenWaveFile ( LPCTSTR lpszWaveFileName );
	int  m_nDataQueueNum;
	BOOL CreateWaveFile ( LPCTSTR lpszWaveFileName );
	void waveErrorMsg(MMRESULT result, LPCTSTR addstr);
	BOOL AddInputBufferToQueue ( int nIndex );
	void SetWaveFormat ( ENUM_CHANNEL eChannel, DWORD nSamplesPerSec, WORD wBitsPerSample );
	BOOL SetRelateParaAfterGetWaveFormat ();
	BOOL Create ( HWND hwndParent, LPRECT lpRect=NULL );
	FUNC_CallbackNotify m_Proc_CallbackNotify;
	FUNC_CallbackAlarm m_Proc_CallbackAlarm;//检测报警的回调函数
	CRITICAL_SECTION   m_csCallbackAlarmCriticalSection;//注册回调函数的关键区
	WPARAM m_wParam;

	CBrush			m_brsBkGnd;					// 背景刷
	COLORREF		m_clrBK;
	CRect			m_rcClient;
	CPen			m_PenB;
	CPen			m_PenG;

	ENUM_STATUS		m_eStatus;
	BOOL			m_bRecording;
	BOOL            m_bIsAlarming;//是否有报警
	UINT			m_uDeviceID;
	WAVEFORMATEX	m_Format;
	ENUM_CHANNEL	m_eChannel;
	HWAVEIN			m_hRecord;
	HWAVEOUT		m_hPlay;

	char			**m_ptrAryInData;
	WAVEHDR			**m_pAryHdr;
	DWORD			m_dwQueuBufferSize;
	WORD			m_wInQueu;

	//记录相关变量
	DWORD            m_dwRecordStartTick;

	//WAV文件相关参数
	HMMIO			m_hWaveFile;
	MMCKINFO		m_MMCKInfoData;
	MMCKINFO		m_MMCKInfoParent;
	MMCKINFO		m_MMCKInfoChild;

	//Mp3文件相关参数
	HINSTANCE			m_ForMp3_hDLL_LameEnc;
	PBYTE				m_ForMp3_pMP3Buffer;
	PBYTE				m_ForMp3_pWaveBuffer;
	DWORD				m_ForMp3_dwWaveBufferSize;
	DWORD				m_ForMp3_dwWaveDataBytes;
	HBE_STREAM			m_ForMp3_Proc_hStream;
	BEINITSTREAM		m_ForMp3_Proc_hInitStream;
	BEENCODECHUNK		m_ForMp3_Proc_hEncodeChunk;
	BEDEINITSTREAM		m_ForMp3_Proc_hDeinitStream;
	BECLOSESTREAM		m_ForMp3_Proc_hCloseStream;
	BEVERSION			m_ForMp3_Proc_hVersion;
	BEWRITEVBRHEADER	m_ForMp3_Proc_hWriteVBRHeader;
	BEWRITEINFOTAG		m_ForMp3_Proc_hWriteInfoTag;
	FILE*				m_pFileMp3;
	CString				m_csMp3FileName;
	DWORD				m_dwSamplesEncodeMp3Block;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOPLAYREC_H__A2C89DE8_176E_43ED_AD32_7CF7BD1F667D__INCLUDED_)
