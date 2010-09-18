#include  <stdio.h>
#include  <string.h>
#include  "xxtea.h"

int main()
{
	long plain[2]={0x23FF28AA,0xA76B4B04 };
	long key[4]={0x21234567,0x82ABCDEF,0x01234567,0x89ABCDEF};

	printf("%X %X\n",plain[0],plain[1]);
	btea(plain,2, key);
	printf("%X %X\n",plain[0],plain[1]);
	
	btea(plain,-2, key);
	printf("%X %X\n",plain[0],plain[1]);
	
	getchar();
}
