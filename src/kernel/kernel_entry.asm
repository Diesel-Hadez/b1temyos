[bits 32]
[extern kernel_main]
[global _start]

section .bss
align 16
stack_bottom:
	resb 16384	;	16 KiB
stack_top:

section .text
_start:
	mov esp, stack_top
	mov ebp, esp
	call kernel_main
	jmp $
.end:
