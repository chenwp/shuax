#define UNICODE
#define _UNICODE
#include <windows.h>
#include <conio.h>
#include <stdio.h>
bool Start_Service(wchar_t * ServiceName);
bool Stop_Service(wchar_t * ServiceName);
bool Create_Service(wchar_t * ServiceName);
bool Delete_Service(wchar_t * ServiceName);
void ReconfigureService(wchar_t * ServiceName, wchar_t * ServiceDes);
void ChangeServiceRun(wchar_t * ServiceName);
void EnumService(void);

int main()
{
    while(1)
    {
        Start_Service(L"APMServ-Apache");
        Sleep(1000*30);
    }
    return 0;
}
    //Start_Service(L"WmdmPmSN");
    //Stop_Service(L"WmdmPmSN");
    //Create_Service(L"ServiceTest");
    //Delete_Service(L"ServiceTest");
    //ReconfigureService(L"ServiceTest",ServiceDisp);
    //EnumService();
bool Start_Service(wchar_t * ServiceName)
{
    SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if(NULL != schSCManager)
    {
        // L"WmdmPmSN"
        SC_HANDLE schService = OpenService(schSCManager,ServiceName,SERVICE_ALL_ACCESS);
        if( NULL != schService)
        {
            if(StartService(schService,0,NULL))
            {
                CloseServiceHandle(schService);
                CloseServiceHandle(schSCManager);
                wprintf(L"Restart Apache Services!\n");
                return 1;
            }
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            wprintf(L"Service is running normally!\n");
            return 0;
        }
        CloseServiceHandle(schSCManager);
        wprintf(L"Open Service failed!\n");
        return 0;
    }
    wprintf(L"OpenSCManager failed!\n");
    CloseServiceHandle(schSCManager);
    return 0;
}

bool Stop_Service(wchar_t * ServiceName)
{
    SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if(NULL != schSCManager)
    {
        // L"WmdmPmSN"
        SC_HANDLE schService = OpenService(schSCManager,ServiceName,SERVICE_ALL_ACCESS);
        if( NULL != schService)
        {
            SERVICE_STATUS ServiceStatus;
            if(ControlService(schService,SERVICE_CONTROL_STOP,&ServiceStatus))
            {
                CloseServiceHandle(schService);
                CloseServiceHandle(schSCManager);
                return 1;
            }
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            wprintf(L"Start Service failed!\n");
            return 0;
        }
        CloseServiceHandle(schSCManager);
        wprintf(L"Open Service failed!\n");
        return 0;
    }
    wprintf(L"OpenSCManager failed!\n");
    CloseServiceHandle(schSCManager);
    return 0;
}

bool Create_Service(wchar_t * ServiceName)
{
    SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if(NULL != schSCManager)
    {
        wchar_t * DisplayName = L"Service Program Test\n";
        wchar_t * FilePathName = L"d:\\cyuyan\\servicetest.exe";
        SC_HANDLE schService = CreateService(
            schSCManager,
            ServiceName,
            DisplayName,
            SC_MANAGER_ALL_ACCESS,
            SERVICE_WIN32_OWN_PROCESS,
            SERVICE_AUTO_START,
            SERVICE_ERROR_IGNORE,
            FilePathName,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL);
        if(schService != NULL)
        {
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return 1;
        }
        else
        {
            CloseServiceHandle(schSCManager);
            return 0;
        }
    }
    else
        return 0;
}

bool Delete_Service(wchar_t * ServiceName)
{
    SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if(NULL != schSCManager)
    {
        // L"WmdmPmSN"
        SC_HANDLE schService = OpenService(schSCManager,ServiceName,SERVICE_ALL_ACCESS);
        if( NULL != schService)
        {
            SERVICE_STATUS ServiceStatus;
            QueryServiceStatus(schService,&ServiceStatus);
            if(ServiceStatus.dwCurrentState != SERVICE_STOPPED)
            {
                ControlService(schService,SERVICE_CONTROL_STOP,&ServiceStatus);
            }
            DeleteService(schService);
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return 1;
        }
        else
            wprintf(L"Open Service failed!\n");
            return 0;
    }
    else
    {
        wprintf(L"OpenSCManager failed!\n");
        CloseServiceHandle(schSCManager);
        return 0;
    }
}


void ReconfigureService(wchar_t * ServiceName, wchar_t * ServiceDisp)
{
SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
if (schSCManager != NULL)
{
    // Need to acquire database lock before reconfiguring.
    SC_LOCK sclLock = LockServiceDatabase(schSCManager);
    if (sclLock != NULL)
    {
      // Open a handle to the service.
      SC_HANDLE schService = OpenService(
          schSCManager,           // SCManager database
          ServiceName,            // name of service
          SERVICE_CHANGE_CONFIG); // need CHANGE access

      if (schService != NULL)
      {
          SERVICE_DESCRIPTION sdBuf;
          sdBuf.lpDescription = ServiceDisp;
          if (ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &sdBuf))
          {
             MessageBox(NULL,L"Change SUCCESS",L" ",MB_OK);
          }
          CloseServiceHandle(schService);
      }
      UnlockServiceDatabase(sclLock);
    }
    CloseServiceHandle(schSCManager);
}
}


void ChangeServiceRun(wchar_t * ServiceName)
{
    SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if(NULL != schSCManager)
    {
        // L"WmdmPmSN"
        SC_HANDLE schService = OpenService(schSCManager,ServiceName,SERVICE_ALL_ACCESS);
        if( NULL != schService)
        {
            if(ChangeServiceConfig(
                schService,
                SERVICE_NO_CHANGE,
                SERVICE_AUTO_START,
                SERVICE_NO_CHANGE,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL))
            {
                wprintf(L"Change Service done!\n");
                return;
            }
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
        }
        wprintf(L"Open Service failed!\n");
    }
    wprintf(L"OpenSCManager failed!\n");
    CloseServiceHandle(schSCManager);
}

void EnumService(void)
{
    LPENUM_SERVICE_STATUS st;
    st=(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR,64*1024);
    DWORD ret=0;
    DWORD size=0;
    SC_HANDLE sc=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

    EnumServicesStatus(sc,SERVICE_WIN32,SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUS)st,1024*64,&size,&ret,NULL);


    for(int i=0;i<ret;i++){
        wprintf(L"%-20s%-50s",st[i].lpServiceName,st[i].lpDisplayName);
        switch(st[i].ServiceStatus.dwCurrentState){
    case(SERVICE_RUNNING):
        wprintf(L"running\n");
        break;
    case(SERVICE_STOPPED):
        wprintf(L"stopped\n");
        break;

        }
    }
}
