#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "resource.h"

HINSTANCE hInst;
DWORD dwPid;

struct Address
{
    DWORD dwBase;

    DWORD dwSun1;
    DWORD dwSun2;
    DWORD dwSunBase;

    DWORD dwMon1;
    DWORD dwMon2;
    DWORD dwMonBase;

    DWORD Coo1;
    DWORD Coo2;
    DWORD CooBase;

    DWORD dwCollect;
    DWORD dwSuspend;
    DWORD dwRepeat;
}ad;
void InitChina()
{
    ad.dwBase =  0x006A9F38;//0x006A9EC0

    ad.dwMon1 = 0x82C;
    ad.dwMon2 = 0x28;
    ad.dwMonBase = 0x0048CAA5;

    ad.dwSun1 = 0x768;
    ad.dwSun2 = 0x5560;
    ad.dwSunBase = 0x0041BA76;

    ad.Coo1 = 0x144;
    ad.Coo2 = 0x28;
    ad.CooBase = 0x00488F7D;

    ad.dwCollect = 0x0040CB4B;
    //
    ad.dwSuspend = 0x004502C0;
    ad.dwRepeat = 0x0040FE2F;
}
void InitEng()
{

}
void InitYear()
{
    ad.dwBase =  0x007794F8;

    ad.dwMon1 = 0x950;
    ad.dwMon2 = 0x50;
    ad.dwMonBase = 0x004B9281;

    ad.dwSun1 = 0x868;
    ad.dwSun2 = 0x5578;
    ad.dwSunBase = 0x00433F86;


    ad.Coo1 = 0x15C;
    ad.Coo2 = 0x70;
    ad.CooBase = 0x004B4CA1;

    ad.dwCollect = 0x0044C5F2;
    ad.dwSuspend = 0;//ox0043A490     ,0x00411A00
    ad.dwRepeat = 0x00425953;
}
bool Sunshine()
{
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess != NULL)
	{
		DWORD dwBase = ad.dwBase;
		DWORD dwValue = 0;
		if (::ReadProcessMemory(hProcess, (LPCVOID)dwBase, &dwValue, sizeof(DWORD), NULL))
		{
			dwBase = dwValue + ad.dwSun1;
			::ReadProcessMemory(hProcess, (LPVOID)dwBase, &dwValue, sizeof(DWORD), NULL);

			dwBase = dwValue + ad.dwSun2;
			dwValue = 9990;
			::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwValue, sizeof(DWORD), NULL);
			//
			dwBase = ad.dwSunBase; //基址
			BYTE dwFill[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
			::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwFill, sizeof(dwFill), false);
		}
		::CloseHandle(hProcess);
		return true;
	}
	return false;
}
bool Money()
{
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess != NULL)
	{
		DWORD dwBase = ad.dwBase;
		DWORD dwValue = 0;
		if (::ReadProcessMemory(hProcess, (LPCVOID)dwBase, &dwValue, sizeof(DWORD), NULL))
		{
			dwBase = dwValue + ad.dwMon1;
			::ReadProcessMemory(hProcess, (LPVOID)dwBase, &dwValue, sizeof(DWORD), NULL);

			dwBase = dwValue + ad.dwMon2;
			dwValue = 99999;
			::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwValue, sizeof(DWORD), NULL);

			//
			dwBase = ad.dwMonBase; //基址
            BYTE dwFill[] = {0x90, 0x90, 0x90};
            ::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwFill, sizeof(dwFill), false);
		}
		::CloseHandle(hProcess);
		return true;
	}
	return false;
}
bool Cooling()
{
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess != NULL)
	{
		DWORD dwBase = ad.dwBase;
		DWORD dwValue = 0;
		DWORD dwCount = 0;
		if (::ReadProcessMemory(hProcess, (LPCVOID)dwBase, &dwValue, sizeof(DWORD), NULL))
		{

			dwBase = dwValue + ad.dwSun1;
			::ReadProcessMemory(hProcess, (LPVOID)dwBase, &dwValue, sizeof(DWORD), NULL);

			dwBase = dwValue + ad.Coo1;
			::ReadProcessMemory(hProcess, (LPVOID)dwBase, &dwValue, sizeof(DWORD), NULL);

			dwBase = dwValue + 0x24;
			::ReadProcessMemory(hProcess, (LPVOID)dwBase, &dwCount, sizeof(DWORD), NULL);
            //
			for(DWORD i=0;i<dwCount;i++)
			{
				DWORD dwTmp = dwValue + ad.Coo2 + i*0x50;
				if(ad.Coo2==0x70)
				{
                    BYTE dwTmp2 = 1;
                    ::WriteProcessMemory(hProcess, (LPVOID)dwTmp, &dwTmp2, sizeof(BYTE), NULL);
				}
                else
                {
                    DWORD dwTmp2 = 0;
                    dwBase = dwTmp + 0x28;

                    ::ReadProcessMemory(hProcess, (LPVOID)dwBase, &dwTmp2, sizeof(DWORD), NULL);
                    dwBase = dwTmp + 0x24;
                    ::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwTmp2, sizeof(DWORD), NULL);
                }
			}
			//
			dwBase = ad.CooBase; //基址
			if(ad.Coo2==0x70)
			{
                BYTE dwFill[] = {0xC6, 0x45, 0x48, 0x01};
                ::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwFill, sizeof(dwFill), false);
			}
			else
			{
                BYTE dwFill[] = {0x90, 0x90, 0x90,};
                ::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwFill, sizeof(dwFill), false);
			}
		}
		::CloseHandle(hProcess);
		return true;
	}
	return false;
}
bool AutoCollect()
{
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess != NULL)
	{
		DWORD dwBase = ad.dwCollect;
		if(ad.Coo2==0x70)
		{
			BYTE dwFill[] = {0xEB, 0x09};
			::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwFill, sizeof(dwFill), false);
        }
        else
        {
			BYTE dwFill2[] = {0xB9, 0x05, 0x00, 0x00, 0x00, 0xEB,0x3F};
			::WriteProcessMemory(hProcess, (LPVOID)0x40CB08, &dwFill2, sizeof(dwFill2), false);
			BYTE dwFill[] = {0xEB, 0xBB, 0x90};
			::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwFill, sizeof(dwFill), false);
        }
		::CloseHandle(hProcess);
		return true;
	}
	return false;
}
bool Suspend()
{
	if(!ad.dwSuspend) return false;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess != NULL)
	{
		DWORD dwBase = ad.dwSuspend;
        BYTE dwFill[] = {0xC3};
        ::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwFill, sizeof(dwFill), false);
		::CloseHandle(hProcess);
		return true;
	}
	return false;
}
bool Repeat()
{
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess != NULL)
	{
		DWORD dwBase = ad.dwRepeat;
        BYTE dwFill[] = {0xE9, 0x20, 0x09, 0x00, 0x00, 0x90};//0f 84 1f 09 00 00
        ::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwFill, sizeof(dwFill), false);
		::CloseHandle(hProcess);
		return true;
	}
	return false;
}

void GetProc(DWORD &pid,HWND hwnd)
{
    HWND hnd = FindWindow("MainWindow", NULL);
    if (hnd)
    {
        GetWindowThreadProcessId(hnd,&pid);
    }
    else
    {
        pid=0;
    }
    if (pid != 0)
    {
        SetDlgItemText(hwnd, IDC_STATIC,"提示：程序已经运行，请注意选择版本。");
    }
    else
    {
        SetDlgItemText(hwnd, IDC_STATIC,"提示：游戏还没运行，现在打开游戏吧。");
    }
}
void Unlimited(bool(*op)())
{
    if (dwPid != 0)
    {
        if(op()==false)
        {
            dwPid=0;
            MessageBeep(MB_ICONASTERISK);
        }
    }
    else
    {
        MessageBeep(MB_ICONASTERISK);
    }
}
BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            //AllocConsole();
            SendMessage(GetDlgItem(hwndDlg,IDC_RADIO1), BM_CLICK, true,NULL);


            GetProc(dwPid,hwndDlg);
            SetTimer(hwndDlg, 1, 500, NULL);
            SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
            return TRUE;
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;
        case WM_TIMER:
            switch (wParam)
            {
                case 1:GetProc(dwPid,hwndDlg);break;
            }
            return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_RADIO1:
                    InitChina();
                    return TRUE;
                case IDC_RADIO2:
                    InitEng();
                    return TRUE;
                case IDC_RADIO3:
                    InitYear();
                    return TRUE;
                case IDC_BTN_SUN:
                    Unlimited(Sunshine);
                    return TRUE;
                case IDC_BTN_MON:
                    Unlimited(Money);
                    return TRUE;
                case IDC_BTN_FRE:
                    Unlimited(Cooling);
                    return TRUE;
                case IDC_BTN_COL:
                    Unlimited(AutoCollect);
                    return TRUE;
                case IDC_BTN_REP:
                    Unlimited(Repeat);
                    return TRUE;
                case IDC_BTN_PAU:
                    Unlimited(Suspend);
                    return TRUE;
            }
    }

    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DialogProc);
}
