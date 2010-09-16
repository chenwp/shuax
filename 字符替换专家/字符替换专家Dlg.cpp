// 字符替换专家Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "字符替换专家.h"
#include "字符替换专家Dlg.h"
#include <conio.h>

#include "MenFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const static char title[] = "字符替换工具";
void CMyDlg::BrowseDir(CString strDir) //此函数用于遍历文件夹下的文件，strDir是一个目录路径 
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
			BrowseDir(strPath); //如果目标是个文件夹，则利用嵌套来遍历 
		}
		else if(!ff.IsDirectory() && !ff.IsDots()) 
		{
			SearchFile((char*)(const char*)strPath,true);
		}
	} 
	ff.Close();
}
unsigned char getHex(unsigned char hex)
{
	if (hex >= '0' && hex <= '9') return hex - '0';
	if (hex >= 'A' && hex <= 'F') return hex - 'A' + 10;
	if (hex >= 'a' && hex <= 'f') return hex - 'a' + 10;
	return 0;
}

HRESULT  ResolveIt(HWND  hwnd,  LPCSTR  lpszLinkFile,  LPSTR  lpszPath)
{
	CoInitialize(NULL);
	HRESULT  hres;
	IShellLink*  psl;
	char  szGotPath[MAX_PATH];
	char  szDescription[MAX_PATH];
	WIN32_FIND_DATA  wfd;
	
	*lpszPath  =  0;  //  assume  failure
	
	//  Get  a  pointer  to  the  IShellLink  interface.
	hres  =  CoCreateInstance(CLSID_ShellLink,  NULL,
		CLSCTX_INPROC_SERVER,  IID_IShellLink,  (LPVOID  *)  & psl);
	if  (SUCCEEDED(hres))
	{
		IPersistFile*  ppf;
		
		//  Get  a  pointer  to  the  IPersistFile  interface.
		hres  =  psl->QueryInterface(IID_IPersistFile,
			(void**)&ppf);
		if  (SUCCEEDED(hres))
		{
			WCHAR  wsz[MAX_PATH];
			
			//  Ensure  that  the  string  is  Unicode.
			MultiByteToWideChar(CP_ACP,  0,  lpszLinkFile,  -1,  wsz,
				MAX_PATH);
			
			//  Load  the  shortcut.
			hres  =  ppf->Load(wsz,  STGM_READ);
			if  (SUCCEEDED(hres))
			{
				
				//  Resolve  the  link.
				hres  =  psl->Resolve(hwnd,  0);
				if  (SUCCEEDED(hres))
				{
					
					//  Get  the  path  to  the  link  target.
					hres  =  psl->GetPath(szGotPath,
						MAX_PATH,  (WIN32_FIND_DATA  *) & wfd,
						SLGP_RAWPATH);
					
					//  Get  the  description  of  the  target.
					hres  =  psl->GetDescription(szDescription,  MAX_PATH);
					lstrcpy(lpszPath,  szGotPath);
				}
			}
			//  Release  the  pointer  to  the  IPersistFile  interface.
			ppf->Release();
		}
		//  Release  the  pointer  to  the  IShellLink  interface.
		psl->Release();
	}
	CoUninitialize();
	return  hres;
}

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
//{{AFX_MSG_MAP(CMyDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_CTLCOLOR()
ON_EN_CHANGE(FindH, OnChangeFindH)
ON_EN_CHANGE(ReplaceH, OnChangeReplaceH)
ON_EN_CHANGE(FindA, OnChangeFindA)
ON_EN_CHANGE(FindU, OnChangeFindU)
ON_EN_CHANGE(ReplaceA, OnChangeReplaceA)
ON_EN_CHANGE(ReplaceU, OnChangeReplaceU)
ON_BN_CLICKED(exit, Onexit)
ON_BN_CLICKED(Replace, OnReplace)
ON_BN_CLICKED(Browse, OnBrowse)
ON_WM_DROPFILES()
	ON_EN_CHANGE(Find8, OnChangeFind8)
	ON_EN_CHANGE(Replace8, OnChangeReplace8)
	ON_WM_LBUTTONDOWN()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers
BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);   // Set big icon
	SetIcon(m_hIcon, FALSE);  // Set small icon
	
	// TODO: Add extra initialization here
	
	DragAcceptFiles(TRUE);
	if(__argc!=1) SetFileType(__argv[1]);
	is_edit = false;
	((CButton *)GetDlgItem(Ask))->SetCheck(TRUE);
	
	((CEdit*)GetDlgItem(FindH))->SetLimitText(1024);
	((CEdit*)GetDlgItem(FindA))->SetLimitText(1024);
	((CEdit*)GetDlgItem(FindU))->SetLimitText(1024);
	((CEdit*)GetDlgItem(ReplaceH))->SetLimitText(1024);
	((CEdit*)GetDlgItem(ReplaceA))->SetLimitText(1024);
	((CEdit*)GetDlgItem(ReplaceU))->SetLimitText(1024);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint()
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
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

HBRUSH CMyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetTextColor(RGB(0, 0, 100));
	}
	
	return hbr;
}
void CMyDlg::FormatHex(char *src)
{
	int i, j;
	
	for (i = 0, j = 0; src[i] != 0; i++)
	{
		if ((src[i] <= 'F' && src[i] >= 'A') || (src[i] <= 'f' && src[i] >= 'a') || (src[i] <= '9' && src[i] >= '0'))
		{
			if (src[i] != ' ')
			{
				if (src[i] <= 'f' && src[i] >= 'a')
					src[i] -= 32;
				temp[j++] = src[i];
			}
		}
	}
	temp[j] = 0;
	
	for (i = 0, j = 0; temp[i] != 0; i++)
	{
		src[j++] = temp[i];
		if ((i - 1) % 2 == 0 && temp[i+1] != 0)
		{
			src[j++] = ' ';
		}
	}
	src[j] = 0;
}
void CMyDlg::H2Hex(int nlDDlgltem)
{
	int dwLen;
	((CEdit*)GetDlgItem(nlDDlgltem))->GetSel(dwLen, dwLen);
	if (dwLen != 0) dwLen++;
	GetDlgItem(nlDDlgltem)->GetWindowText(src, MAX_PATH);
	FormatHex(src);
	GetDlgItem(nlDDlgltem)->SetWindowText(src);
	((CEdit*)GetDlgItem(nlDDlgltem))->SetSel(dwLen, dwLen, TRUE);
}
void CMyDlg::A2Hex(int nlDDlgltem1, int nlDDlgltem2)
{
	static char node[16];
	temp[0] = 0;
	GetDlgItem(nlDDlgltem1)->GetWindowText(src, MAX_PATH);
	
	for (int i = 0; src[i] != 0; i++)
	{
		::wsprintf(node, "%02X", (unsigned char)src[i]);
		::strcat(temp, node);
	}
	strcpy(src,temp);
	FormatHex(src);
	GetDlgItem(nlDDlgltem2)->SetWindowText(src);
}
void CMyDlg::T2Hex(int nlDDlgltem1, int nlDDlgltem2)
{
	static char node[16];
	src[0] = 0;
	GetWindowTextW(::GetDlgItem(m_hWnd, nlDDlgltem1), (wchar_t *)temp, MAX_PATH);
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)temp, -1, src, MAX_PATH, NULL, NULL);
	temp[0] = 0;
	for (int i = 0; src[i] != 0; i++)
	{
		::wsprintf(node, "%02X", (unsigned char)src[i]);
		::strcat(temp, node);
	}
	strcpy(src,temp);
	FormatHex(src);
	GetDlgItem(nlDDlgltem2)->SetWindowText(src);
}
void CMyDlg::U2Hex(int nlDDlgltem1, int nlDDlgltem2)
{
	wchar_t *_src;
	wchar_t *_temp;
	static wchar_t _node[16];
	_src = (wchar_t *)src;
	_temp = (wchar_t *)temp;
	
	_temp[0] = 0;
	GetWindowTextW(::GetDlgItem(m_hWnd, nlDDlgltem1), _src, MAX_PATH);
	
	for (int i = 0; _src[i] != 0; i++)
	{
		::wsprintfW(_node, L"%02X%02X", _src[i]&0xFF, _src[i] >> 8);
		::wcscat(_temp, _node);
	}
	
	SetWindowTextW(::GetDlgItem(m_hWnd, nlDDlgltem2), _temp);
	H2Hex(nlDDlgltem2);
}
int CMyDlg::GetHexValue(char *src)
{
	int i, j, flag;
	
	for (i = 0, j = 0; src[i] != 0; i++)
	{
		if ((src[i] <= 'F' && src[i] >= 'A') || (src[i] <= 'f' && src[i] >= 'a') || (src[i] <= '9' && src[i] >= '0'))
		{
			if (src[i] != ' ')
			{
				temp[j++] = src[i];
			}
		}
	}
	temp[j] = 0;
	if (j % 2 != 0)
	{
		temp[j-1] = 0;
	}
	src[0]=0;
	for (i = 0, j = 0, flag = 1; temp[i] != 0; i++)
	{
		
		char ch = getHex(temp[i]);
		if (ch != -1)
		{
			if (flag == 1) src[j] = ch << 4;
			else src[j++] += ch;
			flag *= -1;
		}
	} 
	src[j] = 0;
	src[j+1] = 0;
	src[j+2] = 0;
	return j;
}
void CMyDlg::H2ASCII(int nlDDlgltem1, int nlDDlgltem2)
{
	GetDlgItem(nlDDlgltem1)->GetWindowText(src, MAX_PATH);
	GetHexValue(src);
	GetDlgItem(nlDDlgltem2)->SetWindowText(src);
	
}
void CMyDlg::H2UTF8(int nlDDlgltem1, int nlDDlgltem2)
{
	GetDlgItem(nlDDlgltem1)->GetWindowText(src, MAX_PATH);
	
	GetHexValue(src);
	wchar_t *_temp;
	_temp = (wchar_t *)temp;
	MultiByteToWideChar(CP_UTF8, 0, src, -1, _temp, MAX_PATH);
	SetWindowTextW(::GetDlgItem(m_hWnd, nlDDlgltem2), _temp);
}
void CMyDlg::H2UNICODE(int nlDDlgltem1, int nlDDlgltem2)
{
	GetDlgItem(nlDDlgltem1)->GetWindowText(src, MAX_PATH);
	GetHexValue(src);
	SetWindowTextW(::GetDlgItem(m_hWnd, nlDDlgltem2), (wchar_t*)src);
}
BOOL CMyDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_CHAR && !GetKeyState(VK_CONTROL))
	{
		if (pMsg->hwnd ==::GetDlgItem(m_hWnd, FindH) || pMsg->hwnd ==::GetDlgItem(m_hWnd, ReplaceH))
		{
			if ((pMsg->wParam <= 'F' && pMsg->wParam >= 'A') || (pMsg->wParam <= 'f' && pMsg->wParam >= 'a') || (pMsg->wParam <= '9' && pMsg->wParam >= '0') || pMsg->wParam == '\b')
			{
				if (pMsg->wParam <= 'f' && pMsg->wParam >= 'a')
					pMsg->wParam -= 32;
			}
			else
			{
				MessageBeep(MB_ICONASTERISK);
				return TRUE;
			}
		}
	}
	
	if( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_F1:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		case VK_RETURN:
			return TRUE; 
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CMyDlg::OnChangeFindH()
{
	if (is_edit) return;
	is_edit = true;
	H2Hex(FindH);
	H2ASCII(FindH, FindA);
	H2UNICODE(FindH, FindU);
	H2UTF8(FindH,Find8);
	is_edit = false;
}
void CMyDlg::OnChangeFindA()
{
	if (is_edit) return;
	is_edit = true;
	A2Hex(FindA,FindH);
	H2UNICODE(FindH,FindU);
	H2UTF8(FindH,Find8);
	is_edit = false;
}
void CMyDlg::OnChangeFind8() 
{
	if (is_edit) return;
	is_edit = true;
	T2Hex(Find8,FindH);
	H2UNICODE(FindH,FindU);
	H2ASCII(FindH,FindA);
	is_edit = false;
}

void CMyDlg::OnChangeFindU()
{
	if (is_edit) return;
	is_edit = true;
	U2Hex(FindU,FindH);
	H2ASCII(FindH,FindA);
	H2UTF8(FindH,Find8);
	is_edit = false;
}
void CMyDlg::OnChangeReplaceH()
{ 
	if (is_edit) return;
	is_edit = true;
	H2Hex(ReplaceH);
	H2ASCII(ReplaceH,ReplaceA);
	H2UNICODE(ReplaceH,ReplaceU);
	H2UTF8(ReplaceH,Replace8);
	is_edit = false;
}
void CMyDlg::OnChangeReplaceA()
{
	if (is_edit) return;
	is_edit = true;
	A2Hex(ReplaceA,ReplaceH);
	H2UNICODE(ReplaceH,ReplaceU);
	H2UTF8(ReplaceH,Replace8);
	is_edit = false;
}

void CMyDlg::OnChangeReplaceU()
{
	if (is_edit) return;
	is_edit = true;
	U2Hex(ReplaceU,ReplaceH);
	H2ASCII(ReplaceH,ReplaceA);
	H2UTF8(ReplaceH,Replace8);
	is_edit = false;
}
void CMyDlg::OnChangeReplace8() 
{
	if (is_edit) return;
	is_edit = true;
	T2Hex(Replace8,ReplaceH);
	H2ASCII(ReplaceH,ReplaceA);
	H2UNICODE(ReplaceH,ReplaceU);
	is_edit = false;
}
void CMyDlg::Onexit()
{
	ExitProcess(0);
	OnOK();
}
int CALLBACK BrowserCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam,LPARAM lpData) 
{ 
	CWnd cw;
	switch (uMsg) 
	{ 
	case BFFM_INITIALIZED: 
		cw.Attach(hWnd);
		cw.CenterWindow();//将窗口置中
		cw.Detach();
		::SendMessage (hWnd, BFFM_SETSELECTION, 1, lpData);//lpData初始目录
		break; 
	default: 
		break; 
	} 
	return 0; 
}
void CMyDlg::OnBrowse()
{
	if(!((CButton *)GetDlgItem(Folder))->GetCheck())
	{
		OPENFILENAME ofn;
		if(GetDlgItem(FilePath)->GetWindowTextLength()<=3) src[0] = 0;
		else GetDlgItem(FilePath)->GetWindowText(src, MAX_PATH);
		
		RtlZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = _T("全部文件\0*.*\0");
		ofn.lpstrFile = src;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST |  0x10000000 | OFN_HIDEREADONLY;
		
		if (GetOpenFileName(&ofn))
		{
			GetDlgItem(FilePath)->SetWindowText(src);
		}
	}
	else
	{
		BROWSEINFO bf; 
		LPITEMIDLIST lpitem; 
		memset(&bf,0,sizeof BROWSEINFO); 
		bf.hwndOwner = m_hWnd; 
		bf.lpfn = BrowserCallbackProc;
		GetDlgItem(FilePath)->GetWindowText(src, MAX_PATH);
		bf.lParam = (LPARAM)src;//初始化位置
		bf.lpszTitle= "选择路径"; 
		bf.ulFlags = BIF_RETURNONLYFSDIRS; //属性你可自己选择 
		lpitem=SHBrowseForFolder(&bf); 
		if(lpitem!=NULL)
		{
			SHGetPathFromIDList(lpitem,src); 
			GetDlgItem(FilePath)->SetWindowText(src);
		}
	}
}
void CMyDlg::SearchFile(char *path,bool files)
{
	GetDlgItem(FindH)->GetWindowText(src, MAX_PATH);
	int len = GetHexValue(src);
	long offset = Search(path, (unsigned char*)src, len);
	switch (offset)
	{
	case -2:
		if(!files) MessageBox("打开文件失败！", title, MB_ICONWARNING);
		return ;
	case -1:
		if(!files) MessageBox("没有找到你要搜索的内容。", title, MB_ICONASTERISK);
		return ;
	default:
		if(GetDlgItem(ReplaceH)->GetWindowTextLength()<=0)
		{
			wsprintf(temp, "文件\"%s\"\n找到关键位置：0x%08X。", path, offset, offset);
			MessageBox(temp, title, MB_ICONQUESTION);
		}
		else
		{
			wsprintf(temp, "文件\"%s\"\n找到关键位置：0x%08X。是否进行替换？", path, offset, offset);
			if(!((CButton *)GetDlgItem(Ask))->GetCheck() || MessageBox(temp, title, MB_ICONQUESTION | MB_YESNO )==IDYES)
			{
				//进行替换
				
				FILE *fp;
				fp = fopen(path,"rb+");
				fseek(fp, offset, SEEK_SET);
				GetDlgItem(ReplaceH)->GetWindowText(src, MAX_PATH);
				int x = GetHexValue(src);
				fwrite(src,x,1,fp);
				fclose(fp);
				
				if(MessageBox("字符替换成功，是否继续替换？", title, MB_ICONQUESTION | MB_YESNO )==IDYES)
				{
					SearchFile(path,files);
				}
			}
		}
		return ;
	}
}
void CMyDlg::OnReplace()
{
	if(GetDlgItem(FilePath)->GetWindowTextLength()<=2)
	{
		OnBrowse();
		return ;
	}
	if(GetDlgItem(FindH)->GetWindowTextLength()<=0)
	{
		return ;
	}
	GetDlgItem(FilePath)->GetWindowText(path, MAX_PATH);
	if(((CButton *)GetDlgItem(Folder))->GetCheck()) BrowseDir(path);
	else SearchFile(path,false);
	return ;
}


void CMyDlg::OnDropFiles(HDROP hDropInfo) 
{
	CDialog::OnDropFiles(hDropInfo);
	UINT count;
	
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count)
	{        
		for(UINT i=0; i<1; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, src, MAX_PATH);
			SetFileType(src);
		}
	}
	DragFinish(hDropInfo);
	
}
void CMyDlg::SetFileType(char *path)
{
	DWORD dwAttributes = GetFileAttributes(path);
	if (dwAttributes != 0xFFFFFFFF)
	{
		if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			((CButton *)GetDlgItem(Folder))->SetCheck(true);
		}
		else
		{
			((CButton *)GetDlgItem(Folder))->SetCheck(false);
			if(stricmp(".lnk",strrchr(path,'.'))==0)
			{
				char temp[1024];
				ResolveIt(m_hWnd,path,temp);
				strcpy(path,temp);
			}
		}
		GetDlgItem(FilePath)->SetWindowText(path);
	}
}


void CMyDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage(WM_NCLBUTTONDOWN,(WPARAM)HTCAPTION,(LPARAM)HTCAPTION);
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CMyDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	MessageBox("获取更多帮助请访问：http://www.shuax.com\n\n日期：2010.08.29\n版本：v1.1", title, MB_ICONQUESTION);
	return TRUE;
}
