?#include "windows.h"
#include "tlhelp32.h"

typedef DWORD (WINAPI *NtProcess)(HANDLE hProcess);

int main()
{
	HINSTANCE h_module = LoadLibrary("ntdll.dll");
	NtProcess mProcess = (NtProcess)GetProcAddress(h_module, "NtSuspendProcess"); //NtResumeProcess NtSuspendProcess

	PROCESSENTRY32 pe32;
	HANDLE hProcessSnap = NULL;
	int flag=0;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
	for (Process32First (hProcessSnap, &pe32);Process32Next (hProcessSnap, &pe32);)
	{
        if(stricmp(pe32.szExeFile, "ProxyClient.exe")==0)
		{
			flag=1;
			//隐藏窗口
			HWND hwnd;
			hwnd=FindWindow(NULL,"金山网游加速器");
			if (hwnd!=NULL) ShowWindow(hwnd,SW_HIDE);
			hwnd=FindWindow(NULL,"加速提示——金山网游加速器");
			if (hwnd!=NULL) ShowWindow(hwnd,SW_HIDE);
			//暂停进程
			HANDLE TargetProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, pe32.th32ProcessID);
			mProcess(TargetProcess);
			break;
		}
	}
	if(flag) MessageBox(0,"恭喜啦，破解成功，按任意键退出。","提示",0);
	else MessageBox(0,"没有找到金山加速器，请加速后再运行本工具","提示",0);
}
