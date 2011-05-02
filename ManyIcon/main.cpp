#define UNICODE
#define _UNICODE
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = TEXT("Many Icons");
NOTIFYICONDATA nid;
int count = 0;
void CreateIcons(HWND hwnd)
{
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER;
    wcscpy(nid.szTip, szClassName);


    TCHAR path[MAX_PATH];
    TCHAR buff[MAX_PATH];

    GetModuleFileName(NULL, path, sizeof(path)/sizeof(TCHAR));
    *(wcsrchr(path,'\\')) = 0;


    wcscpy(buff,path);
    wcscat(buff,TEXT("\\icons\\*.ico"));
    wcscat(path,TEXT("\\icons\\"));


    //MessageBox(0,path,szClassName,0);

    WIN32_FIND_DATA ffbuf;
    HANDLE hfind = FindFirstFile(buff, &ffbuf);
    if (hfind != INVALID_HANDLE_VALUE)
    {
        do
        {
            wcscpy(buff,path);
            wcscat(buff,ffbuf.cFileName);
            nid.uID = 100+count;
            nid.hIcon = ExtractIcon(GetModuleHandle(NULL),buff,0);
            count++;
            Shell_NotifyIcon(NIM_ADD, &nid);
        }
        while (FindNextFile(hfind, &ffbuf));
        FindClose(hfind);
    }
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               szClassName,       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    //ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        CreateIcons(hwnd);
        if(count==0) DestroyWindow(hwnd);
        break;
    case WM_USER:
        switch (lParam)
        {
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
            DestroyWindow(hwnd);
            break;
        }
        break;
    case WM_DESTROY:
        for(;count>=0;count--)
        {
            nid.hWnd = hwnd;
            nid.uID = 100+count;
            Shell_NotifyIcon(NIM_DELETE, &nid);//删除图标
        }
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        if (message == RegisterWindowMessage(TEXT("TaskbarCreated")))//Explorer崩溃时重建图标
            CreateIcons(hwnd);
        else
            return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }

    return 0;
}
