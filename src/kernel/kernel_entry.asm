[bits 32]
[extern kernel_main]
[extern _init]
[extern _fini]
[global _start]

section .bss
align 16
stack_bottom:
	resb 32768	;	32 KiB
stack_top:

section .text
_start:
	mov esp, stack_top
	mov ebp, esp
	call _init
	sti
	call kernel_main
	call _fini
	cli
	hlt
	jmp $
.end:
