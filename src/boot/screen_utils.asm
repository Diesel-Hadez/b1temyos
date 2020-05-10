; Usage: let ax be the 16-bit hex number
print_hex:
	push ax
	push bx
	push cx

	mov dx, ax
	mov cx, 12

	mov ax, zero_x
	call print_str
	
	mov ah, TELETYPE
	.loop_de_loop:
		mov bx, 0xf
		shl bx, cl
		and bx, dx 
		shr  bx, cl
		mov al, [hex_lookup+bx]
		int 0x10
		cmp cx, 0
		sub cx, 4
		jge .loop_de_loop
		

	pop ax
	pop cx
	pop bx
	ret

; Usage: let ax be address of null-terminated string
print_str:
	push bx
	push cx
	mov bx, ax
	mov ah, TELETYPE 
	mov al, [bx]
	.str_loop:
		int 0x10
		inc bx
		mov al, [bx]
		cmp al, 0
		jne .str_loop
	pop cx
	pop bx
	ret

clear_screen:
	mov dh, 0x00
	mov al, ' '
	.loop_de_loop:
		mov cx, 0x00
		mov ah, 0x02
		mov dl, 0x00
		int 0x10
		mov ah, TELETYPE 
		.loop_de_loop_two:
			int 0x10
			inc cx
			cmp cx, 0xb0
			jle .loop_de_loop_two
		inc dh
		cmp dh, 0x10
		jle .loop_de_loop
	mov ah, 0x02
	mov dl, 0x00
	mov dh, 0x00
	int 0x10
	ret

zero_x:
	db '0x', 0

; Doesn't need to be null-terminated
hex_lookup:
	db '0123456789ABCDEF'
