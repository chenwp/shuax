#define UNICODE
#include <windows.h>

#include "GdiPlus.h" //GDI+
using namespace Gdiplus;
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

//Vista Aero
typedef struct _MARGINS
{
	int cxLeftWidth;
	int cxRightWidth;
	int cyTopHeight;
	int cyBottomHeight;
}MARGINS, *PMARGINS;
MARGINS mar = { -1};
typedef BOOL (WINAPI *MYFUN)(HWND, MARGINS*);
MYFUN DwmExtendFrameIntoClientArea;

wchar_t szClassName[] = L"SX-Color";
wchar_t szTips[][32] ={L"Ctrl+Shift+C暂停鼠标取色",L"点击右键、按下Esc退出程序",L"双击界面打开调色板",L"默认复制HEX值到粘贴板",L"SX-Color v1.2"};
POINT pos[2];
COLORREF clr;
HBITMAP hbmp;
bool isAero;
bool isRun;
bool isCol;

void CopyBoard()
{
    static wchar_t text[12];
    wsprintf(text,L"#%02X%02X%02X",GetRValue(clr),GetGValue(clr),GetBValue(clr));
    HGLOBAL hSetData = NULL;
    LPBYTE lpSetData = NULL;
    INT nTextBytes = (wcslen(text) + 1) * sizeof(WCHAR);
    hSetData = GlobalAlloc(GPTR, nTextBytes);
    lpSetData = (LPBYTE)GlobalLock(hSetData);
    memcpy(lpSetData, text, nTextBytes);

    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, lpSetData);
    CloseClipboard();
    SetProcessWorkingSetSize(GetCurrentProcess(),(size_t)-1,(size_t)-1);
}
int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    if(FindWindow(szClassName,NULL)!=NULL) return 1;
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (hThisInstance, L"A");
    wincl.hIconSm = LoadIcon (hThisInstance, L"A");
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)   GetStockObject   (WHITE_BRUSH);


    if (!RegisterClassEx (&wincl))
        return 0;

    int x=190;
    int y=80;

    hwnd = CreateWindowEx (WS_EX_TOOLWINDOW,szClassName,
           szTips[4],
           WS_CAPTION,
           CW_USEDEFAULT,CW_USEDEFAULT,x,y,
           NULL,NULL,hThisInstance,NULL
           );
    SetWindowPos(hwnd,HWND_TOPMOST,GetSystemMetrics(SM_CXSCREEN)-x-10,10,x,y,SWP_HIDEWINDOW);

    isRun=true;
    isCol=false;
    isAero=false;

    HINSTANCE hFuncInst = LoadLibrary(L"dwmapi.DLL");
    if (hFuncInst != NULL)
    {
        DwmExtendFrameIntoClientArea = (MYFUN)GetProcAddress(hFuncInst, "DwmExtendFrameIntoClientArea");
        DwmExtendFrameIntoClientArea(hwnd, &mar);
        isAero=true;
        FreeLibrary(hFuncInst);
    }
    AnimateWindow(hwnd,200,AW_BLEND);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    while (GetMessage (&messages, NULL, 0, 0))
    {

        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    GdiplusShutdown(gdiplusToken);
    return messages.wParam;
}
void OnPaint(HDC hdc,HWND hwnd)
{
    Graphics graphics(hdc);
    if(isAero)graphics.Clear(0);
    else graphics.Clear(Color(255,255,255));
    static RECT rect;
    GetClientRect(hwnd,&rect);
    Bitmap CacheImage(rect.right-rect.left,rect.bottom-rect.top);
    Graphics buffer(&CacheImage);
    buffer.SetTextRenderingHint(TextRenderingHintAntiAlias);
    FontFamily fontFamily(isAero?L"微软雅黑":L"宋体");
    Font font(&fontFamily,isAero?10:9,FontStyleBold);
    //
    static WCHAR tips[1024];
    wsprintf(tips,L"坐标 %d,%d\nRGB(%d,%d,%d)\n#%02X%02X%02X",pos[1].x,pos[1].y,GetRValue(clr),GetGValue(clr),GetBValue(clr),GetRValue(clr),GetGValue(clr),GetBValue(clr));
    SolidBrush brush(Color(200,0,0,0));
    buffer.DrawString( tips,-1,&font,Gdiplus::PointF(0,0),&brush);
    //
    Bitmap bmp(hbmp,NULL);
    buffer.DrawImage(&bmp,130,0,50,50);
    Pen pen(Color(255,255,180,180));
    buffer.DrawRectangle(&pen,130+23,23,4,4);
    graphics.DrawImage(&CacheImage, 0, 0);
    graphics.ReleaseHDC(hdc);
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static HDC hdc;
static HDC dc;
static HDC mdc;
static PAINTSTRUCT ps;
static CHOOSECOLOR cc;
static COLORREF acrCustClr[16];
static int i=0;

    switch (message)
    {
        case WM_CREATE:
            ZeroMemory(&cc, sizeof(cc));
            cc.lStructSize = sizeof(cc);
            cc.lpCustColors = (LPDWORD)acrCustClr;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            RegisterHotKey(hwnd,1,MOD_CONTROL+MOD_SHIFT,'C');
            SetTimer(hwnd,1,100,NULL);
            SetTimer(hwnd,2,3000,NULL);
            dc=GetWindowDC(NULL);
            mdc = CreateCompatibleDC(dc);
            hbmp = CreateCompatibleBitmap(dc, 25, 25);
            SelectObject(mdc, hbmp);
            break;
        case WM_HOTKEY:
            if(isRun)
            {
                CopyBoard();
                isRun=false;
            }
            else isRun=true;
            break;
        case WM_PAINT:
            hdc = BeginPaint (hwnd, &ps) ;
            OnPaint(hdc,hwnd);
            EndPaint (hwnd, &ps) ;
            break;
        case WM_TIMER:
            switch(wParam)
            {
                case 1:
                    if(!isRun)break;
                    GetCursorPos(&pos[1]);
                    if(pos[1].x==pos[0].x&&pos[1].y==pos[0].y)break;
                    if(WindowFromPoint(pos[1])==hwnd)break;
                    clr=GetPixel(dc, pos[1].x, pos[1].y);
                    pos[0].y=pos[1].y;
                    pos[0].x=pos[1].x;
                    BitBlt(mdc, 0, 0, 25, 25, dc, pos[1].x-12, pos[1].y-12, SRCCOPY);
                    InvalidateRect(hwnd,0,false);
                    break;
                case 2:
                    if(i==5) i=0;
                    SetWindowText(hwnd,szTips[i++]);
                    break;
            }
            break;
        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_ESCAPE:SendMessage(hwnd,WM_DESTROY,wParam,lParam);
            }
            break;
        case WM_LBUTTONDBLCLK:
            if(isCol) break;
            isCol=true;
            cc.rgbResult = clr;
            if(!ChooseColor(&cc))
            {
                isCol=false;
                break;
            }
            clr = cc.rgbResult;
            CopyBoard();
            SelectObject (mdc, (HBRUSH)CreateSolidBrush(clr)) ;
            SelectObject(mdc,  GetStockObject (NULL_PEN));
            Rectangle(mdc,0,0,26,26);
            InvalidateRect(hwnd,0,false);
            isCol=false;
            break;
        case WM_RBUTTONUP:
            SendMessage(hwnd,WM_DESTROY,wParam,lParam);
            break;
        case WM_LBUTTONDOWN:
            SendMessage(hwnd,WM_NCLBUTTONDOWN,(WPARAM)HTCAPTION,(LPARAM)HTCAPTION);
            break;
        case WM_DESTROY:
            AnimateWindow(hwnd,300,AW_HIDE|AW_BLEND);
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
