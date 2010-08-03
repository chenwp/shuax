#ifndef __MEM_FILE_H_
#define __MEM_FILE_H_

char src[1024];
char temp[1024];
char path[1024];
	
long getFileSize(const char *path)
{
	long file_size ;
	FILE* fp;
	fp = fopen( path, "rb");
	fseek( fp, 0, SEEK_END);
	file_size = ftell(fp);
	fclose(fp);
	return file_size;
}

#define MAX_CHAR_SIZE 257

long *setCharStep(const unsigned char *subStr, long subStrLen)
{
	long i;
	static long charStep[MAX_CHAR_SIZE];
	for (i = 0; i < MAX_CHAR_SIZE; i++)
		charStep[i] = subStrLen + 1;
	for (i = 0; i < subStrLen; i++)
	{
		charStep[(unsigned char)subStr[i]] = subStrLen - i;
	}
	return charStep;
}
/*
   �㷨����˼�룬��������ƥ�䣬������ƥ��Ŀ�����ƥ�䷶Χ֮����Ҳ��һ���ַ���С���е�����λ��
   �������ȼ���õ��ƶ������ƶ���ָ�룬ֱ��ƥ��
*/
long sundaySearch(const unsigned char *mainStr, const unsigned char *subStr, long *charStep, long mainStrLen, int subStrLen)
{
	long main_i = 0;
	long sub_j = 0;
	while (main_i < mainStrLen)
	{
		//�����ÿ�ο�ʼƥ�����ʼλ�ã������ƶ�ָ��
		long tem = main_i;
		while (sub_j < subStrLen)
		{
			if (mainStr[main_i] == subStr[sub_j])
			{
				main_i++;
				sub_j++;
				continue;
			}
			else
			{
				//���ƥ�䷶Χ���Ѿ��Ҳ����Ҳ��һ���ַ�����ƥ��ʧ��
				if (tem + subStrLen > mainStrLen)
					return -1;
				//���� �ƶ����� ����ƥ��
				unsigned char firstRightChar = mainStr[tem + subStrLen];
				main_i += charStep[(unsigned char)firstRightChar];
				sub_j = 0;
				break; //�˳�����ʧ��ƥ�� ����һ��ƥ��
			}
		}
		if (sub_j == subStrLen)
			return main_i - subStrLen;
	}
	return -1;
}
long Search(const char *filename, const unsigned char *DATA,unsigned int count)
{

	HANDLE hfile = CreateFile(filename,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		return -2;
	}

	HANDLE hfilemap = CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);
	CloseHandle(hfile);

	unsigned char *pfile = (unsigned char*) MapViewOfFile(hfilemap, FILE_MAP_READ, 0, 0, 0);
	CloseHandle(hfilemap);

	long *charStep = setCharStep(DATA, count);
	long answer;
	if ((answer = sundaySearch(pfile, DATA, charStep, getFileSize(filename), count)) >= 0)
	{
		UnmapViewOfFile(pfile);
		return answer;
	}
	UnmapViewOfFile(pfile);
	return -1;
}

#endif /* __MEM_FILE_H_ */