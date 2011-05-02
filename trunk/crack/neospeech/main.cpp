#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		MessageBox(0,L"请把需要破解的dll拖动到本程序图标上！\n\n例如：vt_chi_liang16.dll",L"提示",0);
		return -1;	
	}
	HINSTANCE hinstance = LoadLibraryA(argv[1]);
	DWORD offset = (DWORD)GetProcAddress(hinstance,"VT_CheckLicense_CHI");
	if(offset!=0)
	{
		//
		offset = offset - (DWORD)hinstance;
		//printf("%X",offset);
		FreeLibrary(hinstance);
		
		SetFileAttributesA(argv[1], FILE_ATTRIBUTE_NORMAL);
		FILE *fp;
		fp = fopen(argv[1], "rb+");
		if (fp == NULL)
		{
			MessageBox(0,L"文件无法写入，请检查文件状态，稍后再进行破解！",L"警告",0);
			return -1;
		}
		char bakpath[512];
		strcpy(bakpath,argv[1]);
		strcat(bakpath,".bak");
		CopyFileA(argv[1], bakpath, 1);
		
		
		fseek(fp, offset, 0);
		fputc(0x33,fp);
		fputc(0xC0,fp);
		fputc(0xC3,fp);
		fclose(fp);
		
		MessageBox(0,L"恭喜你，破解已经完成。并且生成了备份文件。",L"提示",0);
		return 0;
	}
	else MessageBox(0,L"您选择的dll文件不正确，请重新选择！",L"警告",0);
	return -1;
}
