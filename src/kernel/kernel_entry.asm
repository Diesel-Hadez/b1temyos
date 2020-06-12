KERNEL_ENTRY_POINT equ 0xC0000000

%define virtual_to_physical(addr) addr-KERNEL_ENTRY_POINT

; Side-note, not sure I should be using primitive directives
; instead of the user directives ([bits 32] vs bits 32)
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
	; Shamelessly stolen from https://wiki.osdev.org/Higher_Half_x86_Bare_Bones
	; Though converted to nasm syntax
	install_paging:
	mov edi, virtual_to_physical(kernel_page_table)
	mov esi, 0
	;1023 pages (1024th is for VGA text buffer)
	mov ecx, 1023

	; from linker.ld
	[extern _kernel_start]
	[extern _kernel_end]
	a1:
		cmp esi, virtual_to_physical(_kernel_start)
		; the f and b is "use the label with the name which is forward or backward".
		; I think it forces relative addressing rather than absolute
		jl a2
		cmp esi, virtual_to_physical(_kernel_end)
		jge a3

		mov edx, esi
		; Present and writable
		or edx, 0x03
		mov [edi], edx

	a2:
		; 4096 bytes per page
		add esi, 4096
		; 4 bytes per entry
		add edi, 4
		loop a1

	a3:
		mov [virtual_to_physical(kernel_page_table)+1023*4], long (0xb8000 | 0x03)

		mov [virtual_to_physical(kernel_page_directory)], long virtual_to_physical(kernel_page_table + 0x03)
		mov [virtual_to_physical(kernel_page_directory) + 768*4], long virtual_to_physical(kernel_page_table + 0x03)

		mov ecx, virtual_to_physical(kernel_page_directory)
		mov cr3, ecx

		mov ecx, cr3
		or ecx, 0x80010000
		mov ecx, cr0
		mov ecx, _continue_main
		jmp ecx
_continue_main:
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

section .data

; Testing expansion of the kernel
;%rep 512*7+253
;dd 0xdeadbabe
;%endrep

; Testing Higher Half Kernel
; With some paging
[global kernel_page_table]
align 0x1000
kernel_page_table:
%assign addr 0
%rep 1024
; Enable
dd addr | 1
%assign addr addr+0x1000
%endrep

[global kernel_page_directory]
align 0x1000
kernel_page_directory:
%rep 1024
dd virtual_to_physical(kernel_page_table) + 1
%endrep
