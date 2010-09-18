// Picasa3.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Picasa3.h"
#include "Picasa3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicasa3App

BEGIN_MESSAGE_MAP(CPicasa3App, CWinApp)
	//{{AFX_MSG_MAP(CPicasa3App)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicasa3App construction

CPicasa3App::CPicasa3App()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPicasa3App object

CPicasa3App theApp;


/////////////////////////////////////////////////////////////////////////////
// CPicasa3App initialization

BOOL CPicasa3App::InitInstance()
{
	
	ULONG_PTR m_gdiplusToken; 
	GdiplusStartupInput  m_gdiplusStartupInput; 
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL); 
	// Standard initialization
	CPicasa3Dlg dlg;
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
