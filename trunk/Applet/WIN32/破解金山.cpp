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
			//���ش���
			HWND hwnd;
			hwnd=FindWindow(NULL,"��ɽ���μ�����");
			if (hwnd!=NULL) ShowWindow(hwnd,SW_HIDE);
			hwnd=FindWindow(NULL,"������ʾ������ɽ���μ�����");
			if (hwnd!=NULL) ShowWindow(hwnd,SW_HIDE);
			//��ͣ����
			HANDLE TargetProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, pe32.th32ProcessID);
			mProcess(TargetProcess);
			break;
		}
	}
	if(flag) MessageBox(0,"��ϲ�����ƽ�ɹ�����������˳���","��ʾ",0);
	else MessageBox(0,"û���ҵ���ɽ������������ٺ������б�����","��ʾ",0);
}
