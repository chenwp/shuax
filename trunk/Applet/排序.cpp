#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 20
/**************************************************
*   Quick sort by keefo 2008.03.16
**************************************************/
void quicksort(int a[], int p, int r)
{
	int m = p, n = r, pivot = a[(p+r)/2], temp;
	/*选取pivot,这个选取pivot有其它一些简单技巧以防治程序在特殊数据下的恶化*/
	/*在计算机程序设计艺术搜索卷有介绍*/
	do
	{
		/*比pivot小的元素放在左边，比pivot大的放在右边*/
		while (a[m] < pivot && m < r) m++;
		while (a[n] > pivot && n > p) n--;
		if (m <= n)
		{
			temp = a[m];
			a[m] = a[n];
			a[n] = temp;
			m++;
			n--;
		}
	}
	while (m <= n);

	if (m < r) quicksort(a, m, r);
	if (n > p) quicksort(a, p, n);
}


/**************************************************
*   Merge sort by keefo 2008.03.16
**************************************************/
int temp[NUM*2];

void mergesort(int a[], int l, int r)
{
	if (l == r) return;
	int i, j, k, m = (l + r) >> 1;
	mergesort(a, l, m);
	mergesort(a, m + 1, r);
	i = l; j = m + 1;
	for (k = l; k <= r; k++)
	{
		if (i <= m && a[i] <= a[j] || j > r) temp[k] = a[i++];
		else temp[k] = a[j++];
	}
	memcpy(a + l, temp + l, (r - l + 1)*sizeof(int));
	return;
}

/**************************************************
*   Counting sort by keefo 2008.03.16
**************************************************/
void countingsort(int a[], int n)
{
	long *C, *B;
	int max = -1;
	int i;
	for (int i = 0; i < n; i++)
	{
		if (a[i] > max)
			max = a[i];
	}

	C = (long*)malloc(sizeof(long) * (max + 1));
	B = (long*)malloc(sizeof(long) * (n + 1));
	for (i = 0; i <= max; i++)
		C[i] = 0;
	for (i = 0; i < n; i++)
		C[a[i]]++;
	for (i = 1; i <= max; i++)
		C[i] += C[i-1];
	for (i = n - 1; i >= 0; i--)
	{
		B[C[a[i]]] = a[i];
		C[a[i]] = C[a[i]] - 1;
	}
	for (i = 0; i < n; i++)
	{
		a[i] = B[i+1];
	}
	free(C);
	free(B);
}

/**************************************************
*   Test sort by keefo 2008.03.16
**************************************************/
int main(int argc, char* argv[])
{
	int a[10] = {2, 3, 1, 5, 8, 7, 6};
	quicksort(a, 0, 6);
	mergesort(a, 0, 6);
	countingsort(a, 7);
	getchar();
	return 0;
}