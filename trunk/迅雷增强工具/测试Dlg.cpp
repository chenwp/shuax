// 测试Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "测试.h"
#include "测试Dlg.h"
#include <stdio.h>
#include <dwmapi.h>
HINSTANCE hFuncInst;

typedef BOOL (WINAPI *MYFUNC)(HWND,MARGINS*);
typedef BOOL (WINAPI *MYFUN)(BOOL*);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

wchar_t title[]=L"迅雷增强工具 v1.1";
wchar_t success[]=L"恭喜您，设置已经完成。欢迎您访问我的网站寻找更多软件。";
wchar_t error[]=L"文件打开失败或正在使用中，您确认已经关闭了迅雷吗？";
int runing;
CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetWindowTextW(this->GetSafeHwnd(),title);
	OSVERSIONINFO info;   
    info.dwOSVersionInfoSize=sizeof(info);   
    GetVersionEx(&info);   
	if (info.dwMajorVersion<6) version=0;
	else version=1;
	
	path[0]='\0';
	
	//自定义
	strcpy(regkey,"SOFTWARE\\Thunder Network\\ThunderOem\\thunder_backwnd");
	strcpy(insdir,"instdir");
	strcpy(filepath,"\\Program\\XLUser.dll");
	
	
	//玻璃界面
	if (version)
	{
		hFuncInst = LoadLibrary("dwmapi.DLL");
		MYFUN DwmIsCompositionEnabled;
		DwmIsCompositionEnabled = (MYFUN)GetProcAddress(hFuncInst, "DwmIsCompositionEnabled");
		DwmIsCompositionEnabled(&bIsAero);
		if (bIsAero)
		{
			MARGINS mar = {-1};
			MYFUNC DwmExtendFrameIntoClientArea;
			DwmExtendFrameIntoClientArea = (MYFUNC)GetProcAddress(hFuncInst, "DwmExtendFrameIntoClientArea");
			DwmExtendFrameIntoClientArea (this->GetSafeHwnd(), &mar);
		}
		FreeLibrary(hFuncInst);
	}
	return TRUE;
}


void CMyDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	Graphics  graphics(dc); 
	if (version&&bIsAero) graphics.Clear(Color.Black);
	else graphics.Clear(Color.White);
	
	Bitmap CacheImage(rcClient.Width(),rcClient.Height());
	Graphics buffer(&CacheImage);
	buffer.SetSmoothingMode(SmoothingModeAntiAlias);
	buffer.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
	Image  *logo;
	Image  *button;
	ImageFromIDResource(2,"png",logo);
	ImageFromIDResource(1,"png",button);
	buffer.DrawImage(logo, -5,20);
	buffer.DrawImage(button, 165,122);
	buffer.DrawImage(button, 350,122);
	
	FontFamily fontFamily(version?L"微软雅黑":L"宋体");
	StringFormat strformat;
	wchar_t pszbuf[512];
	wsprintfW(pszbuf,L"本程序适用于 迅雷5.9 系列\n如果本软件有错，我概不负责，但我会尽力解决。\n只有极少数时候需要您指定安装目录(比如非官方版)。\n如果会员补丁失效，重新破解即可。\n\n\n请选择：　　　　一键增强　　　　　　　%s取消增强\n\n2010年4月7日更新　　www.shuax.com",version?L"　":L" ");
	GraphicsPath path;
	path.AddString(pszbuf, wcslen(pszbuf), &fontFamily, FontStyleRegular, version?15:16, Gdiplus::Point(version?90:80,version?9:20), &strformat );
	
	Pen pen(Color(18, 255, 255, 255), 3.0);
	//pen.SetLineJoin(LineJoinRound);
	buffer.DrawPath(&pen, &path);
	
	SolidBrush brush(Color(0,0,0));
	buffer.FillPath(&brush, &path);
	graphics.DrawImage(&CacheImage, 0, 0);
	graphics.ReleaseHDC(dc);
}

void CMyDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint Button1(165,122);
	CPoint Button2(350,122);
	if (isRect(point,Button1))
	{
		crack();
	}
	if (isRect(point,Button2)) restore();
	SendMessage(WM_NCLBUTTONDOWN,(WPARAM)HTCAPTION,(LPARAM)HTCAPTION);
	CDialog::OnLButtonDown(nFlags, point);
}
void CMyDlg::getPath()
{
	if (strlen(path)!=0) return;
	DWORD len = 256;
	
	int i = 0;
	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, regkey, 0, KEY_ALL_ACCESS, &hKey);
	if (RegQueryValueEx(hKey, insdir, 0, 0, (unsigned char*)&path, &len) != NULL)
	{
		char dir[MAX_PATH];
		dir[0] = '\0';
		
		BROWSEINFO bi;
		bi.hwndOwner = NULL;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = dir;
		bi.lpszTitle = "请选择迅雷的安装目录";
		bi.ulFlags = 0;
		bi.lpfn = NULL;
		bi.lParam = NULL;
		bi.iImage = NULL;
		
		SHGetPathFromIDList(SHBrowseForFolder(&bi), dir);
		if(strlen(dir)<2)
		{
			MessageBoxW(0,L"你没有正确的选择迅雷安装目录。",title, MB_OK | MB_ICONWARNING);
			path[0]='\0';
			return;
		}
		strcpy(path,dir);
	}
	strcat(path, filepath);
	strcpy(bakpath,path);
	strcat(bakpath,".bak");
}
void CMyDlg::restore()
{
	//获得文件路径
	getPath();
	if (strlen(path)==0) return;
	char dll[512];
	char reg[512];
	strcpy(dll,path);
	dll[strlen(dll)-strlen(filepath)]='\0';
	strcat(dll,"\\ComDlls\\LinkSimulate.dll");
	wsprintf(reg,"/u /s \"%s\"",dll);
	ShellExecute(0, NULL,"regsvr32",reg, NULL, SW_SHOWNORMAL);
	DeleteFile(dll);
	
	FILE *bak;
	if ((bak = fopen(bakpath, "r")) == NULL)
	{
		MessageBoxW(0,L"你还没有备份文件，你确认曾经进行过破解吗？", title, MB_OK | MB_ICONWARNING);
		return;
	}
	fclose(bak);
	
	if (DeleteFile(path) == 0)
	{
		MessageBoxW(0,error, title, MB_OK | MB_ICONWARNING);
		path[0]='\0';
		return;
	}
	CopyFile(bakpath, path, 1);
	DeleteFile(bakpath);
	MessageBoxW(0,success,title, MB_OK | MB_ICONINFORMATION);
}
void CMyDlg::LinkSimulate2()
{

	MessageBoxW(0,success,title, MB_OK | MB_ICONINFORMATION);
}
void CMyDlg::crack()
{
	//获得文件路径
	getPath();
	if (strlen(path)==0) return;
	FILE *fp;
	if ((fp = fopen(path, "rb+")) == NULL)
	{
		MessageBoxW(0,error, title, MB_OK | MB_ICONWARNING);
		path[0]='\0';
		return;
	}
	if (!CopyFile(path, bakpath, 1))
	{
		if (MessageBoxW(0,L"备份文件已经存在，是否覆盖？建议选择否。", title, MB_YESNO | MB_ICONQUESTION |MB_DEFBUTTON2)==IDYES)
		{
			CopyFile(path, bakpath, 0);
		}
	}
	
	int key[] = {0xB8, 0x04, 0x40, 0x00,0x80, 0xEB, 0x0F};
	long filelen = 100000L;
	long feature=0;
	long point=0;
	int length=200;
	long *seek=(long *)malloc(length*sizeof(long));
	
	int i=0;
    int j=0;
	
	//建立索引 
	fseek(fp, 0L, 0);
	while (ftell(fp) < filelen )
	{
		if (fgetc(fp) == key[0]&&fgetc(fp) == key[1])
        {
            feature++;
            if (feature==length)
            {
                seek = (long *)realloc(seek, (length + 40) * sizeof(long));
				length+=40;
            }
            seek[feature]=ftell(fp);
        }
	}
	//根据索引查找 
	for (i=0;i<feature;i++)
	{
        fseek(fp, seek[i], 0);
        for(j=2; j<7;j++)
        {
			if(fgetc(fp)!=key[j]) break; 
        }
        if (j>6) point=ftell(fp);
    }
	if(point==0)
	{
		fclose(fp);
		MessageBoxW(0,L"读取文件时发生错误，您是否已经进行过破解操作？", title, MB_OK | MB_ICONWARNING);
		return;
	}
	fseek(fp, point, 0);
	//替换数据
	fputc(0xB8, fp);
	fputc(0x01, fp);
	fputc(0x00, fp);
	fputc(0x00, fp);
	fputc(0x00, fp);
	fputc(0x90, fp);
	fputc(0x90, fp);
	
	fclose(fp);
	//

	char dll[512];
	char reg[512];
	getPath();
	if (strlen(path)==0) return;
	strcpy(dll,path);
	dll[strlen(dll)-strlen(filepath)]='\0';
	strcat(dll,"\\ComDlls\\LinkSimulate.dll");
	ReleaseRes(dll,LinkSimulate,"DLL");
	wsprintf(reg,"/s \"%s\"",dll);
	ShellExecute(0, NULL,"regsvr32",reg, NULL, SW_SHOWNORMAL);

	MessageBoxW(0,success,title, MB_OK | MB_ICONINFORMATION);
}

void CMyDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnMouseMove(nFlags, point);
}

UINT CMyDlg::isRect(CPoint pt,CPoint Button)
{
	if (pt.x>Button.x && pt.x<Button.x+120)
	{
		if (pt.y>Button.y && pt.y<Button.y+36)
			return 1;
	}
	return 0;
}
BOOL CMyDlg::ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg)
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
BOOL CMyDlg::ReleaseRes(CString strFileName,WORD wResID,CString strFileType)   
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
