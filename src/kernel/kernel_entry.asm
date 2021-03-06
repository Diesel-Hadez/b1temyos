KERNEL_ENTRY_POINT equ 0xC0000000

; Just to be clear, this macro is only for in the context of this
; specific temporary kernel page directory and for data after KERNEL_ENTRY_POINT 

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

; Increased from 32 KiB to 64KiB because it was getting dangerously close to 0x10000 physical
stack_bottom:
	resb 65536 ;	64 KiB
stack_top:

; Testing Higher Half Kernel
; With some paging
; Maybe have this kernel_page_table and kernel_page_directory in C++ and 
; extern it here.
[global kernel_page_table]
align 0x1000
kernel_page_table:
resb 4096

[global kernel_page_directory]
align 0x1000
kernel_page_directory:
resb 4096

; Linker is set up to interpret the start as 0xC00010000 instead of 
; the actual 0x10000 we are at now. Be careful before setting up paging
; not to use a virtual address instead of a physical one.
section .text
_start:
	; Shamelessly stolen from https://wiki.osdev.org/Higher_Half_x86_Bare_Bones
	; Though converted to nasm syntax
	install_paging:
	xchg bx, bx
	mov [0xb8000], byte 0x41
	mov edi, virtual_to_physical(kernel_page_table)
	mov esi, 0
	;1023 pages (1024th is for VGA text buffer)
	mov ecx, 1023

	; from linker.ld
	[extern _kernel_start]
	[extern _kernel_end]
	[extern _heap_start]
	a1:
		; comment out to map everything and not just the kernel
		; Useful for say, A20 line checks

		; 1MiB Kernel Heap
		; Probably not enough, expand in the future.
		; Address depends on how large the OS currently is, get the address with _heap_start
		cmp esi, virtual_to_physical(_heap_start)
		jge a4
		cmp esi, virtual_to_physical(_heap_start + 0x100000)
		jge a3

		; Map kernel_start to kernel_end
		cmp esi, virtual_to_physical(_kernel_start)
		jl a2
		cmp esi, virtual_to_physical(_kernel_end)
		jge a2
	a4:
		mov edx, esi
		; Present and writable
		or edx, 0x03
		mov [edi], edx

	a2:
		; 4096 bytes per page
		add esi, 4096
		; 4 bytes per entry
		add edi, 4
		loop a1 ; loop is equivalent to "dec ecx; jnz"

	a3:
		; VGA
		; virtual(kernel_page_table) + 1023 entries * 0x1000 bytes per page = 0xC03FF000
		; 0x03 is to set flags present and writable
		mov [virtual_to_physical(kernel_page_table)+1023*4], long (0x000b8000 | 0x03)
		
		; 0x03 is present and writable
		; first pagetable array in array kernel_page_directory
		; identity map (0x00) so we can continue code
		mov [virtual_to_physical(kernel_page_directory)], long virtual_to_physical(kernel_page_table + 0x03)

		; 768th pagetable array in array kernel_page_directory
		; Each of the entries (total entries being 768/4) 
		; points to a 4096 byte area with their own each
		; entry having 4096 bytes
		; 4096*(768/4)*4096 = 0xC0000000
		mov [virtual_to_physical(kernel_page_directory) + 768*4], long virtual_to_physical(kernel_page_table + 0x03)

		mov ecx, virtual_to_physical(kernel_page_directory)
		mov cr3, ecx

		mov ecx, cr0
		; Bit 31 is paging, Bit 16 is write-protect
		or ecx, 0x80010000

		mov cr0, ecx

		lea ecx, [_continue_main]
		jmp ecx
_continue_main:
	;unload first page
	mov [kernel_page_directory], long 0

	; force reload
	mov ecx, cr3
	mov cr3, ecx

	mov esp, stack_top
	mov ebp, esp
	call _init
	sti
	xchg bx, bx
	call kernel_main
	call _fini
	cli
	hlt
	jmp $
.end:
