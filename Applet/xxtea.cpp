#include  <string.h>
#include  <stdio.h>
//#include  <absacc.h>
//#include  <intrins.h>

 

#define MX (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z)
#define  delta     0x9e3779b9

/* 注意：delta的取值是随机的，但是为了避免不良的取值，采取的是
黄金分割数（根号5－2）/2与2的32次方的乘积。为0x9e3779b9。   
在解密中，sum＝delta×round，如：delta×32＝13C6EF3720*/     
//unsigned long buffer[];
//unsigned long keybuffer[]; 

long btea(long* v,char n,long* k);

long  btea(long* v, char n, long* k) 
{ 
    unsigned long z=v[n-1], y=v[0], sum=0, e, DELTA=0x9e3779b9;
    long p, q ;
    if (n > 1) 
 {          
 /* Coding Part */
      q = 6 + 52/n;
      while (q-- > 0) {
        sum += DELTA;
        e = (sum >> 2) & 3;
        for (p=0; p<n-1; p++) y = v[p+1], z = v[p] += MX;
        y = v[0];
        z = v[n-1] += MX;
      }
      return 0 ; 
    } 
 else if (n < -1) 
 {  
  /* Decoding Part */
      n = -n;
      q = 6 + 52/n;
      sum = q*DELTA ;
      while (sum != 0) {
        e = (sum >> 2) & 3;
        for (p=n-1; p>0; p--) z = v[p-1], y = v[p] -= MX;
        z = v[n-1];
        y = v[0] -= MX;
        sum -= DELTA;
      }
      return 0;
    }
    return 1;
}

int main()
 {
 long plainbuffer[2];
 long testbuffer[4];
 long plain[2]={0x23FF28AA,0xA76B4B04 };
 long test[4]={0x01122334,0x45566778,0x899AABBC,0xCDDEEFF0};
 long key[4]={0x01234567,0x89ABCDEF,0x01234567,0x89ABCDEF};

 while(1)
 {
  memcpy(plainbuffer,plain,8);
  memcpy(testbuffer,test,16);
  btea(plainbuffer, 2,key);
  btea(plainbuffer, -2,key);
  btea(testbuffer, 4,key);
  btea(testbuffer, -4,key);
  //_nop_;
 // btea(buffer, -2,keybuffer);

  }
}
