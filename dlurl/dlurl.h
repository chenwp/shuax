// dlurl.h : main header file for the DLURL application
//

#if !defined(AFX_DLURL_H__CA702612_83D3_4179_8C11_27170BD8E114__INCLUDED_)
#define AFX_DLURL_H__CA702612_83D3_4179_8C11_27170BD8E114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDlurlApp:
// See dlurl.cpp for the implementation of this class
//

class CDlurlApp : public CWinApp
{
public:
	CDlurlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlurlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDlurlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLURL_H__CA702612_83D3_4179_8C11_27170BD8E114__INCLUDED_)
