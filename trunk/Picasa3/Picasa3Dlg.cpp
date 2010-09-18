// Picasa3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Picasa3.h"
#include "Picasa3Dlg.h"
//Vista Aero
typedef struct _MARGINS
{
	int cxLeftWidth;
	int cxRightWidth;
	int cyTopHeight;
	int cyBottomHeight;
}MARGINS, *PMARGINS;
MARGINS mar = { -1};
typedef BOOL (WINAPI *MYFUN)(HWND, MARGINS*);
MYFUN DwmExtendFrameIntoClientArea;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BIF_NEWDIALOGSTYLE 0x0040
wchar_t title[]=L"Picasa3�����";
/////////////////////////////////////////////////////////////////////////////
// CPicasa3Dlg dialog

CPicasa3Dlg::CPicasa3Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CPicasa3Dlg::IDD, pParent)
{
	
	//{{AFX_DATA_INIT(CPicasa3Dlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPicasa3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicasa3Dlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPicasa3Dlg, CDialog)
//{{AFX_MSG_MAP(CPicasa3Dlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicasa3Dlg message handlers

BOOL CPicasa3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	//
	OutputDebugString("�濡�� 2010��5��23��");

	SetWindowTextW(this->GetSafeHwnd(),title);
    isAero=false;
	
    HINSTANCE hFuncInst = LoadLibrary("dwmapi.DLL");
    if (hFuncInst != NULL)
    {
        DwmExtendFrameIntoClientArea = (MYFUN)GetProcAddress(hFuncInst, "DwmExtendFrameIntoClientArea");
        DwmExtendFrameIntoClientArea(this->GetSafeHwnd(), &mar);
        isAero=true;
        FreeLibrary(hFuncInst);
    }
	OnPaint();
	return TRUE;
}

void CPicasa3Dlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	Graphics graphics(dc); 
	if (isAero) graphics.Clear(Color.Black);
	else graphics.Clear(Color(255, 250, 250, 250));
	
	static bool run = false;
	
	static Bitmap CacheImage(rcClient.Width(),rcClient.Height());
	if(!run){
		Graphics buffer(&CacheImage);
		buffer.SetSmoothingMode(SmoothingModeAntiAlias);
		buffer.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		buffer.SetTextRenderingHint(TextRenderingHintAntiAlias);
		
		Image  *logo;
		Image  *button;
		ImageFromIDResource(IDR_PNG1,"png",logo);
		ImageFromIDResource(IDR_PNG2,"png",button);
		buffer.DrawImage(button, 160,288);
		buffer.DrawImage(button, 365,288);
		buffer.DrawImage(logo, -5,10);
		
		static wchar_t welcome[]=L"��ӭʹ�� Picasa3.8 �����";
		static wchar_t comment1[]=L"����PicasaԭΪ�����շѵ�ͼ����������������������ۻ���������\n�ḻ��������Google�չ�����Ϊ����������Ϊ��Google��һ���֡���\n���԰������ڼ�����������ҵ����޸ĺ͹�������ͼƬ��Picasaͻ������\n��������Ӳ���е���ƬͼƬ���ٶȺܿ졣��ֵ��˵���������ٶȺܿ죬��\nȫ������ΪϵͳĬ�ϵ�ͼƬ�鿴����\n֧��ͼƬ���ͣ�jpg��bmp��gif��png��psd��tif��";
		static wchar_t comment2[]=L"��������������Ӧһ�����ѵĽ������ġ���˵������ֻ����Picasa�����ȫ�����ͼƬ�Ĺ��ܣ�\n��û�����־���棿����ʵ���еģ�һ��ֻ��ͼƬ�鿴���������ֻ������û������- -��ֻ������\n�������Ǹ������þ�û�����ˣ����һ���Ӣ�İ棬�����Ҿ�ֱ�Ӹ����������ˣ�ֻ����\"PicasaP\nhotoViewer.exe\"��\"/runtime/slingshot/respack.yt\"�����ļ�����ȻĿ¼λ��Ҫ���ֲ��䣬Ȼ��\n�����ļ��������ɡ���˵������Picasa3��װ���Ѿ�80��M�ˣ������ͼ���߲�3M��һ��㣬Ҳ��\n�ܶ��˶���ϲ�������ܣ�ֻ�������ͼ�����أ�������(*^__^*) ��˵����ô��ϻ�������������\n�����Լ�������һ������棬Ȼ��һ������ѧ����NSIS���ͼ������ó���MFC���ɡ�\n\n�汾��Picasa 3.8\n���ڣ�2010.08.18";
		static wchar_t btn1[] = L"��װ";
		static wchar_t btn2[] = L"ж��";
		
		FontFamily fontFamily(isAero?L"΢���ź�":L"����");
		StringFormat strformat;
		SolidBrush brush(Color(255,0,0,0));
		int i;
		
		//����
		GraphicsPath title;
		title.AddString(welcome, wcslen(welcome), &fontFamily, FontStyleBold, 25, Gdiplus::Point(150,10), &strformat );
		for(i=2; i<6; ++i)
		{   
			Pen pen(Color(24, 64, 64, 64), (float)i);
			pen.SetLineJoin(LineJoinRound);
			buffer.DrawPath(&pen, &title);
		}
		buffer.FillPath(&brush, &title);
		
		GraphicsPath comment;
		comment.AddString(comment1, wcslen(comment1), &fontFamily, FontStyleRegular, 12.0f, Gdiplus::Point(150,60), &strformat );
		comment.AddString(comment2, wcslen(comment2), &fontFamily, FontStyleRegular, 12.0f, Gdiplus::Point(15,160), &strformat );
		for(i=1; i<3; ++i)
		{   
			Pen pen(Color(32, 128, 128, 128), (float)i);
			pen.SetLineJoin(LineJoinRound);
			buffer.DrawPath(&pen, &comment);
		}
		buffer.FillPath(&brush, &comment);
		
		GraphicsPath path3;
		path3.AddString(btn1, wcslen(btn1), &fontFamily, FontStyleBold, 16, Gdiplus::Point(225,293), &strformat );
		path3.AddString(btn2, wcslen(btn2), &fontFamily, FontStyleBold, 16, Gdiplus::Point(432,293), &strformat );
		Pen pen(Color(128, 80, 80, 80), 1);
		pen.SetLineJoin(LineJoinRound);
		buffer.DrawPath(&pen, &path3);
		buffer.FillPath(&brush, &path3);
		
	}
	run = true;
	graphics.DrawImage(&CacheImage, 0, 0);
	graphics.ReleaseHDC(dc);
	//CDialog::OnPaint();
}

HCURSOR CPicasa3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
BOOL CPicasa3Dlg::ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),sTR); // type
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
	pImg=Gdiplus::Image::FromStream(pstm);
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	return TRUE;
}
BOOL CPicasa3Dlg::ReleaseRes(CString strFileName,WORD wResID,CString strFileType)   
{    
    DWORD   dwWrite=0;         
    HANDLE  hFile = CreateFile(strFileName, GENERIC_WRITE,FILE_SHARE_WRITE,NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);    
    HRSRC   hrsc =  FindResource(NULL, MAKEINTRESOURCE(wResID), strFileType);   
    HGLOBAL hG = LoadResource(NULL, hrsc);   
    DWORD   dwSize = SizeofResource( NULL,  hrsc);   
    WriteFile(hFile,hG,dwSize,&dwWrite,NULL);      
    CloseHandle(hFile);   
    return TRUE;   
}
UINT CPicasa3Dlg::isRect(CPoint pt,CPoint Button)
{
	if (pt.x>Button.x && pt.x<Button.x+168)
	{
		if (pt.y>Button.y && pt.y<Button.y+33)
			return 1;
	}
	return 0;
}
int CALLBACK BrowserCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam,LPARAM lpData) 
{ 
	CWnd cw;
	switch (uMsg) 
	{ 
	case BFFM_INITIALIZED: 
		cw.Attach(hWnd);
		cw.CenterWindow();//����������
		cw.Detach();
		::SendMessage (hWnd, BFFM_SETSELECTION, 1, lpData);//lpData��ʼĿ¼
		break; 
	default: 
		break; 
	} 
	return 0; 
}
void CPicasa3Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint install(160,288);
	CPoint uninstall(365,288);
	
	if (isRect(point,install))
	{
		//�ж��Ƿ��Ѿ���װ
		HKEY hKey;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
		{
			char path[1024];
			unsigned long len=1024;
			if (RegQueryValueEx(hKey, "InstallDir", 0, 0,(unsigned char*)&path, &len) == ERROR_SUCCESS)
			{
				if(MessageBoxW(this->GetSafeHwnd(),L"���Ѿ���װ��Picasa3����棬�㻹Ҫ������",title,MB_YESNO + MB_ICONINFORMATION)==IDNO) return;
			}
		}
		RegCloseKey(hKey);
		wchar_t temppath[1024];
		BROWSEINFOW bi;
		bi.hwndOwner = this->GetSafeHwnd();
		bi.pidlRoot = NULL;
		bi.pszDisplayName = NULL;
		bi.lpszTitle = L"��ѡ��Ҫ��װ Picasa3����� ���ļ���λ��";
		bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
		bi.lpfn = BrowserCallbackProc;
		bi.lParam = (LPARAM)"C:\\Program Files\\";//��ʼ��λ��
		bi.iImage = NULL;
		LPITEMIDLIST   idl =SHBrowseForFolderW(&bi);
		if(idl!=NULL)
		{
			SHGetPathFromIDListW(idl,temppath);
			if(temppath[wcslen(temppath)-1]=='\\') temppath[wcslen(temppath)-1]=0;
			
			//����Picasa3�ļ���
			wchar_t install[1024];
			wsprintfW(install,L"%s\\Picasa3",temppath);
			CreateDirectoryW(install,NULL);
			//����PicasaPhotoViewer.exe
			wsprintfW(install,L"%s\\Picasa3\\PicasaPhotoViewer.exe",temppath);
			ReleaseRes(install,IDR_SHUAX1,"SHUAX");
			//����respack.yt
			wsprintfW(install,L"%s\\Picasa3\\runtime",temppath);
			CreateDirectoryW(install,NULL);
			wsprintfW(install,L"%s\\Picasa3\\runtime\\slingshot",temppath);
			CreateDirectoryW(install,NULL);
			wsprintfW(install,L"%s\\Picasa3\\runtime\\slingshot\\respack.yt",temppath);
			ReleaseRes(install,IDR_SHUAX2,"SHUAX");
			
			//д�밲װλ��
			//HKEY hKey;
			RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0,&hKey, 0);
			if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
			{
				wsprintfW(install,L"%s\\Picasa3\\",temppath);
				char install1[512];
				WideCharToMultiByte(CP_ACP, 0, install, wcslen(install), install1, 512, NULL, NULL);
				RegSetValueEx(hKey, "InstallDir", 0, REG_SZ, (unsigned char *)install1, wcslen(install));
			}
			RegCloseKey(hKey);
			//�ж�ϵͳѡ�����
			OSVERSIONINFO info;   
			info.dwOSVersionInfoSize=sizeof(info);   
			GetVersionEx(&info);   
			if (info.dwMajorVersion<6)
			{
				MessageBoxW(this->GetSafeHwnd(),L"������ʹ�õ���XPϵͳ����������Picasa3�����Դ����ļ��������ܣ���ѡ����Ҫ�����ĸ�ʽ��",title,MB_OK + MB_ICONINFORMATION);
				wsprintfW(install,L"\"%s\\Picasa3\\PicasaPhotoViewer.exe\"",temppath);
				ShellExecuteW(0, NULL,install,L"/config", NULL, SW_SHOWNORMAL);
			}
			else
			{
				if(!Win7Install(temppath)) MessageBoxW(this->GetSafeHwnd(),L"��װ�����з������ִ��󣬳����п������в���ȷ�����ʼ�ղ���ȷ����ϵ�ҡ�",title,MB_OK + MB_ICONINFORMATION);;
				MessageBoxW(this->GetSafeHwnd(),L"������ʹ��Vista����ϵͳ���ڼ����򿪵�Ĭ�ϳ��������У���ѡ����Ҫ�������ļ���ʽ������ѡ��\"���˳�������ΪĬ��ֵ(S)\"��",title,MB_OK + MB_ICONINFORMATION);
				ShellExecuteW(0, NULL,L"control.exe",L"/name Microsoft.DefaultPrograms /page pageDefaultProgram", NULL, SW_SHOWNORMAL);
			}
		}
		return ;
	}
	if (isRect(point,uninstall))
	{
		//�ж��Ƿ��Ѿ���װ
		HKEY hKey;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
		{
			char path[1024];
			unsigned long len=1024;
			if (RegQueryValueEx(hKey, "InstallDir", 0, 0,(unsigned char*)&path, &len) == ERROR_SUCCESS)
			{
				//ɾ���ļ�
				char file[1024];
				strcpy(file,path);
				strcat(file,"PicasaPhotoViewer.exe");
				DeleteFile(file);
				
				strcpy(file,path);
				strcat(file,"runtime\\slingshot\\respack.yt");
				DeleteFile(file);
				
				strcpy(file,path);
				strcat(file,"runtime\\slingshot");
				RemoveDirectory(file);

				strcpy(file,path);
				strcat(file,"runtime");
				RemoveDirectory(file);

				RemoveDirectory(path);
				//ɾ��ע���
				RegDeleteValue(hKey, "InstallDir");
				//�ж�ϵͳѡ�����
				OSVERSIONINFO info;   
				info.dwOSVersionInfoSize=sizeof(info);   
				GetVersionEx(&info);   
				if (info.dwMajorVersion>=6)
				{
					RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\RegisteredApplications", 0, KEY_ALL_ACCESS, &hKey);
					RegDeleteValue(hKey, "Picasa3");
				}
				RegCloseKey(hKey);
				MessageBoxW(this->GetSafeHwnd(),L"Picasa3����� ж����ɣ���л����ʹ�á�",title,MB_OK + MB_ICONINFORMATION);
				return;
			}
		}
		MessageBoxW(this->GetSafeHwnd(),L"����û�а�װ Picasa3����� �أ��Ͽ찲װ�ɡ�",title,MB_OK + MB_ICONINFORMATION);
		return ;
	}
	PostMessage(WM_NCLBUTTONDOWN,(WPARAM)HTCAPTION,(LPARAM)HTCAPTION);
	CDialog::OnLButtonDown(nFlags, point);
}
bool CPicasa3Dlg::Win7Install(wchar_t *temppath)
{
	//д��ע���
	bool result = true;
	wchar_t install[1024];
	
	HKEY hKey;
	
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\RegisteredApplications", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
	{
		char Capabilities[]="SOFTWARE\\Picasa3\\Capabilities";
		RegSetValueEx(hKey, "Picasa3", 0, REG_SZ, (unsigned char *)Capabilities, strlen(Capabilities));
	}
	else result = false;
	
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3\\Capabilities\\FileAssociations", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0,&hKey, 0);
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3\\Capabilities", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
	{
		char Description[]="Picasa ���� Google �ṩ�������Ƭ�޸��������ʹ��Ƭӵ�г�ɫ�ĳ���Ч���� ��ֻ�谴һ�°�ť�Ϳ��Խ��Լ�������Ƭ�����������ѣ�";
		char Name[]="Picasa3 �����";
		RegSetValueEx(hKey, "ApplicationDescription", 0, REG_SZ, (unsigned char *)Description, strlen(Description));
		RegSetValueEx(hKey, "ApplicationName", 0, REG_SZ, (unsigned char *)Name, strlen(Name));
	}else result = false;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3\\Capabilities\\FileAssociations", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
	{
		char Associations[]="PicasaPhotoViewer";
		char File[][6]={".jpg",".jpe",".jpeg",".tif",".tiff",".bmp",".gif",".png",".tga",".raw",".crw",".cr2",".nef",".orf",".dcr",".mrw",".pef",".raf",".srf",".dng",".kdc",".sr2",".arw",".3fr",".x3f",".rw2",".nrw"};
		for(int i=0;i<26;i++)
		{
			RegSetValueEx(hKey, File[i], 0, REG_SZ, (unsigned char *)Associations, strlen(Associations));
		}
	}else result = false;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\PicasaPhotoViewer\\DefaultIcon", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0,&hKey, 0);
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\PicasaPhotoViewer\\Shell\\Open\\Command", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0,&hKey, 0);
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\PicasaPhotoViewer\\DefaultIcon", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
	{
		wsprintfW(install,L"\"%s\\Picasa3\\PicasaPhotoViewer.exe\",-102",temppath);
		char install1[512];
		WideCharToMultiByte(CP_ACP, 0, install, wcslen(install), install1, 512, NULL, NULL);
		RegSetValueEx(hKey, "", 0, REG_SZ, (unsigned char *)install1, wcslen(install));
	}
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\PicasaPhotoViewer\\Shell\\Open\\Command", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
	{
		wsprintfW(install,L"\"%s\\Picasa3\\PicasaPhotoViewer.exe\" \"%%1\"",temppath);
		char install1[512];	 
		WideCharToMultiByte(CP_ACP, 0, install, wcslen(install), install1, 512, NULL, NULL);
		RegSetValueEx(hKey, "", 0, REG_SZ, (unsigned char *)install1, wcslen(install));
		RegCloseKey(hKey);
	}else result = false;
	return result;
}
BOOL CPicasa3Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			//case VK_ESCAPE:
			//return TRUE;
		case VK_RETURN:
			return TRUE; 
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
