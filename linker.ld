OUTPUT_FORMAT(binary)
OUTPUT(obj/kernel.bin)
ENTRY(_start)

SECTIONS
{
	. = 0x1000;
	_kernel_start = .;
	.text ALIGN (4K) : ALIGN(4K)
	{
		*(.text)
	}

	.rodata ALIGN (4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data ALIGN (4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss ALIGN (4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
	_kernel_end = .;
}
