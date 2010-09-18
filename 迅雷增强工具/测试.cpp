// 测试.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "测试.h"
#include "测试Dlg.h"
#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyApp
BEGIN_MESSAGE_MAP(CMyApp, CWinApp)
//{{AFX_MSG_MAP(CMyApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyApp construction

CMyApp::CMyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyApp object

CMyApp theApp;
//ULONG_PTR m_pGdiToken;

/////////////////////////////////////////////////////////////////////////////
// CMyApp initialization

BOOL CMyApp::InitInstance()
{
	ULONG_PTR m_gdiplusToken; 
	GdiplusStartupInput  m_gdiplusStartupInput; 
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL); 
	AfxEnableControlContainer();
	
	/*
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	
	  OpenProcessToken( GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) ;
	  
		
		  LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) ;
		  
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Luid = sedebugnameValue;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			
			  AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) ;
	*/
	
	HANDLE hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
	
	PROCESSENTRY32 pe32;
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	for (Process32First (hProcessSnap, &pe32);Process32Next (hProcessSnap, &pe32);)
	{
        if(stricmp(pe32.szExeFile, "Thunder.exe")==0)
		{
			if(MessageBox(0,"程序检测到你的迅雷正在运行，是否立即结束进程？","迅雷增强工具 v1.1",MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				HANDLE TargetProcess = OpenProcess(PROCESS_TERMINATE,FALSE, pe32.th32ProcessID);
				//if(TargetProcess == NULL) break;
				TerminateProcess(TargetProcess, 0);
			}
			else break;
		}
	}
	
	CloseHandle(hProcessSnap);
	/*
    DWORD aProcesses[1024], cbNeeded, cbMNeeded;
    HMODULE hMods[1024];
    HANDLE hProcess;
    char szProcessName[MAX_PATH];
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )  return 0;
    for(int i=0; i< (int) (cbNeeded / sizeof(DWORD)); i++)
    {
	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
	EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
	GetModuleFileNameEx( hProcess, hMods[0], szProcessName,sizeof(szProcessName));
	if(strstr(szProcessName, "Thunder.exe")!=0)
	{
	if(MessageBox(0,"程序检测到你的迅雷正在运行，是否立即结束进程？","迅雷增强工具 v1.0",MB_YESNO | MB_ICONQUESTION)==IDYES)
	{
				HANDLE TargetProcess = OpenProcess(PROCESS_TERMINATE,FALSE, aProcesses[i]);
				if(TargetProcess == NULL) break;
				TerminateProcess(TargetProcess, 0);
				}
				else break;
				}
				}
	*/
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	//SetDialogBkColor (RGB (0,0,0 ), RGB ( 0,0,0)); 
	CMyDlg dlg;
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
	
	//GdiplusStartupInput m_gdiplusStartupInput;
    //GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	return FALSE;
}


