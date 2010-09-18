// dlurlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dlurl.h"
#include "dlurlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlurlDlg dialog
static int	is_edit;
CDlurlDlg::CDlurlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlurlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlurlDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlurlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlurlDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlurlDlg, CDialog)
	//{{AFX_MSG_MAP(CDlurlDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlurlDlg message handlers

BOOL CDlurlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT1)->SetWindowText("http://www.shuax.com/");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlurlDlg::OnPaint() 
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
HCURSOR CDlurlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


unsigned long __fastcall base64_encode(char *p_encode, const char *p_src, unsigned long ul_src_len)
{
	static char base64_alphabet[]	=	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	__asm{
		 mov	esi, edx
		 mov	edi, ecx
		 push	edi
		 mov	ecx, ul_src_len
		 mov	ebx, offset base64_alphabet;

aa10:
		 cmp	ecx, 4
		 jb		aa11
		 sub	ecx, 3
		 lodsd
		 dec	esi
		 bswap	eax

		 rol	eax, 6
		 and	al, 63
		 xlat
		 stosb

		 rol	eax, 6
		 and	al, 63
		 xlat
		 stosb

		 rol	eax, 6
		 and	al, 63
		 xlat
		 stosb

		 rol	eax, 6
		 and	al, 63
		 xlat
		 stosb

		 jmp	aa10
aa11:
		 mov	edx, ecx
aa20:
		 dec	ecx
		 jl		aa21
		 lodsb
		 shl	eax, 8
		 jmp	aa20
aa21:
		 mov	ecx, edx
		 sub	ecx, 3
		 neg	ecx
		 mov	edx, ecx
aa30:
		 dec	ecx
		 jl		aa31
		 shl	eax, 8
		 jmp	aa30
aa31:
		 rol	eax, 6
		 and	al, 63
		 xlat
		 stosb

		 rol	eax, 6
		 and	al, 63
		 xlat
		 stosb

		 rol	eax, 6
		 and	al, 63
		 xlat
		 stosb

		 rol	eax, 6
		 and	al, 63
		 xlat
		 stosb

		 sub	edi, edx
		 mov	al, '='
		 mov	ecx, edx
		 repne	stosb
		 mov	al, 0
		 stosb
		 pop	eax
		 sub	eax, edi
		 neg	eax
		 dec	eax
	}
}

unsigned long __fastcall base64_decode(char *p_decode, const char *p_src, unsigned long ul_src_len)
{
	__asm{
		 mov	esi, edx
		 mov	edi, ecx
		 mov	ebx, ecx
		 mov	ecx, ul_src_len
		 shr	ecx, 2

aa10:
		 dec	ecx
		 jl		aa18
		 xor	edx, edx
		 lodsd

aa11:		 
		 //	A..Z
		 cmp	al, 'A'
		 jb		aa13		//	0..9 , + , /
		 cmp	al, 'Z'
		 jg		aa12		//	a..z
		 sub	al, 'A'
		 jmp	aa17		//	success

aa12:	//	a..z
		 cmp	al, 'a'
		 jb		aa16		//	error: bad symbol
		 cmp	al, 'z'
		 jg		aa16		//	error: bad symbol
		 sub	al, 'a' - 'Z' + 'A' - 1
		 jmp	aa17		//	success

aa13:	//	0..9
		 cmp	al, '0'
		 jb		aa14		//	+ , /
		 cmp	al, '9'
		 jg		aa16		//	error: bad symbol
		 sub	al, '0' - 'Z' + 'A' - 'z' + 'a' - 2;
		 jmp	aa17		//	success

aa14:	//	+ , /
		 cmp	al, '+'
		 jnz	aa15		//	/
		 mov	al, 62
		 jmp	aa17		//	success
aa15:
		 cmp	al, '/'
		 jnz	aa16		//	error: bad symbol
		 mov	al, 63
		 jmp	aa17		//	success
aa16:
		 xor	ecx, ecx

aa20:
		 dec	ecx
		 shl	edx, 6
		 shr	eax, 8
		 jnz	aa20		 
		 shl	edx, 2
		 mov	eax, edx
		 bswap	eax
		 stosd
		 sub	ebx, edi
		 neg	ebx
		 lea	ebx, [ebx + ecx - 1]
		 
		 jmp	aa19


aa17:	//	symbol was decoding
		 or		dl, al
		 shl	edx, 6
		 shr	eax, 8
		 jnz	aa11

		 shl	edx, 2
		 bswap	edx
		 mov	eax, edx

		 stosd
		 dec	edi
		 jmp	aa10
aa18:
		 sub	ebx, edi
		 neg	ebx	
aa19:
		 mov	eax, ebx
	}
}

void CDlurlDlg::OnChangeEdit1() 
{
	if (is_edit) return;
	is_edit	=	true;
	char src[1024];
	char des[1024];
	char temp[1024];
	int len=GetDlgItem(IDC_EDIT1)->GetWindowTextLength()+1;
	GetDlgItem(IDC_EDIT1)->GetWindowText(src, len);
	
	//迅雷处理
	strcpy(temp,"AA");
	strcat(temp,src);
	strcat(temp,"ZZ");
	len=base64_encode(des,temp, strlen(temp));
	des[len]='\0';
	strcpy(temp,"thunder://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT2)->SetWindowText(temp);
	//快车处理
	strcpy(temp,"[FLASHGET]");
	strcat(temp,src);
	strcat(temp,"[FLASHGET]");
	len=base64_encode(des,temp, strlen(temp));
	des[len]='\0';
	strcpy(temp,"flashget://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT3)->SetWindowText(temp);
	//旋风处理
	len=base64_encode(des,src, strlen(src));
	des[len]='\0';
	strcpy(temp,"qqdl://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT4)->SetWindowText(temp);
	//RayFile处理
	strrev(src);
	src[strlen(src)-7]='\0';
	strrev(src);
	len=base64_encode(des,src, strlen(src));
	des[len]='\0';
	strcpy(temp,"fs2you://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT5)->SetWindowText(temp);

	is_edit	=	false;
}

void CDlurlDlg::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	if (is_edit) return;
	is_edit	=	true;
	char src[1024];
	char des[1024];
	char temp[1024];
	int len=GetDlgItem(IDC_EDIT2)->GetWindowTextLength()+1;
	GetDlgItem(IDC_EDIT2)->GetWindowText(src, len);

	//原始地址
	strrev(src);
	src[strlen(src)-10]='\0';
	strrev(src);
	len=base64_decode(des,src, len);
	des[len-2]='\0';
	strrev(des);
	des[len-4]='\0';
	strrev(des);
	GetDlgItem(IDC_EDIT1)->SetWindowText(des);
	strcpy(src,des);
	//快车处理
	strcpy(temp,"[FLASHGET]");
	strcat(temp,src);
	strcat(temp,"[FLASHGET]");
	len=base64_encode(des,temp, strlen(temp));
	des[len]='\0';
	strcpy(temp,"flashget://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT3)->SetWindowText(temp);
	//旋风处理
	len=base64_encode(des,src, strlen(src));
	des[len]='\0';
	strcpy(temp,"qqdl://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT4)->SetWindowText(temp);
	//RayFile处理
	strrev(src);
	src[strlen(src)-7]='\0';
	strrev(src);
	len=base64_encode(des,src, strlen(src));
	des[len]='\0';
	strcpy(temp,"fs2you://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT5)->SetWindowText(temp);
	
	is_edit	=	false;
}

void CDlurlDlg::OnChangeEdit3() 
{
	if (is_edit) return;
	is_edit	=	true;
	char src[1024];
	char des[1024];
	char temp[1024];
	int len=GetDlgItem(IDC_EDIT3)->GetWindowTextLength()+1;
	GetDlgItem(IDC_EDIT3)->GetWindowText(src, len);

	//原始地址
	int i;
	for (i=0;src[i]!=0;i++)
		if (src[i]=='&') src[i]='\0';
	strrev(src);
	src[strlen(src)-11]='\0';
	strrev(src);
	len=base64_decode(des,src, len);
	des[len-10]='\0';
	strrev(des);
	des[len-20]='\0';
	strrev(des);
	GetDlgItem(IDC_EDIT1)->SetWindowText(des);
	strcpy(src,des);
	//迅雷处理
	strcpy(temp,"AA");
	strcat(temp,src);
	strcat(temp,"ZZ");
	len=base64_encode(des,temp, strlen(temp));
	des[len]='\0';
	strcpy(temp,"thunder://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT2)->SetWindowText(temp);
	//旋风处理
	len=base64_encode(des,src, strlen(src));
	des[len]='\0';
	strcpy(temp,"qqdl://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT4)->SetWindowText(temp);
	//RayFile处理
	strrev(src);
	src[strlen(src)-7]='\0';
	strrev(src);
	len=base64_encode(des,src, strlen(src));
	des[len]='\0';
	strcpy(temp,"fs2you://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT5)->SetWindowText(temp);
	
	is_edit	=	false;
}

void CDlurlDlg::OnChangeEdit4() 
{
	if (is_edit) return;
	is_edit	=	true;
	char src[1024];
	char des[1024];
	char temp[1024];
	int len=GetDlgItem(IDC_EDIT4)->GetWindowTextLength()+1;
	GetDlgItem(IDC_EDIT4)->GetWindowText(src, len);

	//原始地址
	strrev(src);
	src[strlen(src)-7]='\0';
	strrev(src);
	len=base64_decode(des,src, len);
	GetDlgItem(IDC_EDIT1)->SetWindowText(des);
	strcpy(src,des);
	//迅雷处理
	strcpy(temp,"AA");
	strcat(temp,src);
	strcat(temp,"ZZ");
	len=base64_encode(des,temp, strlen(temp));
	des[len]='\0';
	strcpy(temp,"thunder://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT2)->SetWindowText(temp);
	//快车处理
	strcpy(temp,"[FLASHGET]");
	strcat(temp,src);
	strcat(temp,"[FLASHGET]");
	len=base64_encode(des,temp, strlen(temp));
	des[len]='\0';
	strcpy(temp,"flashget://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT3)->SetWindowText(temp);
	//RayFile处理
	strrev(src);
	src[strlen(src)-7]='\0';
	strrev(src);
	len=base64_encode(des,src, strlen(src));
	des[len]='\0';
	strcpy(temp,"fs2you://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT5)->SetWindowText(temp);
	
	is_edit	=	false;
	
}

void CDlurlDlg::OnChangeEdit5() 
{
	if (is_edit) return;
	is_edit	=	true;
	char src[1024];
	char des[1024];
	char temp[1024];
	int len=GetDlgItem(IDC_EDIT5)->GetWindowTextLength()+1;
	GetDlgItem(IDC_EDIT5)->GetWindowText(src, len);

	//原始地址
	strrev(src);
	src[strlen(src)-9]='\0';
	strrev(src);
	len=base64_decode(des,src, len);
	strcpy(temp,"http://");
	strcat(temp,des);
	strcpy(des,temp);
	int i;
	for (i=0;des[i]!=0;i++)
		if (des[i]=='|') des[i]='\0';
	GetDlgItem(IDC_EDIT1)->SetWindowText(des);
	strcpy(src,des);
	//迅雷处理
	strcpy(temp,"AA");
	strcat(temp,src);
	strcat(temp,"ZZ");
	len=base64_encode(des,temp, strlen(temp));
	des[len]='\0';
	strcpy(temp,"thunder://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT2)->SetWindowText(temp);
	//快车处理
	strcpy(temp,"[FLASHGET]");
	strcat(temp,src);
	strcat(temp,"[FLASHGET]");
	len=base64_encode(des,temp, strlen(temp));
	des[len]='\0';
	strcpy(temp,"flashget://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT3)->SetWindowText(temp);
	//旋风处理
	len=base64_encode(des,src, strlen(src));
	des[len]='\0';
	strcpy(temp,"qqdl://");
	strcat(temp,des);
	GetDlgItem(IDC_EDIT4)->SetWindowText(temp);
	
	is_edit	=	false;
	
}

void CDlurlDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	char src[1024];
	int len=GetDlgItem(IDC_EDIT1)->GetWindowTextLength()+1;
	GetDlgItem(IDC_EDIT1)->GetWindowText(src, len);
	ShellExecute(0, NULL, src, NULL, NULL, SW_SHOWMAXIMIZED);
}
BOOL CDlurlDlg::PreTranslateMessage(MSG* pMsg) 
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
