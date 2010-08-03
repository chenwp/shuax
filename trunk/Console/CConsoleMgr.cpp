#include "consolemgr.h"

#define MAX_SHADOW_CONSOLE_INPUT_BUFFER  50

DWORD WINAPI ConsoleInputMonitor(LPVOID lParam);
BOOL WINAPI MyHandlerRoutine(DWORD  dwCtrlType);

CConsoleMgr::CConsoleMgr(void)
{
	ResetThreadInfo();
}

void CConsoleMgr::ResetThreadInfo()
{
	m_hConsoleThread = 0;
	m_idConsoleThread = 0;
}

CConsoleMgr::~CConsoleMgr(void)
{
	if (m_hConsoleThread && m_idConsoleThread)
		CloseConsole();
}

BOOL CConsoleMgr::CreateConsole(TCHAR * tstrName, BOOL bFullDosMode)
{
	m_hConsoleThread = CreateThread(NULL, 0, ConsoleInputMonitor,
			this, CREATE_SUSPENDED, &m_idConsoleThread);

	if (m_hConsoleThread)
	{
		SetThreadPriority(m_hConsoleThread, THREAD_PRIORITY_ABOVE_NORMAL);

		if (AllocConsole())
		{
			SetConsoleTitle(tstrName);
			SetConsoleCtrlHandler((PHANDLER_ROUTINE)MyHandlerRoutine, TRUE);
			if (bFullDosMode)
			{
				SetConsoleTitle("Sharing Full Screen DOS");
				keybd_event(VK_MENU, (BYTE)MapVirtualKey(VK_MENU, 0),
						0, GetMessageExtraInfo());
				// Simulate ENTER is pressed,
				keybd_event(VK_RETURN, (BYTE)MapVirtualKey(VK_RETURN, 0),
						0, GetMessageExtraInfo());
				// Simulate ENTER is released,
				keybd_event(VK_RETURN, (BYTE)MapVirtualKey(VK_RETURN, 0),
						KEYEVENTF_KEYUP, GetMessageExtraInfo());
				// Simulate ALT is released,
				keybd_event(VK_MENU, (BYTE)MapVirtualKey(VK_MENU, 0),
						KEYEVENTF_KEYUP, GetMessageExtraInfo());
			}

			SetThreadPriority(m_hConsoleThread, THREAD_PRIORITY_NORMAL);
			ResumeThread(m_hConsoleThread);
		}
		return TRUE;
	}
	return FALSE;
}

void CConsoleMgr::SetConsoleBufferSize( SMALL_RECT &writeRegion,
		COORD &bufferSize,
		COORD &bufferCoord)
{
	bufferSize.X  = 80;
	bufferSize.Y  = 25;
	bufferCoord.X = 0;
	bufferCoord.Y = 0;
	writeRegion.Top = 0;
	writeRegion.Left = 0;
	writeRegion.Bottom = 24;
	writeRegion.Right = 79;
}

BOOL CConsoleMgr::OutputToConsole(CConsoleOutInfo &outInfo)
{
	HANDLE    hConsoleOutput;
	COORD    CursorPosition;
	COORD    bufferSize;
	COORD    bufferCoord;
	SMALL_RECT   writeRegion;
	CHAR_INFO   buffer[25*80];
	CONSOLE_CURSOR_INFO ConsoleCursorInfo;

	CursorPosition.X = (SHORT)outInfo.m_cursorX;
	CursorPosition.Y = (SHORT)outInfo.m_cursorY;
	if ( outInfo.m_cursorStartLine == 0 )
		ConsoleCursorInfo.dwSize = 14; //Default size
	else
		ConsoleCursorInfo.dwSize = (DWORD)(0x10 - outInfo.m_cursorStartLine) * 100 / 0x10;

	ConsoleCursorInfo.bVisible = (BOOL)outInfo.m_cursorVisible;

// Convert the raw data to console screen buffer format.
	for (int i = 0; i < 25*80; i++)
	{
		buffer[i].Char.AsciiChar = outInfo.m_consoleBuffer[i*2];
		buffer[i].Attributes  = outInfo.m_consoleBuffer[i*2+1];
	}

	SetConsoleBufferSize(writeRegion, bufferSize, bufferCoord);

// Write texts to screen.
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleOutput(hConsoleOutput, buffer, bufferSize,
			bufferCoord, &writeRegion);

// Set cursor information.
	SetConsoleCursorInfo(hConsoleOutput, &ConsoleCursorInfo);

// Set cursor position.
	SetConsoleCursorPosition(hConsoleOutput, CursorPosition);
	return FALSE;
}

BOOL CConsoleMgr::CloseConsole()
{
	DWORD   dwEventNum;
	INPUT_RECORD Input_Record;

	Input_Record.EventType = MOUSE_EVENT;
	Input_Record.Event.MouseEvent.dwMousePosition.X = 0;
	Input_Record.Event.MouseEvent.dwMousePosition.Y = 0;
	Input_Record.Event.MouseEvent.dwButtonState  = 0;
	Input_Record.Event.MouseEvent.dwControlKeyState = 0;
	Input_Record.Event.MouseEvent.dwEventFlags  = MOUSE_MOVED;

	WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &Input_Record, 1, &dwEventNum);

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)MyHandlerRoutine, FALSE);

//Waiting for the thread termination.
	if ( WaitForSingleObject(m_hConsoleThread, 10000) == WAIT_TIMEOUT )
	{
		TerminateThread(m_hConsoleThread, 0);
	}
	CloseHandle(m_hConsoleThread);

	FreeConsole();
	ResetThreadInfo();

	return FALSE;
}

DWORD WINAPI ConsoleInputMonitor(LPVOID lParam)
{
	CConsoleMgr *pConsoleMgr = (CConsoleMgr *)lParam;
	if (pConsoleMgr == NULL) return -1;

	HANDLE   hConsoleInput;
	DWORD   dwInputs;
	INPUT_RECORD Input_Record[MAX_SHADOW_CONSOLE_INPUT_BUFFER];

	hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

	FlushConsoleInputBuffer(hConsoleInput);

	while ( pConsoleMgr->ThreadIsStart() )
	{
		hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

		// If there are input events in buffer, this object is signaled
		WaitForSingleObject(hConsoleInput, INFINITE);

		GetNumberOfConsoleInputEvents(hConsoleInput, &dwInputs);

		ReadConsoleInput(hConsoleInput, Input_Record,
				min(dwInputs, MAX_SHADOW_CONSOLE_INPUT_BUFFER),
				&dwInputs);

		pConsoleMgr->ProcessConsoleInput(Input_Record, 0);
	}

	return 0;
}

BOOL WINAPI MyHandlerRoutine(DWORD  dwCtrlType)
{
	return TRUE;
}