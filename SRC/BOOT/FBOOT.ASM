use16
cpu 8086
org 0x7c00
start:
jmp entry
nop

OEMid           db "AGAMEA  "       ; OEM id
BytesPerSector  dw 512              ; размер сектора
SecsPerCluster  db 1                ; количество секторов на клустер
ReserveCount    dw 1                ; резерный сктор под boot
FATcount        db 2                ; количество FAT 
DirEntries      dw 224              ; ; количество файлов в роот папке
TotalSectors    dw 2880             ; количество секторов
MediaDescriptor db 0xF0             ; эта значит что 1.44 дискета
SectorsPerFAT   dw 9                ; размер фат
SectorsPerTrack dw 18               ; рамер track
HeadCount       dw 2                ; количество Head
HiddenSectors   dd 0                ;
                dd 0
DriveNumber     db 0                ; номер диска
DirtyBit        db 1                ; WinNT 
extBootSig      db 0x29             ; 
VolumeID        dd 0x0              ; 
VolumeLabel     db "MY OS     ",0    ; Название
FSType          db "FAT12   "       ; Файловая система
stop:
	mov si,TextError
	call print
	jmp reset
	jmp $
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
entry:
	mov  ax, cs
	mov  ds, ax
	mov  es, ax
	mov [DriveID],dl
	cld 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov ax,0x0003
	int 10h
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;FileInfoSectorID=SectorsPerFAT*FATcount+ReserveCount
	mov ax,[SectorsPerFAT]
	mul BYTE [FATcount]
	add ax,[ReserveCount]
	mov [FileInfoSectorID],ax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push ax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;FileDataSectorID=((DirEntries*32)/BytesPerSector)+IDSectorFileInfo
	mov ax,32
	mul WORD [DirEntries]
	div WORD [BytesPerSector]
	add ax,[FileInfoSectorID]
	mov [FileDataSectorID],ax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop ax
	mov bx,0x8000
	call ReadSector

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov si,KernelName
	mov di,0x8000
	mov cx,11
	repe cmpsb
	jnz stop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov al,[0x800B]
	cmp al,0x88
	jnz stop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov bx,0x800
	mov es,bx
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov ax,[FileDataSectorID]
	mov bx,0x0
	mov cx,0x80
loop_load:
	call ReadSector
	inc ax
	add bx,0x200
 loop loop_load
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	xor dx,dx
	mov dl,[DriveID]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	jmp	0x800:0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SectorID=x%SectorsPerTrack+1
;HeadID=(x/SectorsPerTrack)%HeadCount
;TrackID=(x/SectorsPerTrack)/HeadCount
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CalcPossition:
	push ax
	push dx
	xor dx,dx
	div WORD [SectorsPerTrack]
	inc dl
	mov [SectorID],dl
	xor dx,dx
	div WORD [HeadCount]
	mov [HeadID],dl
	mov [TrackID],ax
	pop dx
	pop ax
	ret
SectorID:
	db 0
HeadID:
	db 0
TrackID:
	dw 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;drive    :DL
;adress   :DS:BX 
;head     :DH
;         :---CH--- ---CL---
;track    :76543210 98
;sector   :            543210
ReadSector:
	call CalcPossition
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push ax
	push cx
	push dx
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov al,1
	mov ch,[TrackID]
	mov cl,[TrackID+1]
	and cl,11b
	shl cl,1
	shl cl,1
	shl cl,1
	shl cl,1
	shl cl,1
	shl cl,1
	mov dl,[SectorID]
	and dl,00111111b
	or cl,dl
	mov dh,[HeadID]
	mov dl,[DriveID]
	mov ah,0x2
	int 13h
	jb stop_disk
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop dx
	pop cx	
	pop ax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
stop_disk:
	push ax
	mov si,TextDiskError
	call print 
	pop ax
	mov al,ah
	xor ah,ah
	call print_number16
	mov bh,0x0
	mov ah,0x0e
	mov al,','
	int 10h
	mov al,[DriveID ]
	xor ah,ah
	call print_number16
	mov si,TextPressKey
	call print 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
reset:
	xor	ax,ax
	int 16h
	mov ax,0xFFFF
	mov ds,ax
	mov es,ax
	jmp 0xFFFF:0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print_number16:
	push di
	push dx
	push si
	push ax
	std
	mov di,TempNumber
	add di,3
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push ax	
print_number16_loop:
	xor dx,dx
	pop ax
	div WORD [DivNumber]
	push ax
	mov ax,dx
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	cmp ax,0xA
	jb print_number16_loop2
	add ax,0x7
	print_number16_loop2:
	add ax,'0'
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	stosb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop ax
	test ax,ax
	jz print_number16_end
	push ax
	jmp print_number16_loop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	print_number16_end:
	cld
	mov si,di
	inc si
	call print
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop ax
	pop si
	pop dx
	pop di
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
DivNumber: 
	db 10h
TempNumber:
	db 0,0,0,0,0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print:
	push bx
	push ax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov bx,0
	mov ah,0x0e
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	loop_print:
	lodsb
	test al,al
	jz print_end
	int 0x10
	jmp loop_print
	print_end:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop ax
	pop bx
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	ret

;DATA
DriveID:
db 0
FileInfoSectorID:
dw 0
FileDataSectorID:
dw 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
KernelName:
	db "KERNEL  ",'BIN'
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TextError:
	db 'NO KERNEL FILE',
TextPressKey:
	db 10,13,"PRESS ALL KEY FOR RESTART",0
TextDiskError:
	db 'DISK ERROR CODE:',0
TextNewLine:
	db 10,13,0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
finish:
times 0x1fe+start-finish db 0
db 0x55,0xAA