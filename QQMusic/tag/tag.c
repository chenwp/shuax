#include "tag.h"

const unsigned char asf_file_guid[16] = {0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C};/*ASF文件标志*/
const unsigned char asf_tags_guid[16] = {0x33, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C};/*ASF标志信息*/

typedef unsigned short uint16_t;
struct wma_tags
{
	uint16_t titleLength;
	uint16_t artistLength;
	uint16_t copyrightLength;
	uint16_t commentLength;
	uint16_t ratingLength;
} wma_tags;

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
	long *charStep = (long *)malloc(MAX_CHAR_SIZE * sizeof(long));
	for (i = 0; i < MAX_CHAR_SIZE; i++)
		charStep[i] = subStrLen + 1;
	for (i = 0; i < subStrLen; i++)
	{
		charStep[(unsigned char)subStr[i]] = subStrLen - i;
	}
	return charStep;
}
/*
   算法核心思想，从左向右匹配，遇到不匹配的看大串中匹配范围之外的右侧第一个字符在小串中的最右位置
   根据事先计算好的移动步长移动大串指针，直到匹配
*/
long sundaySearch(const unsigned char *mainStr, const unsigned char *subStr, long *charStep, long mainStrLen, int subStrLen)
{
	long main_i = 0;
	long sub_j = 0;
	while (main_i < mainStrLen)
	{
		//保存大串每次开始匹配的起始位置，便于移动指针
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
				//如果匹配范围外已经找不到右侧第一个字符，则匹配失败
				if (tem + subStrLen > mainStrLen)
					return -1;
				//否则 移动步长 重新匹配
				unsigned char firstRightChar = mainStr[tem + subStrLen];
				main_i += charStep[(unsigned char)firstRightChar];
				sub_j = 0;
				break; //退出本次失败匹配 重新一轮匹配
			}
		}
		if (sub_j == subStrLen)
			return main_i - subStrLen;
	}
	return -1;
}

DLLIMPORT int GetWmaTag(const char *filename, wchar_t *f_name)
{

	HANDLE hfile = CreateFile(filename,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	HANDLE hfilemap = CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);
	CloseHandle(hfile);

	unsigned char *pfile = (unsigned char*) MapViewOfFile(hfilemap, FILE_MAP_READ, 0, 0, 0);
	CloseHandle(hfilemap);
	if (memcmp( asf_file_guid, pfile, 16) != 0)
	{
		UnmapViewOfFile(pfile);
		return -1;
	}
	long *charStep = setCharStep(asf_tags_guid, 16);
	long answer;
	if ((answer = sundaySearch(pfile, asf_tags_guid, charStep, getFileSize(filename), 16)) >= 0)
	{
		answer += 24;
		struct wma_tags *tag = (struct wma_tags *) & pfile[answer];
		answer += sizeof(struct wma_tags);
		answer += tag->titleLength;
		wsprintfW(f_name, L"%s - %s.wma", tag->artistLength == 0 ? L"艺术家" : (wchar_t*)&pfile[answer], tag->titleLength == 0 ? L"标题" : (wchar_t*)&pfile[answer-tag->titleLength]);
	}
	else
	{
		UnmapViewOfFile(pfile);
		return -1;
	}
	UnmapViewOfFile(pfile);
	return 0;
}
