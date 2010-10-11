#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x0600

#include <windows.h>
#include <tchar.h>
//定义钩子
HWND g_hook;

NOTIFYICONDATA nid;

TCHAR szClass[] = _T("AllWheel - 所有窗口获得滚轮");

//鼠标钩子函数
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT *pmouse = (MSLLHOOKSTRUCT *)lParam;
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_MOUSEWHEEL)
		{
			//short zDelta = (short)HIWORD(pmouse->mouseData);
			PostMessage(WindowFromPoint(pmouse->pt), WM_MOUSEWHEEL, pmouse->mouseData, MAKELPARAM(pmouse->pt.x, pmouse->pt.y));
			return 1;
		}
	}
	return CallNextHookEx((HHOOK)g_hook, nCode, wParam, lParam );
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, szClass, szClass, WS_POPUPWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		return FALSE;
	}

	if (!RegisterHotKey(hwnd, 1000, MOD_WIN, VK_END))
	{
		MessageBox(hwnd, _T("程序退出热键注册失败，你只能通过再次运行本程序退出。"), szClass, MB_OK + MB_ICONERROR);
	}

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = hwnd;
		nid.uID = 101;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = WM_USER;
		nid.hIcon = LoadIcon(GetModuleHandle(NULL), _T("B"));
		_tcscpy(nid.szTip, szClass);
		Shell_NotifyIcon(NIM_ADD, &nid);
		break;
	case WM_USER:
		switch (lParam)
		{
		//case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:SendMessage(hwnd, WM_DESTROY, 0, 0); break;
		}
		break;
	case WM_HOTKEY:
	case WM_DESTROY:
		nid.hWnd = hwnd;
		nid.uID = 101;
		Shell_NotifyIcon(NIM_DELETE, &nid);
		PostQuitMessage (0);
		break;
	default:
		if (message == RegisterWindowMessage(_T("TaskbarCreated")))
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

	HWND hwnd = FindWindow(szClass, szClass);
	if (hwnd)
	{
		SendMessage(hwnd, WM_DESTROY, 0, 0);
		MessageBox(0, _T("现在程序已经停止运行。"), szClass, MB_OK + MB_ICONINFORMATION);
		return 0;
	}

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
