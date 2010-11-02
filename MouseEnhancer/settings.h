#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <windows.h>
#include <shlwapi.h>

#include <PROCESS.H>

TCHAR xiufushubiao[] = _T("修复功能");
TCHAR xiufucanshu [] = _T("修复参数");
TCHAR shebeixuanze[] = _T("设备选择");
TCHAR gngnngshezhi[] = _T("功能设置");

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

        switch(HtKey)
        {
        case 1:
            MyVol = VK_MENU;
            break;
        case 2:
            MyVol = VK_CONTROL;
            break;
        case 3:
            MyVol = VK_SHIFT;
            break;
        case 4:
            MyVol = VK_LWIN;
            break;
        case 5:
            MyVol = VK_RBUTTON;
            break;
        case 6:
            MyVol = VK_LBUTTON;
            break;
        default:
            MyVol = VK_MENU;
            HtKey = 1;
            break;
        }
        if(MyVol!=VK_RBUTTON&&MyVol!=VK_LBUTTON) MyKey = MyVol;
        else MyKey = VK_MENU;
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

        SetPrivateProfileInt(shebeixuanze, _T("左键"), isLem, path);
        SetPrivateProfileInt(shebeixuanze, _T("右键"), isRim, path);

        SetPrivateProfileInt(gngnngshezhi, _T("滚轮穿透"), isWeh, path);
        SetPrivateProfileInt(gngnngshezhi, _T("音量控制"), isVol, path);
        SetPrivateProfileInt(gngnngshezhi, _T("鼠标手势"), isGus, path);
        //
        SetPrivateProfileInt(gngnngshezhi, _T("显示图标"), isIco, path);
    }
private:
    void SetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName,INT nDefault,LPCTSTR lpFileName)
    {
        TCHAR buffer[1024];
        wsprintf(buffer,_T("%d"),nDefault);
        WritePrivateProfileString(lpAppName,lpKeyName, buffer, lpFileName);
    }
};

settings myset;

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
    if (GetParent(hwnd)==NULL  &&  IsWindowVisible(hwnd)) //可见顶层窗口
    {
        TCHAR buff[256];
        GetClassName(hwnd, buff, 255);
        if (lstrcmp(buff, (TCHAR *)lParam) == 0) PostMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
    }
    return 1;
}


int found;

BOOL CALLBACK MySetForegroundWindow(HWND hwnd, LPARAM lParam)
{
    if (GetParent(hwnd)==NULL  &&  IsWindowVisible(hwnd)) //可见顶层窗口
    {
        DWORD   ProcID   =   0;
        GetWindowThreadProcessId(hwnd,&ProcID);
        if (ProcID==(DWORD)lParam)
        {
            SetWindowPos(hwnd,   HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
            found = true;
        }
    }
    return 1;
}

void KeepTop(PVOID pvoid)
{
    LPARAM lParam = (LPARAM)pvoid;
    found = false;
    long StartTime = GetCurrentTime();
    while(!found)
    {
        Sleep(50);
        EnumWindows(MySetForegroundWindow, lParam);
        if(GetCurrentTime()-StartTime>10000) break;//超时强制退出
    }
}
void doSomething(TCHAR *op)
{
    POINT pt;
    TCHAR buff[256];
    GetCursorPos(&pt);
    HWND hwnd = WindowFromPoint(pt);

    while(GetParent(hwnd))
    {
        hwnd=GetParent(hwnd);
    }


    bool isDesktop = false;
    if (!(GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE)) isDesktop = true;

    GetWindowText(hwnd, buff, 255);
    if (!_tcsicmp(buff, _T("Program Manager"))) isDesktop = true;

    GetClassName(hwnd, buff, 255);
    if (!_tcsicmp(buff, _T("tooltips_class32"))) isDesktop = true;
    if (!_tcsicmp(buff, _T("Shell_TrayWnd"))) isDesktop = true;
    if (!_tcsicmp(buff, _T("WorkerW"))) isDesktop = true;


    _cprintf("Class:%S\n",buff);

    //根据exe文件获得ini文件
    TCHAR path[MAX_PATH];
    GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
    PathRenameExtension(path, _T(".ini"));
    //
    int opr = GetPrivateProfileInt(_T("鼠标手势"), op, 0, path);
    switch(opr)
    {
    case 1:
        if(!isDesktop)SetWindowPos(hwnd,   HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
        break;
    case 2:
        if(!isDesktop)SetWindowPos(hwnd, HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
        break;
    case 3:
        if(!isDesktop)PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
        break;
    case 4:
        if(!isDesktop)PostMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        break;
    case 5:
        if(!isDesktop)PostMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE,  0);
        break;
    case 6:
        if(!isDesktop)PostMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE,    0);
        break;
    case 7:
        if(!isDesktop)EnumWindows(CloseSimilarWindows, LPARAM(buff));
        break;
    case 8:
        PostMessage(HWND_BROADCAST, WM_SYSCOMMAND,   SC_SCREENSAVE, 0);
        break;
    case 9:
        PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
        break;
    case 10:
        hwnd = FindWindow(szClass, szClass);
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        break;
    case 0:
        GetPrivateProfileString(_T("鼠标手势"), op, _T(""),buff,255, path);
        STARTUPINFO StartInfo = {sizeof(StartInfo)};
        PROCESS_INFORMATION ProcInfo;
        memset(&ProcInfo, 0, sizeof(ProcInfo));
        if(CreateProcess(NULL, buff, NULL, NULL, FALSE, NULL, NULL, NULL, &StartInfo, &ProcInfo)!=0)
        {
            _cprintf("Start:%X %S\n\n",ProcInfo.dwProcessId,buff);
            _beginthread(KeepTop,0,(void*)ProcInfo.dwProcessId);
        }
    }
}
bool ImageFromIDResource(UINT nID, Image *&pImg)
{
    HINSTANCE hInst = ::GetModuleHandle(0);

    HRSRC hRsrc = ::FindResource (hInst, MAKEINTRESOURCE(nID), _T("VOL"));
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

    pstm->Release();

    FreeResource(lpRsrc);
    return TRUE;
}
#endif /* _SETTINGS_H_ */
