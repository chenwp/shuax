//www.shuax.com
//Êæ¿¡½Ü 2010.01.02
#include "windows.h"
#include "tlhelp32.h"
HINSTANCE hFuncInst;
typedef struct _MARGINS
{
	int cxLeftWidth;
	int cxRightWidth;
	int cyTopHeight;
	int cyBottomHeight;
}
MARGINS, *PMARGINS;
MARGINS mar = { -1};

int length = 200;
int count = 0;
HWND *nHwnd = (HWND *)malloc(length * sizeof(HWND));
typedef BOOL (WINAPI *MYFUNC)(HWND, MARGINS*);
MYFUNC DwmExtendFrameIntoClientArea;

char szClass[] = "GlassCMD";
HWND hwnd;
ATOM	MyRegisterClass(HINSTANCE hInstance);
BOOL	InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwID ;
	GetWindowThreadProcessId(hwnd, &dwID) ;
	if (dwID == (DWORD)lParam && IsWindowVisible(hwnd) && GetParent(hwnd) == NULL)
	{
		int i;
		int flag = 1;
		for (i = 0; i < count; i++)
		{
			if (nHwnd[i] == hwnd) {flag = 0; break;}
		}
		if (flag)
		{
			count++;
			if (count == length)
			{
				nHwnd = (HWND *)realloc(nHwnd, (length + 40) * sizeof(HWND));
				length += 40;
			}
			nHwnd[count] = hwnd;
			DwmExtendFrameIntoClientArea (hwnd, &mar);
		}
	}
	return true;
}
void StartGlassCMD()
{
	PROCESSENTRY32 pe32;
	HANDLE hProcessSnap = NULL;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
	for (Process32First (hProcessSnap, &pe32); Process32Next (hProcessSnap, &pe32);)
	{
		if (stricmp(pe32.szExeFile, "cmd.exe") == 0)
		{
			EnumWindows((WNDENUMPROC)EnumWndProc, pe32.th32ProcessID);
		}
	}
	CloseHandle(hProcessSnap);

}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	hFuncInst = LoadLibrary("dwmapi.DLL");
	if (!hFuncInst)
	{
		MessageBox(0, "This program does not support the system!", "GlassCMD", MB_OK + MB_ICONINFORMATION);
		return 0;
	}
	DwmExtendFrameIntoClientArea = (MYFUNC)GetProcAddress(hFuncInst, "DwmExtendFrameIntoClientArea");

	hwnd=FindWindow(szClass, szClass);
	if (hwnd)
	{
		SendMessage(hwnd,WM_DESTROY,0,0);
		return 0;
	}

	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, szClass, szClass, WS_POPUPWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		return FALSE;
	}

	RegisterHotKey(hwnd, 10000, MOD_WIN, VK_END);
	StartGlassCMD();
    SetTimer(hwnd, 1, 500, NULL);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_HOTKEY:
		SendMessage(hwnd,WM_DESTROY,0,0);
		break;
	case WM_DESTROY:
		for (int i = 0; i < count; i++)
		{
			MARGINS mar2 = { 0};
			DwmExtendFrameIntoClientArea (nHwnd[i], &mar2);
		}
		PostQuitMessage (0);
		break;
	case WM_TIMER:
		StartGlassCMD();
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}
	return 0;
}
