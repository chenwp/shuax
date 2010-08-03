#include <stdio.h>

int sum = 0, lim, p;

void dfs(int row, int ld, int rd)
{
	int pos;
	if (row == lim)
	{
		sum ++;
	}
	else
	{
		pos = lim & ~(row | ld | rd);
		while (pos)
		{
			p = pos & (-pos);
			pos -= p;
			dfs(row | p, (ld | p) << 1, (rd | p) >> 1);
		}
	}
}
int main()
{
	//int n;
	int n = 6;
	//std::cin >> n;
	lim = (1 << n) - 1;
	dfs(0, 0, 0);
	printf("%d",sum);
	getchar();
}