#include <windows.h>
#include <stdio.h>
void RunAndWait(char *path)
{
	STARTUPINFO StartInfo = {sizeof(StartInfo)}; 
	PROCESS_INFORMATION ProcInfo;
	memset(&ProcInfo, 0, sizeof(ProcInfo)); 
	if (CreateProcess(NULL, path, NULL, NULL, FALSE, 0, NULL, NULL, &StartInfo, &ProcInfo))
	{
		//   等待这个进程结束
		//WaitForSingleObject(ProcInfo.hProcess, INFINITE);
		//CloseHandle(ProcInfo.hThread);
		//CloseHandle(ProcInfo.hProcess);
	}
	return ;
}
static wchar_t * ANSI2Unicode(const char * strin)
{
	wchar_t * strout;

	// 预计算所需空间大小（已包含结束字符）,单位wchar_t
	int dwNum = MultiByteToWideChar (CP_ACP, 0, strin, -1, 0 , 0);
	wchar_t * pBuffer = (wchar_t*)malloc(sizeof(wchar_t)*dwNum);
	if (!pBuffer)
	{
		return strout;
	}
	memset(pBuffer, 0, (dwNum)*sizeof(wchar_t));

	if (MultiByteToWideChar(CP_ACP, 0, strin, -1, pBuffer, dwNum) >= 0)
	{
		strout = pBuffer;
	}

	//free(pBuffer);
	return strout;
}
static char *  Unicode2UTF8(const wchar_t * strin)
{
	char * strout;

	//测试所需存储空间大小（已包含结束字符）,单位char
	int dwNum = WideCharToMultiByte(CP_UTF8, 0, strin, -1, 0, 0, 0, 0);
	char* pBuffer = (char*)malloc(sizeof(char)*dwNum);
	if (!pBuffer)
	{
		return strout;
	}
	memset(pBuffer, 0, dwNum);

	if (WideCharToMultiByte(CP_UTF8, 0, strin, -1, pBuffer, dwNum, 0, 0) >= 0)
	{
		strout = pBuffer;
	}

	//free(pBuffer);
	return strout;
}
static char* ANSI2UTF8(const char* strin)
{
	return Unicode2UTF8(ANSI2Unicode(strin));
}
int main(int argc,char *argv[])
{
	if(argc!=1)
	{
		FILE *out;
		out = fopen("list.txt","wb");
		int i = 1;
		for(;i<argc;i++)
		{            
            fputs(ANSI2UTF8(argv[i]),out);
			fputs("\n",out);
		}
		fclose(out);
		
		//SetFileAttributes("list.txt", FILE_ATTRIBUTE_HIDDEN);
		RunAndWait("Portraiture.exe -l list.txt");
		Sleep(100);
		remove("list.txt");
	}
	return 0;
}

