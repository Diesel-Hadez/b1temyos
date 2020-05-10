#include "ports.h"
typedef char byte;
#define VIDEO_MEMORY 0xb8000;

//Quick and dirty kprint for starting. Will replace later.
void kprint(const char * text) {
	static byte * video_memory = (byte*) VIDEO_MEMORY;
	static unsigned int counter = 0;
	unsigned int index = 0;
	char a = text[index];
	while (a != 0) {
		video_memory[counter++] = a;
		//White on Black
		video_memory[counter++] = 0x0f;
		a = text[++index];
	}
}

//Don't have a heap yet, so I can't return a char*
//Because if I do, it would be a local variable for the function
//on the stack which is removed when the function returns.
//Be sure that ret has space for 10 bytes, plus the NULL Terminator
void hex_to_str(const unsigned int num, char * ret) {
	static const char *  HEX_REF = "0123456789ABCDEF";
	ret[0] = '0';
	ret[1] = 'x';

	ret[2] = HEX_REF[num & 0xF];
	ret[3] = HEX_REF[(num >> 4) & 0xF];
	ret[4] = HEX_REF[(num >> 8) & 0xF];
	ret[5] = HEX_REF[(num >> 12) & 0xF];
	ret[6] = HEX_REF[(num >> 16) & 0xF];
	ret[7] = HEX_REF[(num >> 20) & 0xF];
	ret[8] = HEX_REF[(num >> 24) & 0xF];
	ret[9] = HEX_REF[(num >> 28) & 0xF];
	ret[10] = '\x00';
}

bool test_a20_enabled() {
	static byte * test_memory_1 = (byte*) 0x12345;
	static byte * test_memory_2 = (byte*) (0x12345 | 0x100000);
	// Is overwriting potentially important areas of memory just for the sake of checking if the A20
	// line is enabled fine? Probably.
	test_memory_1[0] = 'Z';
	test_memory_2[0] = 'Q';
	return test_memory_1[0] != test_memory_2[0];
}

//Unable to test this
void fast_enable_a20() {
	byte a20 = os::port::in_byte(0x92);
	a20 |= 2;
	os::port::out_byte(0x92, a20);
}

extern "C"
void kernel_main(){
	//This IO input supposedly make qemu disable the A20 line for testing purposes
	char test = os::port::in_byte(0xEE);
	char to_print[11] = {0};
	hex_to_str(test & 0xFFFFFFFF, to_print);
	kprint((const char*)to_print);

	if (test_a20_enabled()) {
		kprint("A20 Enabled");
	}
	else {
		kprint("A20 Disabled");
	}
}
