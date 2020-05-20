[global asm_gdt_flush]
; 0x00 would be NULL segment entry
; 0x08 would be Kernel Code segment entry
; 0x10 would be Kernel Data segment entry

KERNEL_CODE_SEGMENT_OFFSET	equ 0x08
KERNEL_DATA_SEGMENT_OFFSET 	equ 0x10

asm_gdt_flush:
	mov eax, [esp+4]
	lgdt [eax]

	mov ax, KERNEL_DATA_SEGMENT_OFFSET
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp KERNEL_CODE_SEGMENT_OFFSET:.flush

.flush:
	ret
