#include "ports.h"
#include "utils.h"
#include "gdt.h"
#include "terminal.h"

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
	os::GDT::Init();
	os::Term::ClearScreen();
	os::Term::kprintf("%s has jumped %d times (In hex, that's %x times)", "James", 296, 296);

	for (unsigned int i=0; i < num_destructors; i++) {
		destructors[i]();
	}
}
