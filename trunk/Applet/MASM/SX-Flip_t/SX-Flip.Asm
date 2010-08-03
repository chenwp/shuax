.386

.model flat,stdcall
option casemap:none
include		windows.inc
include		user32.inc
include		kernel32.inc
include		shell32.inc

includelib	user32.lib
includelib	kernel32.lib
includelib	shell32.lib

WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD

.data
szError	db	'请注意，热键注册失败，你必须手动结束进程。',0

sxKey		db	'LOVEXH4EVER-Flip',0
ClassName	db	"SX-Flip 特别版",0

szRun		db	'rundll32.exe',0
szArg		db	'DwmApi #105',0

sztip	db	'%d %d %d %d',0
.data?
hInstance	dd	?
x	dd	?
y	dd	?
stat	dd	?
.code
WinMain proc hInst:HINSTANCE,hPrevInst:HINSTANCE,CmdLine:LPSTR,CmdShow:DWORD
    LOCAL wc:WNDCLASSEX
    LOCAL msg:MSG
    LOCAL hwnd:HWND
    
    invoke FindWindow, NULL, addr sxKey
    .if eax!=0
	invoke ExitProcess, 0
    .endif
    mov   wc.cbSize,SIZEOF WNDCLASSEX
    mov   wc.style, CS_HREDRAW or CS_VREDRAW
    mov   wc.lpfnWndProc, OFFSET WndProc
    mov   wc.cbClsExtra,NULL
    mov   wc.cbWndExtra,NULL
    push  hInst
    pop   wc.hInstance
    mov   wc.hbrBackground,COLOR_APPWORKSPACE
    mov   wc.lpszMenuName,NULL
    mov   wc.lpszClassName,OFFSET ClassName
    invoke LoadIcon,hInstance,NULL
    mov   wc.hIcon,eax
    mov   wc.hIconSm,eax
    invoke LoadCursor,NULL,IDC_ARROW
    mov   wc.hCursor,eax
    invoke RegisterClassEx, addr wc
    invoke CreateWindowEx,WS_EX_TOOLWINDOW,ADDR ClassName,ADDR sxKey,WS_POPUP,CW_USEDEFAULT,CW_USEDEFAULT,\
           CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL
    mov   hwnd,eax
    mov stat,0
    .while TRUE
        invoke GetMessage, ADDR msg,NULL,0,0
        .BREAK .IF (!eax)
        invoke TranslateMessage, ADDR msg
        invoke DispatchMessage, ADDR msg
    .endw
    mov eax,msg.wParam
    ret
WinMain endp
WndProc proc hwnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    LOCAL pt:POINT
    LOCAL rect:RECT
    LOCAL hwnds:HWND
    LOCAL tip[512]:BYTE
    .if uMsg==WM_CREATE
  	invoke RegisterHotKey,hwnd, 1, MOD_WIN, VK_END
  	.if eax==0
  		invoke MessageBox,0,addr szError,addr ClassName,MB_OK + MB_ICONINFORMATION
  	.endif
  	invoke SetTimer,hwnd,1,1000,NULL
    .elseif uMsg==WM_HOTKEY
    	invoke PostQuitMessage,NULL
    .elseif uMsg==WM_TIMER
  	;获得屏幕分辨率x，y
  	invoke GetSystemMetrics,SM_CXSCREEN
  	sub eax,40
  	mov x,eax
  	invoke GetSystemMetrics,SM_CYSCREEN
  	sub eax,40
  	mov y,eax
        ;获得鼠标位置pt
        invoke GetCursorPos,addr pt
        
        mov eax,pt.y
        .if eax>y
        	mov eax,pt.x
        	.if eax>x
    		    	.if stat==0
    				invoke ShellExecute,0, NULL, addr szRun, addr szArg, NULL, SW_SHOWNORMAL
    				mov stat,1		
    			.endif
    		.else
    			mov stat,0
    		.endif
    		
    	.else
    		mov stat,0
    	.endif
    .else
        invoke DefWindowProc,hwnd,uMsg,wParam,lParam
    ret
    .endif
    xor eax,eax
    ret
WndProc endp
start:
    invoke GetModuleHandle, NULL
    mov    hInstance,eax
    invoke WinMain, hInstance,NULL,NULL, SW_SHOW
    invoke ExitProcess,eax
end start