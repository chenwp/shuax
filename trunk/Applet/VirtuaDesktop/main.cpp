#undef WINVER
#define WINVER 0x600
#define _WIN32_IE 0x600

#include <windows.h>
//#include <conio.h>
//GDI+
#include <GdiPlus.h>
using namespace Gdiplus;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;


//全局变量
//#define DESKTOPNUM 9
#define Height 20*2
#define Width 19
char szClassName[] = "VirtuaDesktop";
#include "lib\VirtualDesktop.cpp"
HINSTANCE hInst;
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

SIZE sizeWindow = {Height*9, Height};
POINT ptSrc = {0, 0};
RECT rct;
BLENDFUNCTION m_Blend;


//程序开始
bool ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * &pImg)
{
	HRSRC hRsrc = ::FindResource (hInst, MAKEINTRESOURCE(nID), sTR);
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	pImg = Gdiplus::Image::FromStream(pstm);
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	return TRUE;
}
int isRect(POINT pt, POINT Button)
{
	if (pt.x > Button.x && pt.x < Button.x + Width)
	{
		if (pt.y > Button.y && pt.y < Button.y + Height / 2)
			return 1;
	}
	return 0;
}
void DrawBarControlStrip(HDC hdc,HWND hWnd)
{
    HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP hBitMap = CreateCompatibleBitmap(hdc, Height * 9, Height);
	SelectObject(mdc, hBitMap);
	Graphics graphics(mdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	//
	Image *remove;
	Image *add;
	Image *next;
	Image *hide;
	ImageFromIDResource(3, "png", remove);
	ImageFromIDResource(4, "png", add);
	ImageFromIDResource(5, "png", next);
	ImageFromIDResource(6, "png", hide);
	graphics.DrawImage(remove, 5*Height / 2, 0);
	graphics.DrawImage( add, 6*Height / 2, 0);
	graphics.DrawImage(next, 7*Height / 2, 0);
	graphics.DrawImage(hide, 8*Height / 2, 0);
	//
	Image  *button;
	Image  *button2;
	ImageFromIDResource(1, "png", button);
	ImageFromIDResource(2, "png", button2);
	FontFamily fontFamily(L"Verdana");
	Font font(&fontFamily, 10, FontStyleBold, UnitPixel);
	SolidBrush brush2(Color(255, 255, 255));
	for (int i = 0; i < DESKTOPNUM; i++)
	{
		if (i == now_desk_num) graphics.DrawImage(button2, (i + 9 - DESKTOPNUM)*Height / 2, Height / 2);
		else graphics.DrawImage(button, (i + 9 - DESKTOPNUM)*Height / 2, Height / 2);
		char tips[] = "100";
		tips[0] = i + '1';
		tips[1] = tips[2] = tips[3] = 0;
		graphics.DrawString((wchar_t *)tips, -1, &font, PointF(4.5 + (i + 9 - DESKTOPNUM)*Height / 2, 3 + Height / 2), &brush2);
	}
	m_Blend.BlendOp = 0;
	m_Blend.BlendFlags = 0;
	m_Blend.AlphaFormat = 1;
	m_Blend.SourceConstantAlpha = 255;
	GetWindowRect(hWnd, &rct);
	POINT ptWinPos = {rct.left, rct.top};
	UpdateLayeredWindow(hWnd, hdc, &ptWinPos, &sizeWindow, mdc, &ptSrc, 0, &m_Blend, 2);
}
void VirtualDesktop(WPARAM wParam,HWND hWnd)
{
	ContorlVirtualDesktop(wParam,hWnd);
}
void SwitchDesktop2(WPARAM wParam,HWND hWnd)
{
	if (DESKTOPNUM==1) return;
	ContorlVirtualDesktop((now_desk_num+DESKTOPNUM+1)%DESKTOPNUM,hWnd);
}
void AddDesktop(WPARAM wParam,HWND hWnd)
{
	if (DESKTOPNUM<9)
	{
		//message_map[vStart+DESKTOPNUM].show=1;
		DESKTOPNUM++;
		InvalidateRect(hWnd, NULL, false);
	}
}
void RemoveDesktop(WPARAM wParam,HWND hWnd)
{
	if (DESKTOPNUM>1)
	{
		DESKTOPNUM--;
		if(DESKTOPNUM==now_desk_num) ContorlVirtualDesktop(now_desk_num-1,hWnd);
		InvalidateRect(hWnd, NULL, false);
	}
}
void BarMenu(LPARAM lParam,HWND hWnd)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	for (int i = 0; i < DESKTOPNUM; i++)
	{
		POINT button;
		button.x = (i + 9 - DESKTOPNUM) * Height / 2;
		button.y = Height / 2;
		if (isRect(pt, button))
		{
			//_cprintf("%d\n",i);
			ContorlVirtualDesktop(i,hWnd);
		}
	}
	POINT button;
	button.y = 0;

	button.x = 5 * Height / 2;
	if (isRect(pt, button)) RemoveDesktop(0,hWnd);

	button.x = 6 * Height / 2;
	if (isRect(pt, button)) AddDesktop(0,hWnd);

	button.x = 7 * Height / 2;
	if (isRect(pt, button)) SwitchDesktop2(0,hWnd);

	button.x = 8 * Height / 2;
	if (isRect(pt, button)) SendMessage(hWnd,WM_DESTROY,0,0);
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    //AllocConsole();

    //单实例
    hwnd=FindWindow(szClassName, szClassName);
	if (hwnd)
	{
		return 0;
	}

    MessageBox(0,"仅供测试！点击右键退出程序。\n上面按钮：减少桌面，增加桌面，切换桌面，退出程序，下面按钮为直接选择。","",0);
    MSG messages;
    WNDCLASSEX wincl;

    hInst = hThisInstance;
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
           WS_EX_TOOLWINDOW,szClassName,szClassName,WS_POPUPWINDOW,
           CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
           HWND_DESKTOP,NULL,hThisInstance,NULL
           );

    //ShowWindow (hwnd, nCmdShow);
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | 0x80000);
    SetWindowPos(hwnd, HWND_TOPMOST, GetSystemMetrics(SM_CXSCREEN) - Height/2*9, GetSystemMetrics(SM_CYMAXIMIZED) - Height - 16, Height/2*9, Height, SWP_SHOWWINDOW);

    InvalidateRect(hwnd, NULL, false);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    GdiplusShutdown(gdiplusToken);
    return messages.wParam;
}

void HotKeyRegister(HWND hwnd)
{
    int x=1;
    for(int i=0;i<=9;i++)
    {
        x*=RegisterHotKey(hwnd, 1000 + i, MOD_ALT, i+'0');
        x*=RegisterHotKey(hwnd, 1000 + i, MOD_ALT, i+VK_NUMPAD0);
    }
    if(x==0)
    {
        MessageBox(0,"部分热键注册失败，可能会影响使用。","",0);
    }
}
void HotKey(WPARAM wParam,HWND hwnd)
{
    wParam-=1000;
    if(wParam==0)
    {
        SwitchDesktop2(0,hwnd);
    }
    if(wParam>0&&wParam<=9)
    {
        ContorlVirtualDesktop(wParam-1,hwnd);
    }
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static  HDC          hdc;
    static  PAINTSTRUCT  ps;

    switch (message)
    {
        case WM_CREATE:
            HotKeyRegister(hwnd);
            break;
        case WM_HOTKEY:
            HotKey(wParam,hwnd);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            DrawBarControlStrip(hdc,hwnd);
            EndPaint(hwnd, &ps);
            break;
        case WM_LBUTTONDOWN:
			BarMenu(lParam,hwnd);
			break;
        case WM_RBUTTONUP:
        case WM_DESTROY:
        	if (now_desk_num != pre_desk_num)
            {
                EnumWindows(EnumWindowsProc, 0);
                int i = 1;
                while (i <= now_desk.d_getnCount())
                    ShowWindow(now_desk.d_read(i++), SW_SHOW);
            }
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
