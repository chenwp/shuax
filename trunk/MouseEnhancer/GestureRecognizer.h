#ifndef _GESTURE_H_
#define _GESTURE_H_

#include <math.h>

TCHAR str[][4]= {_T(""),_T("U"),_T("R"),_T("D"),_T("L")};
class Gesture
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
    bool isrun;
    void begin(POINT pt);
    void add(POINT pt);
    TCHAR* end();
};
Gesture::Gesture()
{

}
void Gesture::begin(POINT pt)
{
    g_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, szClass, szClass, WS_POPUP,
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);
    isrun = true;
    time = GetCurrentTime();
    now = pt;
    for(int i=0; i<5; i++) count[i]=0;
    command[0]=0;
    result=0;
}
TCHAR* Gesture::end()
{
    if(GetCurrentTime()-time>1500) command[0]=0;
    isrun = false;
    return command;
}
void Gesture::add(POINT pt)
{
    if(GetCurrentTime()-time>1500)
    {
        isrun = false;
        return;
    }
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
Gesture gr;

void ShowMouse(PVOID pvoid)
{
    static bool notrun = true;
    if(notrun)
    {
        notrun = false;
        //
        long time = GetCurrentTime();
        //显示窗口
        HWND hwnd = gr.g_hwnd;
        //SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN ), SWP_SHOWWINDOW);
        ShowWindow (hwnd, SW_SHOW);
        //窗口参数
        SIZE sizeWindow = {GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN )};
        BLENDFUNCTION m_Blend;
        m_Blend.BlendOp = 0;
        m_Blend.BlendFlags = 0;
        m_Blend.AlphaFormat = 1;
        m_Blend.SourceConstantAlpha = 128+32;
        RECT rct;
        GetWindowRect(hwnd, &rct);
        POINT ptSrc = {0, 0};
        POINT ptWinPos = {rct.left, rct.top};
        //创建画布
        HDC hdc = GetDC(hwnd);
        HDC mdc = CreateCompatibleDC(hdc);
        HBITMAP hBitMap = CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN ));
        SelectObject(mdc, hBitMap);

        Graphics graphics(mdc);
        graphics.SetSmoothingMode(SmoothingModeHighQuality);
        //graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

        //graphics.Clear(Color(255,255,255));
        Bitmap CacheImage(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN ));
        Graphics buffer(&CacheImage);

        //显示界面
        POINT pt1;
        POINT pt2;
        GetCursorPos(&pt1);
        Pen pen(Color(255, 0, 155, 0), 3.5);
        while(GetCurrentTime()-time<1500 && (GetKeyState(myset.MyKey)& 0x8000)!=0 && gr.isrun)
        {
            GetCursorPos(&pt2);
            GraphicsPath path;
            if(pt1.x == pt2.x && pt1.y == pt2.y) continue;
            path.AddLine((INT)pt1.x,pt1.y,pt2.x,pt2.y);
            buffer.DrawPath(&pen,&path);
            pt1 = pt2;
            graphics.DrawImage(&CacheImage, 0, 0);
            graphics.ReleaseHDC(mdc);
            UpdateLayeredWindow(hwnd, hdc, &ptWinPos, &sizeWindow, mdc, &ptSrc, 0, &m_Blend, 2);
            ReleaseDC(hwnd,hdc);
            DeleteObject(hBitMap);
            Sleep(10);
        }
        DeleteDC(mdc);
        ShowWindow (hwnd, SW_HIDE);
        notrun = true;
    }
}
#endif /* _GESTURE_H_ */
