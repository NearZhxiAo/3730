// IdentifierDlg.h : header file
//

#if !defined(AFX_IDENTIFIERDLG_H__EA4E068D_7D96_42E3_8781_5CE238242082__INCLUDED_)
#define AFX_IDENTIFIERDLG_H__EA4E068D_7D96_42E3_8781_5CE238242082__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIdentifierDlg dialog
#include "Recognizer.h"
#include "IdentClient.h"
#include "IdentCenter.h"
#include "WorkMode.h"
#include "Audio_codec.h"
//#include "AppSV/ParamGenerator.h"

class CIdentifierDlg : public CDialog
{
// Construction
public:
	CIdentifierDlg(CWnd* pParent = NULL);	// standard constructor
	

	//识别器添加变量
	HANDLE m_hThreadForIdentMainHandle;//客户端监测文件是否有新文件产生的线程句柄
	UINT m_uiThreadForIdentMainID;//客户端监测文件是否有新文件产生的线程ID
	static unsigned __stdcall ThreadForIdentMainFunc(void * pParam);

	//Recognizer m_Recognizer;
	//IdentCenter m_IdentCenter;
	//IdentClient m_IdentClient;
	RecognizerFactory *m_pFactory;
	Recognizer *m_pRecognizer;
	IdentClient *m_pIdentClient;
	IdentCenter *m_pIdentCenter;
	CAudio_Codec m_AudioCodec;


	void SetAutoRun(bool run);
//	CParamGenerator m_paramGenration;
// Dialog Data
	//{{AFX_DATA(CIdentifierDlg)
	enum { IDD = IDD_IDENTIFIER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIdentifierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIdentifierDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDENTIFIERDLG_H__EA4E068D_7D96_42E3_8781_5CE238242082__INCLUDED_)
