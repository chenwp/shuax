#ifndef _SHOWVOL_H_
#define _SHOWVOL_H_

bool ImageFromIDResource(UINT nID, Image *&pImg)
{
    HINSTANCE hInst = ::GetModuleHandle(0);

    HRSRC hRsrc = ::FindResource (hInst, MAKEINTRESOURCE(nID), _T("VOL"));
    if (!hRsrc) return FALSE;

    DWORD len = SizeofResource(hInst, hRsrc);
    BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
    if (!lpRsrc) return FALSE;
    HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);

    BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
    memcpy(pmem, lpRsrc, len);
    IStream* pstm;
    CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
    pImg = Image::FromStream(pstm);

    GlobalUnlock(m_hMem);

    pstm->Release();

    FreeResource(lpRsrc);
    return TRUE;
}

int GetVolume()
{
    typedef bool(__stdcall* GetVolumnVista)(UINT&);
    typedef void(__stdcall* SetVolumnVista)(UINT);

    typedef bool(__stdcall* GetXPVolume)(UINT&);
    typedef void(__stdcall* SetXPVolume)(UINT);

    HINSTANCE   hInst = ::LoadLibrary(_T("CoreAudioApi.dll"));
    UINT dwVolume = 0;
    if (hInst)
    {
        GetVolumnVista m_GetVolumnVista;
        SetVolumnVista m_SetVolumnVista;

        GetXPVolume m_GetXPVolume;
        SetXPVolume m_SetXPVolume;

        m_GetVolumnVista = (GetVolumnVista)GetProcAddress(hInst, "?GetVolumnVista@@YA_NAAI@Z");
        m_SetVolumnVista = (SetVolumnVista)GetProcAddress(hInst, "?SetVolumnVista@@YAXI@Z");

        m_GetXPVolume = (GetXPVolume)GetProcAddress(hInst, "?GetXPVolume@@YA_NAAI@Z");
        m_SetXPVolume = (SetXPVolume)GetProcAddress(hInst, "?SetXPVolume@@YAXI@Z");

        //检测系统版本
        OSVERSIONINFO info;
        info.dwOSVersionInfoSize=sizeof(info);
        GetVersionEx(&info);
        if (info.dwMajorVersion<6)
        {
            m_GetXPVolume(dwVolume);
        }
        else
        {
            m_GetVolumnVista(dwVolume);
        }
    }
    return dwVolume;
}
void OnPaint(HWND hwnd,int vol,int alpha)
{
    HDC hdc = GetDC(hwnd);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP hBitMap = CreateCompatibleBitmap(hdc, size.x,size.y);
    SelectObject(mdc, hBitMap);

    Graphics graphics(mdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    //graphics.Clear(Color(255,255,255));
    Bitmap CacheImage(size.x,size.y);
    Graphics buffer(&CacheImage);

    if(vol)
    {
        Image *BG=NULL;
        ImageFromIDResource(1, BG);
        buffer.DrawImage(BG, 10, 10,BG->GetWidth(),BG->GetHeight());

        Image *JD=NULL;
        ImageFromIDResource(3, JD);
        int meter = (vol*16-10)/100+1;
        if(vol==0) meter=0;
        if(vol>95) meter=16;
        for(int i=0; i<meter; i++)
        {
            buffer.DrawImage(JD, 43+9*i,175,JD->GetWidth(),JD->GetHeight());
        }
    }
    else
    {
        Image *BG=NULL;
        ImageFromIDResource(2, BG);
        buffer.DrawImage(BG, 10, 10,BG->GetWidth(),BG->GetHeight());
    }

    graphics.DrawImage(&CacheImage, 0, 0);
    graphics.ReleaseHDC(mdc);


    SIZE sizeWindow = {size.x,size.y};
    BLENDFUNCTION m_Blend;
    m_Blend.BlendOp = 0;
    m_Blend.BlendFlags = 0;
    m_Blend.AlphaFormat = 1;
    m_Blend.SourceConstantAlpha = alpha;
    RECT rct;
    GetWindowRect(hwnd, &rct);
    POINT ptSrc = {0, 0};
    POINT ptWinPos = {rct.left, rct.top};
    UpdateLayeredWindow(hwnd, hdc, &ptWinPos, &sizeWindow, mdc, &ptSrc, 0, &m_Blend, 2);

    ReleaseDC(hwnd,hdc);
    DeleteObject(hBitMap);
    DeleteDC(mdc);
    //ShowWindow (hwnd, SW_SHOW);
}
void ShowVol(PVOID pvoid)
{
    static bool notrun = true;
    static double fade = 40;
    if(notrun)
    {
        notrun = false;

        HWND hwnd = (HWND)pvoid;
        ShowWindow (hwnd, SW_SHOW);


        int o_vol = GetVolume();
        long StartTime = GetCurrentTime();

        //淡入
        _cprintf("start\n");
        while(true)
        {
            fade*=1.1;
            if(fade>255)
            {
                fade = 255;
                break;
            }
            OnPaint(m_hwnd,GetVolume(),fade);
            Sleep(30);
        }
        //显示界面
        while(true)
        {

            int n_vol = GetVolume();
            OnPaint(m_hwnd,n_vol,fade);

            //判断音量是否变化，如果变化则重新计时
            if(n_vol!=o_vol)
            {
                o_vol = GetVolume();
                StartTime = GetCurrentTime();
            }

            //判断是否超时
            if(GetCurrentTime()-StartTime>2000)
            {
                _cprintf("exit\n");
                break;
            }
            Sleep(30);
        }
        //淡出
        _cprintf("exit now\n");
        while(true)
        {
            fade/=1.2;
            if(fade<40)
            {
                break;
            }
            OnPaint(m_hwnd,GetVolume(),fade);
            if(GetVolume()!=o_vol) break;
            Sleep(30);
        }
        //
        ShowWindow (hwnd, SW_HIDE);
        notrun = true;
    }
}
#endif /* _SHOWVOL_H_ */

