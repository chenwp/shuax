#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0500
#define _WIN32_IE 0x0600

#include <windows.h>
#include <tchar.h>

TCHAR szClassName[] = _T("CloseSimilar");

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	if (GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE)//任务栏可见窗口
	{
		TCHAR buff[256];
		GetClassName(hwnd, buff, 255);
		//
		if (lstrcmp(buff, (TCHAR *)lParam) == 0)//比较类名
		{
			//DestroyWindow(hwnd);
			if (!GetParent(hwnd))PostMessage(hwnd, WM_CLOSE, 0, 0);
		}
	}
	return 1;
}
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_HOTKEY:
		switch (wParam)
		{
		case 1000:
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			break;
		case 1001:
			TCHAR buff[256];
			GetClassName(GetForegroundWindow(), buff, 255); //获取类名
			EnumWindows(EnumWindowsProc, LPARAM(buff));
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage (0);
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

	hwnd = FindWindow(szClassName, szClassName);
	if (hwnd)
	{
		SendMessage(hwnd, WM_DESTROY, 0, 0);
		MessageBox(0, _T("现在程序已经停止运行。"), szClassName, MB_OK + MB_ICONINFORMATION);
		return 0;
	}

	MSG messages;
	WNDCLASSEX wincl;

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof (WNDCLASSEX);

	wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

	if (!RegisterClassEx (&wincl))
		return 0;

	hwnd = CreateWindowEx (
				WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
				szClassName,
				szClassName,
				WS_POPUP, 0, 0, 0, 0,
				NULL, NULL, hThisInstance, NULL
			);

	//ShowWindow (hwnd, nCmdShow);


	if (!RegisterHotKey(hwnd, 1000, MOD_WIN, VK_END))
	{
		MessageBox(hwnd, _T("程序退出热键注册失败，你只能通过再次运行本程序退出。"), szClassName, MB_OK + MB_ICONERROR);
	}
	if (!RegisterHotKey(hwnd, 1001, MOD_WIN, VK_F4))
	{
		MessageBox(hwnd, _T("程序功能热键Win+F4注册失败，不能继续运行！"), szClassName, MB_OK + MB_ICONSTOP);
		return 0;
	}

	while (GetMessage (&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	return messages.wParam;
}
