KERNEL_DATA_SEGMENT_SELECTOR 	equ 0x10

%macro IRQ 2
[global irq%1]
irq%1:
	push byte 0
	push byte %2
	jmp irq_common_stub
%endmacro

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

[global asm_enable_irq]
asm_enable_irq:
	sti
	ret

[global asm_disable_irq]
asm_disable_irq:
	cli
	ret

[extern irq_handler]
irq_common_stub:
	pusha

	mov ax, ds
	push eax

	mov ax, KERNEL_DATA_SEGMENT_SELECTOR
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	push esp
	
	cld
	call irq_handler
	pop eax
	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	popa
	add esp, 8
# Actually sti isn't needed, but I'm trying to find out why my program isn't working
# and adding it in to see if it fixes it
# sti
	iret
	
