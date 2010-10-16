#include <windows.h>
#include <shlwapi.h>
class settings
{
public:
    bool isClk;
    bool isRls;
    bool isWhl;

    bool isLem;
    bool isRim;

    bool isWeh;
    bool isVol;
    bool isGus;
    bool isIco;


    bool isDbg;

    int t_Clk;
    int t_Rls;
    int t_Whl;

    int g_opr[6];
    settings()
    {
        //根据exe文件获得ini文件
        TCHAR path[MAX_PATH];
        GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
        PathRenameExtension(path, _T(".ini"));
        //
        isClk = GetPrivateProfileInt(_T("修复鼠标"), _T("单击"), 1, path);
        isRls = GetPrivateProfileInt(_T("修复鼠标"), _T("释放"), 1, path);
        isWhl = GetPrivateProfileInt(_T("修复鼠标"), _T("滚动"), 1, path);

        isLem = GetPrivateProfileInt(_T("设备选择"), _T("左键"), 1, path);
        isRim = GetPrivateProfileInt(_T("设备选择"), _T("右键"), 0, path);

        t_Clk = GetPrivateProfileInt(_T("详细参数"), _T("单击间隔"), 100, path);
        t_Rls = GetPrivateProfileInt(_T("详细参数"), _T("释放间隔"), 50, path);
        t_Whl = GetPrivateProfileInt(_T("详细参数"), _T("滚动间隔"), 50, path);

        isWeh = GetPrivateProfileInt(_T("其它设置"), _T("滚轮穿透"), 1, path);
        isVol = GetPrivateProfileInt(_T("其它设置"), _T("音量控制"), 1, path);
        isGus = GetPrivateProfileInt(_T("其它设置"), _T("鼠标手势"), 1, path);
        isIco = GetPrivateProfileInt(_T("其它设置"), _T("显示图标"), 1, path);

        isDbg = GetPrivateProfileInt(_T("其它设置"), _T("调试模式"), 0, path);

        g_opr[0] = 0;
        g_opr[1] = GetPrivateProfileInt(_T("鼠标手势"), _T("往上滑动"), 0, path);
        g_opr[2] = GetPrivateProfileInt(_T("鼠标手势"), _T("往右滑动"), 0, path);
        g_opr[3] = GetPrivateProfileInt(_T("鼠标手势"), _T("往下滑动"), 0, path);
        g_opr[4] = GetPrivateProfileInt(_T("鼠标手势"), _T("往左滑动"), 0, path);
    }
    void save()
    {
        //根据exe文件获得ini文件
        TCHAR path[MAX_PATH];
        GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
        PathRenameExtension(path, _T(".ini"));

        SetPrivateProfileInt(_T("修复鼠标"), _T("单击"), isClk, path);
        SetPrivateProfileInt(_T("修复鼠标"), _T("释放"), isRls, path);
        SetPrivateProfileInt(_T("修复鼠标"), _T("滚动"), isWhl, path);

        SetPrivateProfileInt(_T("设备选择"), _T("左键"), isLem, path);
        SetPrivateProfileInt(_T("设备选择"), _T("右键"), isRim, path);

        SetPrivateProfileInt(_T("详细参数"), _T("单击间隔"), t_Clk, path);
        SetPrivateProfileInt(_T("详细参数"), _T("释放间隔"), t_Rls, path);
        SetPrivateProfileInt(_T("详细参数"), _T("滚动间隔"), t_Whl, path);

        SetPrivateProfileInt(_T("其它设置"), _T("滚轮穿透"), isWeh, path);
        SetPrivateProfileInt(_T("其它设置"), _T("音量控制"), isVol, path);
        SetPrivateProfileInt(_T("其它设置"), _T("鼠标手势"), isGus, path);
        SetPrivateProfileInt(_T("其它设置"), _T("显示图标"), isIco, path);

        SetPrivateProfileInt(_T("鼠标手势"), _T("往上滑动"), g_opr[1], path);
        SetPrivateProfileInt(_T("鼠标手势"), _T("往右滑动"), g_opr[2], path);
        SetPrivateProfileInt(_T("鼠标手势"), _T("往下滑动"), g_opr[3], path);
        SetPrivateProfileInt(_T("鼠标手势"), _T("往左滑动"), g_opr[4], path);
    }
private:
    void SetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName,INT nDefault,LPCTSTR lpFileName)
    {
        TCHAR buffer[1024];
        wsprintf(buffer,_T("%d"),nDefault);
        WritePrivateProfileString(lpAppName,lpKeyName, buffer, lpFileName);
    }
};
#define APP_NAME _T("MouseEnhancer")
bool CheckAutoRun()
{
	HKEY hKey;

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_QUERY_VALUE, &hKey);
    TCHAR buffer[1024];
    DWORD dwLength = 1024;
    RegQueryValueEx(hKey, APP_NAME, NULL, NULL, (LPBYTE)buffer, &dwLength);
    RegCloseKey(hKey);

    TCHAR path[1024];
    GetModuleFileName(NULL, path, 1024);

	if (_tcsicmp(path,buffer) != 0) return false;
	else return true;
}
void AutoStart(HWND hwnd)
{
	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hKey);
	if (!CheckAutoRun())
	{
        TCHAR path[1024];
        GetModuleFileName(NULL, path, 1024);
		if (_tcsstr(path, _T("Temp")) != NULL)
		{
			if (MessageBox(hwnd, _T("你正在临时文件夹中运行本软件，不建议启用开机自动启动。\n你如果依然想要开机启动，请选择 是。"), szClass, MB_YESNO + MB_ICONINFORMATION + MB_DEFBUTTON2) == IDNO) return;
		}
		RegSetValueEx(hKey, APP_NAME, 0, REG_SZ, (LPBYTE)path, _tcslen(path)*sizeof(TCHAR));
	}
	else
	{
		RegDeleteValue(hKey, APP_NAME);
	}
}
void doSomething(int opr)
{
    HWND hwnd = GetForegroundWindow();
    switch(opr)
    {
        case 1:SetWindowPos(hwnd, HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);break;
        case 2:PostMessage(hwnd, WM_CLOSE, 0, 0);break;
        case 3:SetWindowPos(hwnd, HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);break;
        case 4:ShowWindow(hwnd, SW_MINIMIZE);break;
    }
}
