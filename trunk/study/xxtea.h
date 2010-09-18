/******************************************************************
XXTEA�㷨

	long btea(long* v, long n, long* k)
	v��Ҫ���ܵ���Ԫ����ʼ��ַ����32bitΪ��λ��������long��ʵ�֡�
	n��Ҫ���ܵ���Ԫ�����������Ǽ��ܣ������ǽ��ܡ�
	k����Կ����ʼ��ַ������Ϊ4����Ԫ��4*32=128bit��
	����ֵΪ0��1����Ӧn=0��û�м��㣩��
	���ܵĽ����ֱ��д�ص�v�С�

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