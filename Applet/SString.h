#ifndef ___S_STRING_H_
#define ___S_STRING_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <malloc.h>

class String
{
private:
	char *str;
	int len;
	int strlen(const char *str)
	{
		int length = 0;
		while (*str++)
		{
			length++;
		}
		return length;
	}
	char *strcpy(char *dst, const char *src)
	{
		char *tmp = dst;
		while ((*tmp++ = *src++) != '\0');
		return dst;
	}
	char *strcat(char *dst, const char *src)
	{
		char *tmp   =   dst;
		while (*dst) dst++;
		while ((*dst++ =  *src++) != '\0');
		return tmp;
	}
	int strcmp(const char *dst, const char *src)
	{
		while (*dst && *src && (*dst == *src))
		{
			dst ++;
			src ++;
		}
		return *dst - *src;
	}
public:
	//构造、析构函数
	String();
	~String();
	String(const char *src);

	//重载操作符
	operator char *();//这个很强大、没这个好多函数都不能用了
	void  operator = (const char *src);
	char *operator + (const char *src);
	void operator += (const char *src);
	bool operator == (const char *src);
	bool operator != (const char *src);

	//实用函数
	int length() {return len - 1;}
	void replace(const char *dst, const char *src);
};

String::String()
{
	len = 1;
	str = (char *)malloc(len * sizeof(char));
	*str = 0;
}

String::~String()
{
	free(str);
}

String::String(const char *src)
{
	len = strlen(src) + 1;
	str = (char *)malloc(len * sizeof(char));
	strcpy(str, src);
}

String::operator char *()
{
	return str;
}

void String::operator = (const char * src)
{
	len = strlen(src) + 1;
	str = (char *)malloc(len * sizeof(char));
	strcpy(str, src);
}

char *String::operator + (const char *src)
{
	char *tmp = (char *)malloc((len + strlen(src)) * sizeof(char));
	strcpy(tmp , str);
	strcat(tmp , src);
	return tmp;
}

void String::operator += (const char * src)
{
	len += strlen(src);
	char *tmp = (char *)malloc(len * sizeof(char));
	strcpy(tmp , str);
	strcat(tmp , src);
	strcpy(str , tmp);
	free(tmp);
}

bool String::operator == (const char *src)
{
	return !strcmp(str, src);
}

bool String::operator != (const char *src)
{
	return strcmp(str, src);
}
#ifdef	__cplusplus
}
#endif

#endif /* ___S_STRING_H_ */
