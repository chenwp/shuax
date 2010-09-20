#include <windows.h>

#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "Snake.h"
void InitGUI()
{
	srand(time(NULL));
	SetConsoleTitle("Ì°³ÔÉß");
	system("mode con cols=82 lines=32");
	HWND hwnd = FindWindow("ConsoleWindowClass", "Ì°³ÔÉß");
	if (hwnd != NULL)
	{
		//´°¿Ú¾ÓÖÐ
		RECT rect;
		GetWindowRect(hwnd, &rect);
		rect.left = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
		rect.top = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;
		SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		//ÉèÖÃ´°ÌåÑùÊ½
		SetWindowLong(hwnd, GWL_STYLE, WS_CAPTION | WS_VISIBLE | WS_SYSMENU);
	}
}
void check(Snake *snake1,Snake *snake2)
{
	snake1->SetMutex(snake2);
	snake2->SetMutex(snake1);
    while (1)
	{
		Sleep(20);
	}
}
int main()
{
	InitGUI();

	Snake player("¡ò",0);
	Snake computer("¡ö",1);
	//getchar();
	check(&player,&computer);
	return 0;
}
