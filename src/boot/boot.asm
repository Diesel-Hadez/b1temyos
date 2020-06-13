[org 0x7c00]
; Global constants
%include "./globals.asm"

; Might be a better idea to automatically calculate SECTORS_TO_LOAD using kernel_start and kernel_end
; from linker.ld

; Disregard the following as I just moved my kernel to 0x10000 instead of 0x1000
; 56 is the max as otherwise it would overwrite currently executing code
; Huh. I guess that's why other kernels online mostly load their kernel to 0x100000 or 0x10000 instead of 0x1000
; Also GRUB uses 1 MiB
; I wonder why the Writing your own Operating System from Scratch book uses 0x1000 then.

KERNEL_OFFSET 		equ 	0x10000
BYTES_PER_SECTOR	equ 	0x200
SECTORS_TO_LOAD		equ 	63
BOOTLOADER_OFFSET	equ 	0x7c00


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
	; just making sure es is 0x1000
	; This is so when disk_load ing it loads it to ES:BX
	; so 0x1000:0x0000
	; so 0x10000
	mov ax, 0x1000
	mov es, ax

	; Start from the 2nd sector (1st sector is the boot sector)
	; sectors are weird in that they don't start from 0
	mov cx, 0x02
	mov bx, 0x00 
	xchg bx, bx
.kernel_load:
	mov dl, [BOOT_DRIVE]
	; 1 sector at a time because apparently there are some 
	; restrictions which are annoying to get around for
	; more than that (page boundaries/cylinder boundaries)
	mov dh, 1
	call disk_load
	add bx, BYTES_PER_SECTOR
	add cx, 1
	; 63 sectors, the max for 1 track/cylinder is 63
	; Gives us 63*512 bytes to work with 
	; (including code and initialised data from some sections like rodata)
	; in future maybe more complicated logic for moving on to the next cylinder as well
	; See: the wikipedia article https://en.wikipedia.org/wiki/INT_13H
	; Basically the lower 6 bits of CX is used for the sector number (which is why it's capped at 63, 2^6-1)
	; and the other 10 bits is used for the cylinder number (0 to 1023)
	cmp cx, SECTORS_TO_LOAD
	jne .kernel_load
	; actually it's probably more clear if I do it like this
	; actually now that I moved the kernel from 0x1000 to 0x10000 I don't need this
	; I still should be careful of bx reaching 0xFE00 though since then I'll need to modify ES
	;cmp bx, BOOTLOADER_OFFSET
	;jne .kernel_load
	ret



[bits 32]

BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_str_pm
	jmp KERNEL_OFFSET
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
