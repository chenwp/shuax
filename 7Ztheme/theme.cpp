// theme.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "theme.h"
#include "themeDlg.h"

#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeApp

BEGIN_MESSAGE_MAP(CThemeApp, CWinApp)
//{{AFX_MSG_MAP(CThemeApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThemeApp construction

CThemeApp::CThemeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CThemeApp object

CThemeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CThemeApp initialization

BOOL CThemeApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	  
  
	HANDLE hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
	
	PROCESSENTRY32 pe32;
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	for (Process32First (hProcessSnap, &pe32);Process32Next (hProcessSnap, &pe32);)
	{
        if(wcsicmp(pe32.szExeFile, L"7zFM.exe")==0)
		{
			if(MessageBox(0,L"程序检测到你的7-Zip正在运行，是否立即结束进程？\n\n如果不结束进程将会无法替换工具栏图标。",L"7-Zip主题安装工具 v1.1",MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				HANDLE TargetProcess = OpenProcess(PROCESS_TERMINATE,FALSE, pe32.th32ProcessID);
				//if(TargetProcess == NULL) break;
				TerminateProcess(TargetProcess, 0);
			}
			else break;
		}
	}
	CloseHandle(hProcessSnap);
	
	CThemeDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
