#include <Windows.h>
#include <tchar.h>
class CConsoleOutInfo
{
public:
	int   m_cursorX;
	int   m_cursorY;
	int   m_cursorStartLine;
	int   m_cursorVisible;
	LPBYTE  m_consoleBuffer;
};

class CConsoleMgr
{
public:
	CConsoleMgr(void);
	virtual ~CConsoleMgr(void);

	BOOL CreateConsole(TCHAR * tstrName, BOOL bFullDosMode = FALSE);
	BOOL OutputToConsole(CConsoleOutInfo &outInfo);
	BOOL CloseConsole();

	BOOL ThreadIsStart() {return m_hConsoleThread && m_idConsoleThread;}

	virtual DWORD ProcessConsoleInput(INPUT_RECORD* pInputRec, DWORD dwInputs) = 0;
protected:
	void SetConsoleBufferSize(SMALL_RECT &writeRegion,COORD &bufferSize,COORD &bufferCoord);

	void ResetThreadInfo();
private:
	HANDLE m_hConsoleThread;
	DWORD m_idConsoleThread;
};