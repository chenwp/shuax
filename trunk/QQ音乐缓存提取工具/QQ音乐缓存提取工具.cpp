// QQ音乐缓存提取工具.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "QQ音乐缓存提取工具.h"
#include "QQ音乐缓存提取工具Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQQApp

BEGIN_MESSAGE_MAP(CQQApp, CWinApp)
	//{{AFX_MSG_MAP(CQQApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQApp construction

CQQApp::CQQApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CQQApp object

CQQApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CQQApp initialization

BOOL CQQApp::InitInstance()
{
	// Standard initialization
	SetDialogBkColor (RGB (245,245,245), RGB ( 0,0,0));
	CQQDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
