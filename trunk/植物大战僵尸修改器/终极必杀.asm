[ENABLE]
globalalloc(mythread_killall,0x1000)
label(again)
mythread_killall:
mov ecx,5
again:
mov eax,[PlantsVsZombies.exe+329670]
mov eax,[eax+868]
test eax,eax
db 75 01
ret
push ffffffff
push 0
push 0
push eax
mov eax,ecx
dec eax
call PlantsVsZombies.exe+0fa10
loop again
ret

[DISABLE]
createthread(mythread_killall)