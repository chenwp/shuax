#pragma once

TCHAR xiufushubiao[] = _T("�޸�����");
TCHAR xiufucanshu [] = _T("�޸�����");
TCHAR shebeixuanze[] = _T("�豸ѡ��");
TCHAR gngnngshezhi[] = _T("��������");
TCHAR gunlunchuant[] = _T("���ִ�͸");
TCHAR yinliangkgzh[] = _T("��������");
TCHAR shubiaoshous[] = _T("�������");

#define APP_NAME _T("MouseEnhancer")

static class settings
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

	int v_key;
	int v_var;
	bool v_snd;
	bool v_osd;
	bool v_glb;

	int m_key;
	bool m_shw;
	bool m_str;

	TCHAR ex_path[1024];
	int ex_size;

	int s_lne;
	bool s_act;

	void Init()
	{
		//����exe�ļ����ini�ļ�
		TCHAR path[MAX_PATH];
		GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
		PathRenameExtension(path, _T(".ini"));
		//
		ex_size = GetPrivateProfileSection(_T("�ų��б�"),ex_path,1024,path);


		isClk = GetPrivateProfileInt(xiufushubiao, _T("����"), 1, path);
		isRls = GetPrivateProfileInt(xiufushubiao, _T("�ͷ�"), 1, path);
		isWhl = GetPrivateProfileInt(xiufushubiao, _T("����"), 1, path);

		t_Clk = GetPrivateProfileInt(xiufucanshu, _T("�������"), 100, path);
		t_Rls = GetPrivateProfileInt(xiufucanshu, _T("�ͷż��"), 50, path);
		t_Whl = GetPrivateProfileInt(xiufucanshu, _T("�������"), 50, path);

		isLem = GetPrivateProfileInt(shebeixuanze, _T("���"), 0, path);
		isRim = GetPrivateProfileInt(shebeixuanze, _T("�Ҽ�"), 0, path);

		isWeh = GetPrivateProfileInt(gngnngshezhi, _T("���ִ�͸"), 1, path);
		isVol = GetPrivateProfileInt(gngnngshezhi, _T("��������"), 1, path);
		isGus = GetPrivateProfileInt(gngnngshezhi, _T("�������"), 1, path);
		isIco = GetPrivateProfileInt(gngnngshezhi, _T("��ʾͼ��"), 1, path);
		isDbg = GetPrivateProfileInt(gngnngshezhi, _T("����ģʽ"), 0, path);

		s_lne = GetPrivateProfileInt(gunlunchuant, _T("��������"), 1, path);
		if(s_lne<1||s_lne>33) s_lne=1;
		s_act = GetPrivateProfileInt(gunlunchuant, _T("��ǰ����"), 0, path);

		v_key = GetPrivateProfileInt(yinliangkgzh, _T("��ݼ�"), 1, path);

		switch(v_key)
		{
		case 1:
			v_key = VK_MENU;
			break;
		case 2:
			v_key = VK_CONTROL;
			break;
		case 3:
			v_key = VK_SHIFT;
			break;
		case 4:
			v_key = VK_LWIN;
			break;
		case 5:
			v_key = VK_RBUTTON;
			break;
		case 6:
			v_key = VK_LBUTTON;
			break;
		default:
			v_key = VK_MENU;
			break;
		}

		v_var = GetPrivateProfileInt(yinliangkgzh, _T("�����仯"), 3, path);
		v_snd = GetPrivateProfileInt(yinliangkgzh, _T("������Ч"), 1, path);
		v_osd = GetPrivateProfileInt(yinliangkgzh, _T("��ʾ����"), 1, path);
		v_glb = GetPrivateProfileInt(yinliangkgzh, _T("���ȫ��"), 0, path);

		m_key = GetPrivateProfileInt(shubiaoshous, _T("��ݼ�"), 1, path);
		switch(m_key)
		{
		case 1:
			m_key = VK_MENU;
			break;
		case 2:
			m_key = VK_CONTROL;
			break;
		case 3:
			m_key = VK_SHIFT;
			break;
		case 4:
			m_key = VK_LWIN;
			break;
		default:
			m_key = VK_MENU;
			break;
		}
		m_shw = GetPrivateProfileInt(shubiaoshous, _T("��ʾ�켣"), 0, path);
		m_str = GetPrivateProfileInt(shubiaoshous, _T("��Чλ��"), 0, path);
	}
	void save()
	{
		//����exe�ļ����ini�ļ�
		TCHAR path[MAX_PATH];
		GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
		PathRenameExtension(path, _T(".ini"));

		SetPrivateProfileInt(xiufushubiao, _T("����"), isClk, path);
		SetPrivateProfileInt(xiufushubiao, _T("�ͷ�"), isRls, path);
		SetPrivateProfileInt(xiufushubiao, _T("����"), isWhl, path);

		SetPrivateProfileInt(shebeixuanze, _T("���"), isLem, path);
		SetPrivateProfileInt(shebeixuanze, _T("�Ҽ�"), isRim, path);

		SetPrivateProfileInt(gngnngshezhi, _T("���ִ�͸"), isWeh, path);
		SetPrivateProfileInt(gngnngshezhi, _T("��������"), isVol, path);
		SetPrivateProfileInt(gngnngshezhi, _T("�������"), isGus, path);
		//
		SetPrivateProfileInt(gngnngshezhi, _T("��ʾͼ��"), isIco, path);
	}
private:
	void SetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName,INT nDefault,LPCTSTR lpFileName)
	{
		TCHAR buffer[1024];
		wsprintf(buffer,_T("%d"),nDefault);
		WritePrivateProfileString(lpAppName,lpKeyName, buffer, lpFileName);
	}
}myset;

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
			if (MessageBox(hwnd, _T("��������ʱ�ļ��������б���������������ÿ����Զ�������\n�������Ȼ��Ҫ������������ѡ�� �ǡ�"), szClass, MB_YESNO + MB_ICONINFORMATION + MB_DEFBUTTON2) == IDNO) return;
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
	if (GetParent(hwnd)==NULL  &&  IsWindowVisible(hwnd)) //�ɼ����㴰��
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
	if (GetParent(hwnd)==NULL  &&  IsWindowVisible(hwnd)) //�ɼ����㴰��
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
		if(GetCurrentTime()-StartTime>10000) break;//��ʱǿ���˳�
	}
}
bool isExclude(HWND hwnd)
{
	TCHAR buff[256];
	int i,j;
	for(i=0,j=0;i<myset.ex_size;i++)
	{
		if (myset.ex_path[i]!=0) buff[j++]=myset.ex_path[i];
		else
		{
			buff[j++]=0;
			j=0;
			//
			DWORD PID;
			GetWindowThreadProcessId(hwnd,&PID);
			if(PID==FindProcess(buff))
			{
				//�ų�ģʽ
				//_cprintf("Exclude:%S %d\n",buff,FindProcess(buff));
				return false;
			}
		}
	}
	return true;
}
void doSomething(PVOID pvoid)
{
	TCHAR *op = (TCHAR*)pvoid;
	POINT pt;
	TCHAR buff[256];
	HWND hwnd;
	if(myset.m_str)
	{
		//�յ�
		Sleep(50);
		GetCursorPos(&pt);
		hwnd = WindowFromPoint(pt);
		if(!isExclude(hwnd)) return ;
	}
	else hwnd = f_hwnd;

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

	//����exe�ļ����ini�ļ�
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
	PathRenameExtension(path, _T(".ini"));
	//
	int opr = GetPrivateProfileInt(_T("�������"), op, 0, path);
	switch(opr)
	{
	case 1:
		if(!isDesktop)SetTopMost(hwnd,true);
		break;
	case 2:
		if(!isDesktop)SetTopMost(hwnd,false);
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
	case 11:
		if(isExclude(hwnd) && exclude(hwnd,buff))
		{
			buff[_tcslen(buff)+1]=0;
			WritePrivateProfileSectionW(_T("�ų��б�"),buff,path);
			myset.ex_size = GetPrivateProfileSection(_T("�ų��б�"),myset.ex_path,1024,path);
		}
		break;
	case 0:
		GetPrivateProfileString(shubiaoshous, op, _T(""),buff,255, path);
		STARTUPINFO StartInfo = {sizeof(StartInfo)};
		PROCESS_INFORMATION ProcInfo;
		memset(&ProcInfo, 0, sizeof(ProcInfo));
		if(CreateProcess(NULL, buff, NULL, NULL, FALSE, NULL, NULL, NULL, &StartInfo, &ProcInfo)!=0)
		{
			_beginthread(KeepTop,0,(void*)ProcInfo.dwProcessId);
		}
	}
}
