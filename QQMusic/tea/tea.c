#include "tea.h"
#include <stdio.h>
#include <windows.h>
void tea_encrypt(const DWORD* pData, DWORD* pEncData)
{
	DWORD  Key1 =  0x45AD9059;
	
	DWORD  FKey1 = 0xF03E934F;
	DWORD  FKey2 = 0x27BDB886;
	DWORD  LKey1 = 0xD0AAE945;
	DWORD  LKey2 = 0x993BA3AE;
	DWORD FirstData, LastData, Count, Key;
	
	Count = 32;
	Key = 0;
	FirstData = *(pData);
	LastData = *(pData + 1);

	do
	{
		Key = Key + Key1;
		FirstData = FirstData + ((FKey2 + (LastData << 4)) ^ (FKey1 + (LastData >> 5)) ^ (Key + LastData));
		LastData = LastData + ((LKey2 + (FirstData << 4)) ^ (LKey1 + (FirstData >> 5)) ^ (Key + FirstData));
		Count--;
	}
	while (Count != 0);
	*(pEncData) = FirstData;
	*(pEncData + 1) = LastData;
}

void tea_decrypt(DWORD* pData, const DWORD* pEncData)
{
	DWORD  Key1 =  0x45AD9059;

	DWORD  FKey1 = 0xF03E934F;
	DWORD  FKey2 = 0x27BDB886;
	DWORD  LKey1 = 0xD0AAE945;
	DWORD  LKey2 = 0x993BA3AE;
	DWORD FirstData, LastData, Count, Key;

	Count = 32;
	Key = Key1 << 5;//Key=（Key1*循环次数），这里是32即2的5次方
	FirstData = *(pEncData);
	LastData = *(pEncData + 1);

	do
	{
		LastData = LastData - ((LKey2 + (FirstData << 4)) ^ (LKey1 + (FirstData >> 5)) ^ (Key + FirstData));
		FirstData = FirstData - ((FKey2 + (LastData << 4)) ^ (FKey1 + (LastData >> 5)) ^ (Key + LastData));
		Key = Key - Key1;
		Count--;
	}
	while (Count != 0);
	*(pData) = FirstData;
	*(pData + 1) = LastData;
}
DLLIMPORT int decrypt(const char *file,char *path)
{
	FILE *in,*out;
	in=fopen(file,"rb");
	if(in==NULL)
	{
		return -1;
	}
	char temp[1924];
	strcpy(temp,path);
	strcat(temp,strrchr(file,'\\')+1);
	strcat(temp,".wma");
	out=fopen(temp,"wb");
	if(out==NULL)
	{
		return -1;
	}
	DWORD pData[2],pEncData[2];
	int i=0;
	while(!feof(in))
	{
		i = fread(pEncData,1,sizeof(DWORD)*2,in);
		if(i==sizeof(DWORD)*2)
		{
			tea_decrypt(pData,pEncData);
			fwrite(pData,1,sizeof(DWORD)*2,out);
		}
		else
		{
			fwrite(pEncData,1,i,out);
			continue;
		}
	}
	fclose(in);
	fclose(out);
	wchar_t name[1024];
	printf("Save:%s\t",strrchr(file,'\\')+1);
	if(GetWmaTag(temp,name)!=-1)
	{
		wchar_t utext[512];
		wchar_t upath[512];
		MultiByteToWideChar(CP_ACP, 0, temp, -1, utext, 512);
		MultiByteToWideChar(CP_ACP, 0, path, -1, upath, 512);
		wcscat(upath,name);
		if(MoveFileW(utext,upath)) puts("Changed its name");
		else puts("Rename failure");
	}
	else puts("Rename failure");
	return 0;
}
