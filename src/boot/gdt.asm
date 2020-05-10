gdt_start:

;Each GDT segment is 8 bytes

;Null segment
gdt_null:
	dd 0x00 	;dd is 4 bytes
	dd 0x00

;The Code segment and data segment completely overlap
; (This is actually fine because pages used later on can be)
; (used to limit which pages are executable)

;Format:
; dw LIMIT_0_15
; dw BASE_0_15
; db BASE 16_23
; From LSB least significant bit (right to left)
; db+(db/2) TYPE_FLAGS
;	(AC - Set to 0, CPU sets to 1 when segment is accessed)
;	(RW - 1 Yes, 0 No)
;	(DC - 0 grows up, 1 grows down)
;	(Exec - 1 Code i.e: Executable, 0 data selector)
;	(Descriptor type - 1 Code/Data, 0 Traps)
;	(Privilege)
;	(Present/Access)

; dw LIMIT_16_19
;Then more flags (4 bit) (LSB)
;	(AVL - Use for System software i.e: whatever you want)
;	(L - 64bit code segment)
;	(SZ - 0 16bit pmode, 1 32bit pmode)
;	(GR - 0 1 B blocks, 1 4KiB blocks)
; 
;Code segment
gdt_code:
	dw 0xffff	; Limit is 4GiB
	dw 0x00 	; Base (bits 0-15)
	db 0x00 	; Base (bits 16-23)

	db 10011010b
	db 11001111b	

	db 0x00 	; Base (bits 24-31)
	
;Data segment
gdt_data:
	dw 0xffff
	dw 0x00
	db 0x00
	db 10010010b
	db 11001111b
	db 0x00

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

; Some offsets
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data-gdt_start
