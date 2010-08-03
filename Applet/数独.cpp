#include <stdio.h>
#include <conio.h>
#include <windows.h>

class Sudoku
{
private:
	struct Grid
	{
		int number[10];
		bool isok;
		int result;
	} SUDOK[10][10];
	bool isok;
	char sz[81];
	//显示方面的函数
	HANDLE consolehwnd;
	void GotoXY(int x, int y)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(consolehwnd, &csbiInfo);
		csbiInfo.dwCursorPosition.X = x;
		csbiInfo.dwCursorPosition.Y = y;
		SetConsoleCursorPosition(consolehwnd, csbiInfo.dwCursorPosition);
	}
	inline void SetColor(int color)
	{
		SetConsoleTextAttribute(consolehwnd, color);
	}
	//清除行、列、宫的指定数字x
	void ClearRow(int Row, int x)
	{
		for (int i = 1; i < 10; i++)
		{
			if (!SUDOK[Row][i].isok) SUDOK[Row][i].number[x] = 0;
		}
	}
	void ClearCol(int Col, int x)
	{
		for (int i = 1; i < 10; i++)
		{
			if (!SUDOK[i][Col].isok) SUDOK[i][Col].number[x] = 0;
		}
	}
	void ClearAra(int Row, int Col, int x)
	{
		for (int m = Row - (Row - 1) % 3; m < Row - (Row - 1) % 3 + 3; m++)
		{
			for (int n = Col - (Col - 1) % 3; n < Col - (Col - 1) % 3 + 3; n++)
			{
				if (!SUDOK[m][n].isok) SUDOK[m][n].number[x] = 0;
			}
		}
	}
	//
	int GetFill(int Row, int Col)
	{
		int counter = 0;
		if (SUDOK[Row][Col].isok) return 0;
		int temp = 0;
		for (int k = 1; k < 10 ; k++)
			if (SUDOK[Row][Col].number[k] == k)
			{
				counter++;
				temp = k;
			}
		if (counter != 1) temp = 0;
		return temp;
	}
	//清除
	void Clear()
	{
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				if (SUDOK[i][j].isok)
				{
					ClearRow(i, SUDOK[i][j].result);
					ClearCol(j, SUDOK[i][j].result);
					ClearAra(i, j, SUDOK[i][j].result);
				}
			}
		}
	}
	void comp(int Row, int Col, int x)
	{
		if (x == 0) return ;
		SUDOK[Row][Col].isok = true;
		SUDOK[Row][Col].result = x;
		GotoXY(Col*4 - 2, Row*2 - 1);
		char s[] = "０";
		s[1] += x;
		printf(s);
		Sleep(50);
	}
	void showAll()
	{
		for (int k = 0; k < 81; k++)
		{
			int Row = k/9+1;
			int Col = k%9+1;
			if (!SUDOK[Row][Col].isok)
			{
				GotoXY(Col*4 - 2, Row*2 - 1);
				char s[] = "０";
				s[1] += sz[k];
				printf(sz[k] ? s : "　");
				Sleep(50);
			}
			
		}
		isok = true;
		GotoXY(1, 19);
		printf("                                  ");
		GotoXY(1, 19);
	}
	//填数字
	void fill()
	{
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				comp(i, j, GetFill(i, j));
			}
		}
	}
	//检查
	void Check()
	{
		for (int i = 1; i < 10; i++)
		{
			for (int k = 1; k < 10; k++)
			{
				int flag = 0;
				int last[2];
				for (int j = 1; j < 10; j++)
				{
					if (SUDOK[i][j].isok && SUDOK[i][j].result == k)
					{
						flag = 0;
						break;
					}
					if (!SUDOK[i][j].isok && SUDOK[i][j].number[k] == k)
					{
						flag++;
						last[0] = i;
						last[1] = j;
					}
				}
				if (flag == 1) comp(last[0], last[1], k);
			}
		}
		for (int j = 1; j < 10; j++)
		{
			for (int k = 1; k < 10; k++)
			{
				int flag = 0;
				int last[2];
				for (int i = 1; i < 10; i++)
				{
					if (SUDOK[i][j].isok && SUDOK[i][j].result == k)
					{
						flag = 0;
						break;
					}
					if (!SUDOK[i][j].isok && SUDOK[i][j].number[k] == k)
					{
						flag++;
						last[0] = i;
						last[1] = j;
					}
				}
				if (flag == 1) comp(last[0], last[1], k);
			}
		}
		//九个宫格循环i,j
		for (int i = 1; i < 10; i += 3)
		{
			for (int j = 1; j < 10; j += 3)
			{
				//尝试每个数字k
				for (int k = 1; k < 10; k++)
				{
					int flag = 0;
					int last[2];
					int first[2];
					bool b_first = false;

					//宫内循环m,n
					for (int m = i - (i - 1) % 3; m < i - (i - 1) % 3 + 3; m++)
					{
						for (int n = j - (j - 1) % 3; n < j - (j - 1) % 3 + 3; n++)
						{
							if (SUDOK[m][n].isok && SUDOK[m][n].result == k) goto end;
							if (!SUDOK[m][n].isok && SUDOK[m][n].number[k] == k)
							{
								flag++;
								if (b_first)
								{
									first[0] = m;
									first[1] = n;
									b_first = false;
								}
								last[0] = m;
								last[1] = n;
							}
						}
					}

					if (flag == 1) comp(last[0], last[1], k);
					//检查两个数字
					if (flag == 2)
					{
						if (last[0] == first[0])
						{
							SUDOK[first[0]][first[1]].isok = true;
							SUDOK[first[0]][last[1]].isok = true;
							ClearRow(first[0], k);
							SUDOK[first[0]][first[1]].isok = false;
							SUDOK[first[0]][last[1]].isok = false;
						}
						if (last[1] == first[1])
						{
							SUDOK[first[0]][first[1]].isok = true;
							SUDOK[last[0]][first[1]].isok = true;
							ClearCol(first[1], k);
							SUDOK[first[0]][first[1]].isok = false;
							SUDOK[last[0]][first[1]].isok = false;
						}
					}
				}
end:;
			}
		}
	}
	void show()
	{
		system("CLS");
		consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
		SetColor(15);
		GotoXY(0, 0);
		printf("┏━┯━┯━┳━┯━┯━┳━┯━┯━┓\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┠─┼─┼─╂─┼─┼─╂─┼─┼─┨\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┠─┼─┼─╂─┼─┼─╂─┼─┼─┨\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┣━┿━┿━╋━┿━┿━╋━┿━┿━┫\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┠─┼─┼─╂─┼─┼─╂─┼─┼─┨\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┠─┼─┼─╂─┼─┼─╂─┼─┼─┨\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┣━┿━┿━╋━┿━┿━╋━┿━┿━┫\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┠─┼─┼─╂─┼─┼─╂─┼─┼─┨\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┠─┼─┼─╂─┼─┼─╂─┼─┼─┨\n");
		printf("┃　│　│　┃　│　│　┃　│　│　┃\n");
		printf("┗━┷━┻━┻━┷━┷━┻━┷━┷━┛\n");
		SetColor(6);
		for (int j = 1; j < 10; j++)
			for (int i = 1; i < 9; i++)
			{
				GotoXY(4*j - 2, 2*i);
				if (i % 3 != 0)printf("─");
				GotoXY(4*i, 2*j - 1);
				if (i % 3 != 0)printf("│");
				GotoXY(4*i, 2*j);
				if (i % 3 != 0 && j % 3 != 0)printf("┼");
			}
		GotoXY(1, 19);
	}
	void force(int k)
	{
		if (isok) return;
		if (sz[k] == 0)
		{
			for (int m = 1; m <= 9; m++)
			{
				if (SUDOK[k/9+1][k%9+1].number[m] == 0) continue;
				int mm = true;
				for (int n = 0; n < 9; n++)
				{
					if ((m == sz[k/27*27+(k%9/3)*3+n+n/3*6]) || (m == sz[9*n+k%9]) || (m == sz[k/9*9+n]))
					{
						mm = false;
						break;
					}
				}
				if (mm)
				{
					sz[k] = m;
					if (k == 80)
					{
                        showAll();
						return;
					}
					force(k + 1);
				}
			}
			sz[k] = 0;
		}
		else
		{
			if (k == 80)
			{
                showAll();
				return;
			}
			force(k + 1);
		}
	}
public:
	Sudoku(char *FileName)
	{
		show();
		SetColor(14);
		FILE *fp = fopen(FileName, "rb");
		if (fp != NULL) printf("正在读取文件...");
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				for (int k = 1; k < 10; k++) SUDOK[i][j].number[k] = k;
				SUDOK[i][j].isok = false;
				SUDOK[i][j].result = 0;

				char ch = fgetc(fp);
				if (ch == 0x0D)
				{
					ch = fgetc(fp);
					ch = fgetc(fp);
				}
				if (ch >= '1' && ch <= '9')
				{
					SUDOK[i][j].isok = true;
					SUDOK[i][j].result = ch - '0';
					comp(i, j, ch - '0');
				}
			}
		}
		fclose(fp);
		SetColor(10);
	}
	void Logic()
	{
		GotoXY(1, 19);
		printf("                                  ");
		GotoXY(1, 19);
		printf("正在进行第一步处理...");
		Clear();
		fill();
		Check();
	}
	int getNumber()
	{
		int counter = 0;
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				if (SUDOK[i][j].isok) counter++;
			}
		}
		return counter;
	}
	void forceInit()
	{
		GotoXY(1, 19);
		printf("                                  ");
		GotoXY(1, 19);
		printf("正在进行第二步处理...");
		int x = 0;
		isok = false;
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				sz[x++] = SUDOK[i][j].result;
			}
		}
		SetColor(6);
		force(0);
		SetColor(10);
	}
};
int main(int argc, char *argv[])
{
	Sudoku sd(argc == 2 ? argv[1] : (char *)"sudoku.txt");
	while (1)
	{
		int old = sd.getNumber();
		sd.Logic();
		if (sd.getNumber() == old) break;
	}
	sd.forceInit();
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	csbiInfo.dwCursorPosition.X = 1;
	csbiInfo.dwCursorPosition.Y = 19;
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), csbiInfo.dwCursorPosition);
	printf("完成题目");
	getch();
}