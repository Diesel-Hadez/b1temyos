#include "isr.h"
#include "terminal.h"

extern "C"
void isr_handler(os::ISR::Registers * registers) {
	os::Term::kprintf("received interrupt: %d\n", registers->int_no);
}
