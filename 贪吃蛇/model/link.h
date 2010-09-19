#ifndef _LINK_H_
#define _LINK_H_

#include <windows.h>
#include <stdio.h>

//贪吃蛇节点，双向链表
typedef struct SnakeNode
{
	POINT Location;
	SnakeNode *next;
	SnakeNode *back;
} SnakeNode, *SnakeNodeList;

SnakeNodeList Head;
SnakeNodeList Tail;

//创建贪吃蛇链表（头，尾，坐标，方向，长度）
void CreateSnakeNodeList(SnakeNodeList &head, SnakeNodeList &tail, POINT Location, int Orientation, int Length)
{
	head = (SnakeNodeList)malloc(sizeof(SnakeNode));
	head->Location.x = Location.x;
	head->Location.y = Location.y;
	head->next = NULL;
	head->back = NULL;
	tail = head;
	SnakeNodeList temp;
	for (int i = 1; i < Length; i++)
	{
		temp = (SnakeNodeList)malloc(sizeof(SnakeNode));
		switch (Orientation)
		{
		case 0:Location.x++;break;
		case 1:Location.y++;break;
		case 2:Location.x--;break;
		case 3:Location.y--;break;
		}
		temp->Location.x = Location.x;
		temp->Location.y = Location.y;
		temp->next = NULL;
		
		temp->back = tail;
		tail->next = temp;
		
		tail = temp;
	}
}

//添加贪吃蛇头（头，方向）
void AppendSnakeNodeList(SnakeNodeList &head, int Orientation)
{
	SnakeNodeList temp;
	temp = (SnakeNodeList)malloc(sizeof(SnakeNode));

	POINT Location = head->Location;
	switch (Orientation)
	{
	case 0:
		if (Location.x <= 0) Location.x = 40;
		Location.x--;
		break;
	case 1:
		if (Location.y <= 0) Location.y = 30;
		Location.y--;
		break;
	case 2:
		if (Location.x >= 40) Location.x = 0;
		else Location.x++;
		break;
	case 3:
		if (Location.y >= 30) Location.y = 0;
		else Location.y++;
		break;
	}
	temp->Location = Location;
	temp->back = NULL;
	temp->next = head;
	head->back = temp;
	head = temp;
}
//删除贪吃蛇尾（尾）
void DeleteSnakeNodeList(SnakeNodeList &tail)
{
	if(tail->back==NULL) return;
	
	SnakeNodeList p = tail;
	tail = tail->back;
	tail->next = NULL;
	free(p);
}


#endif /*_LINK_H_*/
