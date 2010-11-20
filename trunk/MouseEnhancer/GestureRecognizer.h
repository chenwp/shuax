#ifndef _GESTURE_H_
#define _GESTURE_H_

#include <math.h>
#include <vector>
using namespace std;
TCHAR str[][4]= {_T(""),_T("U"),_T("R"),_T("D"),_T("L")};

BOOL CALLBACK ShowMouseProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static class Gesture
{
private:
    POINT now;

    long time;

    TCHAR command[26];
    int count[5];//0-5
    int result;


    int  getDistance(POINT a, POINT b);
    int  getOrientation(POINT a, POINT b);

public:
    Gesture();
    HWND g_hwnd;
    vector <Gdiplus::Point> v_pt;
    bool isrun;
    void begin(POINT pt);
    void add(POINT pt);
    TCHAR* end();
} gr;
Gesture::Gesture()
{

}
void Gesture::begin(POINT pt)
{
    //显示窗口
    CreateDialog (GetModuleHandle(NULL),_T("101"),0,ShowMouseProc);

    isrun = true;
    time = GetCurrentTime();
    now = pt;
    for(int i=0; i<5; i++) count[i]=0;
    command[0]=0;
    result=0;
    v_pt.clear();
}
TCHAR* Gesture::end()
{
    if(GetCurrentTime()-time>1500) command[0]=0;
    isrun = false;
    SendMessage(g_hwnd, WM_CLOSE, 1, 0);
    v_pt.clear();
    command[0]=0;
    return command;
}
void Gesture::add(POINT pt)
{
    if(GetCurrentTime()-time>15000)
    {
        SendMessage(g_hwnd, WM_CLOSE, 1, 0);
        isrun = false;
        return;
    }

    v_pt.push_back(Point(pt.x,pt.y));
    InvalidateRect(g_hwnd, NULL, false);

    if (getDistance(pt, now) > 4)
    {
        count[getOrientation(pt, now)]++;
        for(int i = 1; i<5; i++)
        {
            if(count[i]>8 && result!=i)
            {
                _tcscat(command,str[i]);
                result = i;
                for(int i=0; i<5; i++) count[i]=0;
            }
        }
        now = pt;
    }
}
inline float SquareRootFloat(float number)
{
    long i;
    float x, y;
    const float f = 1.5F;
    x = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f375a86 - ( i >> 1 );	//注意这一行
    y  = * ( float * ) &i;
    y  = y * ( f - ( x * y * y ) );
    return number * y;
}
inline int _floor(float f)
{
    int a         = *(int*)(&f);
    int sign      = (a>>31);
    int mantissa  = (a&((1<<23)-1))|(1<<23);
    int exponent  = ((a&0x7fffffff)>>23)-127;
    int r         = ((unsigned int)(mantissa)<<8)>>(31-exponent);
    return ((r ^ (sign)) - sign ) &~ (exponent>>31);
}
int Gesture::getDistance(POINT a, POINT b)
{
    //优化sqrt，floor
    return _floor(SquareRootFloat((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)));
}
int Gesture::getOrientation(POINT a, POINT b)
{

    int orientation = 0;
    int x = a.x - b.x;
    int y = a.y - b.y;
    if (x >= 0 && y <= 0)
    {
        //第一象限
        y *= -1;
        if (x > y && SquareRootFloat(3.0)*y < x) orientation = 2;
        if (x < y && SquareRootFloat(3.0)*x < y) orientation = 1;
        y *= -1;
    }
    else if (x >= 0 && y >= 0)
    {
        //第四象限
        if (x > y && SquareRootFloat(3.0)*y < x) orientation = 2;
        if (x < y && SquareRootFloat(3.0)*x < y) orientation = 3;
    }
    else if (x<=0 && y>=0)
    {
        //第三象限
        x *= -1;
        if (x > y && SquareRootFloat(3.0)*y < x) orientation = 4;
        if (x < y && SquareRootFloat(3.0)*x < y) orientation = 3;
        x *= -1;
    }
    else  if (x <= 0 && y <= 0)
    {
        //第二象限
        x *= -1;
        y *= -1;
        if (x > y && SquareRootFloat(3.0)*y < x) orientation = 4;
        if (x < y && SquareRootFloat(3.0)*x < y) orientation = 1;
    }
    //_cprintf("%f\n",atan(-1.732)* 360/(2*3.1415926));
    //_cprintf("%d \n",orientation);
    return orientation;
}
void OnPaintMouse(HDC hdc)
{
    //创建画布
    static bool run = false;
    int sx = GetSystemMetrics(SM_CXSCREEN);
    int sy = GetSystemMetrics(SM_CYSCREEN);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP hBitMap = CreateCompatibleBitmap(hdc, sx, sy);
    SelectObject(mdc, hBitMap);
    Graphics graphics(mdc);
    graphics.Clear(Color(0,255,255,255));
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(CompositingQualityHighQuality);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    //显示界面

    Pen pen(Color(255, 152, 204, 0), 4.0);

    graphics.DrawBeziers(&pen,(Point*)&gr.v_pt[0],(INT)gr.v_pt.size());

    graphics.ReleaseHDC(mdc);


    //窗口参数
    SIZE sizeWindow = {sx,sy};
    BLENDFUNCTION m_Blend;
    m_Blend.BlendOp = 0;
    m_Blend.BlendFlags = 0;
    m_Blend.AlphaFormat = 1;
    m_Blend.SourceConstantAlpha = 255;
    RECT rct;
    GetWindowRect(gr.g_hwnd, &rct);
    POINT ptSrc = {0, 0};
    POINT ptWinPos = {rct.left, rct.top};
    UpdateLayeredWindow(gr.g_hwnd, hdc, &ptWinPos, &sizeWindow, mdc, &ptSrc, 0, &m_Blend, 2);

    //释放资源
	//ReleaseDC(gr.g_hwnd,hdc);
	DeleteObject(hBitMap);
	DeleteDC(mdc);
}
BOOL CALLBACK ShowMouseProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT Ps;
    HDC mdc, hdc;
    switch (message)
    {
    case WM_INITDIALOG:
        gr.g_hwnd = hWnd;
        SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
        SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetWindowPos(hWnd, HWND_TOPMOST,0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
        _cprintf("start mouse\n");
        break;
    case WM_ERASEBKGND :
        break;
    case WM_PAINT:
        _cprintf("start paint\n");
        hdc = BeginPaint(hWnd, &Ps);
        OnPaintMouse(hdc);
        EndPaint(hWnd, &Ps);
        break;
    case WM_CLOSE:
        _cprintf("end paint\n");
        EndDialog(hWnd, wParam);
        break;
    }
    return FALSE;
}

#endif /* _GESTURE_H_ */
