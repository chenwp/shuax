// QQ音乐缓存提取工具Dlg.cpp : implementation file
//

#include <conio.h>
#include "stdafx.h"
#include "QQ音乐缓存提取工具.h"
#include "QQ音乐缓存提取工具Dlg.h"

#include "tea.cpp"
#include "tag.cpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void BrowseDir(CString strDir,CStringList *list,float &size) //此函数用于遍历文件夹下的文件，strDir是一个目录路径 
{
	CFileFind ff; 
	CString szDir = strDir,strPath; 
	if(szDir.Right(1) != "\\") //保证目录是以\结尾的 
		szDir += "\\"; 
	szDir += "*.*"; 
	BOOL res = ff.FindFile(szDir); 
	while( res ) 
	{ 
		res = ff.FindNextFile(); 
		strPath = ff.GetFilePath(); 
		if(ff.IsDirectory() && !ff.IsDots()) 
		{
			BrowseDir(strPath,list,size); //如果目标是个文件夹，则利用嵌套来遍历 
		}
		else if(!ff.IsDirectory() && !ff.IsDots()) 
		{
			if (strstr(strPath,".tdl")!=0)
			{	
				list->AddTail(strPath); //如果目标是个文件，则对它进行处理 
				size += ff.GetLength();
			}
		}
	} 
	ff.Close();
}
BOOL mkdirEx(const char* lpPath)
{
	CString pathname = lpPath;
	if(pathname.Right(1) != "\\")
		pathname += "\\" ;
	int end = pathname.ReverseFind('\\');
	int pt = pathname.Find('\\');
	if (pathname[pt-1] == ':')
		pt = pathname.Find('\\', pt+1);
	CString path;
	while(pt != -1 && pt<=end)
	{
		path = pathname.Left(pt+1);
		if(GetFileAttributes(path)==0xFFFFFFFF)
			CreateDirectory(path,NULL);
		pt = pathname.Find('\\', pt+1);
	}
	return true;
}
//删除[shan chu]文件[wen jian]到回收站中
//pszPath  : 待删除[shan chu]的全路径[lu jing]文件[wen jian]名[wen jian ming]
//bDelete  : TRUE 删除[shan chu]，不移到回收站，FALSE:移到回收站
//返回    : TRUE 删除[shan chu]成功     FALSE 删除[shan chu]失败
Recycle(LPCTSTR pszPath, BOOL bDelete/*=FALSE*/)
{
    SHFILEOPSTRUCT  shDelFile;
    memset(&shDelFile,0,sizeof(SHFILEOPSTRUCT));
    shDelFile.fFlags |= FOF_SILENT;      // don't report progress
    shDelFile.fFlags |= FOF_NOERRORUI;     // don't report errors
    shDelFile.fFlags |= FOF_NOCONFIRMATION;    // don't confirm delete
	
	
    // Copy pathname to double-NULL-terminated string.
    //
    TCHAR buf[_MAX_PATH + 1]; // allow one more character
    _tcscpy(buf, pszPath);   // copy caller's pathname
    buf[_tcslen(buf)+1]=0;   // need two NULLs at end
	
    // Set SHFILEOPSTRUCT params for delete operation
    shDelFile.wFunc = FO_DELETE;       // REQUIRED: delete operation
    shDelFile.pFrom = buf;         // REQUIRED: which file(s)
    shDelFile.pTo = NULL;          // MUST be NULL
    if (bDelete)
    {         // if delete requested..
        shDelFile.fFlags &= ~FOF_ALLOWUNDO;    // ..don't use Recycle Bin
    } 
    else 
    {           // otherwise..
		shDelFile.fFlags |= FOF_ALLOWUNDO;    // ..send to Recycle Bin
    }  
    
	
	return SHFileOperation(&shDelFile);    // do it!
}
void getpath(char *temppath,char *tips)
{	
#define BIF_USENEWUI 0x0050
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = tips;
	bi.ulFlags = BIF_RETURNONLYFSDIRS + BIF_USENEWUI;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	
	if(!SHGetPathFromIDList(SHBrowseForFolder(&bi), temppath))
	{
		temppath[0]=0;
	}
	
}
/////////////////////////////////////////////////////////////////////////////
// CQQDlg dialog

CQQDlg::CQQDlg(CWnd* pParent /*=NULL*/)
: CDialog(CQQDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQQDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQQDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQQDlg, CDialog)
//{{AFX_MSG_MAP(CQQDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQDlg message handlers

BOOL CQQDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	//AllocConsole();
	
	//获得文件大小信息
	size1 = size2 = 0;
	
	HKEY hkey; unsigned long len=1024;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Tencent\\QQMusic", 0, KEY_ALL_ACCESS, &hkey);
	
	if(RegQueryValueEx(hkey, "SaveCachePath", 0, 0,(unsigned char*)&QQPlayer, &len) != ERROR_SUCCESS )
	{
		char AppDataPath[1024];
		LPITEMIDLIST pidl;
		SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&pidl);
		SHGetPathFromIDList(pidl,AppDataPath); 
		strcat(AppDataPath,"\\Tencent\\QQMusic\\CachePath.ini");
		GetPrivateProfileString("Cache","Path",0,QQPlayer,len,AppDataPath);
	}
	if(RegQueryValueEx(hkey, "WhirlCachePath", 0, 0,(unsigned char*)&QZonePlayer, &len) != ERROR_SUCCESS )
	{
		char AppDataPath[1024];
		LPITEMIDLIST pidl;
		SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&pidl);
		SHGetPathFromIDList(pidl,AppDataPath);
		strcpy(QZonePlayer,AppDataPath);
		strcat(QZonePlayer,"\\Tencent\\QQMusic\\WhirlCache");
	}
	RegCloseKey(hkey);
	
	GetDlgItem(IDC_EDIT1)->SetWindowText(QQPlayer);
	GetDlgItem(IDC_EDIT2)->SetWindowText(QZonePlayer);
	BrowseDir(QQPlayer,&list1,size1);
	BrowseDir(QZonePlayer,&list2,size2);
	
	
	((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);
	ChangeCheck();
	
	((CButton *)GetDlgItem(IDC_CHECK4))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK6))->SetCheck(TRUE);
	GetDlgItem(IDC_EDIT3)->SetWindowText("%A - %T");
	
	
	char DocPath[1024];
	
	LPITEMIDLIST pidl;
	SHGetSpecialFolderLocation(NULL,CSIDL_PERSONAL,&pidl);
	SHGetPathFromIDList(pidl,DocPath); 
	strcat(DocPath,"\\QQMusicCollect\\");
	GetDlgItem(IDC_EDIT5)->SetWindowText(DocPath);
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQQDlg::OnPaint() 
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

HCURSOR CQQDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CQQDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			return TRUE;
		case VK_RETURN:
			return TRUE; 
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CQQDlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	ChangeCheck();
}

void CQQDlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	ChangeCheck();
}
void CQQDlg::ChangeCheck()
{
	char Temp[1024];
	
	float _size1;
	float _size2;
	int _count1;
	int _count2;
	if(((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		_size1=size1;
		_count1=list1.GetCount();
	}
	else
	{
		_size1=0;
		_count1=0;
	}
	if(((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck())
	{
		_size2=size2;
		_count2=list2.GetCount();
	}
	else
	{
		_size2=0;
		_count2=0;
	}
	
	sprintf(Temp,"缓存文件总大小：%.02fMB，共有缓存文件%d个。",(_size1+_size2)/1024/1024,_count1+_count2);
	GetDlgItem(IDC_EDIT4)->SetWindowText(Temp);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetRange(0,_count1+_count2);
}

void CQQDlg::OnCheck6() 
{
	// TODO: Add your control notification handler code here
	if(!((CButton *)GetDlgItem(IDC_CHECK6))->GetCheck())
	{
		((CButton *)GetDlgItem(IDC_EDIT3))->EnableWindow(false);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT3))->EnableWindow(true);
	}
}

void CQQDlg::OnCheck4() 
{
	// TODO: Add your control notification handler code here
	if(!((CButton *)GetDlgItem(IDC_CHECK4))->GetCheck())
	{
		((CButton *)GetDlgItem(IDC_CHECK5))->SetCheck(false);
		((CButton *)GetDlgItem(IDC_CHECK5))->EnableWindow(false);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK5))->EnableWindow(true);
	}
}

void CQQDlg::OnButton1() 
{
	char path[1024];
	getpath(path,"请选择保存目录");
	if(path[0]!=0) GetDlgItem(IDC_EDIT5)->SetWindowText(path);
}
void CQQDlg::OnOK() 
{
	//创建保存目录
	char save[1024];
	wchar_t format[128];
	GetWindowTextW(::GetDlgItem(m_hWnd, IDC_EDIT3), format, 128);
	
	GetDlgItem(IDC_EDIT5)->GetWindowText(save,1024);
	if(save[strlen(save)-1]!='\\') strcat(save,"\\");
	mkdirEx(save);
	
	CStringList list;
	if(((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		list.AddTail(&list1);
	}
	if(((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck())
	{
		list.AddTail(&list2);
	}
	int nCount=list.GetCount();
	for(int i=0;i<nCount;i++)
	{
		char *temp;
		temp = decrypt(list.GetAt(list.FindIndex(i)),save);
		if(((CButton *)GetDlgItem(IDC_CHECK6))->GetCheck())
		{
			//改名
			GetWmaTag(temp,format,save);
		}
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(i+1);
	}
	//删除文件夹
	if(((CButton *)GetDlgItem(IDC_CHECK4))->GetCheck())
	{
		if(((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck()) Recycle(QQPlayer,((CButton *)GetDlgItem(IDC_CHECK5))->GetCheck());
		if(((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck()) Recycle(QZonePlayer,((CButton *)GetDlgItem(IDC_CHECK5))->GetCheck());
	}
	MessageBoxW(m_hWnd,L"任务全部完成，程序将会退出并且打开提取目录。",L"QQ音乐缓存提取工具",MB_OK + MB_ICONINFORMATION);
	::ShellExecute(NULL, "open", save,NULL,NULL,SW_SHOW); 
	ExitProcess(0);
	CDialog::OnOK();
}

