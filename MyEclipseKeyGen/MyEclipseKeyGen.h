// MyEclipseKeyGen.h : main header file for the MYECLIPSEKEYGEN application
//

#if !defined(AFX_MYECLIPSEKEYGEN_H__A9E1423E_C355_43F3_9A07_A3D53E581FF9__INCLUDED_)
#define AFX_MYECLIPSEKEYGEN_H__A9E1423E_C355_43F3_9A07_A3D53E581FF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyEclipseKeyGenApp:
// See MyEclipseKeyGen.cpp for the implementation of this class
//

class CMyEclipseKeyGenApp : public CWinApp
{
public:
	CMyEclipseKeyGenApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEclipseKeyGenApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyEclipseKeyGenApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYECLIPSEKEYGEN_H__A9E1423E_C355_43F3_9A07_A3D53E581FF9__INCLUDED_)
