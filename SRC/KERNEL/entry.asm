
.8086
extern KernelMain_ : proc
global ENTRY : proc
.code
org 0x0
ENTRY PROC 
	xor ax,ax
	mov es,ax
	mov ax,0x800
	mov ds,ax
	mov ss,ax
	mov ax,0x0
	mov sp,ax
	push dx
	call KernelMain_
	jmp $
ENTRY ENDP
end