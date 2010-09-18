// Picasa3.h : main header file for the PICASA3 application
//

#if !defined(AFX_PICASA3_H__4FF6EDC4_375B_46E0_954D_2AC4FE501C14__INCLUDED_)
#define AFX_PICASA3_H__4FF6EDC4_375B_46E0_954D_2AC4FE501C14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPicasa3App:
// See Picasa3.cpp for the implementation of this class
//

class CPicasa3App : public CWinApp
{
public:
	CPicasa3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicasa3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPicasa3App)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICASA3_H__4FF6EDC4_375B_46E0_954D_2AC4FE501C14__INCLUDED_)
