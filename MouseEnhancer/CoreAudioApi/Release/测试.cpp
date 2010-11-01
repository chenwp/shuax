/*
		void __declspec(dllexport)SetXPVolume( const DWORD dwVolume );
		BOOL __declspec(dllexport)GetXPVolume( DWORD& dwVolume );
		void __declspec(dllexport)SetVolumnVista( UINT nInputStep );
		BOOL __declspec(dllexport)GetVolumnVista( UINT& uVol );
*/

#include <windows.h>
#include <stdio.h>

int main()
{
	typedef bool(__stdcall* GetVolumnVista)(UINT&);
	typedef void(__stdcall* SetVolumnVista)(UINT);
	
	typedef bool(__stdcall* GetXPVolume)(UINT&);
	typedef void(__stdcall* SetXPVolume)(UINT);

	HINSTANCE   hInst = ::LoadLibrary("CoreAudioApi.dll");
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
		
		UINT x;
		m_GetVolumnVista(x);
		printf("%d\n", x);
		//x++;
		//m_SetVolumnVista(100);
		//m_GetVolumnVista(x);
		//printf("%d\n", x);
		
		getchar();
	}
	return 0;
}
