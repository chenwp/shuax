#pragma once


#define SAFE_RELEASE(punk)  \
	if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

bool GetVistaVolume(UINT& uVol)
{
	CoInitialize(NULL);
	IMMDeviceEnumerator *m_pEnumerator = 0;
	IMMDevice *m_pDevice = 0;
	IAudioEndpointVolume *m_pAEV = 0;

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&m_pEnumerator);
	m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
	m_pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&m_pAEV);

	BOOL temp;
	m_pAEV->GetMute(&temp);
	if(temp==TRUE)
	{
		uVol = 0;
		SAFE_RELEASE(m_pAEV);
		SAFE_RELEASE(m_pDevice);
		SAFE_RELEASE(m_pEnumerator);

		CoUninitialize();
		return TRUE;
	}

	static float test;
	m_pAEV->GetMasterVolumeLevelScalar(&test);
	uVol = (UINT)(test*100);

	SAFE_RELEASE(m_pAEV);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pEnumerator);

	CoUninitialize();
	return TRUE;
}
bool GetXPVolume(UINT& uVol)
{
	HMIXER m_hMixer = NULL;
	MIXERCAPS m_mxcaps;
	::ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));
	if (::mixerGetNumDevs() != 0)
	{
		if (::mixerOpen(&m_hMixer,0,0,NULL,MIXER_OBJECTF_MIXER )
			!= MMSYSERR_NOERROR)
			return FALSE;
		if (::mixerGetDevCaps((UINT)m_hMixer, &m_mxcaps, sizeof(MIXERCAPS))
			!= MMSYSERR_NOERROR)
			return FALSE;
	}
	if (m_hMixer == NULL)    return FALSE;
	MIXERLINE mxl;
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer,
		&mxl,
		MIXER_OBJECTF_HMIXER |
		MIXER_GETLINEINFOF_COMPONENTTYPE)
		!= MMSYSERR_NOERROR)        return FALSE;

	DWORD m_controlnum=mxl.cChannels; 
	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType =MIXERCONTROL_CONTROLTYPE_VOLUME; 
	mxlc.cControls =1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer,
		&mxlc,
		MIXER_OBJECTF_HMIXER |
		MIXER_GETLINECONTROLSF_ONEBYTYPE )
		!= MMSYSERR_NOERROR)
		return FALSE;
	// PCHAR m_name=mxc.szName; 
	DWORD m_controlid = mxc.dwControlID;
	LONG m_min= mxc.Bounds.lMinimum;
	LONG m_max= mxc.Bounds.lMaximum;

	DWORD dVol = ( uVol * m_max ) / 100;

	MIXERCONTROLDETAILS_SIGNED mxcdVolume[2] = {dVol, dVol};
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_controlid;
	mxcd.cChannels = m_controlnum;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED);
	mxcd.paDetails = &mxcdVolume;
	if (::mixerGetControlDetails((HMIXEROBJ)m_hMixer,
		&mxcd,
		MIXER_OBJECTF_HMIXER |
		MIXER_SETCONTROLDETAILSF_VALUE)
		!= MMSYSERR_NOERROR)    return FALSE;

	mixerClose( m_hMixer );

	uVol = ( mxcdVolume[0].lValue * 100 ) / m_max;

	return TRUE;
}
int GetVolume()
{
	UINT uVol = 0;
	if(IsVistaSystem())
	{
		GetVistaVolume(uVol);
	}
	else
	{
		GetXPVolume(uVol);
	}
	return uVol;
}

POINT v_size;
Bitmap* m_DoubleBuffer;
void UpdateTransparency(HWND hWnd,int alpha)
{
	//显示界面
	//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	HDC dcScreen = GetDC(hWnd);//GetDC(GetDesktopWindow());
	HDC dcMemory = CreateCompatibleDC(dcScreen);

	HBITMAP dbBitmap;
	m_DoubleBuffer->GetHBITMAP(Color(0, 0, 0, 0), &dbBitmap);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(dcMemory, dbBitmap);

	BLENDFUNCTION blendPixelFunction= {AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA};
	SIZE sizeWindow = {v_size.x,v_size.y};
	POINT ptSrc = {0, 0};
	UpdateLayeredWindow(hWnd, dcScreen, NULL, &sizeWindow, dcMemory, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
	SetTopMost(hWnd,true);
	ReleaseDC(GetDesktopWindow(), dcScreen);
	SelectObject(dcMemory, oldBitmap);
	DeleteDC(dcMemory);
	DeleteObject(dbBitmap);
}
static    float              fade = 40;
BOOL CALLBACK ShowVolumeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static    PAINTSTRUCT        Ps;
	static    HDC                hdc;
	static    bool               isrun;
	static    bool               Time_isrun;
	static    int                o_vol;
	static    int                n_vol;
	static    long               o_tim;
	static    long               n_tim;
	switch (message)
	{
	case WM_INITDIALOG:
		//参数初始化
		v_size.x = 250;
		v_size.y = 250;
		if(fade<40) fade = 40;
		m_DoubleBuffer = new Bitmap(v_size.x, v_size.y, PixelFormat32bppARGB);
		isrun = true;
		o_vol = GetVolume();
		o_tim = GetCurrentTime();
		//界面初始化
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetWindowPos(hWnd, HWND_TOPMOST, (GetSystemMetrics(SM_CXSCREEN) - v_size.x)/2, (GetSystemMetrics(SM_CYMAXIMIZED) -v_size.y)/2, v_size.x, v_size.y, SWP_SHOWWINDOW | SWP_NOACTIVATE);
		ShowWindow(hWnd,SW_SHOW);
		InvalidateRect(hWnd, NULL, false);
		SetTimer(hWnd,1,40,0);
		Time_isrun = true;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &Ps);
		if(Time_isrun)
		{
			Time_isrun = false;
			Graphics graphics(hdc);
			Graphics buffer(m_DoubleBuffer);
			buffer.Clear(0);
			//buffer.SetSmoothingMode(SmoothingModeAntiAlias);
			int vol = GetVolume();
			Image *BG;
			Image *JD;
			ImageFromIDResource(vol?IDB_PNG1:IDB_PNG3,BG);
			if(BG) buffer.DrawImage(BG, 10, 10,BG->GetWidth(),BG->GetHeight());

			ImageFromIDResource(IDB_PNG2,JD);
			int meter = (vol*16-10)/100+1;
			if(vol==0) meter=0;
			if(vol>95) meter=16;
			for(int i=0; i<meter; i++)
			{
				if(JD) buffer.DrawImage(JD, 43+9*i,175,JD->GetWidth(),JD->GetHeight());
			}
			BG->~Image();
			JD->~Image();
			graphics.DrawImage(m_DoubleBuffer, 0, 0);
			UpdateTransparency(hWnd,fade);
			SetTopMost(hWnd,true);
			Time_isrun = true;
		}
		EndPaint(hWnd, &Ps);
		break;
	case WM_TIMER:
		if(isrun)
		{
			fade*=1.2f;
			if(fade>255) fade=255;
			//
			if(GetVolume()!=o_vol)//判断音量是否变化，如果变化则重新计时
			{
				o_vol = GetVolume();
				o_tim = GetCurrentTime();
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else
			{
				if(fade!=255) InvalidateRect(hWnd, NULL, FALSE);
			}
			//判断是否超时
			if(GetCurrentTime()-o_tim>2000) isrun = false;
		}
		else
		{
			fade/=1.1f;
			if(fade<10)
			{
				SendMessage(hWnd, WM_CLOSE, 1, 0);
			}
			InvalidateRect(hWnd, NULL, FALSE);
			if(GetVolume()!=o_vol) SendMessage(hWnd, WM_CLOSE, 1, 0);
		}
		break;
	case WM_CLOSE:
		if(m_DoubleBuffer) delete (m_DoubleBuffer);
		KillTimer(hWnd,1);
		EndDialog(hWnd, wParam);
		break;
	}
	return FALSE;
}

void ShowVolume(PVOID pvoid)
{
	static bool notrun = true;
	if(notrun)
	{
		notrun = false;
		_cprintf("OSD\n");
		DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1),0,ShowVolumeProc,0);
		SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
		notrun = true;
	}
}