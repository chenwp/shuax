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
wchar_t title[]=L"Picasa3精简版";
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
	OutputDebugString("舒俊杰 2010年5月23日");

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
		
		static wchar_t welcome[]=L"欢迎使用 Picasa3.8 精简版";
		static wchar_t comment1[]=L"　　Picasa原为独立收费的图像管理、处理软件，其界面美观华丽，功能\n丰富。后来被Google收购并改为免费软件，成为了Google的一部分。它\n可以帮助您在计算机上立即找到、修改和共享所有图片，Picasa突出的优\n点是搜索硬盘中的相片图片的速度很快。更值得说的是启动速度很快，完\n全可以做为系统默认的图片查看器！\n支持图片类型：jpg、bmp、gif、png、psd、tif。";
		static wchar_t comment2[]=L"　　这个精简版是应一个网友的建议做的。他说：“我只想用Picasa里面的全屏浏览图片的功能，\n有没有这种精简版？”其实是有的，一个只有图片查看器的软件，只不过我没告诉他- -。只不过网\n上流传那个精简版好久没更新了，而且还是英文版，所以我就直接告诉他方法了：只保留\"PicasaP\nhotoViewer.exe\"和\"/runtime/slingshot/respack.yt\"两个文件，当然目录位置要保持不变，然后\n设置文件关联即可。据说完整的Picasa3安装好已经80多M了，这个看图工具才3M多一点点，也许\n很多人都不喜欢管理功能，只用这个看图功能呢，比如我(*^__^*) 。说了这么多废话，所以我最终\n决定自己动手做一个精简版，然而一两天又学不会NSIS，就继续用擅长的MFC做吧。\n\n版本：Picasa 3.8\n日期：2010.08.18";
		static wchar_t btn1[] = L"安装";
		static wchar_t btn2[] = L"卸载";
		
		FontFamily fontFamily(isAero?L"微软雅黑":L"黑体");
		StringFormat strformat;
		SolidBrush brush(Color(255,0,0,0));
		int i;
		
		//标题
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
		cw.CenterWindow();//将窗口置中
		cw.Detach();
		::SendMessage (hWnd, BFFM_SETSELECTION, 1, lpData);//lpData初始目录
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
		//判断是否已经安装
		HKEY hKey;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
		{
			char path[1024];
			unsigned long len=1024;
			if (RegQueryValueEx(hKey, "InstallDir", 0, 0,(unsigned char*)&path, &len) == ERROR_SUCCESS)
			{
				if(MessageBoxW(this->GetSafeHwnd(),L"您已经安装了Picasa3精简版，你还要继续吗？",title,MB_YESNO + MB_ICONINFORMATION)==IDNO) return;
			}
		}
		RegCloseKey(hKey);
		wchar_t temppath[1024];
		BROWSEINFOW bi;
		bi.hwndOwner = this->GetSafeHwnd();
		bi.pidlRoot = NULL;
		bi.pszDisplayName = NULL;
		bi.lpszTitle = L"请选择要安装 Picasa3精简版 的文件夹位置";
		bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
		bi.lpfn = BrowserCallbackProc;
		bi.lParam = (LPARAM)"C:\\Program Files\\";//初始化位置
		bi.iImage = NULL;
		LPITEMIDLIST   idl =SHBrowseForFolderW(&bi);
		if(idl!=NULL)
		{
			SHGetPathFromIDListW(idl,temppath);
			if(temppath[wcslen(temppath)-1]=='\\') temppath[wcslen(temppath)-1]=0;
			
			//创建Picasa3文件夹
			wchar_t install[1024];
			wsprintfW(install,L"%s\\Picasa3",temppath);
			CreateDirectoryW(install,NULL);
			//创建PicasaPhotoViewer.exe
			wsprintfW(install,L"%s\\Picasa3\\PicasaPhotoViewer.exe",temppath);
			ReleaseRes(install,IDR_SHUAX1,"SHUAX");
			//创建respack.yt
			wsprintfW(install,L"%s\\Picasa3\\runtime",temppath);
			CreateDirectoryW(install,NULL);
			wsprintfW(install,L"%s\\Picasa3\\runtime\\slingshot",temppath);
			CreateDirectoryW(install,NULL);
			wsprintfW(install,L"%s\\Picasa3\\runtime\\slingshot\\respack.yt",temppath);
			ReleaseRes(install,IDR_SHUAX2,"SHUAX");
			
			//写入安装位置
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
			//判断系统选择操作
			OSVERSIONINFO info;   
			info.dwOSVersionInfoSize=sizeof(info);   
			GetVersionEx(&info);   
			if (info.dwMajorVersion<6)
			{
				MessageBoxW(this->GetSafeHwnd(),L"您正在使用的是XP系统，即将调用Picasa3程序自带的文件关联功能，请选择你要关联的格式。",title,MB_OK + MB_ICONINFORMATION);
				wsprintfW(install,L"\"%s\\Picasa3\\PicasaPhotoViewer.exe\"",temppath);
				ShellExecuteW(0, NULL,install,L"/config", NULL, SW_SHOWNORMAL);
			}
			else
			{
				if(!Win7Install(temppath)) MessageBoxW(this->GetSafeHwnd(),L"安装过程中发生部分错误，程序有可能运行不正确。如果始终不正确请联系我。",title,MB_OK + MB_ICONINFORMATION);;
				MessageBoxW(this->GetSafeHwnd(),L"您正在使用Vista以上系统，在即将打开的默认程序设置中，请选择你要关联的文件格式，建议选择\"将此程序设置为默认值(S)\"。",title,MB_OK + MB_ICONINFORMATION);
				ShellExecuteW(0, NULL,L"control.exe",L"/name Microsoft.DefaultPrograms /page pageDefaultProgram", NULL, SW_SHOWNORMAL);
			}
		}
		return ;
	}
	if (isRect(point,uninstall))
	{
		//判断是否已经安装
		HKEY hKey;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Picasa3", 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
		{
			char path[1024];
			unsigned long len=1024;
			if (RegQueryValueEx(hKey, "InstallDir", 0, 0,(unsigned char*)&path, &len) == ERROR_SUCCESS)
			{
				//删除文件
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
				//删除注册表
				RegDeleteValue(hKey, "InstallDir");
				//判断系统选择操作
				OSVERSIONINFO info;   
				info.dwOSVersionInfoSize=sizeof(info);   
				GetVersionEx(&info);   
				if (info.dwMajorVersion>=6)
				{
					RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\RegisteredApplications", 0, KEY_ALL_ACCESS, &hKey);
					RegDeleteValue(hKey, "Picasa3");
				}
				RegCloseKey(hKey);
				MessageBoxW(this->GetSafeHwnd(),L"Picasa3精简版 卸载完成，感谢您的使用。",title,MB_OK + MB_ICONINFORMATION);
				return;
			}
		}
		MessageBoxW(this->GetSafeHwnd(),L"您还没有安装 Picasa3精简版 呢，赶快安装吧。",title,MB_OK + MB_ICONINFORMATION);
		return ;
	}
	PostMessage(WM_NCLBUTTONDOWN,(WPARAM)HTCAPTION,(LPARAM)HTCAPTION);
	CDialog::OnLButtonDown(nFlags, point);
}
bool CPicasa3Dlg::Win7Install(wchar_t *temppath)
{
	//写入注册表
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
		char Description[]="Picasa 是由 Google 提供的免费照片修改软件，可使照片拥有出色的呈现效果。 您只需按一下按钮就可以将自己最不错的照片分享给亲朋好友！";
		char Name[]="Picasa3 精简版";
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
