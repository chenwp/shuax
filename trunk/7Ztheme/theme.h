// theme.h : main header file for the THEME application
//

#if !defined(AFX_THEME_H__80965664_8C91_4DC9_9F2F_E64E2903FE58__INCLUDED_)
#define AFX_THEME_H__80965664_8C91_4DC9_9F2F_E64E2903FE58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CThemeApp:
// See theme.cpp for the implementation of this class
//

class CThemeApp : public CWinApp
{
public:
	CThemeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CThemeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEME_H__80965664_8C91_4DC9_9F2F_E64E2903FE58__INCLUDED_)
