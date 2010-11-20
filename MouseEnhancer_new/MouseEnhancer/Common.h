#pragma once

bool ImageFromIDResource(UINT nID, Image *&pImg)
{
	HINSTANCE hInst = ::GetModuleHandle(0);

	HRSRC hRsrc = ::FindResource (hInst, MAKEINTRESOURCE(nID), _T("PNG"));
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
	GlobalFree(m_hMem);
	pstm->Release();

	FreeResource(lpRsrc);
	return TRUE;
}
inline void SetTopMost(HWND hWnd,bool enable)
{
	if(enable) ::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSENDCHANGING | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	else ::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSENDCHANGING | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}
bool IsVistaSystem(void) 
{ 
	static OSVERSIONINFO info; 
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&info); 
	if (info.dwMajorVersion >= 6) return true; 
	else return false;
	return false; 
}
inline int mi_muldiv(int x, int y, int z)
{
	return (x*y)/z;
}
int ToolTipClassic(HWND hwnd, TCHAR szTexto[MAX_PATH])
{
	TOOLINFO ti;
	InitCommonControls();
	HWND hTool = CreateWindowEx(0,TOOLTIPS_CLASS,NULL,TTS_ALWAYSTIP,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hwnd,NULL,GetModuleHandle(NULL),NULL);
	GetClientRect(hwnd, &ti.rect); 
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hwnd;
	ti.lpszText = szTexto;

	HDC hdc = GetDC(NULL);
    int lfHeight = -mi_muldiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ReleaseDC(NULL, hdc);

	HFONT tempfont = CreateFont(lfHeight, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, IsVistaSystem()?L"Î¢ÈíÑÅºÚ":L"ËÎÌå");
	SendMessage(hTool, WM_SETFONT, (WPARAM)tempfont, 0);
	SendMessage(hTool, TTM_ADDTOOL, 0, (LPARAM)&ti);
	return 0;
}
DWORD FindProcess(TCHAR *Path)
{
	DWORD dwPID = 0;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	DWORD Pid = 0;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	for (Process32First (hProcessSnap, &pe32); Process32Next (hProcessSnap, &pe32);)
	{
		if(_tcsicmp(pe32.szExeFile, Path)==0)
		{
			dwPID = pe32.th32ProcessID;
			break;
		}
	}
	CloseHandle(hProcessSnap);
	return dwPID;
}
bool exclude(HWND hWnd,TCHAR *path)
{
	DWORD dwPID = 0;
	GetWindowThreadProcessId(hWnd,&dwPID);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	DWORD Pid = 0;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	for (Process32First (hProcessSnap, &pe32); Process32Next (hProcessSnap, &pe32);)
	{
		if(pe32.th32ProcessID==dwPID)
		{
			_tcscpy(path,pe32.szExeFile);
			return true;
		}
	}
	CloseHandle(hProcessSnap);
	return false;
}