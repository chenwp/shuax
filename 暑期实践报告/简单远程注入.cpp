#include<stdio.h>
#include"windows.h"
typedef struct
{
	DWORD address;
	char a[10];
} info;
typedef int (_stdcall *msgbox)(HWND, LPCTSTR, LPCTSTR, UINT);
DWORD WINAPI t(LPVOID p)
{
	info *p1 = (info*)p;
	msgbox m = (msgbox)p1->address;
	m(0, p1->a, p1->a, 0);
	return 0;
}
int main()
{
	HANDLE ph =::OpenProcess(PROCESS_ALL_ACCESS, false, 5388);
	void *s;
	s =::VirtualAllocEx(ph, 0, 1024 * 4, MEM_COMMIT, PAGE_READWRITE);
	::WriteProcessMemory(ph, s, t, 1024*4, 0);
	info i;
	ZeroMemory(&i, sizeof(i));
	::strcpy(i.a, "a");
	HINSTANCE m =::LoadLibrary("user32.dll");
	i.address = (DWORD)::GetProcAddress(m, "MessageBoxA");
	info *s1;
	s1 = (info *)::VirtualAllocEx(ph, 0, sizeof(info), MEM_COMMIT, PAGE_READWRITE);
	::WriteProcessMemory(ph, s1, &i, sizeof(info), 0);
	HANDLE hrt =::CreateRemoteThread(ph, 0, 0, (LPTHREAD_START_ROUTINE)s, s1, 0, 0);
}