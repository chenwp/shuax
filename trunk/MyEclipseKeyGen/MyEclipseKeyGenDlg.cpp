// MyEclipseKeyGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyEclipseKeyGen.h"
#include "MyEclipseKeyGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static char LL[] = "Decompiling this copyrighted software is a violation of both your license agreement and the Digital Millenium Copyright Act of 1998 (http://www.loc.gov/copyright/legislation/dmca.pdf). Under section 1204 of the DMCA, penalties range up to a $500,000 fine or up to five years imprisonment for a first offense. Think about it; pay for a license, avoid prosecution, and feel better about yourself.";
static int year=3;             //年限1-3 
static int license=20;          //许可数0-999,0为不限制 
static char type[]="YE3MP";    //YE3MB,YE3MP

/////////////////////////////////////////////////////////////////////////////
// CMyEclipseKeyGenDlg dialog

CMyEclipseKeyGenDlg::CMyEclipseKeyGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyEclipseKeyGenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyEclipseKeyGenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyEclipseKeyGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyEclipseKeyGenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyEclipseKeyGenDlg, CDialog)
	//{{AFX_MSG_MAP(CMyEclipseKeyGenDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEclipseKeyGenDlg message handlers

BOOL CMyEclipseKeyGenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT1)->SetWindowText("http://www.shuax.com");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyEclipseKeyGenDlg::OnPaint() 
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

HCURSOR CMyEclipseKeyGenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyEclipseKeyGenDlg::OnChangeEdit1() 
{
	char src[128];
	int len=GetDlgItem(IDC_EDIT1)->GetWindowTextLength()+1;
	GetDlgItem(IDC_EDIT1)->GetWindowText(src, len);
	GetDlgItem(IDC_EDIT2)->SetWindowText(MyEclipseKeyGen(src));
}
long CMyEclipseKeyGenDlg::decode(char *s) {
        long ret=0;
        for (int i=0;s[i]!=0;i++)
            ret = (31 * ret) + s[i];
        return abs(ret);
}
char* CMyEclipseKeyGenDlg::change(char *s) {
        char j;
        int k=strlen(s);
        for (int i=0;i<k;i++) {
            j = s[i];
            if ((j >= 48) && (j <= 57)) {
                j = (((j - 48) + 5) % 10) + 48;
            } else if ((j >= 65) && (j <= 90)) {
                j = (((j - 65) + 13) % 26) + 65;
            } else if ((j >= 97) && (j <= 122)) {
                j = (((j - 97) + 13) % 26) + 97;
            }
            s[i] = j;
        }
        return s;
    }
char* CMyEclipseKeyGenDlg::MyEclipseKeyGen(char *userId) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    time.wYear+=year;
    time.wDay-=1;
    char res[512];
    wsprintf(res,"%c%s-300%03d-%02d%02d%02d0%s%s\0",userId[0],type,license,time.wYear-2000,time.wMonth,time.wDay,LL,userId);
    //printf(res);
    long suf=decode(res);
    wsprintf(res,"%c%s-300%03d-%02d%02d%02d0%d",userId[0],type,license,time.wYear-2000,time.wMonth,time.wDay,suf);
    return change(res);
}