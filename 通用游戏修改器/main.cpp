#define UNICODE
#define _UNICODE
#include <windows.h>
#include <tlhelp32.h>

#include "lib\xmlParser.cpp"
#include <conio.h>
#include "config.h"
#include "function.h"

#include <GdiPlus.h>
using namespace Gdiplus;

LOGFONT lf;
config cfg;//获取配置
Function fuc;//获取功能
DWORD dwPid = 0;//程序PID
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void OnPaint(HDC dc)
{
    Graphics graphics(dc);
    graphics.Clear(Color(255,255,255));
    Bitmap CacheImage(cfg.size.cx,cfg.size.cy);
    Graphics buffer(&CacheImage);
    Image bg(cfg.szImagePath);
    buffer.DrawImage(&bg, 0, 0);

    buffer.SetTextRenderingHint(TextRenderingHintSystemDefault);
    FontFamily fontFamily(_T("宋体"));
    Font font(&fontFamily,9,FontStyleRegular);
    SolidBrush brush(Color(255,0,0,0));
    for(int i=0; i<fuc.count; i++)
    {
        buffer.DrawString(fuc.items[i].szName,-1,&font,Gdiplus::PointF(sz.cx+15, sz.cy+i*25),&brush);
    }

    graphics.DrawImage(&CacheImage, 0, 0);
    graphics.ReleaseHDC(dc);
}
DWORD EnumProcess(TCHAR *Path)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    DWORD Pid = 0;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    for (Process32First (hProcessSnap, &pe32); Process32Next (hProcessSnap, &pe32);)
    {
        if(_tcsicmp(pe32.szExeFile, Path)==0)
        {
            Pid = pe32.th32ProcessID;
            break;
        }
    }
    CloseHandle(hProcessSnap);
    return Pid;
}
void FindProcess()
{
    static HWND hwnd;
    switch(cfg.pro)
    {
    case 0:
        dwPid = EnumProcess(cfg.szProcess);
        break;
    case 1:
        hwnd = FindWindow(NULL, cfg.szProcess);
        if (hwnd)
        {
            GetWindowThreadProcessId(hwnd,&dwPid);
        }
        else dwPid = 0;
        break;
    case 2:
        hwnd = FindWindow(cfg.szProcess, NULL);
        if (hwnd)
        {
            GetWindowThreadProcessId(hwnd,&dwPid);
        }
        else dwPid = 0;
        break;
    }
}
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        AllocConsole();
        for(int i=0; i<fuc.count; i++)
        {
            CreateWindow(_T("Button"),_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,sz.cx, sz.cy+i*25,13, 13,hwnd, (HMENU)(1000+i), ((LPCREATESTRUCT) lParam) -> hInstance, NULL) ;
            RegisterHotKey(hwnd, 1000 + i, 0, VK_F1+i);
        }//0x94CA00C4
        break;
    case WM_SIZE:
        OnPaint(GetDC(hwnd));
        break;
    case WM_HOTKEY:
    _cprintf("%d\n",wParam);
        PostMessage(GetDlgItem(hwnd,wParam), BM_CLICK, true, NULL) ;
	case WM_COMMAND:
		break;
    case WM_ERASEBKGND:
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            FindProcess();
            break;
        case 2:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage (0);
        break;
    case WM_LBUTTONDOWN:
        SendMessage(hwnd,WM_SYSCOMMAND,0xF012,0);
        break;
    default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    //单实例
    if(cfg.single)
    {
        if (FindWindow(cfg.szClassName, cfg.szTitleText))
        {
            return 0;
        }
    }

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = cfg.szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    //加载图标
    HICON  icon = ExtractIcon(hThisInstance,cfg.szIconPath,0);
    if(icon!=NULL)
    {
        wincl.hIcon = icon;
        wincl.hIconSm = icon;
    }
    else
    {
        wincl.hIcon = LoadIcon (hThisInstance, _T("A"));
        wincl.hIconSm = LoadIcon (hThisInstance, _T("A"));
    }
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = CreateSolidBrush(RGB(240,240,240));//(HBRUSH) GetStockObject (WHITE_BRUSH) ;

    if (!RegisterClassEx (&wincl)) return 0;

    hwnd = CreateWindowEx (
               0,
               cfg.szClassName,
               cfg.szTitleText,
               WS_SYSMENU | WS_MINIMIZEBOX,
               CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
               HWND_DESKTOP,NULL,hThisInstance,NULL
           );

    SetWindowPos(hwnd, HWND_TOPMOST, (GetSystemMetrics(SM_CXSCREEN) - cfg.size.cx)/2, (GetSystemMetrics(SM_CYMAXIMIZED) -cfg.size.cy)/2, cfg.size.cx, cfg.size.cy, SWP_SHOWWINDOW);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    OnPaint(GetDC(hwnd));
    FindProcess();
    SetTimer(hwnd, 1, cfg.timer, NULL);
    SetTimer(hwnd, 2, 1000, NULL);
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    GdiplusShutdown(gdiplusToken);
    return messages.wParam;
}
