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
		puts("典型用法：QQ音乐解密.exe 源文件目录 保存新文件目录");
		puts("例如命令：QQ音乐解密.exe C:\\Users\\shuax\\AppData\\Roaming\\Tencent\\QQMusic\\Cache\\musiccache C:\\");
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
