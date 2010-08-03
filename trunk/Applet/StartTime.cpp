#include <windows.h>

int Check()
{
	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS + 0x100, &hKey);
	if (RegQueryValueEx(hKey, "StartTime", 0, 0, 0, 0) != ERROR_SUCCESS) return 1;
	else return 0;
}
void AutoStart(char *path)
{
	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS + 0x100, &hKey);
	if (Check())
	{
		if (strstr(path, "Temp") != NULL)
		{
			MessageBoxW(0, L"程序检测到你正在临时文件中运行，设置开机自动运行失败！", L"开机时间测试", MB_OK + MB_ICONINFORMATION); 
			exit(0);
		}
		RegSetValueEx(hKey, "StartTime", 0, REG_SZ, (unsigned char *)path, strlen(path));
	}
	else
	{
		RegDeleteValue(hKey, "StartTime");
	}
}
void Shutdown()
{
	HANDLE hToken = 0;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	DWORD dwRtn = 0;
	OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) ;
	LookupPrivilegeValue( NULL, SE_SHUTDOWN_NAME, &sedebugnameValue ) ;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, &tkp, &dwRtn) ;
	ExitWindowsEx(EWX_REBOOT, 0);
	CloseHandle(hToken);
}
int main(int argc, char *argv[])
{
    AutoStart(argv[0]);
    if (!Check())
	{
		if (MessageBoxW(0, L"程序将会在下次开机时自动运行。你是否需要立即重启计算机？", L"开机时间测试", MB_YESNO + MB_ICONINFORMATION) == IDYES)
		{
			Shutdown();
		}
	}
	else
	{
		wchar_t tips[64];
		wsprintfW(tips, L"测试完成，你开机共用时：%d秒。", GetTickCount() / 1000);
		MessageBoxW(0, tips, L"开机时间测试", MB_OK);
	}
}
