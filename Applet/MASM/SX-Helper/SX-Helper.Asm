.586

.model flat,stdcall
option casemap:none
include		windows.inc
include		user32.inc
include		gdi32.inc
include		kernel32.inc
include		shell32.inc
include		winmm.inc
include		advapi32.inc
include		masm32.inc
include		msvcrt.inc
include		Comdlg32.inc
Include		gdiplus.inc

includelib	user32.lib
includelib	kernel32.lib
includelib	shell32.lib
includelib	winmm.lib
includelib	advapi32.lib
includelib	masm32.lib
includelib	gdi32.lib
includelib	msvcrt.lib
include	SX-Helper.inc

includelib user32.lib
includelib kernel32.lib
includelib Comctl32.lib
includelib ole32.lib
IncludeLib Gdi32.lib
includelib shell32.lib
includelib gdiplus.lib
includelib Comdlg32.lib
includelib masm32.lib

include	C:\RadASM\masm32\macros\macros.asm

NIF_INFO    = 010h

WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD
movGUID macro arg1,arg2
	push edi
	push esi
	push ecx
	mov ecx,4
	mov edi,arg1
	mov esi,arg2
	rep movsd
	pop ecx
	pop esi
	pop edi
endm
jmpif macro val1,val2,val3,val4
	cmp val1,val3
	j&val2& val4
endm
.data
szTCaption	db	'SX-Helper 定时提醒',0
istimer	dd	0
szTimeerror	db	'你的输入不正确，请重新输入。',0
temp		db	'Temp',0
state		dd	0
szWelcome	db	'SX-Helper v1.6 beta',0
szWelTip	db	'程序设计：舒俊杰　　QQ：369339353',0dh,0ah
		db	'立志做小巧的系统辅助软件，简单、方便、快捷。',0
sxKey		db	'LOVEXHFOREVER',0
ClassName	db	"LOVEXH",0
iconrc		db	'SX',0
menurc		db	'MENU',0
timerc		db	'TIME',0
powerrc		db	'POWER',0
uprc		db	'UP',0
uperrc		db	'UPER',0
downrc		db	'DOWN',0
lowrc		db	'LOW',0
resrc		db	'RES',0
exitrc		db	'EXIT',0
szCaption	db	'SX-Helper',0
szRegkey	db	'注册热键失败，部分功能你将无法正常使用。',0
szRepeat	db	'你不用开很多次的，已经够啦！',0
szWebsite	db	'http://www.shuax.com/?page_id=425',0
szResc		db	'分辨率设置失败，已经恢复默认设置。　',0
szExit		db	'您确认退出程序？　　',0
szOpen		db	'set CDAudio door open',0
szClose		db	'set CDAudio door closed',0
szAbout		db	'版本：v1.6 beta',0dh,0ah
		db	'时间：2009年10月21日',0dh,0ah
		db	'是否打开我的网站检查软件更新？',0
szLock		db	'rundll32.exe',0
szLockp		db	'user32.dll,LockWorkStation',0
szReg		db	'regedit.exe',0
szApp		db	'appwiz.cpl',0
szGpe		db	'gpedit.msc',0
szCmd		db	'cmd.exe',0
szTas		db	'taskmgr.exe',0
szDev		db	'devmgmt.msc',0
szCal		db	'calc.exe',0
szSys		db	'msinfo32.exe',0
SubKey		db	'Control Panel\Desktop\WindowMetrics',0
hkcon		db	'Shell Icon Size',0
taskb		db	'TaskbarCreated',0
bn		dd	256
SHUTDOWN_NAME	db	"SeShutdownPrivilege",0
szShutTips	db	'你确认已经把所有工作都保存了吗？　　　',0dh,0ah,0dh,0ah
		db	'取消操作请点击否。',0
szShutdown1	db	'NtDll.dll',0
szShutdown2	db	'ZwShutdownSystem',0
szShutdown3	db	'RtlAdjustPrivilege',0
data_run 	db	'Software\Microsoft\Windows\CurrentVersion\Run',0
szRar		db	'检测到程序正在临时文件夹中运行，不建议执行此操作。',0dh,0ah
		db	'选择否可以取消本次选择。',0
create		dd	0
astart		dd	0
avol		dd	0
ghdc		dd	0
hbmp		dd	0
dwFileNameCount	dd	0
aPng 		db	"png格式",0,"*.png",0,0
CapTilte	db	"SX-Helper 截图保存",0
FileName	db	128 dup(0)
isScr		dd	0
mouseRup_flag	BOOL	FALSE
Rct		RECT	<>
IClsid          CLSID	<>
fType           db	"image/png",0

ope	dd 0
.data?
ProcessName    BYTE MAX_PATH DUP(?)
direc		BYTE MAX_PATH DUP(?)
note		NOTIFYICONDATA	<>
hInstance	dd	?
hMenu		dd	?
buff		db	?
hKey		dd	?
hWin		dd ?
hWmain		dd ?
ofn		OPENFILENAMEA	<?>
x1		dd ?
y1		dd ?
x2		dd ?
y2		dd ?

timer	dd ?
timetip	db  1024 dup(?)
ptimer	dd ?
.code
Shutdown proc op:UINT
	local    @hToken, @hCurrentProcess, @tkp:TOKEN_PRIVILEGES
        invoke    GetCurrentProcess
        mov    @hCurrentProcess,eax
        invoke    OpenProcessToken, @hCurrentProcess,TOKEN_ADJUST_PRIVILEGES or TOKEN_QUERY,addr @hToken
        invoke    LookupPrivilegeValue,NULL,addr SHUTDOWN_NAME,addr @tkp.Privileges[0].Luid
        mov    @tkp.PrivilegeCount,1
        mov    @tkp.Privileges[0].Attributes,SE_PRIVILEGE_ENABLED
        invoke    AdjustTokenPrivileges, @hToken,FALSE,addr @tkp,NULL,NULL,NULL
        .if op==1
        	invoke ExitWindowsEx,EWX_SHUTDOWN or EWX_FORCE, 0
        .elseif op==2
        	invoke ExitWindowsEx,EWX_REBOOT or EWX_FORCE, 0
        .elseif op==3
        	invoke ExitWindowsEx,EWX_LOGOFF or EWX_FORCE, 0
        .elseif op==4
        	invoke SetSystemPowerState,1,0
        .elseif op==5
        	invoke SetSystemPowerState,0,1
        .endif
        ret
Shutdown    endp
SCR_CAPTURE proc uses ebx edx ecx lpRect:DWORD              
	local _x,_y,_w,_h
	Local hdc:HDC
	Local mdc:HDC
	
	xor eax,eax 
	.if lpRect==eax		;All screen
		mov _x,eax
		mov _y,eax
		invoke GetSystemMetrics,SM_CXSCREEN 
		mov _w,eax
		invoke GetSystemMetrics,SM_CYSCREEN 
		mov _h,eax
	.else			;Select DC
		mov edx,lpRect
		assume edx:ptr RECT
		mov eax,[edx].right
		mov ecx,[edx].left
		mov _x,ecx
		sub eax,ecx
		mov _w,eax
		mov eax,[edx].bottom
		mov ecx,[edx].top
		mov _y,ecx
		sub eax,ecx
		mov _h,eax
		assume edx:nothing
		xor eax,eax
	.endif
	invoke GetDC,0
	mov hdc,eax
	invoke CreateCompatibleDC,0
	mov mdc,eax	
	invoke CreateCompatibleBitmap,hdc,_w,_h
	mov hbmp,eax
	invoke SelectObject,mdc,eax
	invoke BitBlt,mdc,0,0,_w,_h,hdc,_x,_y,SRCCOPY
	invoke DeleteDC,mdc
	invoke ReleaseDC,0,hdc
	ret
SCR_CAPTURE endp
GetEncoderClsid proc uses esi edi ecx mtype:dword,clsid:dword
	local ansiTxt [40]:byte
	local ptrUtxt     :dword
	local Array       :dword
	local ptrArray    :dword
	local cnum        :dword
	local csize       :dword
	local offMimeType :dword

        mov offMimeType,48
        invoke GdipGetImageEncodersSize,addr cnum,addr csize   
        jmpif cnum,e,0,error
        invoke GlobalAlloc,GPTR,csize
        jmpif eax,e,0,error
        mov Array,eax
        mov ptrArray,eax
        invoke GdipGetImageEncoders,cnum,csize,Array

@@: 
	mov eax,ptrArray
        add eax,offMimeType
        mov eax,[eax]
        mov ptrUtxt,eax

        invoke WideCharToMultiByte,CP_ACP,0,ptrUtxt,-1,addr ansiTxt,40,0,0
        invoke lstrcmp,addr ansiTxt,mtype

        jmpif eax,e,0,@F 
        add ptrArray,sizeof ImageCodecInfo
        dec cnum  
        jne @B

        invoke GlobalFree,Array
        jmp error

@@: 
	mov edi,clsid
        mov esi,ptrArray 
        mov ecx,4
        rep movsd
        invoke GlobalFree,Array
        mov eax,0
	ret
error: 
	mov eax,21
	ret
GetEncoderClsid endp
SaveImage proc hBmp1:HBITMAP,szFileName:dword		
	LOCAL gdipSTI:GdiplusStartupInput
	LOCAL GpImage:DWORD
	LOCAL utFileName[MAX_PATH*2]:BYTE
	LOCAL GdiPlusToken:dword
	
	invoke RtlZeroMemory,addr gdipSTI,sizeof gdipSTI
	mov    gdipSTI.GdiplusVersion,1
	invoke GdiplusStartup,addr GdiPlusToken,addr gdipSTI,NULL  
        .if eax==0		
		invoke GdipCreateBitmapFromHBITMAP,hBmp1,0,addr GpImage
		.if eax==0	
			invoke GetEncoderClsid,addr fType,addr IClsid
			invoke MultiByteToWideChar,CP_ACP,0,szFileName,-1,addr utFileName,MAX_PATH*2
			invoke GdipSaveImageToFile,GpImage,addr utFileName,addr IClsid,0
			invoke GdipDisposeImage,GpImage
		.endif
		invoke GdiplusShutdown,GdiPlusToken
	.endif
	ret
SaveImage endp
FillFileStruct proc uses ecx edx s_hwnd:HANDLE
	local dwFileNameSize:dword

	invoke	RtlZeroMemory, addr ofn, sizeof ofn

	mov ofn.lStructSize,	88
	m2m	ofn.hwndOwner,		s_hwnd 
	m2m	ofn.lpstrFilter,	offset aPng  
	mov	ofn.nMaxFile,		82806h
	m2m	ofn.lpstrTitle,		offset CapTilte
    m2m	ofn.hInstance,		hInstance
	m2m	ofn.lpstrFile,		offset FileName

	invoke GetSaveFileName,addr ofn

	ret
FillFileStruct endp
settimer proc uses ebx hWnd:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
	.if uMsg == WM_CLOSE
                invoke EndDialog,hWnd,wParam
         .elseif uMsg == WM_COMMAND
         	.if wParam == IDOK
                     invoke IsDlgButtonChecked,hWnd,51003
                            .if eax != 0
                            mov ope,1
                            .endif
                     invoke IsDlgButtonChecked,hWnd,51004
                            .if eax != 0
                            mov ope,2
                            .endif
                     invoke IsDlgButtonChecked,hWnd,51005
                            .if eax != 0
                            mov ope,3
                            .endif
		invoke  GetDlgItem,hWnd,51000
		invoke GetWindowText,eax,addr timetip,1024

		invoke  GetDlgItemInt,hWnd,51001,NULL,TRUE
		mov timer,eax
		.if timer<1 || timer >1440
			invoke MessageBox,NULL,addr szTimeerror,addr szTCaption,MB_OK
			ret
		.endif
		mov eax,timer
		mov ecx,60000
		imul ecx
         	invoke SetTimer,hWnd,1,eax,NULL
        	mov ptimer,eax
        	mov istimer,1
        	invoke ShowWindow,hWnd,SW_HIDE
         	.endif
        .elseif uMsg==WM_TIMER
    		mov istimer,0
    		invoke KillTimer,hWnd,ptimer
    		invoke MessageBox,NULL,addr timetip,addr szTCaption,MB_OK
    		.if ope!=0
    			invoke Shutdown,ope
    		.endif
	.else
		mov eax,FALSE
		ret
	.endif
	mov eax,TRUE
	ret
settimer endp
CapDlg proc uses ebx hWnd:HWND,uMsg1:UINT,wParam1:WPARAM,lParam1:LPARAM
	LOCAL Ps:PAINTSTRUCT
	local hImg,hPen
	Local mdc:HDC

	.IF uMsg1== WM_INITDIALOG
		mov isScr,1
		m2m    hWin,hWnd
		invoke LoadCursor,0,IDC_CROSS
		invoke SetCursor,EAX
		invoke GetSystemMetrics,SM_CXSCREEN
		push   eax
		invoke GetSystemMetrics,SM_CYSCREEN
		pop    edx
		invoke SetWindowPos,hWnd,0FFFFFFFFh,0,0,EDX,EAX,NULL
		xor    eax, eax
		mov    x1, eax
		mov    y1, eax
		mov    x2, eax
		mov    y2, eax
		invoke SCR_CAPTURE, eax
	.elseif uMsg1 == WM_LBUTTONDOWN
		invoke LoadCursor,0,IDC_CROSS
		invoke SetCursor,EAX
		invoke SetCapture, hWnd
		xor    eax,eax
		xor    ecx,ecx
		movzx  eax, word ptr lParam1
		mov    x1, eax
		movzx  eax, word ptr lParam1+2
		mov    y1, eax
		MOV    mouseRup_flag, TRUE
	.elseif uMsg1 == WM_LBUTTONUP
		MOV    mouseRup_flag, FALSE
		invoke ReleaseCapture
			mov     eax, x1
			mov     ebx, y1
			mov     ecx, x2
			mov     edx, y2
			.if (eax == ecx) || (ebx == edx)
				invoke MessageBox,hWnd,SADD('选择区域无效'),NULL,30h
				jmp @F
			.endif
			.if (eax < ecx) && (ebx < edx)
				jmp ok
			.endif
			.if (eax > ecx) && (ebx > edx)
				xchg eax, ecx
				xchg ebx, edx
				jmp ok
			.endif
			.if (eax > ecx) && (ebx < edx)
				xchg eax, ecx
				jmp ok
			.endif
			.if (eax < ecx) && (ebx > edx)
				xchg ebx, edx
				jmp ok
			.endif
 ok:                     
			inc     eax
			inc     ebx
			dec     ecx
			dec     edx
			mov     Rct.left, eax
			mov     Rct.top, ebx
			mov     Rct.right, ecx
			mov     Rct.bottom, edx
		INVOKE	SCR_CAPTURE,ADDR Rct
		invoke  wsprintf,offset FileName,SADD("截图%02u.png"),dwFileNameCount
		invoke  FillFileStruct,hWnd
		.if eax==0
		INVOKE OpenClipboard,0
		.if eax==1
			INVOKE EmptyClipboard
			INVOKE SetClipboardData,CF_BITMAP, hbmp
			INVOKE CloseClipboard
		.endif
			invoke  SendMessage,hWnd,WM_CLOSE,1,0
			mov isScr,0
			jmp     @F
		.endif
		inc     dwFileNameCount
		invoke  crt_strstr,addr FileName,SADD(".png")
		.if eax==0
			invoke  lstrcat,addr FileName,SADD(".png")
		.endif
		invoke  SaveImage,hbmp,offset FileName
		invoke  MessageBeep,MB_ICONASTERISK
		mov isScr,0	
		invoke  SendMessage,hWnd,WM_CLOSE,1,0
                invoke  DeleteObject,hbmp
		invoke  InvalidateRect,hWnd,NULL,NULL
		
@@:		
	.elseif uMsg1 == WM_MOUSEMOVE
		invoke LoadCursor,0,IDC_CROSS
		invoke SetCursor,EAX
		.if mouseRup_flag == TRUE
			movzx  eax, word ptr lParam1
			mov    x2, eax
			movzx  eax, word ptr lParam1+2
			mov    y2, eax
			invoke InvalidateRect,hWnd,NULL,NULL
		.endif
	.elseif uMsg1 == WM_RBUTTONUP
			mov isScr,0
			invoke  SendMessage,hWnd,WM_CLOSE,1,0
            invoke  DeleteObject,hbmp
			invoke  InvalidateRect,hWnd,NULL,NULL	
	.elseif uMsg1 == WM_DEVMODECHANGE
                invoke SendMessage,hWnd,WM_CLOSE,0,0
	.elseif uMsg1 == WM_PAINT
		invoke BeginPaint,hWnd,addr Ps
		invoke GetClientRect,hWnd,ADDR Rct
		invoke CreateCompatibleDC,Ps.hdc
		mov mdc, eax
		invoke CopyImage,hbmp,NULL,NULL,NULL,4
		mov hImg,eax
		invoke SelectObject,mdc,eax
		invoke CreatePen,PS_DASHDOTDOT,1,0FF0000h
		mov hPen,eax
		invoke SelectObject,mdc,eax
		invoke MoveToEx,mdc,x1,y1,NULL
		invoke LineTo,mdc,x1,y2
		invoke LineTo,mdc,x2,y2
		invoke LineTo,mdc,x2,y1
		invoke LineTo,mdc,x1,y1
		invoke DeleteObject,hPen
		invoke BitBlt,Ps.hdc,0,0,Rct.right,Rct.bottom,mdc,0,0,0C000CAh
		invoke DeleteObject,hImg
		invoke DeleteDC,mdc
		invoke EndPaint,hWnd,ADDR Ps
	.elseif uMsg1 == WM_CLOSE
                invoke EndDialog,hWnd,wParam1
	.else
		mov eax,FALSE
		ret
	.endif
	mov eax,TRUE
	ret
CapDlg endp
Check PROC
	LOCAL dwLength:DWORD
	mov dwLength,MAX_PATH

	invoke RegOpenKeyEx,HKEY_LOCAL_MACHINE,addr data_run, 0, KEY_ALL_ACCESS, addr hKey
	invoke RegQueryValueEx,hKey, addr szCaption, 0, 0, addr direc, addr dwLength
	.if eax==ERROR_SUCCESS
		invoke lstrcmp,addr ProcessName, addr direc
	   	.if eax==0
	   	 	mov astart,1
		.else	
			mov astart,0
		.endif
	.else
		mov astart,0
	.endif
	
	ret
Check endp
StrLen PROC SRC:DWORD
mov ecx,-1
mov edi,SRC
mov al,0
repnz scasb
mov eax,ecx
not eax
dec eax
ret
StrLen ENDP
AutoStart proc
	LOCAL lens:DWORD
	invoke RegOpenKeyEx,HKEY_LOCAL_MACHINE,addr data_run, 0, KEY_ALL_ACCESS, addr hKey
	.if astart==0
		invoke crt_strstr,addr ProcessName,addr temp
		.if eax!=0
			invoke MessageBox,NULL,addr szRar,addr szCaption,MB_YESNO + MB_ICONINFORMATION
			.if eax==IDNO
				ret
			.endif
		.endif
		invoke StrLen,addr ProcessName
		mov lens,eax
		invoke RegSetValueEx,hKey,addr szCaption, 0, REG_SZ, addr ProcessName,lens
		mov astart,1
	.else
		invoke RegDeleteValue,hKey, addr szCaption
		mov astart,0
	.endif
	ret
AutoStart endp
ShowContextMenu PROC hWnd:HWND
	LOCAL pt:POINT
	LOCAL mmi:MENUITEMINFO
	LOCAL time:HBITMAP
	LOCAL popmenu:HMENU
	invoke GetCursorPos,addr pt
	
	invoke LoadMenu,hInstance,addr menurc
        mov hMenu,eax
        invoke GetSubMenu,hMenu,0
        mov hMenu,eax
        
        .if astart==1
       		invoke CheckMenuItem,hMenu, 10036, MF_CHECKED
        .else
        	invoke CheckMenuItem,hMenu, 10036, MF_UNCHECKED
        .endif
	.if istimer==1
       		invoke EnableMenuItem,hMenu, 10040, MF_GRAYED
        .else
        	invoke EnableMenuItem,hMenu, 10040, MF_ENABLED
	.endif
	
        .if avol==1
       		invoke CheckMenuItem,hMenu, 10003, MF_CHECKED
        .else
        	invoke CheckMenuItem,hMenu, 10003, MF_UNCHECKED
        .endif
        .if bn==256
       		invoke CheckMenuItem,hMenu, 10006, MF_CHECKED
        .else
        	invoke CheckMenuItem,hMenu, 10006, MF_UNCHECKED
        .endif
	invoke LoadBitmap,hInstance, addr timerc
	invoke SetMenuItemBitmaps,hMenu, 10040, MF_BYCOMMAND,eax,eax
	invoke LoadBitmap,hInstance, addr powerrc
	invoke SetMenuItemBitmaps,hMenu, 3, MF_BYPOSITION,eax,eax
	invoke LoadBitmap,hInstance, addr uprc
	invoke SetMenuItemBitmaps,hMenu, 10004, MF_BYCOMMAND,eax,eax
	invoke LoadBitmap,hInstance, addr uperrc
	invoke SetMenuItemBitmaps,hMenu, 10002, MF_BYCOMMAND,eax,eax
	invoke LoadBitmap,hInstance, addr downrc
	invoke SetMenuItemBitmaps,hMenu, 10001, MF_BYCOMMAND,eax,eax
	invoke LoadBitmap,hInstance, addr lowrc
	invoke SetMenuItemBitmaps,hMenu, 10005, MF_BYCOMMAND,eax,eax
	invoke LoadBitmap,hInstance, addr exitrc
	invoke SetMenuItemBitmaps,hMenu, 10008, MF_BYCOMMAND,eax,eax
	invoke GetSubMenu,hMenu,2
	mov popmenu,eax
	invoke LoadBitmap,hInstance, addr resrc
	invoke SetMenuItemBitmaps,popmenu, 11, MF_BYPOSITION,eax,eax
	
	
        mov mmi.cbSize,sizeof MENUITEMINFO
 	mov mmi.fMask,MIIM_STATE
 	mov mmi.fType,0
 	mov mmi.fState, MFS_DEFAULT
	invoke SetMenuItemInfo,hMenu, 0, 1, addr mmi      
        
        invoke SetForegroundWindow,hWnd
        invoke TrackPopupMenu,hMenu,TPM_RIGHTALIGN,pt.x,pt.y,NULL,hWnd,NULL
        invoke DestroyMenu,hMenu
	ret

ShowContextMenu endp
iconC PROC


	LOCAL Iconw:DWORD
	invoke RegOpenKeyEx,HKEY_CURRENT_USER,addr SubKey, 0, KEY_ALL_ACCESS, addr hKey
	invoke GetSystemMetrics,SM_CXICON
	mov Iconw,eax
	invoke dw2a,Iconw - 1,addr buff
	invoke RegSetValueEx,hKey, addr hkcon, 0, REG_SZ, addr buff,sizeof buff
	invoke SendMessage,HWND_BROADCAST, WM_WININICHANGE, 0, 0
	invoke dw2a,Iconw - 1,addr buff
	invoke RegSetValueEx,hKey,addr hkcon, 0, REG_SZ, addr buff,sizeof buff
	invoke SendMessage,HWND_BROADCAST, WM_WININICHANGE, 0, 0
	
	ret

iconC endp
ResetNow proc iCode
        local buf[256],pClose,pAdjust,hNtDll
        
        invoke MessageBox,NULL,addr szShutTips,addr szCaption,MB_YESNO + MB_ICONINFORMATION
        .if eax==IDNO
        ret
        .endif
  	
  	invoke LoadLibrary,addr szShutdown1
  	.if eax
    	mov hNtDll,eax
    	invoke GetProcAddress,hNtDll,addr szShutdown2
    	mov pClose,eax
    	invoke GetProcAddress,hNtDll,addr szShutdown3
    	mov pAdjust,eax
    	.if eax && pClose
     	 lea eax,buf
     	 push eax
     	 push 1
      	push 1
      	push 13H
      	mov eax,pAdjust
     	 call eax
      	.if eax==0C000007CH
        lea eax,buf
        push eax
        push 0
        push 1
        push 13H
        mov eax,pAdjust
        call eax
      .endif
      push iCode   ;关机方式：1- 重启, 2 - 关机
      mov eax,pClose
      call eax
    .endif
  .endif
  ret
   ResetNow endp
gamma proc wBrightness:UINT
	LOCAL index:UINT
	LOCAL g1[768]:DWORD
    	invoke GetDC, 0
	mov ghdc,eax
	    	
    	mov index,0
    	xor edi, edi
    	xor ecx, ecx
    	.while index<256
    	mov eax,index
    	mov ecx,wBrightness
    	imul ecx
    	.if eax>65535
    		mov eax,65535
    	.endif
	lea ebx, g1
	add ebx, edi
	mov word ptr [ebx], ax
	add ebx, 512
	mov word ptr [ebx], ax
	add ebx, 512
	mov word ptr [ebx], ax
	inc edi
	inc edi
    		add index,1
    	.endw
    	invoke SetDeviceGammaRamp,ghdc, ADDR g1
	ret
gamma endp
Mode proc x:DWORD,y:DWORD

	LOCAL lpDevMode:DEVMODE
	mov lpDevMode.dmSize ,sizeof DEVMODE
	mov lpDevMode.dmDriverExtra,0
	mov lpDevMode.dmBitsPerPel,32
	push x
	pop lpDevMode.dmPelsWidth
	push y
	pop lpDevMode.dmPelsHeight
	mov lpDevMode.dmFields,DM_BITSPERPEL or DM_PELSWIDTH
	
	invoke ChangeDisplaySettings,addr lpDevMode,0
        .if eax==DISP_CHANGE_SUCCESSFUL
        	invoke ChangeDisplaySettings,addr lpDevMode, CDS_UPDATEREGISTRY
        .else
        	invoke ChangeDisplaySettings,NULL,0
        	invoke MessageBox,NULL,addr szResc,addr szCaption,MB_OK + MB_ICONWARNING
    	.endif
	ret
Mode endp
WinMain proc hInst:HINSTANCE,hPrevInst:HINSTANCE,CmdLine:LPSTR,CmdShow:DWORD
    LOCAL wc:WNDCLASSEX
    LOCAL msg:MSG
    LOCAL hwnd:HWND
    
    invoke FindWindow, NULL, addr sxKey
        .if eax
		invoke MessageBox,NULL,addr szRepeat,addr szCaption,MB_OK + MB_ICONWARNING
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
    invoke LoadIcon,hInstance,addr iconrc
    mov   wc.hIcon,eax
    mov   wc.hIconSm,eax
    invoke LoadCursor,NULL,IDC_ARROW
    mov   wc.hCursor,eax
    invoke RegisterClassEx, addr wc
    invoke CreateWindowEx,WS_EX_TOOLWINDOW,ADDR ClassName,ADDR sxKey,WS_POPUP,CW_USEDEFAULT,CW_USEDEFAULT,\
           CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL
    mov   hwnd,eax

        mov note.cbSize,sizeof NOTIFYICONDATA
        mov eax,hwnd
        mov note.hwnd,eax
        mov note.uID,1
        mov note.uFlags,NIF_ICON + NIF_MESSAGE + NIF_TIP + NIF_INFO
        
        mov note.uCallbackMessage,WM_USER
        invoke LoadIcon,hInstance,addr iconrc
        mov note.hIcon,eax
        invoke lstrcpy,addr note.szTip,addr szCaption
        mov note.dwInfoFlags, NIIF_INFO + NIIF_NOSOUND
        mov note.uVersion,NOTIFYICON_VERSION
        invoke lstrcpy,addr note.szInfo,addr szWelTip
        invoke lstrcpy,addr note.szInfoTitle,addr szWelcome
        invoke Shell_NotifyIcon,NIM_ADD,addr note
        mov state,1
    .while TRUE
        invoke GetMessage, ADDR msg,NULL,0,0
        .BREAK .IF (!eax)
        invoke TranslateMessage, ADDR msg
        invoke DispatchMessage, ADDR msg
    .endw
    mov eax,msg.wParam
    ret
WinMain endp
WndProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    
    .if uMsg==WM_CREATE
    	;自启动检查
    	invoke GetModuleFileName,0,addr ProcessName,MAX_PATH
    	invoke Check
    	
    	;注册热键
    	invoke RegisterHotKey,hWnd, 2010, MOD_WIN, VK_K
    	xor ebx,ebx
    	invoke RegisterHotKey,hWnd, 10001, MOD_CONTROL, VK_RIGHT
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10002, MOD_CONTROL, VK_LEFT
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10003, MOD_CONTROL, VK_END
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10004, MOD_CONTROL, VK_UP
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10005, MOD_CONTROL, VK_DOWN
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10006, MOD_CONTROL, VK_HOME
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10007, MOD_CONTROL, VK_F12
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10008, MOD_SHIFT, VK_END
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10021, MOD_SHIFT, VK_INSERT
    	.IF eax==0
    	mov ebx,1
    	.endif
    	invoke RegisterHotKey,hWnd, 10037, MOD_CONTROL, VK_F3
    	.IF eax==0
    	mov ebx,1
    	.endif
    	.if  ebx==1
    		invoke MessageBox,NULL,addr szRegkey,addr szCaption,MB_OK + MB_ICONWARNING
    	.endif
        ;启动图标
        .if create==0
        	mov create,1
        .else
        mov note.cbSize,sizeof NOTIFYICONDATA
        push hWnd
        pop note.hwnd
        mov note.uID,1
        mov note.uCallbackMessage,WM_USER
        invoke LoadIcon,hInstance,addr iconrc
        mov note.hIcon,eax
        invoke lstrcpy,addr note.szTip,addr szCaption
        mov note.uFlags,NIF_ICON + NIF_MESSAGE + NIF_TIP
        invoke Shell_NotifyIcon,NIM_ADD,addr note
	mov state,1
	.endif
    .elseif uMsg==WM_DESTROY
    	invoke DestroyMenu,hMenu
        push hWnd
        pop note.hwnd
		mov note.uID,1
        invoke Shell_NotifyIcon,NIM_DELETE,addr note
        invoke DestroyMenu,hMenu
        invoke PostQuitMessage,NULL
        
    .elseif uMsg== WM_COMMAND || uMsg==WM_HOTKEY
        ;backdoor
        .if wParam==2010
        	invoke MessageBox,NULL,SADD("亲爱的老婆大人，我永远永远都爱你。"),addr szCaption,MB_OK
        ;音量控制    
        .elseif wParam==10001
        	invoke SendMessage,hWnd, 793, 197266, 655360
        .elseif wParam==10002
        	invoke SendMessage,hWnd, 793, 197266, 589824
        .elseif wParam==10003
        	.if avol==0
        		mov avol,1
        	.else
        		mov avol,0
        	.endif
        	invoke SendMessage,hWnd, 793, 2100912, 524288
        ;屏幕亮度控制
        .elseif wParam==10004
	       	add bn,2
        	invoke gamma,bn
        .elseif wParam==10005
        	sub bn,2
        	invoke gamma,bn
        .elseif wParam==10006
        	mov bn,256
        	invoke gamma,256
        ;显示隐藏图标
        .elseif wParam==10007
       		push hWnd
        	pop note.hwnd
		mov note.uID,1
		.if state == 0
        		mov note.uFlags,NIF_ICON + NIF_MESSAGE + NIF_TIP
        		mov note.uCallbackMessage,WM_USER
        		invoke LoadIcon,hInstance,addr iconrc
        		mov note.hIcon,eax
        		invoke lstrcpy,addr note.szTip,addr szCaption
        		invoke Shell_NotifyIcon,NIM_ADD,addr note
			mov state,1

		.else
			invoke Shell_NotifyIcon,NIM_DELETE,addr note
			mov state,0
		.endif
        ;系统工具
        .elseif wParam==10009
        	invoke ShellExecute,0, NULL, addr szReg, NULL, NULL, SW_SHOWNORMAL
        .elseif wParam==10010
        	invoke ShellExecute,0, NULL, addr szApp, NULL, NULL, SW_SHOWNORMAL
        .elseif wParam==10011
        	invoke ShellExecute,0, NULL, addr szGpe, NULL, NULL, SW_SHOWNORMAL
        .elseif wParam==10012
        	invoke ShellExecute,0, NULL, addr szCmd, NULL, NULL, SW_SHOWNORMAL
        .elseif wParam==10013
        	invoke ShellExecute,0, NULL, addr szTas, NULL, NULL, SW_SHOWNORMAL
        .elseif wParam==10014
        	invoke ShellExecute,0, NULL, addr szDev, NULL, NULL, SW_SHOWNORMAL
        .elseif wParam==10015
        	invoke ShellExecute,0, NULL, addr szCal, NULL, NULL, SW_SHOWNORMAL
        .elseif wParam==10038
        	invoke ShellExecute,0, NULL, addr szSys, NULL, NULL, SW_SHOWNORMAL
        ;光驱控制
        .elseif wParam==10016
        	invoke mciSendString,addr szOpen, NULL, 0, 0
        .elseif wParam==10017
        	invoke mciSendString,addr szClose, NULL, 0, 0
        ;显示桌面
        .elseif wParam==10018
        	invoke keybd_event,VK_LWIN, 0, 0, 0
        	invoke keybd_event,'D', 0, 0, 0
		invoke keybd_event,VK_LWIN, 0, KEYEVENTF_KEYUP, 0
		invoke keybd_event,'D', 0, KEYEVENTF_KEYUP, 0
        ;打开屏保
        .elseif wParam==10019
        	invoke Sleep,200
        	invoke SendMessage,HWND_BROADCAST, WM_SYSCOMMAND, SC_SCREENSAVE, 0
        ;重建图标缓存
        .elseif wParam==10020
        	invoke iconC
        ;关闭显示器
        .elseif wParam==10021
        	invoke Sleep,200
        	invoke PostMessage,hWnd, WM_SYSCOMMAND, SC_MONITORPOWER, 2
        	invoke ShellExecute,0, NULL, addr szLock, addr szLockp, NULL, SW_SHOWNORMAL
        ;设置屏幕分辨率
        .elseif wParam==10022
        	invoke Mode,1024,768
        .elseif wParam==10023
        	invoke Mode,1280,800
        .elseif wParam==10024
        	invoke Mode,1440,900
        .elseif wParam==10025
        	invoke Mode,1680,1050
        .elseif wParam==10026
        	invoke Mode,1366,768
        .elseif wParam==10027
        	invoke Mode,800,600
        ;关机、重启、注销、睡眠、休眠、锁定
        .elseif wParam==10028
        	invoke Shutdown,1
        .elseif wParam==10029
        	invoke Shutdown,2
        .elseif wParam==10030
        	invoke Shutdown,3
        .elseif wParam==10031
        	invoke Shutdown,4
        .elseif wParam==10032
        	invoke Shutdown,5
        .elseif wParam==10033
        	invoke ResetNow,2
        .elseif wParam==10034
        	invoke ResetNow,1
        .elseif wParam==10041
        	invoke ShellExecute,0, NULL, addr szLock, addr szLockp, NULL, SW_SHOWNORMAL
        ;关于，退出
        .elseif wParam==10035
        	invoke MessageBox,NULL,addr szAbout,addr szCaption,MB_YESNO + MB_ICONQUESTION
        	.if eax==IDYES
        	invoke ShellExecute,0, NULL, addr szWebsite, NULL, NULL, SW_SHOWMAXIMIZED
        	.endif
        .elseif wParam==10008
        	invoke MessageBox,NULL,addr szExit,addr szCaption,MB_YESNO + MB_ICONQUESTION
        	.if eax==IDYES
        	invoke SendMessage,hWnd, WM_DESTROY, wParam, lParam
        	.endif
        ;自启动
        .elseif wParam==10036
        	invoke AutoStart
        ;定时提醒
        .elseif wParam==10040
        	invoke DialogBoxParam,hInstance,102,0,offset settimer,NULL

        ;屏幕截图  
        .elseif wParam==10037
        	invoke Sleep,300
        	.if isScr==0
		       invoke DialogBoxParam,hInstance,101,hWmain,offset CapDlg,NULL
		.endif
        .endif
    .elseif uMsg==WM_USER
        .if lParam==WM_LBUTTONDOWN || lParam==WM_RBUTTONDOWN
	    invoke ShowContextMenu,hWnd
        .endif
    .else
    	invoke RegisterWindowMessage,addr taskb
    	.if uMsg == eax
    		invoke SendMessage,hWnd, WM_CREATE, wParam, lParam
    	.elseif
        	invoke DefWindowProc,hWnd,uMsg,wParam,lParam
        .endif
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