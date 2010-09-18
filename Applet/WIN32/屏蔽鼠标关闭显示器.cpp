#include   "windows.h"

#define   MAX_LOADSTRING   100

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND glhKeyHook;
HWND glhMouseHook;
//HWND hWnd;
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	/*
    if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_MOUSEMOVE:
			break;
		default:
			return 1;
		}
	}
	return CallNextHookEx((HHOOK)glhMouseHook, nCode, wParam, lParam );
	*/
	PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
	return 1;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	 exit(0);
}

int APIENTRY WinMain(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow)
{
	MSG msg;

	strcpy(szTitle, "LOVEXH4EVER-Close");
	strcpy(szWindowClass, "SX-Close v1.0");
 
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
    
	glhKeyHook = (HWND)SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
	glhMouseHook = (HWND)SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	//Ð¶ÔØHook
	UnhookWindowsHookEx((HHOOK)glhMouseHook);
	UnhookWindowsHookEx((HHOOK)glhKeyHook);
	return msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			50, 50, 50, 50, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
    case WM_CREATE:
         PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
         break; 
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
