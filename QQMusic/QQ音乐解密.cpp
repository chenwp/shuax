#include <stdio.h>
#include "tea/tea.h"
#include "tag/tag.h"
#include "dir/dir.h"
void op(char *filename,char *path)
{
	decrypt(filename,path);
}

int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		puts("�����÷���QQ���ֽ���.exe Դ�ļ�Ŀ¼ �������ļ�Ŀ¼");
		puts("�������QQ���ֽ���.exe C:\\Users\\shuax\\AppData\\Roaming\\Tencent\\QQMusic\\Cache\\musiccache C:\\");
		getchar();
		return 0;
	}
	char filepath[2048];
	strcpy(filepath,argv[1]);
	char savepath[1024];
	strcpy(savepath,argv[2]);
	dir(filepath,savepath,op);
	getchar();
	return 0;
}
