[org 0x7c00]
; Global constants
%include "./globals.asm"
KERNEL_OFFSET equ 	0x1000

start:
	mov [BOOT_DRIVE], dl ; Initially dl is set to the BOOT_DRIVE

	; This gives us about 512 bytes of safe stack space
	mov bp, 0x8000
	mov sp, bp

	call clear_screen
	mov ax, sys_string
	call print_str

	call load_kernel
	call switch_to_pm
	
	jmp $

load_kernel:
	;ES segment is by default set to zero, so just set bx
	; so ES:BX is 0x0000:0x1000
	mov ax, 0x00
	mov es, ax
	mov bx, 0x1000
	mov dl, [BOOT_DRIVE]
	mov dh, 31		; 31 sectors I guess, gives us 31*512 bytes of code to work with
	mov cx, 0x02 		; Start from 2nd sector (1st sector is boot sector)
	call disk_load

	mov ax, 0x00
	mov es, ax
	; I really hope this works
	mov bx, 0x4e00
	mov dl, [BOOT_DRIVE]
	; Interestingly, my manual tests show that each sector is 128 bytes? No idea why
	; Probably a bug
	; Scratch that last comment, but now I can't load more than 23 sectors? Maybe 
	; something to do with the CHS system, but even then there should be 63 (or 50, sources
	; on the internet are conflicting) sectors
	; in a track/cylinder, and I can only load until sector 56?
	; in osdev.org, there is a note about "cannot cross ES page boundary, or a cylinder boundary, and must be < 128"
	; so perhaps that is why? It also recommends copying one sector at a time, which I may end up doing in the future
	mov dh, 23
	mov cl, 33
	call disk_load
	ret



[bits 32]

BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_str_pm
	;call 0x00:0x1000
	; idk why but nothing else works (besides jmp 0x1000 but then I can't modify ES)
	push word 0x0000
	push word 0x1000
	ret
	;jmp 0000h:1000h
	;jmp KERNEL_OFFSET
	;call KERNEL_OFFSET
	jmp $

[bits 16]
%include "./screen_utils.asm"	
%include "./disk_utils.asm"
%include "./gdt.asm"
%include "./pmode.asm"
%include "./screen_utils_pmode.asm"

; Global variables

; Ideally would be a half-byte, but eh...
BOOT_DRIVE:
	db 0

sys_string:
	db 'Initialising B1TE_OS...', 0

MSG_PROT_MODE:
	db 'prot-mode',0

times 510-($-$$) db 0	; Pad (510 - current number of bytes) times

dw	0xaa55
