#ifndef _SHOWVOL_H_
#define _SHOWVOL_H_

typedef bool(__stdcall* MyGetVolume)(UINT&);
typedef void(__stdcall* MySetVolume)(UINT);

int GetVolume()
{
    //return 0;
    static HINSTANCE hFuncInst  = ::LoadLibrary(_T("CoreAudioApi.dll"));
    static UINT dwVolume;
    static MyGetVolume m_GetVolume = (MyGetVolume)GetProcAddress(hFuncInst, "GetVolume");;
    dwVolume = 0;
    if(hFuncInst && m_GetVolume) m_GetVolume(dwVolume);
    return dwVolume;
}
void SetVolume(int dwVolume)
{
    static HINSTANCE hFuncInst;
    hFuncInst = ::LoadLibrary(_T("CoreAudioApi.dll"));
    if (hFuncInst)
    {
        MySetVolume m_SetVolume;
        m_SetVolume = (MySetVolume)GetProcAddress(hFuncInst, "SetVolume");
        if(m_SetVolume) m_SetVolume(dwVolume);
        FreeLibrary(hFuncInst);
    }
}
POINT size;

void OnPaintVol(HWND hwnd,int vol,int alpha)
{
    static HDC hdc;
    static HDC mdc;
    static HBITMAP hBitMap;
    static Image *BG;
    static Image *JD;
    hdc = GetDC(hwnd);
    mdc = CreateCompatibleDC(hdc);
    hBitMap = CreateCompatibleBitmap(hdc, size.x,size.y);
    SelectObject(mdc, hBitMap);

    Graphics graphics(mdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    if(vol)
    {
        BG = ImageFromIDResource(1, _T("PNG"));
    }
    else
    {
        BG = ImageFromIDResource(2, _T("PNG"));
    }
    graphics.DrawImage(BG, 10, 10,BG->GetWidth(),BG->GetHeight());

    JD = ImageFromIDResource(3, _T("PNG"));
    int meter = (vol*16-10)/100+1;
    if(vol==0) meter=0;
    if(vol>95) meter=16;
    for(int i=0; i<meter; i++)
    {
        graphics.DrawImage(JD, 43+9*i,175,JD->GetWidth(),JD->GetHeight());
    }


    static SIZE sizeWindow = {size.x,size.y};
    static BLENDFUNCTION m_Blend;
    m_Blend.BlendOp = 0;
    m_Blend.BlendFlags = 0;
    m_Blend.AlphaFormat = 1;
    m_Blend.SourceConstantAlpha = alpha;
    static RECT rct;
    GetWindowRect(hwnd, &rct);
    static POINT ptSrc = {0, 0};
    static POINT ptWinPos = {rct.left, rct.top};
    UpdateLayeredWindow(hwnd, hdc, &ptWinPos, &sizeWindow, mdc, &ptSrc, 0, &m_Blend, 2);

    graphics.ReleaseHDC(mdc);
    BG->~Image();
    JD->~Image();

    ReleaseDC(hwnd,hdc);
    DeleteObject(hBitMap);
    DeleteDC(mdc);
    //ShowWindow (hwnd, SW_SHOW);
}
void ShowVol(PVOID pvoid)
{
    static bool notrun = true;
    static double fade = 40;
    static HWND hwnd;
    static long StartTime;
    static int o_vol;
    static int n_vol;
    if(notrun)
    {
        notrun = false;

        hwnd = (HWND)pvoid;


        size.x = 250;
        size.y = 250;
        SetWindowPos(hwnd, HWND_TOPMOST, (GetSystemMetrics(SM_CXSCREEN) - size.x)/2, (GetSystemMetrics(SM_CYMAXIMIZED) -size.y)/2, size.x, size.y, SWP_HIDEWINDOW);
        ShowWindow (hwnd, SW_SHOW);


        o_vol = GetVolume();
        StartTime = GetCurrentTime();

        //淡入
        while(true)
        {
            fade*=1.1;
            if(fade>255) break;
            OnPaintVol(m_hwnd,GetVolume(),fade);
            Sleep(30);
        }
        //显示界面
        while(true)
        {

            n_vol = GetVolume();
            OnPaintVol(m_hwnd,n_vol,255);

            //判断音量是否变化，如果变化则重新计时
            if(n_vol!=o_vol)
            {
                o_vol = GetVolume();
                StartTime = GetCurrentTime();
            }

            //判断是否超时
            if(GetCurrentTime()-StartTime>2000) break;
            Sleep(100);
        }
        //淡出
        while(true)
        {
            fade/=1.2;
            if(fade<40) break;
            OnPaintVol(m_hwnd,GetVolume(),fade);
            if(GetVolume()!=o_vol) break;
            Sleep(30);
        }
        //
        ShowWindow (hwnd, SW_HIDE);
        notrun = true;
    }
}
#endif /* _SHOWVOL_H_ */

