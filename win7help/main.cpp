#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>
BOOL ReleaseRes(wchar_t *strFileName,WORD wResID,wchar_t *strFileType)   
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
BOOL IsWow64() 
{ 
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process; 
    BOOL bIsWow64 = FALSE; 
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(L"kernel32"),"IsWow64Process"); 
    if (NULL != fnIsWow64Process) 
    { 
        fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
    } 
    return bIsWow64; 
} 
int main()
{
	wchar_t title[]=L"Windows7帮助文件关联工具";
    OSVERSIONINFO info;   
    info.dwOSVersionInfoSize=sizeof(info);   
    GetVersionEx(&info);   
	if(info.dwMajorVersion!=6||info.dwMinorVersion!=1)
	{
         MessageBox(0,L"对不起，本程序不支持您正在使用的系统。",title,MB_OK | MB_ICONWARNING);
         return 0;                                             
    }
    wchar_t tips[512];
    wsprintf(tips,L"您正在使用的是Windos7 x%d位系统，如果有误请选择“否”退出程序。",IsWow64()?64:86);         
    if(IDNO==MessageBox(0,tips,title,MB_YESNO)) return 0;
    GetTempPath(512,tips);
    wcscat(tips,L"sx.msu");
	ReleaseRes(tips,IsWow64()?64:86,L"MSU");
	wcscat(tips,L"\"");
	wcsrev(tips);
	wcscat(tips,L"\" teiuq/");
	wcsrev(tips);
	//printf("%ls",tips);
    ShellExecute(0, NULL,L"wusa",tips, NULL, SW_SHOWNORMAL);
    MessageBox(0,L"恭喜你，程序安装成功，现在可以打开.hlp文件。\n\n我的博客：www.shuax.com",title,MB_OK | MB_ICONINFORMATION);
}
