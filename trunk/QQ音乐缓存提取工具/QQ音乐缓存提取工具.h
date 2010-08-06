// QQ音乐缓存提取工具.h : main header file for the QQ音乐缓存提取工具 application
//

#if !defined(AFX_QQ_H__0C0F62ED_B7A0_4322_9747_853732D0D187__INCLUDED_)
#define AFX_QQ_H__0C0F62ED_B7A0_4322_9747_853732D0D187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQQApp:
// See QQ音乐缓存提取工具.cpp for the implementation of this class
//

class CQQApp : public CWinApp
{
public:
	CQQApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQQApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQ_H__0C0F62ED_B7A0_4322_9747_853732D0D187__INCLUDED_)
