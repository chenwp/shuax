// MyEclipseKeyGen.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyEclipseKeyGen.h"
#include "MyEclipseKeyGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEclipseKeyGenApp

BEGIN_MESSAGE_MAP(CMyEclipseKeyGenApp, CWinApp)
	//{{AFX_MSG_MAP(CMyEclipseKeyGenApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEclipseKeyGenApp construction

CMyEclipseKeyGenApp::CMyEclipseKeyGenApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyEclipseKeyGenApp object

CMyEclipseKeyGenApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyEclipseKeyGenApp initialization

BOOL CMyEclipseKeyGenApp::InitInstance()
{
	// Standard initialization

	CMyEclipseKeyGenDlg dlg;
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
