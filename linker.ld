OUTPUT_FORMAT(binary)
OUTPUT(obj/kernel.bin)
ENTRY(_start)

KERNEL_VPAGE 	= 0xC0000000;
KERNEL_OFFSET 	= 0xC0010000; 

SECTIONS
{
	. = KERNEL_OFFSET;
	_kernel_start = .;
	.text ALIGN (4K) : AT (ADDR (.text) - KERNEL_VPAGE)
	{
		*(.text)
	}

	.rodata ALIGN (4K) :AT (ADDR (.rodata) - KERNEL_VPAGE)
	{
		*(.rodata)
	}

	.data ALIGN (4K) : AT (ADDR (.data) - KERNEL_VPAGE)
	{
		*(.data)
	}

	.bss ALIGN (4K) : AT (ADDR (.bss) - KERNEL_VPAGE)
	{
		*(COMMON)
		*(.bss)
	}
	_kernel_end = .;

	. = ALIGN (4K);
	/* https://stackoverflow.com/a/50612846 */
	_heap_start = .;
}
