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

const static char title[] = "字符替换专家";
unsigned char getHex(unsigned char hex)
{
	if (hex >= '0' && hex <= '9') return hex - '0';
	if (hex >= 'A' && hex <= 'F') return hex - 'A' + 10;
	if (hex >= 'a' && hex <= 'f') return hex - 'a' + 10;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

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
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers
BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	DragAcceptFiles(TRUE);
	if(__argc!=1) GetDlgItem(FilePath)->SetWindowText(__argv[1]);
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
		::wsprintf(node, "%02X", src[i]);
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
	
	return CDialog::PreTranslateMessage(pMsg);
}
void CMyDlg::OnChangeFindH()
{
	if (is_edit) return;
	is_edit	=	true;
	H2Hex(FindH);
	H2ASCII(FindH, FindA);
	H2UNICODE(FindH, FindU);
	is_edit	=	false;
}

void CMyDlg::OnChangeReplaceH()
{	
	if (is_edit) return;
	is_edit	= true;
	H2Hex(ReplaceH);
	H2ASCII(ReplaceH,ReplaceA);
	H2UNICODE(ReplaceH,ReplaceU);
	is_edit	=	false;
}

void CMyDlg::OnChangeFindA()
{
	if (is_edit) return;
	is_edit	=	true;
	A2Hex(FindA,FindH);
	H2UNICODE(FindH,FindU);
	is_edit	=	false;
}

void CMyDlg::OnChangeFindU()
{
	if (is_edit) return;
	is_edit	=	true;
	U2Hex(FindU,FindH);
	H2ASCII(FindH,FindA);
	is_edit	=	false;
}

void CMyDlg::OnChangeReplaceA()
{
	if (is_edit) return;
	is_edit	=	true;
	A2Hex(ReplaceA,ReplaceH);
	H2UNICODE(ReplaceH,ReplaceU);
	is_edit	=	false;
}

void CMyDlg::OnChangeReplaceU()
{
	if (is_edit) return;
	is_edit	=	true;
	U2Hex(ReplaceU,ReplaceH);
	H2ASCII(ReplaceH,ReplaceA);
	is_edit	=	false;
}

void CMyDlg::Onexit()
{
	ExitProcess(0);
	OnOK();
}
void CMyDlg::OnBrowse()
{
	OPENFILENAME ofn;
	src[0] = 0;//一定要清零，不然他返回NULL！！！因为这个莫名其妙的东西我才贴上这个用法的……
	
	RtlZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFilter = _T("全部文件\0*.*\0");
	ofn.lpstrFile = src;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	if (GetOpenFileName(&ofn))
	{
		GetDlgItem(FilePath)->SetWindowText(src);
	}
}
void CMyDlg::OnReplace()
{
	if(GetDlgItem(FilePath)->GetWindowTextLength()<=2)
	{
		MessageBox("您还没有选择文件！", title, MB_ICONASTERISK);
		OnBrowse();
		return ;
	}
	if(GetDlgItem(FindH)->GetWindowTextLength()<=0)
	{
		return ;
	}
	GetDlgItem(FilePath)->GetWindowText(path, MAX_PATH);
	GetDlgItem(FindH)->GetWindowText(src, MAX_PATH);
	int len = GetHexValue(src);
	long offset = Search(path, (unsigned char*)src, len);
	switch (offset)
	{
	case -2:
		MessageBox("打开文件失败！", title, MB_ICONWARNING);
		return ;
	case -1:
		MessageBox("没有找到你要搜索的内容。", title, MB_ICONASTERISK);
		return ;
	default:
		if(GetDlgItem(ReplaceH)->GetWindowTextLength()<=0)
		{
			wsprintf(temp, "找到关键位置位于：%d 0x%x。", offset, offset);
			MessageBox(temp, title, MB_ICONQUESTION);
		}
		else
		{
			wsprintf(temp, "找到关键位置位于：%d 0x%x。是否进行替换？", offset, offset);
			if(!((CButton *)GetDlgItem(Ask))->GetCheck() || MessageBox(temp, title, MB_ICONQUESTION | MB_YESNO )==IDYES)
			{
				//进行替换
				
				FILE *fp;
				fp = fopen(path,"rb+");
				fseek(fp, offset, SEEK_SET);
				GetDlgItem(ReplaceH)->GetWindowText(src, MAX_PATH);
				int x = GetHexValue(src);
				fwrite(src,len,1,fp);
				fclose(fp);
				
				if(MessageBox("字符替换成功，是否继续替换？", title, MB_ICONQUESTION | MB_YESNO )==IDYES)
				{
					OnReplace();
				}
			}
		}
		return ;
	}
	return ;
}


void CMyDlg::OnDropFiles(HDROP hDropInfo) 
{
	UINT count;
	
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count)
	{        
		for(UINT i=0; i<count; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, path, MAX_PATH);
			GetDlgItem(FilePath)->SetWindowText(path);
		}
	}
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}
