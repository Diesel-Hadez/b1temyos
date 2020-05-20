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
	mov bx, 0x1000
	mov dl, [BOOT_DRIVE]
	mov dh, 31		; 31 sectors I guess, gives us 31*512 bytes of code to work with
	call disk_load
	ret



[bits 32]

BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_str_pm
	call KERNEL_OFFSET 
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
