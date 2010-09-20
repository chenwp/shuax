#ifndef _GUI_H_
#define _GUI_H_

#include <windows.h>
#include <stdio.h>
#include <time.h>

void InitGUI()
{
	//定义
	typedef struct _MARGINS
	{
		int cxLeftWidth;
		int cxRightWidth;
		int cyTopHeight;
		int cyBottomHeight;
	}
	MARGINS, *PMARGINS;
	MARGINS mar = { -1};
	typedef BOOL (WINAPI *MYFUN)(HWND, MARGINS*);
	MYFUN DwmExtendFrameIntoClientArea;
	//
	srand(time(NULL));
	//标题与大小
	SetConsoleTitle("贪吃蛇");
	system("mode con cols=81 lines=30");
	HWND hwnd = FindWindow("ConsoleWindowClass", "贪吃蛇");
	if (hwnd != NULL)
	{
		//动态加载DWMAPI，使得窗口玻璃化，VISTA以上系统适用
		HINSTANCE hFuncInst = LoadLibrary("dwmapi.DLL");
		if (hFuncInst != NULL)
		{
			DwmExtendFrameIntoClientArea = (MYFUN)GetProcAddress(hFuncInst, "DwmExtendFrameIntoClientArea");
			//DwmExtendFrameIntoClientArea(hwnd, &mar);
		}
		//窗口居中
		RECT rect;
		GetWindowRect(hwnd, &rect);
		rect.left = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
		rect.top = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;
		SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		//设置窗体样式
		SetWindowLong(hwnd, GWL_STYLE, WS_CAPTION | WS_VISIBLE | WS_SYSMENU);
	}
}

void DrawString(int x, int y, const char* format, ...)
{
	if (x < 0 || x >= 40) return;
	if (y < 0 || y >= 30) return;

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	csbiInfo.dwCursorPosition.X = 2 * x;
	csbiInfo.dwCursorPosition.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), csbiInfo.dwCursorPosition);
	va_list arg;
	va_start(arg, format);
	vprintf(format, arg);
	va_end(arg);
}

void InitWall()
{
	for (int i = 0; i < 40; i++)
	{
		DrawString(i,  0, "●");
		DrawString(i, 29, "●");
	}
	for (int i = 0; i < 29; i++)
	{
		DrawString( 0, i, "●");
		DrawString(39, i, "●");
	}
}

void InitLOGO()
{
	int i=8;
	DrawString(10,i++," _____   __   _       ___   _   _    _____  ");
	DrawString(10,i++,"/  ___/ |  \\ | |     /   | | | / /  | ____| ");
	DrawString(10,i++,"| |___  |   \\| |    / /| | | |/ /   | |__   ");
	DrawString(10,i++,"\\___  \\ | |\\   |   / / | | | |\\ \\   |  __|  ");
	DrawString(10,i++," ___| | | | \\  |  / /  | | | | \\ \\  | |___  ");
	DrawString(10,i++,"/_____/ |_|  \\_| /_/   |_| |_|  \\_\\ |_____| ");
	DrawString(10,++i,"贪吃蛇 v1.0 - By shuax build:2010.09.19");
	i=17;
	DrawString(10,i++,"◎：蛇的身体，不能吃到自己，不能去啃墙- -!");
	DrawString(10,i++,"◆：普通食物，吃掉一个加一分");
	DrawString(10,i++,"★：高分食物，吃掉一个加五分（每吃5个普通食物才会产生）");
	i=24;
	DrawString(13,i++,"-- 按下任意键开始游戏 --");
	DrawString(19,i++,"");
	getch();
}

#endif /*_GUI_H_*/
