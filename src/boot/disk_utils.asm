; Loads DH sectors to ES:BX from drive DL
; FYI, the ES:BX is part of the BIOS function, not initialised here
disk_load:
	push dx

	mov ah, READ_SECTOR	; bios function read_sectors
	mov al, dh		; num_sectors
	mov ch, 0x00		; cylinder 0
	mov dh, 0x00 		; head 0
	mov cl, 0x02		; start from 2nd sector (1st sector is boot sector)

	int 0x13

	jc disk_error

	pop dx
	cmp dh, al		; ret al is sectors_read. Check with dx expected num sectors
	jne disk_error
	ret

disk_error:
	mov ax, DISK_ERROR_MSG
	call print_str
	jmp $

DISK_ERROR_MSG db "Disk read error!", 0
