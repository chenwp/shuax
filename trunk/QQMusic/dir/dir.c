#include "dir.h"
#include <windows.h>
DLLIMPORT void dir(char *path,char *ppath,void(*op)(char*,char*))
{
	WIN32_FIND_DATA ffbuf;
	char temp[1024];
	strcpy(temp,path);
	strcat(temp,"\\*.*");
	HANDLE hfind = FindFirstFile(temp, &ffbuf);
	if (hfind != INVALID_HANDLE_VALUE)
	{
		FindNextFile(hfind, &ffbuf);
		int count = 0;
		while (FindNextFile(hfind, &ffbuf))
		{
			wsprintf(temp,"%s\\%s\\%s.000.tdl",path,ffbuf.cFileName,ffbuf.cFileName);
			op(temp,ppath);
			count++;
		}
		printf("����ִ����ϣ�һ��������%d���ļ�",count);
		FindClose(hfind);
	}
}
