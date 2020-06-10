KERNEL_DATA_SEGMENT_SELECTOR	equ 0x10

%macro ISR_NOERRCODE 1
[global isr%1]
isr%1:
	push byte 0
	push byte %1
	jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
[global isr%1]
isr%1:
	push byte %1
	jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7

ISR_ERRCODE 8

ISR_NOERRCODE 9

ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14

ISR_NOERRCODE 15
ISR_NOERRCODE 16

ISR_ERRCODE 17

ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20

ISR_ERRCODE 21

ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

[extern isr_handler]

isr_common_stub:
	pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
	mov ax, ds
	push eax
	mov ax, KERNEL_DATA_SEGMENT_SELECTOR
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	push esp	; pointer to these registers
	
	; Unsure what this is for actually
	cld 		
	call isr_handler

	pop eax
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8	; Clean up the pushed error code and the pushed ISR number
	iret	; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP, modifying the EFlags also has the same effect as sti
