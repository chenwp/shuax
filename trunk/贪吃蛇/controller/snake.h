#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <windows.h>
#include <stdio.h>

int Orientation;//蛇头方向，0-3：左上右下

int food;//食物
POINT food_loca;//食物坐标

int score;//分数

int node;//吃掉的食物

int award;//奖励
POINT award_loca;//奖励坐标

//初始化贪吃蛇
void InitSnake(char* src)
{
	Orientation = rand() % 4;
	
	food = 0;
	score = 0;
	node = 0;
	award = 0;
	
	POINT Location;
	Location.x = 20 + rand()%5;
	Location.y = 10 + rand()%5;
	
	int Length = rand() % 3 + 8;
	
	CreateSnakeNodeList(Head, Tail, Location, Orientation, Length);
	SnakeNodeList temp = Head;
	while (temp)
	{
		DrawString(temp->Location.x, temp->Location.y, src);
		temp = temp->next;
	}
}
void CreateFood()
{
	static int time = 0;

	if (time > 20 || time == 0)
	{
		if (food == 0)
		{
			food_loca.x = rand() % 35 + 2;
			food_loca.y = rand() % 25 + 2;
			DrawString(food_loca.x, food_loca.y, "◆");
			food = 1;
			time = 15;
		}
		else
		{
			if (time > 80)
			{
				DrawString(food_loca.x, food_loca.y, "　");
				food_loca.x = rand() % 35 + 2;
				food_loca.y = rand() % 25 + 2;
				DrawString(food_loca.x, food_loca.y, "◆");
				time = 15;
			}
		}
	}
	if (!award) time ++;
}
void CreateAward()
{
	static int time = 0;
	static int old = 0;
	if (node && node % 5 == 0 && award == 0 && node != old)
	{
		while(1)
		{
			award_loca.x = rand() % 35 + 2;
			award_loca.y = rand() % 25 + 2;
			if(award_loca.x != food_loca.x && award_loca.y != food_loca.y) break;
		}
		DrawString(award_loca.x, award_loca.y, "★");
		award = 1;
		node = old;
		time = 0;
	}
	if (time > 30 && award)
	{
		DrawString(award_loca.x, award_loca.y, "　");
		award = 0;
		time = 0;
	}
	if (award) time++;
}
bool CheckSnake()
{
	bool result = false;
	SnakeNodeList temp = Head->next;
	while (temp)
	{
		if (temp->Location.x == Head->Location.x && temp->Location.y == Head->Location.y)
		{
			result = true;
			break;
		}
		temp = temp->next;
	}
	return result;
}
bool CheckWall()
{
	bool result = false;
	if (Head->Location.x == 0 || Head->Location.x == 39 || Head->Location.y == 0 || Head->Location.y == 29) result = true;
	return result;
}
bool CheckFood()
{
	if (!food) return false;
	bool result = false;
	SnakeNodeList temp = Head;
	while (temp)
	{
		if (temp->Location.x == food_loca.x && temp->Location.y == food_loca.y)
		{
			result = true;
			node++;
			score += 1;
			food = 0;
			break;
		}
		temp = temp->next;
	}
	return result;
}
bool CheckAward()
{
	if (!award) return false;
	bool result = false;
	SnakeNodeList temp = Head;
	while (temp)
	{
		if (temp->Location.x == award_loca.x && temp->Location.y == award_loca.y)
		{
			result = true;
			score += 5;
			award = 0;
			break;
		}
		temp = temp->next;
	}
	return result;
}
void LevelUp()
{
	//50高分 
}

#endif /*_SNAKE_H_*/
