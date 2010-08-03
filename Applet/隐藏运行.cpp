#include <windows.h>
//#include <stdio.h>
HDESK hVirtualDesk;
HDESK hCurrent;
int main(int argc,char *argv[])
{
	if(argc!=2) return 0;
	
	hCurrent = GetThreadDesktop(GetCurrentThreadId());
    hVirtualDesk = CreateDesktop("Virtual",NULL,NULL,DF_ALLOWOTHERACCOUNTHOOK,GENERIC_ALL,NULL);
    
    STARTUPINFO si;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    si.lpDesktop = "Virtual";
    
	PROCESS_INFORMATION ProcInfo;
	ZeroMemory( &ProcInfo, sizeof(ProcInfo) );
	CreateProcess(NULL, argv[1], NULL, NULL, false, 0, NULL, NULL, &si, &ProcInfo);
	//MessageBox(0,argv[1],argv[1],0);
	return 0;
}
