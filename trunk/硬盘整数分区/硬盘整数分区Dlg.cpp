// 硬盘整数分区Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "硬盘整数分区.h"
#include "硬盘整数分区Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CMyDlg::FAT32(int GB)
{
	if(GB==0) return GB;
	if(GB>=32)
	{
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(true);
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(false);
		OnChangeEdit1();
		return NTFS(GB);
	}
	GB = (GB-1)*4+1024*GB;
	return GB;
}
int CMyDlg::NTFS(int GB)
{
	static double ZHU = 512.0*255.0*63.0/1024.0/1024.0;
	if(GB==0) return GB;
	GB = ((int)(1024.0*GB/ZHU+1.0))*ZHU;
	return GB+1;
}
/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

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

HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CMyDlg::PreTranslateMessage(MSG* pMsg) 
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

void CMyDlg::OnChangeEdit1() 
{
	if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		//NTFS
		SetDlgItemInt(IDC_EDIT2,NTFS(GetDlgItemInt(IDC_EDIT1)));
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		//FAT32
		SetDlgItemInt(IDC_EDIT2,FAT32(GetDlgItemInt(IDC_EDIT1)));
	}
	else
	{
		//不会发生
	}
}

void CMyDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	OnChangeEdit1();
}

void CMyDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	OnChangeEdit1();
}
