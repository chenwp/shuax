#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "model\link.h"
#include "view\gui.h"
#include "controller\snake.h"

void DispatchMessage()
{
	InitSnake("◎");
	//InitSnake("■");
	InitWall();

	while (1)
	{
		CreateFood();
		CreateAward();

		DrawString(10, 0, " Score:%04d ", score);
		AppendSnakeNodeList(Head, Orientation);
		CheckAward();
		if (CheckSnake() || CheckWall())
		{
			DrawString(2, 2, "你死啦！ 按任意键重新开始玩！");
			getch();
			break;
		}
		DrawString(Head->Location.x, Head->Location.y, "◎");

		if (!CheckFood())
		{
			DrawString(Tail->Location.x, Tail->Location.y, "　");
            DeleteSnakeNodeList(Tail);
		}
		int keep = 0;

		if (kbhit())
		{
			switch (getch())
			{
			case 0x1b://按下ESC，暂停游戏
				getch();
				continue;
			case 0xE0://按下特殊键
				switch (getch())
				{
				case 75: if(Orientation==0) keep = 1;if (Orientation%2!=0) Orientation=0; break;
				case 72: if(Orientation==1) keep = 1;if (Orientation%2==0) Orientation=1; break;
				case 77: if(Orientation==2) keep = 1;if (Orientation%2!=0) Orientation=2; break;
				case 80: if(Orientation==3) keep = 1;if (Orientation%2==0) Orientation=3; break;
				default: ;
				}
				break;
			}
		}

		if(!keep&&(100-score*2)>10)Sleep(100-score*2);//难度提升 
		else Sleep(10);
	}
}
int main()
{
	InitGUI();
	InitLOGO();
	
	while(1)
	{
		system("CLS");
		DispatchMessage();
	}

	return 0;
}
