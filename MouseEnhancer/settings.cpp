#include <windows.h>
#include <shlwapi.h>

TCHAR xiufushubiao[] = _T("修复鼠标");
TCHAR xiufucanshu [] = _T("修复参数");
TCHAR shebeixuanze[] = _T("设备选择");
TCHAR gngnngshezhi[] = _T("功能设置");
TCHAR shubishoushi[] = _T("鼠标手势");

#define APP_NAME _T("MouseEnhancer")

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

    int HtKey;
    int MyKey;
    int MyVol;

    int g_opr[6];
    void Init()
    {
        //根据exe文件获得ini文件
        TCHAR path[MAX_PATH];
        GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
        PathRenameExtension(path, _T(".ini"));
        //
        isClk = GetPrivateProfileInt(xiufushubiao, _T("单击"), 1, path);
        isRls = GetPrivateProfileInt(xiufushubiao, _T("释放"), 1, path);
        isWhl = GetPrivateProfileInt(xiufushubiao, _T("滚动"), 1, path);

        t_Clk = GetPrivateProfileInt(xiufucanshu, _T("单击间隔"), 100, path);
        t_Rls = GetPrivateProfileInt(xiufucanshu, _T("释放间隔"), 50, path);
        t_Whl = GetPrivateProfileInt(xiufucanshu, _T("滚动间隔"), 50, path);

        isLem = GetPrivateProfileInt(shebeixuanze, _T("左键"), 0, path);
        isRim = GetPrivateProfileInt(shebeixuanze, _T("右键"), 0, path);

        isWeh = GetPrivateProfileInt(gngnngshezhi, _T("滚轮穿透"), 1, path);
        isVol = GetPrivateProfileInt(gngnngshezhi, _T("音量控制"), 1, path);
        isGus = GetPrivateProfileInt(gngnngshezhi, _T("鼠标手势"), 1, path);
        isIco = GetPrivateProfileInt(gngnngshezhi, _T("显示图标"), 1, path);
        isDbg = GetPrivateProfileInt(gngnngshezhi, _T("调试模式"), 0, path);
        HtKey = GetPrivateProfileInt(gngnngshezhi, _T("快捷键"), 1, path);

        g_opr[0] = 0;
        g_opr[1] = GetPrivateProfileInt(shubishoushi, _T("往上滑动"), 0, path);
        g_opr[2] = GetPrivateProfileInt(shubishoushi, _T("往右滑动"), 0, path);
        g_opr[3] = GetPrivateProfileInt(shubishoushi, _T("往下滑动"), 0, path);
        g_opr[4] = GetPrivateProfileInt(shubishoushi, _T("往左滑动"), 0, path);

        switch(HtKey)
        {
            case 1:MyVol = VK_MENU;break;
            case 2:MyVol = VK_CONTROL;break;
            case 3:MyVol = VK_SHIFT;break;
            case 4:MyVol = VK_LWIN;break;
            case 5:MyVol = VK_RBUTTON;break;
            default:
                MyVol = VK_MENU;
                HtKey = 1;
                break;
        }
        if(MyVol!=VK_RBUTTON) MyKey = MyVol;
        else MyKey = VK_MENU;;
    }
    void save()
    {
        //根据exe文件获得ini文件
        TCHAR path[MAX_PATH];
        GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
        PathRenameExtension(path, _T(".ini"));

        SetPrivateProfileInt(xiufushubiao, _T("单击"), isClk, path);
        SetPrivateProfileInt(xiufushubiao, _T("释放"), isRls, path);
        SetPrivateProfileInt(xiufushubiao, _T("滚动"), isWhl, path);

//        SetPrivateProfileInt(xiufucanshu, _T("单击间隔"), t_Clk, path);
//        SetPrivateProfileInt(xiufucanshu, _T("释放间隔"), t_Rls, path);
//        SetPrivateProfileInt(xiufucanshu, _T("滚动间隔"), t_Whl, path);

        SetPrivateProfileInt(shebeixuanze, _T("左键"), isLem, path);
        SetPrivateProfileInt(shebeixuanze, _T("右键"), isRim, path);

        SetPrivateProfileInt(gngnngshezhi, _T("滚轮穿透"), isWeh, path);
        SetPrivateProfileInt(gngnngshezhi, _T("音量控制"), isVol, path);
        SetPrivateProfileInt(gngnngshezhi, _T("鼠标手势"), isGus, path);
        SetPrivateProfileInt(gngnngshezhi, _T("显示图标"), isIco, path);
//        SetPrivateProfileInt(gngnngshezhi), _T("快捷键"), HtKey, path);
//
//        SetPrivateProfileInt(shubishoushi, _T("往上滑动"), g_opr[1], path);
//        SetPrivateProfileInt(shubishoushi, _T("往右滑动"), g_opr[2], path);
//        SetPrivateProfileInt(shubishoushi, _T("往下滑动"), g_opr[3], path);
//        SetPrivateProfileInt(shubishoushi, _T("往左滑动"), g_opr[4], path);
    }
private:
    void SetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName,INT nDefault,LPCTSTR lpFileName)
    {
        TCHAR buffer[1024];
        wsprintf(buffer,_T("%d"),nDefault);
        WritePrivateProfileString(lpAppName,lpKeyName, buffer, lpFileName);
    }
};
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
BOOL CALLBACK CloseSimilarWindows(HWND hwnd, LPARAM lParam)
{
	if (GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE)//任务栏可见窗口
	{
		TCHAR buff[256];
		GetClassName(hwnd, buff, 255);
		//
		if (lstrcmp(buff, (TCHAR *)lParam) == 0)//比较类名
		{
			//DestroyWindow(hwnd);
			if (!GetParent(hwnd))PostMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		}
	}
	return 1;
}
void doSomething(int opr)
{
    POINT pt;
    TCHAR buff[256];
    GetCursorPos(&pt);
    HWND hwnd = WindowFromPoint(pt);

    while(GetParent(hwnd))
    {
        hwnd=GetParent(hwnd);
    }

    if (!(GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE)) return;

    GetWindowText(hwnd, buff, 255);
    if (!_tcsicmp(buff, _T("Program Manager"))) return;

    GetClassName(hwnd, buff, 255);
    if (!_tcsicmp(buff, _T("tooltips_class32"))) return;
    if (!_tcsicmp(buff, _T("Shell_TrayWnd"))) return;
    if (!_tcsicmp(buff, _T("WorkerW"))) return;

    _cprintf("Class:%S\n",buff);
    switch(opr)
    {
    case 1:SetWindowPos(hwnd,   HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);break;
    case 2:SetWindowPos(hwnd, HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);break;

    case 3:PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);break;
    case 4:PostMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);break;
    case 5:PostMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE,  0);break;
    case 6:PostMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE,    0);break;

    case 7:EnumWindows(CloseSimilarWindows, LPARAM(buff));break;

    case 8:PostMessage(HWND_BROADCAST, WM_SYSCOMMAND,   SC_SCREENSAVE, 0);break;
    case 9:PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);break;
    }
}
