// themeDlg.cpp : implementation file
//
#include "stdio.h"
#include "stdafx.h"
#include "theme.h"
#include "themeDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static wchar_t THIS_FILE[] = __FILE__;
#endif

wchar_t exepath[512];
wchar_t path[512];
wchar_t iconPath[512];
wchar_t ToolbarPath[512];
wchar_t res[128];

wchar_t title[]=L"7-Zip主题安装工具 v1.1";

void RebuildIconCache()
{
    int Iconw = GetSystemMetrics(SM_CXICON);
    HKEY hKey;
    RegOpenKeyExA(HKEY_CURRENT_USER,"Control Panel\\Desktop\\WindowMetrics", 0, KEY_ALL_ACCESS, &hKey);
    char buff[20];
    itoa(Iconw - 1, buff, 10);
    RegSetValueExA(hKey, "Shell Icon Size", 0, REG_SZ, (unsigned char *)buff, sizeof(buff));
    SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0, 0);
    itoa(Iconw, buff, 10);
    RegSetValueExA(hKey, "Shell Icon Size", 0, REG_SZ, (unsigned char *)buff, sizeof(buff));
    SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0, 0);
	SHChangeNotify(SHCNE_ALLEVENTS, SHCNF_FLUSH, 0, 0); 
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST + SHCNF_FLUSH, 0, 0);
	BroadcastSystemMessage(BSF_FLUSHDISK,  0,   WM_WININICHANGE,   0,   0)   ;
}
LPITEMIDLIST ParsePidlFromPath(wchar_t *path)
{
     LPSHELLFOLDER IpsfDeskTop;    
     LPITEMIDLIST lpifq;    
     ULONG ulEaten, ulAttribs;    
     HRESULT hres;    
     SHGetDesktopFolder(&IpsfDeskTop);  
     hres = IpsfDeskTop ->ParseDisplayName(NULL, NULL, path, &ulEaten, &lpifq, &ulAttribs);    
     hres=IpsfDeskTop->Release( );        
     if(FAILED(hres))
         return NULL;
     return lpifq;
}

bool ReleaseRes(CString strFileName,WORD wResID,CString strFileType)   
{    
    DWORD   dwWrite=0;         
    HANDLE  hFile = CreateFile(strFileName, GENERIC_WRITE,FILE_SHARE_WRITE,NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);    
    HRSRC   hrsc =  FindResource(NULL, MAKEINTRESOURCE(wResID), strFileType);   
    HGLOBAL hG = LoadResource(NULL, hrsc);   
    DWORD   dwSize = SizeofResource( NULL,  hrsc);   
    WriteFile(hFile,hG,dwSize,&dwWrite,NULL);      
    CloseHandle( hFile );   
    return TRUE;   
}
void getpath(wchar_t *temppath,wchar_t *tips)
{	
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = ParsePidlFromPath(exepath);
	bi.pszDisplayName = NULL;
	bi.lpszTitle = tips;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	
	if(!SHGetPathFromIDList(SHBrowseForFolder(&bi), temppath)||wcslen(temppath)<2)
	temppath[0]=0;
}
void RunAndWait(wchar_t *path)
{
	STARTUPINFO StartInfo = {sizeof(StartInfo)}; 
	PROCESS_INFORMATION ProcInfo;
	memset(&ProcInfo, 0, sizeof(ProcInfo)); 
	if (CreateProcess(NULL, path, NULL, NULL, NULL, NULL, NULL, NULL, &StartInfo, &ProcInfo))
	{
		WaitForSingleObject(ProcInfo.hProcess, INFINITE);
		CloseHandle(ProcInfo.hThread);
		CloseHandle(ProcInfo.hProcess);
	}
	return ;
}
/////////////////////////////////////////////////////////////////////////////
// CThemeDlg dialog

CThemeDlg::CThemeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThemeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThemeDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThemeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThemeDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThemeDlg, CDialog)
	//{{AFX_MSG_MAP(CThemeDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThemeDlg message handlers

BOOL CThemeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	
	//获得7-Zip安装位置
	DWORD len = 256;
	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\7-Zip", 0, 0x100 + KEY_READ, &hKey);
	
	if (RegQueryValueEx(hKey, L"Path", 0, 0, (unsigned char *)&path, &len) != NULL)
	{
		getpath(path,L"请选择7-Zip安装目录");
		if(wcslen(path)<2)
		{
			MessageBox(L"程序未找到7-Zip安装目录，必须退出！",title,MB_OK + MB_ICONWARNING);
			exit(0);
		}
	}
	
	//补足字符串
	if(path[wcslen(path)-1]!='\\')wcscat(path,L"\\");
	
	wchar_t dll[1024];
	wcscpy(dll,path);
	wcscat(dll,L"7z.dll");
	CFileFind   find;   
	if (!find.FindFile(dll))
	{
		MessageBox(L"程序找到的7-Zip安装目录不正确，必须退出！",title,MB_OK + MB_ICONWARNING);
		exit(0);
	}
	//释放ResHacker.exe
	GetTempPath(512,res);
    wcscat(res,L"ResHacker.exe");
	ReleaseRes(res,IDR_EXERC1,"EXERC");
	//填充默认位置

	GetCurrentDirectory(1024,exepath);
	wchar_t temp[1024];
	
	swprintf(temp,L"%s\\ToolBar\\Alert",exepath);
	GetDlgItem(IDC_EDIT2)->SetWindowText(temp);

	swprintf(temp,L"%s\\FileIcon\\Tango",exepath);
	GetDlgItem(IDC_EDIT3)->SetWindowText(temp);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CThemeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThemeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CThemeDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	getpath(ToolbarPath,L"请选择工具栏目录");
	if(ToolbarPath[0]!=0)GetDlgItem(IDC_EDIT2)->SetWindowText(ToolbarPath);
}

void CThemeDlg::OnButton3() 
{
	getpath(iconPath,L"请选择文件图标目录");
	if(iconPath[0]!=0) GetDlgItem(IDC_EDIT3)->SetWindowText(iconPath);
}

void CThemeDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}
void CThemeDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	//生成脚本
	GetDlgItem(IDC_EDIT3)->GetWindowText(iconPath,512);
	GetDlgItem(IDC_EDIT2)->GetWindowText(ToolbarPath,512);
	wchar_t temp[512];
	wchar_t script[4096];
	wchar_t file[512];
	wchar_t command[128];
	FILE *fp;
	int flag=0;
	if(ToolbarPath[0]!=0)
	{
		swprintf(temp,L"%s7zFM.exe",path);
		swprintf(script,L"[FILENAMES]\n  Exe=      \"%s\"\n  SaveAs=   \"%s\"\n  Log=      \"7z.log\"\n[COMMANDS]\n-modify %s\\48x36\\add.bmp, BITMAP,2002,\n-modify %s\\48x36\\Extract.bmp,BITMAP,2003,\n-modify %s\\48x36\\Test.bmp,BITMAP,2004,\n-modify %s\\48x36\\Copy.bmp,BITMAP,2010,\n-modify %s\\48x36\\Move.bmp,BITMAP,2011,\n-modify %s\\48x36\\Delete.bmp,BITMAP,2012,\n-modify %s\\48x36\\Info.bmp,BITMAP,2013,\n-modify %s\\24x24\\add.bmp, BITMAP,2082,\n-modify %s\\24x24\\Extract.bmp,BITMAP,2083,\n-modify %s\\24x24\\Test.bmp,BITMAP,2084,\n-modify %s\\24x24\\Copy.bmp,BITMAP,2090,\n-modify %s\\24x24\\Move.bmp,BITMAP,2091,\n-modify %s\\24x24\\Delete.bmp,BITMAP,2092,\n-modify %s\\24x24\\Info.bmp,BITMAP,2093,\n",temp,temp,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath,ToolbarPath);
		GetTempPath(512,file);
		wcscat(file,L"1.txt");
		fp=_wfopen(file,L"wb");	
		int   iLen=WideCharToMultiByte   (936,   0,   (PWSTR)script,   -1,   NULL,0,   NULL,   NULL); 
		char*   pszGbt=new   char[iLen+1];   
		WideCharToMultiByte   (936,   0,   (PWSTR)script,   -1,   pszGbt,iLen,   NULL,   NULL); 
		fputs(pszGbt,fp);
		fclose(fp);
		swprintf(command,L"%s -script \"%s\"",res,file);
		RunAndWait(command);
		DeleteFile(file);
		flag++;
	}
	if(iconPath[0]!=0)
	{
		swprintf(temp,L"%s7z.dll",path);
		swprintf(script,L"[FILENAMES]\n  Exe=      \"%s\"\n  SaveAs=   \"%s\"\n  Log=      \"7z.log\"\n[COMMANDS]\n-modify %s\\7z.ico, ICONGROUP,0,\n-modify %s\\zip.ico,ICONGROUP,1,\n-modify %s\\bz2.ico,ICONGROUP,2,\n-modify %s\\rar.ico,ICONGROUP,3,\n-modify %s\\arj.ico,ICONGROUP,4,\n-modify %s\\z.ico,  ICONGROUP,5,\n-modify %s\\lha.ico,ICONGROUP,6,\n-modify %s\\cab.ico,ICONGROUP,7,\n-modify %s\\iso.ico,ICONGROUP,8,\n-modify %s\\001.ico,ICONGROUP,9,\n-modify %s\\rpm.ico,ICONGROUP,10,\n-modify %s\\deb.ico,ICONGROUP,11,\n-modify %s\\cpio.ico,ICONGROUP,12,\n-modify %s\\tar.ico,ICONGROUP,13,\n-modify %s\\gz.ico,ICONGROUP,14,\n-modify %s\\wim.ico,ICONGROUP,15,\n-modify %s\\lzh.ico,ICONGROUP,16,\n-modify %s\\dmg.ico,ICONGROUP,17,\n-modify %s\\hfs.ico,ICONGROUP,18,\n-modify %s\\xar.ico,ICONGROUP,19,\n",temp,temp,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath,iconPath);
		GetTempPath(512,file);
		wcscat(file,L"2.txt");
		fp=_wfopen(file,L"wb");
		int   iLen=WideCharToMultiByte   (936,   0,   (PWSTR)script,   -1,   NULL,0,   NULL,   NULL); 
		char*   pszGbt=new   char[iLen+1];   
		WideCharToMultiByte   (936,   0,   (PWSTR)script,   -1,   pszGbt,iLen,   NULL,   NULL); 
		fputs(pszGbt,fp);
		fclose(fp);
		swprintf(command,L"%s -script \"%s\"",res,file);
		RunAndWait(command);
		DeleteFile(file);
		flag++;
	}
	if(!flag)return;
	DeleteFile(L"7z.log");
	
	RebuildIconCache();
	MessageBox(L"您的选择执行完毕！",title,MB_OK + MB_ICONINFORMATION);
}

void CThemeDlg::OnDestroy() 
{
	DeleteFile(res);
	GetTempPath(512,res);
	wcscat(res,L"ResHacker.ini");
	DeleteFile(res);
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	
}
