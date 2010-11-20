#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x0600
#define _CRT_SECURE_NO_DEPRECATE

#pragma warning(disable:4800)
#pragma warning(disable:4244)

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"comctl32.lib")

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <shlwapi.h>
#include <tlhelp32.h>
#include <PROCESS.H>
#include <mmsystem.h>
#include <commctrl.h>
#include <tchar.h>
#include <conio.h>
#include <math.h>
#include <vector>
using namespace std;

#include "Audioclient.h"
#include "atlbase.h"
#include "Mmdeviceapi.h"
#include "Endpointvolume.h"

#include <GdiPlus.h>
using namespace Gdiplus;

HWND m_hwnd;
HWND f_hwnd;

TCHAR szClass[] = _T("MouseEnhancer v1.61");

#include "resource.h"
#include "Common.h"
#include "Configure.h"
#include "Gesture.h"
#include "Volume.h"

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

	MSLLHOOKSTRUCT *pmouse = (MSLLHOOKSTRUCT *)lParam;
	if (nCode == HC_ACTION)
	{
		switch(wParam)
		{
		case WM_MOUSEWHEEL:
			//修复功能
			nowTime = GetCurrentTime() - oldTime;
			oldTime = GetCurrentTime();

			nowDest = (short)HIWORD(pmouse->mouseData);
			if(nowDest!=oldDest && nowTime<myset.t_Whl) return 1;
			oldDest = nowDest;

			//音量控制功能
			if((GetAsyncKeyState(myset.v_key)& 0x8000)!=0 && myset.isVol && isExclude(WindowFromPoint(pmouse->pt)))
			{
				//ctrl down
				short zDelta = GET_WHEEL_DELTA_WPARAM(pmouse->mouseData);
				for(int i =0 ; i<myset.v_var; i++)
				{
					if(zDelta>0)
					{
						//up
						SendMessage(WindowFromPoint(pmouse->pt), 793, 197266, 655360);
					}
					else
					{
						//down
						SendMessage(WindowFromPoint(pmouse->pt), 793, 197266, 589824);
					}
				}
				//显示OSD、播放音乐、退出消息循环
				if(!myset.v_glb && myset.v_snd) PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_NOWAIT | SND_NODEFAULT);
				if(!myset.v_glb && myset.v_osd) _beginthread(ShowVolume,0,0);
				return 1;
			}
			//向鼠标焦点窗口发送滚动信息
			if(myset.isWeh)
			{
				//获得按键和方向
				int fwKeys = GET_KEYSTATE_WPARAM(pmouse->mouseData);
				int zDelta = GET_WHEEL_DELTA_WPARAM(pmouse->mouseData)*myset.s_lne;
				//修正按键
				if((GetAsyncKeyState(VK_SHIFT) & 0x8000)!=0) fwKeys = MK_SHIFT;
				if((GetAsyncKeyState(VK_CONTROL) & 0x8000)!=0) fwKeys = MK_CONTROL;
				if((GetAsyncKeyState(VK_LBUTTON) & 0x8000)!=0) fwKeys = MK_LBUTTON;
				if((GetAsyncKeyState(VK_RBUTTON) & 0x8000)!=0) fwKeys = MK_RBUTTON;
				if((GetAsyncKeyState(VK_MBUTTON) & 0x8000)!=0) fwKeys = MK_MBUTTON;
				//发送消息，并且退出消息循环
				PostMessage(WindowFromPoint(pmouse->pt), WM_MOUSEWHEEL ,MAKELONG(fwKeys,zDelta), MAKELPARAM(pmouse->pt.x, pmouse->pt.y));
				if(myset.s_act)
				{
					//置顶窗口，并不改变激活状态
					HWND hwnd = WindowFromPoint(pmouse->pt);
					while(GetParent(hwnd))
					{
						hwnd=GetParent(hwnd);
					}						
					SetTopMost(hwnd,true);
					SetTopMost(hwnd,false);
				}
				return 1;
			}
			break;
		case WM_RBUTTONDOWN:
			//
			if( myset.isGus && (GetAsyncKeyState(myset.m_key) & 0x8000)!=0  && isExclude(WindowFromPoint(pmouse->pt)))
			{
				gr.begin(pmouse->pt);
				return 1;
			}
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
			if(myset.isGus && gr.isrun &&  (GetAsyncKeyState(myset.m_key)& 0x8000)!=0)// && isExclude(pmouse->pt)
			{
				if(gr.end()[0])
				{
					//执行鼠标手势
					//keybd_event(myset.m_key, 0, KEYEVENTF_KEYUP,0);
					_beginthread(doSomething,0,gr.end());
				}
				return 1;
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
		case WM_MOUSEMOVE:
			if(myset.isGus && gr.isrun && (GetKeyState(myset.m_key)& 0x8000)!=0)
			{
				gr.add(pmouse->pt);
				//SetCursorPos(pmouse->pt.x,pmouse->pt.y);
				//return 1;
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
	AppendMenu(hMenu, MF_BYPOSITION | (myset.isClk?MF_CHECKED:0) | (myset.isLem?0:(myset.isRim?0:MF_DISABLED)), message++, _T("屏蔽无效单击"));
	AppendMenu(hMenu, MF_BYPOSITION | (myset.isRls?MF_CHECKED:0) | (myset.isLem?0:(myset.isRim?0:MF_DISABLED)), message++, _T("屏蔽无效释放"));
	AppendMenu(hMenu, MF_BYPOSITION | (myset.isWhl?MF_CHECKED:0) | (myset.isLem?0:(myset.isRim?0:MF_DISABLED)), message++, _T("屏蔽无效滚动"));
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

	AppendMenu(hPopMenu, MF_BYPOSITION, SWM_SETT, L"编辑配置文件");
	AppendMenu(hPopMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hPopMenu, MF_BYPOSITION | (myset.isIco?0:MF_CHECKED), message++, L"隐藏图标");
	AppendMenu(hPopMenu, MF_BYPOSITION | (isAutoRun?MF_CHECKED:0), SWM_AUTO, L"开机启动");



	AppendMenu(hMenu, MF_POPUP | MF_BYPOSITION, (UINT)hPopMenu, _T("设置"));


	AppendMenu(hMenu, MF_BYPOSITION, SWM_HELP, _T("帮助"));
	AppendMenu(hMenu, MF_BYPOSITION, SWM_EXIT, _T("退出"));
	//Track menu
	SetForegroundWindow(hwnd);
	TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
	DestroyMenu(hMenu);
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
		//等待这个进程结束
		WaitForSingleObject(ProcInfo.hProcess, INFINITE);
		CloseHandle(ProcInfo.hThread);
		CloseHandle(ProcInfo.hProcess);
	}
	return ;
}

void create(HWND hwnd)
{
	myset.Init();
	myset.save();

	if(myset.v_glb) SetTimer(hwnd, 1, 40, NULL);
	else KillTimer(hwnd,1);
	if(myset.isDbg) AllocConsole();
	else FreeConsole();

	if(!myset.isIco) return;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = 101;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_USER;
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));
	_tcscpy(nid.szTip, szClass);
	Shell_NotifyIcon(NIM_ADD, &nid);
	return;
}
void ManSet(PVOID pvoid)
{
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
	PathRenameExtension(path, _T(".ini"));
	RunAndWait(path);
	create((HWND)pvoid);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int oldv = 0;
	static int newv;
	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case SWM_AUTO:
			AutoStart(hwnd);
			break;
		case SWM_SETT:
			_beginthread(ManSet,0,hwnd);
			break;
		case SWM_EXIT:
			DestroyWindow(hwnd);
			break;
		case SWM_HELP:
			MessageBox(hwnd,_T("MouseEnhancer v1.61\n\n本软件是免费软件，欢迎你的使用！\n更多信息请访问：www.shuax.com"),szClass,MB_OK | MB_ICONINFORMATION);
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
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			ShowContextMenu(hwnd);
			break;
		case WM_LBUTTONDBLCLK:
			//SendMessage(hwnd, WM_COMMAND, SWM_HELP, 0);
			break;
		}
		break;
	case WM_TIMER:
		if(!myset.isVol) break;
		switch(wParam)
		{
		case 1:
			if(oldv==0)
			{
				oldv = GetVolume();
				break;
			}
			newv = GetVolume();
			if(newv!=oldv)
			{
				if(myset.v_osd) _beginthread(ShowVolume,0,NULL);
				if(myset.v_snd) PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_NOSTOP | SND_NODEFAULT);
				oldv = newv;
			}
			break;
		}
		break;
	case WM_HOTKEY:
	case WM_DESTROY:
		if(myset.isIco)
		{
			nid.hWnd = m_hwnd;
			nid.uID = 101;
			Shell_NotifyIcon(NIM_DELETE, &nid);//删除图标
		}
		PostQuitMessage (0);
		break;
	default:
		if (message == RegisterWindowMessage(_T("TaskbarCreated")))//Explorer崩溃时重建图标
			create(hwnd);
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
	wcex.hIcon	= LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor	= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szClass;
	wcex.hIconSm	= LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON2));

	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, szClass, szClass, WS_POPUPWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		return FALSE;
	}
	m_hwnd = hwnd;
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	create(hwnd);
	//SetLayeredWindowAttributes(hwnd, 0, 0, 2);
	return TRUE;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//GDI++
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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
	GdiplusShutdown(gdiplusToken);
	UnhookWindowsHookEx((HHOOK)g_hook);

	return msg.wParam;
}
