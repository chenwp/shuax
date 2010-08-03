#include "stdio.h"
#include "windows.h"
static char B64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char *base64_decode(char *s)
{
	char *p = s, *e, *r, *_ret;
	int len = strlen(s);
	unsigned char i, unit[4];
	
	e = s + len;
	len = len / 4 * 3 + 1;
	r = _ret = (char *)malloc(len);
	
	while (p < e) {
		memcpy(unit,p,4);
		if (unit[3] == '=')
			unit[3] = 0;
		if (unit[2] == '=')
			unit[2] = 0;
		p += 4;
		
		for (i=0; unit[0]!=B64[i] && i<64; i++);
		unit[0] = i==64 ? 0 : i;
		for (i=0; unit[1]!=B64[i] && i<64; i++);
		unit[1] = i==64 ? 0 : i;
		for (i=0; unit[2]!=B64[i] && i<64; i++);
		unit[2] = i==64 ? 0 : i;
		for (i=0; unit[3]!=B64[i] && i<64; i++);
		unit[3] = i==64 ? 0 : i;
		*r++ = (unit[0]<<2) | (unit[1]>>4);
		*r++ = (unit[1]<<4) | (unit[2]>>2);
		*r++ = (unit[2]<<6) | unit[3];
	}
	*r = 0;
	
	return _ret;
}
int main()
{
	system("wmic process where name=\"QQGame.exe\" list full > 1.txt");
	SetFileAttributes("1.txt", FILE_ATTRIBUTE_HIDDEN);
	puts("�ڳ����Ŀ¼�������� 1.txt �ļ���������һ��");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	puts("CommandLine=\"QQ��Ϸ·��\" /START QQUIN:QQ���� PWDHASH:����HASHֵ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	puts("������HASHֵ���Ƶ��˴������磺ETm5LK05yOVlHHfKc2xgsA==");
	system("notepad.exe 1.txt");
	char s[512];
	printf("PWDHASH:");
    gets(s);
	printf("����md5Ϊ:");
	char *de;
	de = base64_decode(s);
	int i;
	for(i=0;de[i]!=0;i++)
	printf("%02X",(unsigned char)de[i]);
	free(de);
	getchar();
	DeleteFile("1.txt");
	return 0;
}
