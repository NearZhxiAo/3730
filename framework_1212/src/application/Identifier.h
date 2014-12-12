// Identifier.h : main header file for the IDENTIFIER application
//

#if !defined(AFX_IDENTIFIER_H__A2758785_D61A_4AF4_A97A_DDC8F093591B__INCLUDED_)
#define AFX_IDENTIFIER_H__A2758785_D61A_4AF4_A97A_DDC8F093591B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIdentifierApp:
// See Identifier.cpp for the implementation of this class
//

class CIdentifierApp : public CWinApp
{
public:
	CIdentifierApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIdentifierApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIdentifierApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDENTIFIER_H__A2758785_D61A_4AF4_A97A_DDC8F093591B__INCLUDED_)
