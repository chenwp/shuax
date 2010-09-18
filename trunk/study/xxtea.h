/******************************************************************
XXTEA算法

	long btea(long* v, long n, long* k)
	v是要加密的组元的起始地址，以32bit为单位，这里用long来实现。
	n是要加密的组元个数，正数是加密，负数是解密。
	k是密钥的起始地址，长度为4个组元，4*32=128bit。
	返回值为0或1（对应n=0，没有计算）。
	加密的结果会直接写回到v中。

******************************************************************/
#ifndef ___XX_TEA_H_
#define ___XX_TEA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define MX (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);

long btea(long* v, long n, long* k)
{
	unsigned long z = v[n-1], y = v[0], sum = 0, e, DELTA = 0x9e3779b9;
	long p, q ;
	if (n > 1)
	{
		q = 6 + 52 / n;
		while (q-- > 0)
		{
			sum += DELTA;
			e = (sum >> 2) & 3;
			for (p = 0; p < n - 1; p++) y = v[p+1], z = v[p] += MX;
			y = v[0];
			z = v[n-1] += MX;
		}
		return 0 ;
	}
	else if (n < -1)
	{
		n = -n;
		q = 6 + 52 / n;
		sum = q * DELTA ;
		while (sum != 0)
		{
			e = (sum >> 2) & 3;
			for (p = n - 1; p > 0; p--) z = v[p-1], y = v[p] -= MX;
			z = v[n-1];
			y = v[0] -= MX;
			sum -= DELTA;
		}
		return 0;
	}
	return 1;
}

#ifdef __cplusplus
}
#endif

#endif /* ___XX_TEA_H_ */