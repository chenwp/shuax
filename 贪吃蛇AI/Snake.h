#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <math.h>
void DrawString(int x, int y, const char* format, ...)
{
	static int run = 0;
	while(run) Sleep(1);
	run=1;
	
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	csbiInfo.dwCursorPosition.X = 2 * x;
	csbiInfo.dwCursorPosition.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), csbiInfo.dwCursorPosition);
	va_list arg;
	va_start(arg, format);
	vprintf(format, arg);
	va_end(arg);
	
	run=0;
}

//̰���߽ڵ㣬˫������
typedef struct SnakeNode
{
	POINT Location;
	SnakeNode *next;
	SnakeNode *back;
} SnakeNode, *SnakeNodeList;

class Snake
{
private:
	SnakeNodeList Tail;		//�ߵ�β��

	int Orientation;		//�������
	POINT Location;			//��ʼ����
	char node[8];			//������ʾ

	void Init();			//��ʼ��
	void Add();				//�ƶ�ͷ��
	void Del();				//�ƶ�β��
	bool CheckSnake();		//����Ƿ�Ե����Լ�
	void Rebuild();			//�ؽ�����
	
	friend void Player(LPVOID param);
	friend void Computer(LPVOID param);
	
	void Show();			//��ʾ����
	void MoveManu();		//�ֶ��ƶ�
	void MoveAuto();		//�ֶ��ƶ�
	
	Snake *Mutex;			//�������
public:
	
	SnakeNodeList Head;		//�ߵ�ͷ��
	Snake(const char* style,bool Automatic);		//��ʼ��̰���ߣ�����Ϊ������ʽ���Ƿ��Զ��ƶ�
	void SetMutex(Snake *snake);					//�����������
};

Snake::Snake(const char* style,bool Automatic)
{
	strcpy(node, style);
	Init();
	if(Automatic) _beginthread(Computer, 0, this);
	else  _beginthread(Player, 0, this);
}

void Snake::SetMutex(Snake *snake)
{
	Mutex = snake;
}
//˽�к���

void Player(LPVOID param)
{
	Snake *handle = (Snake *) param;
	handle->MoveManu();
}
void Computer(LPVOID param)
{
	Snake *handle = (Snake *) param;
	handle->MoveAuto();
}
void Snake::Show()
{
	int i = 0;
	SnakeNodeList temp = Tail;
    while (temp)
	{
		DrawString(temp->Location.x, temp->Location.y, node);
		temp = temp->back;
		i += 20;
		Sleep(i > 100 ? 100 : i);
	}
}
void Snake::MoveManu()
{
	Show();
	while (1)
	{
		Add();
		if (CheckSnake())
		{

			DrawString(1, 1, "�����!");
			getch();
			Rebuild();
		}
		DrawString(Head->Location.x, Head->Location.y, node);
		DrawString(Tail->Location.x, Tail->Location.y, "��");
		Del();

		if (kbhit())
		{
			switch (getch())
			{
			case 0x1b://����ESC����ͣ��Ϸ
				getch();
				break;
			case 0xE0://���·�������ı���ͷ����
				switch (getch())
				{
				case 75: if (Orientation%2!=0) Orientation=0; break;
				case 72: if (Orientation%2==0) Orientation=1; break;
				case 77: if (Orientation%2!=0) Orientation=2; break;
				case 80: if (Orientation%2==0) Orientation=3; break;
				}
				break;
			}
		}
		Sleep(100);
	}
}
void Snake::MoveAuto()
{
	Show();
	while (1)
	{
		Add();
		if (CheckSnake())
		{
			DrawString(1, 1, "�����!");
			getch();
			Rebuild();
		}
		DrawString(Head->Location.x, Head->Location.y, node);
		DrawString(Tail->Location.x, Tail->Location.y, "��");
		Del();
		
		if(rand() % 100>95) Orientation = 1 + Orientation%2;
		
		SnakeNodeList temp = Mutex->Head;
		double Distance;
		while (temp)
		{
			Distance = sqrt((temp->Location.x-Head->Location.x)*(temp->Location.x-Head->Location.x)+(temp->Location.y-Head->Location.y)*(temp->Location.y-Head->Location.y));
			if(Distance<2)
			{
				if(Orientation%2!=0)
				{
					//��ֱ����
					if(temp->Location.x-Head->Location.x<0) Orientation = 2;
					else Orientation = 0;
				}
				else
				{
					if(temp->Location.y-Head->Location.y<0) Orientation = 3;
					else Orientation = 1;
				}
				break;
			}
			temp = temp->back;
		}
		
		Sleep(100);
	}
}
bool Snake::CheckSnake()
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
void Snake::Rebuild()
{
	SnakeNodeList temp = Head;
	while (temp)
	{
		DrawString(temp->Location.x, temp->Location.y, "��");
		temp = temp->next;
	}
	Init();
}
void Snake::Init()
{
	Orientation = rand() % 4;
	Location.x = 14 + rand() % 10;
	Location.y = 14 + rand() % 5;
	int Length = rand() % 4 + 8;//���峤��

	Head = (SnakeNodeList)malloc(sizeof(SnakeNode));
	Head->Location.x = Location.x;
	Head->Location.y = Location.y;
	Head->next = NULL;
	Head->back = NULL;
	Tail = Head;
	SnakeNodeList temp;
	for (int i = 1; i < Length; i++)
	{
		temp = (SnakeNodeList)malloc(sizeof(SnakeNode));
		switch (Orientation)
		{
		case 0:Location.x++; break;
		case 1:Location.y++; break;
		case 2:Location.x--; break;
		case 3:Location.y--; break;
		}
		temp->Location.x = Location.x;
		temp->Location.y = Location.y;
		temp->next = NULL;

		temp->back = Tail;
		Tail->next = temp;

		Tail = temp;
	}
	temp = Tail;
}
//���̰����ͷ��ͷ������
void Snake::Add()
{
	SnakeNodeList temp;
	temp = (SnakeNodeList)malloc(sizeof(SnakeNode));

	POINT Location = Head->Location;
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
	temp->next = Head;
	Head->back = temp;
	Head = temp;
}
//ɾ��̰����β��β��
void Snake::Del()
{
	SnakeNodeList p = Tail;
	Tail = Tail->back;
	Tail->next = NULL;
	free(p);
}
#endif /*_SNAKE_H_*/
