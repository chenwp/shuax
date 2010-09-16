#define UNICODE
#define _UNICODE
#include <windows.h>
#include <tlhelp32.h>

#include "lib\xmlParser.cpp"
XMLNode xMainNodeSet;
XMLNode xMainNodeData;

#include "lib\midi.h"
#include <conio.h>
#include "config.h"
#include "function.h"

#include <GdiPlus.h>
using namespace Gdiplus;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;


LOGFONT lf;
config cfg;//获取配置
CMIDI midiplay;
Function fuc;//获取功能
DWORD dwPid = 0;//程序PID
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
HWND hwnd2;
#include "memope.h"
bool ImageFromIDResource(UINT nID, Image *&pImg)
{
    HINSTANCE hInst = ::GetModuleHandle(0);

    HRSRC hRsrc = ::FindResource (hInst, MAKEINTRESOURCE(nID), _T("IMG"));
    if (!hRsrc) return FALSE;

    DWORD len = SizeofResource(hInst, hRsrc);
    BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
    if (!lpRsrc) return FALSE;
    HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);

    BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
    memcpy(pmem, lpRsrc, len);
    IStream* pstm;
    CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
    pImg = Image::FromStream(pstm);

    GlobalUnlock(m_hMem);

    pstm->Release();

    FreeResource(lpRsrc);
    return TRUE;
}
void OnPaint(HDC dc)
{
    Graphics graphics(dc);
    graphics.Clear(Color(255,255,255));
    Bitmap CacheImage(cfg.size.cx,cfg.size.cy);
    Graphics buffer(&CacheImage);
    Image *bg=NULL;
    ImageFromIDResource(1, bg);
    buffer.DrawImage(bg, 0, 0);

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
    if(dwPid==0)
    {
        for(int i=0; i<fuc.count; i++)
        {
            EnableWindow (GetDlgItem(hwnd2,1000+i), FALSE) ;
        }
        midiplay.Continue();
        midiplay.SetVolume(100);
    }
    else
    {
        for(int i=0; i<fuc.count; i++)
        {
            EnableWindow (GetDlgItem(hwnd2,1000+i), TRUE) ;
        }
        midiplay.Pause();
    }
}
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static    HFONT hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("宋体"));
    switch (message)
    {
    case WM_CREATE:
        hwnd2 = hwnd;
        CreateWindow(_T("Button"),_T("关于"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,cfg.about.cx, cfg.about.cy,100, 28,hwnd, (HMENU)(999), ((LPCREATESTRUCT) lParam) -> hInstance, NULL) ;
        for(int i=0; i<fuc.count; i++)
        {
            CreateWindow(_T("Button"),_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,sz.cx, sz.cy+i*25,13, 13,hwnd, (HMENU)(1000+i), ((LPCREATESTRUCT) lParam) -> hInstance, NULL) ;
            RegisterHotKey(hwnd, 1000 + i, fuc.items[i].mod, fuc.items[i].key);
        }
        break;
    case WM_SIZE:
        PostMessage(GetDlgItem(hwnd,999), WM_SETFONT, (WPARAM)hFont, NULL) ;
        OnPaint(GetDC(hwnd));
        break;
    case WM_HOTKEY:
        if(dwPid==0) break;
        PostMessage(GetDlgItem(hwnd,wParam), BM_CLICK, true, NULL) ;
        break;
    case WM_COMMAND:
        if(dwPid==0) break;
        switch (wParam)
        {
        case 999:
            MessageBox(hwnd,cfg.szAbout,cfg.szTitleText,MB_OK + MB_ICONINFORMATION);
            ShellExecute(0, NULL, cfg.szWebsite, NULL, NULL, SW_SHOWNORMAL);
        default:
        {
            int i = wParam-1000;
            if(!fuc.items[i].bEnabled)
            {
                int count = xMainNodeData.getChildNode(_T("功能"), i).nChildNode(_T("修改"));
                for(int j=0; j<count; j++)
                {
                    if(_tcscmp(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("类型")).getText(),_T("数据"))==0)
                    {
                        //数据类型修改
                        DWORD dwBase=GetBase(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("基址")).getText());
                        TCHAR oValue[32];
                        _stprintf(oValue,_T("%d"),GetMemValue(dwBase));
                        if(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).nChildNode(_T("还原数据"))==0)
                        xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).addChild(_T("还原数据")).updateText(oValue);
                        else xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("还原数据")).updateText(oValue);
                        DWORD dwValue;
                        _stscanf(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("内容")).getText(),_T("%d"),&dwValue);
                        SetMemValue(dwBase,dwValue);
                    }
                    else
                    {
                        //代码类型修改
                        DWORD dwBase=GetBase(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("基址")).getText());
                        DWORD CodeLen = GetCodeLen(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("内容")).getText());
                        BYTE oCode[CodeLen];
                        TCHAR oCodeText[128];
                        oCodeText[0]=0;
                        GetMemData(dwBase,(BYTE*&)oCode,sizeof(oCode));
                        for(DWORD n=0;n<CodeLen;n++)
                        {
                            TCHAR temp[16];
                            _stprintf(temp,_T("%02X "),oCode[n]);
                            _tcscat(oCodeText,temp);
                        }
                        oCodeText[_tcslen(oCodeText)-1]=0;
                        if(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).nChildNode(_T("还原数据"))==0)
                        xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).addChild(_T("还原数据")).updateText(oCodeText);
                        else xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("还原数据")).updateText(oCodeText);
                        //
                        TCHAR code[128];
                        _tcscpy(code,xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("内容")).getText());
                        const TCHAR *nptr = code;
                        TCHAR* result;
                        TCHAR temp[32];
                        int x=0;
                        do
                        {
                            result=_tcschr(nptr,' ');
                            _stprintf(temp,_T("%.*s"), result-nptr, nptr);
                            nptr = result+1;
                            _stscanf(temp,_T("%02x"),&oCode[x]);
                            x++;
                        }while(result!=0);
                        SetMemData(dwBase,(BYTE*&)oCode,sizeof(oCode));
                    }
                }
                fuc.items[i].bEnabled=true;
            }
            else
            {
                int count = xMainNodeData.getChildNode(_T("功能"), i).nChildNode(_T("修改"));
                for(int j=0; j<count; j++)
                {
                    if(_tcscmp(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("类型")).getText(),_T("数据"))==0)
                    {
                        //数据类型修改
                        DWORD dwBase=GetBase(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("基址")).getText());
                        DWORD dwValue;
                        _stscanf(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("还原数据")).getText(),_T("%d"),&dwValue);
                        SetMemValue(dwBase,dwValue);
                    }
                    else
                    {
                       //代码类型修改
                        DWORD dwBase=GetBase(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("基址")).getText());
                        DWORD CodeLen = GetCodeLen(xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("内容")).getText());
                        BYTE oCode[CodeLen];
                        TCHAR code[128];
                        _tcscpy(code,xMainNodeData.getChildNode(_T("功能"), i).getChildNode(_T("修改"), j).getChildNode(_T("还原数据")).getText());
                        const TCHAR *nptr = code;
                        TCHAR* result;
                        TCHAR temp[32];
                        int x=0;
                        do
                        {
                            result=_tcschr(nptr,' ');
                            _stprintf(temp,_T("%.*s"), result-nptr, nptr);
                            nptr = result+1;
                            _stscanf(temp,_T("%02x"),&oCode[x]);
                            x++;
                        }while(result!=0);
                        SetMemData(dwBase,(BYTE*&)oCode,sizeof(oCode));
                    }
                }
                fuc.items[i].bEnabled=false;
            }
        }
        break;
        }
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
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    //单实例
    if(cfg.single)
    {
        if (FindWindow(cfg.szClassName, cfg.szTitleText))
        {
            return 0;
        }
    }

    //初始化背景音乐
    midiplay.Create(1,0);//资源ID，资源名一定要 = "MIDI"
    midiplay.SetInfinitePlay(TRUE);//循环
    midiplay.Play(TRUE);//
    midiplay.SetVolume(0);


    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = cfg.szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    //加载图标
    //HICON  icon = ExtractIcon(hThisInstance,cfg.szIconPath,0);

    wincl.hIcon = LoadIcon (hThisInstance, _T("A"));
    wincl.hIconSm = LoadIcon (hThisInstance, _T("A"));
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
