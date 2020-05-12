#include "ports.h"
#include "utils.h"
#include "gdt.h"
/*
extern "C"
void __cxa_atexit(void) {
	kprint("Unsupported feature: __cxa_atexit");
}
*/
class ConstructTest{
	public:
		void DoStuff(){os::util::kprint("DoStuff");}
		ConstructTest(){os::util::kprint("ARG");}
		~ConstructTest(){os::util::kprint("GRA");}
};

typedef void (*Destructor)();
// If I'm not mistaken the standard says 32?
Destructor destructors[32]	= {0};
unsigned int num_destructors 	= 0;

extern "C"
int atexit(Destructor d){
	if (num_destructors+1 >= 32)
		return -1;
	destructors[num_destructors++] = d;	
	return 0;
}

extern "C"
void kernel_main(){
	{ConstructTest a;a.DoStuff();}
	os::GDT::get().Init();
	//This IO input supposedly make qemu disable the A20 line for testing purposes
	unsigned char test = os::port::in_byte(0xEE);
	char to_print[11] = {0};
	os::util::hex_to_str(test & 0xFFFFFFFF, to_print);
	os::util::kprint((const char*)to_print);

	if (os::util::test_a20_enabled()) {
		os::util::kprint("A20 Enabled");
	}
	else {
		os::util::kprint("A20 Disabled");
	}

	for (unsigned int i=0; i < num_destructors; i++) {
		destructors[i]();
	}
}
