[global asm_idt_flush]

asm_idt_flush:
	mov eax, [esp+4]
	lidt [eax]
	ret
