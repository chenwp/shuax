#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		MessageBox(0,L"�����Ҫ�ƽ��dll�϶���������ͼ���ϣ�\n\n���磺vt_chi_liang16.dll",L"��ʾ",0);
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
			MessageBox(0,L"�ļ��޷�д�룬�����ļ�״̬���Ժ��ٽ����ƽ⣡",L"����",0);
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
		
		MessageBox(0,L"��ϲ�㣬�ƽ��Ѿ���ɡ����������˱����ļ���",L"��ʾ",0);
		return 0;
	}
	else MessageBox(0,L"��ѡ���dll�ļ�����ȷ��������ѡ��",L"����",0);
	return -1;
}
