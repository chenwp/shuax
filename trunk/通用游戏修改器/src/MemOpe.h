#ifndef _MEMOPE_H_
#define _MEMOPE_H_
DWORD GetMemValue(DWORD dwBase)
{
    DWORD dwValue=0;
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (hProcess != NULL)
    {
        ::ReadProcessMemory(hProcess, (LPCVOID)dwBase, &dwValue, sizeof(DWORD), NULL);
        ::CloseHandle(hProcess);
    }
    return dwValue;
}
bool SetMemValue(DWORD dwBase,DWORD dwValue)
{
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (hProcess != NULL)
    {
        ::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwValue, sizeof(DWORD), NULL);
        ::CloseHandle(hProcess);
        return true;
    }
    return false;
}
bool GetMemData(DWORD dwBase,BYTE *&dwValue,int length)
{
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    if (hProcess != NULL)
    {
        ::ReadProcessMemory(hProcess, (LPCVOID)dwBase, &dwValue, length, NULL);
        ::CloseHandle(hProcess);
        return true;
    }
    return false;
}
bool SetMemData(DWORD dwBase,BYTE *&dwValue,int length)
{
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (hProcess != NULL)
    {
        ::WriteProcessMemory(hProcess, (LPVOID)dwBase, &dwValue, length, NULL);
        ::CloseHandle(hProcess);
        return true;
    }
    return false;
}

bool isNum(const TCHAR*nptr)
{
    static const TCHAR hex[]=_T("01234566789ABCDEFabcdef");
    while (*nptr)
    {
        if(_tcschr(hex,*nptr)==0) return false;
        nptr++;
    }
    return true;
}
DWORD getMemmoryValue(const TCHAR *address)
{
    DWORD dwBase;
    _stscanf(address,_T("%x"),&dwBase);
    return GetMemValue(dwBase);
}
DWORD GetBase(const TCHAR *_base)
{
    if(_tcschr(_base,'+')==0)
    {
        DWORD dwBase;
        _stscanf(_base,_T("%x"),&dwBase);
        return dwBase;
    }
    int start,end,length;
    TCHAR base[1024];
    _tcscpy(base,_base);
    TCHAR Front[128];
    TCHAR Middle[128];
    TCHAR Rear[128];
    DWORD result;
    do
    {
        if(_tcsrchr(base,'[')) start = _tcsrchr(base,'[')-base;
        else start=0;
        if(_tcschr(base,']')) end = _tcschr(base,']')-base+1;
        else end=0;
        if(start==0&&end==0)
        {
            DWORD a,b;
            _stscanf(base,_T("%x+%x"),&a,&b);
            result = a+b;
            _stprintf(base,_T("%x"),result);
        }
        else
        {
            length = _tcslen(base);
            _stprintf(Front,_T("%.*s"),start,base);
            _stprintf(Middle,_T("%.*s"),end-start-2,base+start+1);
            _stprintf(Rear,_T("%.*s"),length-end,base+end);
            if(isNum(Middle))
            {
                result = getMemmoryValue(Middle);
            }
            else
            {
                //÷¥––º∆À„
                int a,b;
                _stscanf(Middle,_T("%x+%x"),&a,&b);
                result = a+b;
            }
            _stprintf(base,_T("%s%x%s"),Front,result,Rear);
        }
    }
    while(!isNum(base));
    DWORD dwBase;
    _stscanf(base,_T("%x"),&dwBase);
    return dwBase;
}
int GetCodeLen(const TCHAR *_code)
{
    int length = 1;
    while (*_code++)
    {
        if(*_code==' ')length++;
    }
    return length;
}
//void GetCode(const TCHAR *_code,BYTE *&dwValue,int length)
//{
//	TCHAR code[128];
//	_tcscpy(code,_code);
//	const TCHAR *nptr = code;
//    static BYTE value[128];
//    TCHAR* result;
//    TCHAR temp[32];
//    int i=0;
//	do
//	{
//	    result=_tcschr(nptr,' ');
//        _stprintf(temp,_T("%.*s"), result-nptr, nptr);
//        nptr = result+1;
//        _stscanf(temp,_T("%02x"),&value[i]);
//        i++;
//	}while(result!=0);
//	*dwValue = &value;
//}
#endif /*_MEMOPE_H_*/
