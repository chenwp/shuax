#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x0600

#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include <math.h>
TCHAR szClass[] = _T("鼠标增强器 - MouseEnhancer");

#include "settings.cpp"

settings myset;
//定义钩子
HWND g_hook;

NOTIFYICONDATA nid;


#define SWM_EXIT (WM_APP+100)
#define SWM_HELP (WM_APP+101)
#define SWM_AUTO (WM_APP+102)
#define SWM_SETT (WM_APP+103)
//鼠标钩子函数
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static long Time = GetCurrentTime();
    static long clickTime = 0;
    static long releaseTime = 0;

    static long nowTime = 0;
    static long oldTime = GetCurrentTime();
    static short nowDest;
    static short oldDest = 0;

    //鼠标手势
    static POINT startPoint;
    static POINT endPoint;
    static double Distance;
    static int orientation;

    MSLLHOOKSTRUCT *pmouse = (MSLLHOOKSTRUCT *)lParam;
    if (nCode == HC_ACTION)
    {
        switch(wParam)
        {
        case WM_MOUSEWHEEL:
            nowTime = GetCurrentTime() - oldTime;
            oldTime = GetCurrentTime();

            nowDest = (short)HIWORD(pmouse->mouseData);
            if(nowDest!=oldDest) _cprintf("Reverse Roller:\t%d\n",nowTime);
            if(nowDest!=oldDest && nowTime<myset.t_Whl) return 1;
            oldDest = nowDest;

            if((GetKeyState(VK_MENU)& 0x8000)!=0 && myset.isVol)
            {
                //ctrl down
                short zDelta = (short)HIWORD(pmouse->mouseData);
                _cprintf("Wheel direction:\t%d\n",zDelta);
                if(zDelta==120)
                {
                    //up
                    SendMessage(WindowFromPoint(pmouse->pt), 793, 197266, 655360);
                }
                else
                {
                    //down
                    SendMessage(WindowFromPoint(pmouse->pt), 793, 197266, 589824);
                }
                break;
            }
            if(myset.isWeh)
            {
                //向鼠标焦点窗口发送滚动信息
                PostMessage(WindowFromPoint(pmouse->pt), WM_MOUSEWHEEL, pmouse->mouseData, MAKELPARAM(pmouse->pt.x, pmouse->pt.y));
                return 1;
            }
            break;
        case WM_RBUTTONDOWN:
            //
            if((GetKeyState(VK_MENU)& 0x8000)!=0 && myset.isGus) startPoint = pmouse->pt;
            if(!myset.isRim) break;
        case WM_LBUTTONDOWN:
            if(!myset.isLem) break;
            if(!myset.isClk) break;
            clickTime = GetCurrentTime() - Time;
            Time = GetCurrentTime();

            if(clickTime<myset.t_Clk)
            {
                _cprintf("ClickTime:\t%d\n",clickTime);
                return 1;//无效单击
            }
            break;
        case WM_RBUTTONUP:
            if((GetKeyState(VK_MENU)& 0x8000)!=0 && myset.isGus)
            {

                endPoint = pmouse->pt;
                int x = endPoint.x-startPoint.x;
                int y = endPoint.y-startPoint.y;
                Distance = sqrt((double)(x*x+y*y));
                if(Distance>30)
                {
                    //
                    orientation = 0;
                    if(x>0&&y<0)
                    {
                        //第一象限
                        y *= -1;
                        if (x>y&&sqrt(3.0)*y<x) orientation = 2;
                        if (x<y&&sqrt(3.0)*x<y) orientation = 1;
                        y *= -1;
                    }
                    else if(x>0&&y>0)
                    {
                        //第四象限
                        if (x>y&&sqrt(3.0)*y<x) orientation = 2;
                        if (x<y&&sqrt(3.0)*x<y) orientation = 3;
                    }
                    else if(x<0&&y>0)
                    {
                        //第三象限
                        x *= -1;
                        if (x>y&&sqrt(3.0)*y<x) orientation = 4;
                        if (x<y&&sqrt(3.0)*x<y) orientation = 3;
                        x *= -1;
                    }
                    else  if(x<0&&y<0)
                    {
                        //第二象限
                        x *= -1;
                        y *= -1;
                        if (x>y&&sqrt(3.0)*y<x) orientation = 4;
                        if (x<y&&sqrt(3.0)*x<y) orientation = 1;
                    }
                     _cprintf("Distance,orientation:\t%f,%d\n",Distance,orientation);
                     //
                     doSomething(myset.g_opr[orientation]);
                }
            }
            if(!myset.isRim) break;
        case WM_LBUTTONUP:
            if(!myset.isLem) break;
            if(!myset.isRls) break;
            releaseTime = GetCurrentTime() - Time;
            if(releaseTime>2&&releaseTime<myset.t_Rls)
            {
                _cprintf("ReleaseTime:\t%d\n",releaseTime);
                return 1;//无效释放
            }
            break;
        }
    }
    return CallNextHookEx((HHOOK)g_hook, nCode, wParam, lParam );
}

void ShowContextMenu(HWND hwnd)
{
    POINT pt;
    GetCursorPos(&pt);
    HMENU hMenu = CreatePopupMenu();
    HMENU hPopMenu;
    int message = WM_APP + 1;
    bool isAutoRun = CheckAutoRun();
    //
    AppendMenu(hMenu, MF_BYPOSITION | (myset.isClk?MF_CHECKED:0), message++, _T("屏蔽无效单击"));
    AppendMenu(hMenu, MF_BYPOSITION | (myset.isRls?MF_CHECKED:0), message++, _T("屏蔽无效释放"));
    AppendMenu(hMenu, MF_BYPOSITION | (myset.isWhl?MF_CHECKED:0), message++, _T("屏蔽无效滚动"));
    hPopMenu = CreatePopupMenu();
    AppendMenu(hPopMenu, MF_BYPOSITION | (myset.isLem?MF_CHECKED:0), message++, L"鼠标左键");
    AppendMenu(hPopMenu, MF_BYPOSITION | (myset.isRim?MF_CHECKED:0), message++, L"鼠标右键");
    AppendMenu(hMenu, MF_POPUP | MF_BYPOSITION, (UINT)hPopMenu, _T("设备"));
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    //
    AppendMenu(hMenu, MF_BYPOSITION | (myset.isWeh?MF_CHECKED:0), message++, _T("滚轮穿透"));
    AppendMenu(hMenu, MF_BYPOSITION | (myset.isVol?MF_CHECKED:0), message++, _T("音量控制"));
    AppendMenu(hMenu, MF_BYPOSITION | (myset.isGus?MF_CHECKED:0), message++, _T("鼠标手势"));
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);

    hPopMenu = CreatePopupMenu();
    AppendMenu(hPopMenu, MF_BYPOSITION | (myset.isIco?0:MF_CHECKED), message++, L"隐藏图标");
    AppendMenu(hPopMenu, MF_BYPOSITION, SWM_SETT, L"手动设置");
    AppendMenu(hPopMenu, MF_BYPOSITION | (isAutoRun?MF_CHECKED:0), SWM_AUTO, L"开机启动");
    AppendMenu(hMenu, MF_POPUP | MF_BYPOSITION, (UINT)hPopMenu, _T("设置"));


    AppendMenu(hMenu, MF_BYPOSITION, SWM_HELP, _T("帮助"));
    AppendMenu(hMenu, MF_BYPOSITION, SWM_EXIT, _T("退出"));
    //Track menu
    SetForegroundWindow(hwnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
    DestroyMenu(hMenu);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, szClass, szClass, WS_POPUPWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    if (!hwnd)
    {
        return FALSE;
    }
    return TRUE;
}
void RunAndWait(TCHAR *path)
{
	STARTUPINFO StartInfo = {sizeof(StartInfo)};
	PROCESS_INFORMATION ProcInfo;
	memset(&ProcInfo, 0, sizeof(ProcInfo));
	TCHAR temp[MAX_PATH];
	wsprintf(temp,_T("notepad.exe \"%s\""),path);
	if (CreateProcess(NULL,temp, NULL, NULL, NULL, NULL, NULL, NULL, &StartInfo, &ProcInfo))
	{
		//   等待这个进程结束
		WaitForSingleObject(ProcInfo.hProcess, INFINITE);
		CloseHandle(ProcInfo.hThread);
		CloseHandle(ProcInfo.hProcess);
	}
	return ;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:

        if(myset.isDbg) AllocConsole();
        myset.save();

        if(!myset.isIco) break;

        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = hwnd;
        nid.uID = 101;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_USER;
        nid.hIcon = LoadIcon(GetModuleHandle(NULL), _T("B"));
        _tcscpy(nid.szTip, szClass);
        Shell_NotifyIcon(NIM_ADD, &nid);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case SWM_AUTO:
            AutoStart(hwnd);
            break;
        case SWM_SETT:
            TCHAR path[MAX_PATH];
            GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
            PathRenameExtension(path, _T(".ini"));
            ShellExecute(0, NULL, path, NULL, NULL, SW_SHOWNORMAL);
            break;
        case SWM_EXIT:
            DestroyWindow(hwnd);
            break;
        case SWM_HELP:
            MessageBox(hwnd,_T("鼠标增强器 v1.0\n\n本软件是免费软件！任何人不能用于盈利。\n更多信息请访问：www.shuax.com"),szClass,MB_OK | MB_ICONINFORMATION);
            InvalidateRect(hwnd, NULL, false);
            break;
        default:
            //
            switch(wParam)
            {
            case WM_APP + 1 :
                myset.isClk = ! myset.isClk;
                break;
            case WM_APP + 2 :
                myset.isRls = ! myset.isRls;
                break;
            case WM_APP + 3 :
                myset.isWhl = ! myset.isWhl;
                break;
            case WM_APP + 4 :
                myset.isLem = ! myset.isLem;
                break;
            case WM_APP + 5 :
                myset.isRim = ! myset.isRim;
                break;
            case WM_APP + 6 :
                myset.isWeh = ! myset.isWeh;
                break;
            case WM_APP + 7 :
                myset.isVol = ! myset.isVol;
                break;
            case WM_APP + 8 :
                myset.isGus = ! myset.isGus;
                break;
            case WM_APP + 9 :
            nid.hWnd = hwnd;
            nid.uID = 101;
            Shell_NotifyIcon(NIM_DELETE, &nid);//删除图标
            myset.isIco = ! myset.isIco;
                break;
            }
            myset.save();
        }
        break;
    case WM_USER:
        switch (lParam)
        {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            ShowContextMenu(hwnd);
            break;
        case WM_LBUTTONDBLCLK:
            //SendMessage(hwnd, WM_COMMAND, SWM_HELP, 0);
            break;
        }
        break;
    case WM_HOTKEY:
    case WM_DESTROY:
        if(myset.isIco)
        {
            nid.hWnd = hwnd;
            nid.uID = 101;
            Shell_NotifyIcon(NIM_DELETE, &nid);//删除图标
        }

        PostQuitMessage (0);
        break;
    default:
        if (message == RegisterWindowMessage(_T("TaskbarCreated")))//Explorer崩溃时重建图标
            SendMessage(hwnd, WM_CREATE, wParam, lParam);
        else
            return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }
    return 0;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style	= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)WndProc;
    wcex.cbClsExtra	= 0;
    wcex.cbWndExtra	= 0;
    wcex.hInstance	= hInstance;
    wcex.hIcon	= LoadIcon (NULL, IDI_APPLICATION);
    wcex.hCursor	= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= szClass;
    wcex.hIconSm	= LoadIcon (NULL, IDI_APPLICATION);

    return RegisterClassEx(&wcex);
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;



    HWND hwnd = FindWindow(szClass, szClass); //单实例运行
    if (hwnd) return 0;

    //注册程序类、窗口
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    //WH_MOUSE_LL
    g_hook = (HWND)SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInstance, 0);
    if (g_hook == NULL)
    {
        MessageBox(0, _T("SetWindowsHookEx Failed!"), szClass, MB_OK);
        return 0;
    }

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //
    UnhookWindowsHookEx((HHOOK)g_hook);
    return msg.wParam;
}
