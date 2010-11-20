#pragma once

#define N 8
TCHAR str[][4]= {_T(""),_T("↑"),_T("→"),_T("↓"),_T("←"),_T("↗"),_T("↘"),_T("↙"),_T("↖")};

void ShowMouse(PVOID pvoid);

static class Gesture
{
private:
	POINT now;
	long time;
	TCHAR command[26];
	int count[N];//0-8
	int result;
	int  getOrientation(POINT a, POINT b);
public:
	Gesture();
	bool isrun;
	HWND hWnd;
	vector <POINT> m_pt;
	void begin(POINT pt);
	void add(POINT pt);
	int  getDistance(POINT a, POINT b);
	TCHAR* end();
} gr;
Gesture::Gesture()
{

}
void Gesture::begin(POINT pt)
{
	f_hwnd = WindowFromPoint(pt);

	isrun = true;
	time = GetCurrentTime();
	now = pt;
	for(int i=0; i<N; i++) count[i]=0;
	command[0]=0;
	result=0;
	//显示窗口
	m_pt.clear();
	m_pt.push_back(pt);
	if(myset.m_shw) _beginthread(ShowMouse,0,NULL);
}
TCHAR* Gesture::end()
{
	if(GetCurrentTime()-time>1500) command[0]=0;
	isrun = false;
	if(myset.m_shw) SendMessage(hWnd, WM_CLOSE, 1, 0);
	return command;
}
void Gesture::add(POINT pt)
{
	//超时
	if(GetCurrentTime()-time>1500)
	{
		if(myset.m_shw) SendMessage(hWnd, WM_CLOSE, 1, 0);
		return;
	}
	//刷新界面
	if (myset.m_shw && getDistance(pt, now) > 2)
	{
		m_pt.push_back(pt);
		InvalidateRect(hWnd, NULL, FALSE);
		ShowWindow(hWnd,SW_SHOW);
	}

	if (getDistance(pt, now) > 4)
	{
		//统计方向
		count[getOrientation(pt, now)]++;
		for(int i = 1; i<N; i++)
		{
			if(count[i]>8 && result!=i)
			{
				wcscat_s(command,str[i]);
				result = i;
				for(int i=0; i<N; i++) count[i]=0;
			}
		}
		now = pt;
	}
}
inline float SquareRootFloat(float number)
{
	long i;
	float x, y;
	const float f = 1.5F;
	x = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;
	i  = 0x5f375a86 - ( i >> 1 );	//注意这一行
	y  = * ( float * ) &i;
	y  = y * ( f - ( x * y * y ) );
	return number * y;
}
inline int _floor(float f)
{
	int a         = *(int*)(&f);
	int sign      = (a>>31);
	int mantissa  = (a&((1<<23)-1))|(1<<23);
	int exponent  = ((a&0x7fffffff)>>23)-127;
	int r         = ((unsigned int)(mantissa)<<8)>>(31-exponent);
	return ((r ^ (sign)) - sign ) &~ (exponent>>31);
}
int Gesture::getDistance(POINT a, POINT b)
{
	//优化sqrt，floor
	return _floor(SquareRootFloat((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)));
}
int Gesture::getOrientation(POINT a, POINT b)
{
	int orientation = 0;
	int x = a.x - b.x;
	int y = a.y - b.y;
	float delta = 2.414f;
	if (x >= 0 && y <= 0)
	{
		//第一象限
		y *= -1;
		if (x > y && delta*y < x) orientation = 2;
		if (x < y && delta*x < y) orientation = 1;
		if(orientation==0) orientation = 5;
		y *= -1;
	}
	else if (x >= 0 && y >= 0)
	{
		//第四象限
		if (x > y && delta*y < x) orientation = 2;
		if (x < y && delta*x < y) orientation = 3;
		if(orientation==0) orientation = 6;
	}
	else if (x<=0 && y>=0)
	{
		//第三象限
		x *= -1;
		if (x > y && delta*y < x) orientation = 4;
		if (x < y && delta*x < y) orientation = 3;
		if(orientation==0) orientation = 7;
		x *= -1;
	}
	else  if (x <= 0 && y <= 0)
	{
		//第二象限
		x *= -1;
		y *= -1;
		if (x > y && delta*y < x) orientation = 4;
		if (x < y && delta*x < y) orientation = 1;
		if(orientation==0) orientation = 8;
	}
	return orientation;
}

BOOL CALLBACK ShowMouseProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static    PAINTSTRUCT        Ps;
	static    HDC                Hdc;
	static    bool               isrun;
	static    long               time;
	switch (message)
	{
	case WM_INITDIALOG:
		gr.hWnd = hWnd;
		//界面初始化
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) &~ WS_EX_TRANSPARENT);
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW | SWP_NOACTIVATE);
		SetLayeredWindowAttributes(hWnd, 0xFFFFFF, 255, 2);
		ShowWindow(hWnd,SW_HIDE);
		ToolTipClassic(hWnd,L"鼠标手势已经启动，请在1.5秒内完成动作。");
		//
		isrun = true;
		time = GetCurrentTime();
		SetTimer(hWnd,1,40,0);
		break;
	case WM_CTLCOLORDLG: 
		return (int)GetStockObject(NULL_BRUSH);
	case WM_PAINT:
		if(isrun)
		{
			isrun = false;
			Hdc = BeginPaint(hWnd, &Ps);
			Graphics graphics(Hdc);
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);
			//graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			Pen pen(Color(255, 152, 204, 0), 3.5);
			if(gr.m_pt.size()>1)
			{
				int i =  gr.m_pt.size();
				for(;i>2;i--)
				{
					graphics.DrawLine(&pen,gr.m_pt[i-1].x,gr.m_pt[i-1].y,gr.m_pt[i-2].x,gr.m_pt[i-2].y);
				}
			}
			//graphics.DrawBeziers(&pen,(Point*)&gr.m_pt[0],(INT)gr.m_pt.size());
			pen.~Pen();
			graphics.ReleaseHDC(Hdc);

			EndPaint(hWnd, &Ps);
			SetTopMost(hWnd,true);
			isrun = true;
		}
		break;
	case WM_TIMER:
		if(GetCurrentTime()-time>1500) SendMessage(hWnd, WM_CLOSE, 1, 0);
		break;
	case WM_CLOSE:
		gr.m_pt.clear();
		EndDialog(hWnd, wParam);
		break;
	}
	return FALSE;
}
void ShowMouse(PVOID pvoid)
{
	static bool notrun = true;
	if(notrun)
	{
		notrun = false;
		_cprintf("Mouse\n");
		DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG2),0,ShowMouseProc,0);
		SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
		notrun = true;
	}
}