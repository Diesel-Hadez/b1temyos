#include "isr.h"
#include "terminal.h"
#include "ports.h"

typedef os::ISR::isr_t isr_t;

namespace {
	isr_t interrupt_handlers[256];
}

void os::ISR::register_interrupt_handler(uint8_t n, isr_t handler) {
	interrupt_handlers[n] = handler;
}

extern "C"
void isr_handler(os::ISR::Registers * registers) {
	os::Term::kprintf("received interrupt: %d\n", registers->int_no);
}

extern "C"
void irq_handler(os::ISR::Registers * registers) {
	os::Term::kprintf("IRQ received interrupt: %d\n", registers->int_no);
	os::port::Port pic_slave(0xa0);
	os::port::Port pic_master(0x20);
	if (registers->int_no >= 40) {
		//Reset
		pic_slave.Send(static_cast<uint8_t>(0x20));
	}
	pic_master.Send(static_cast<uint8_t>(0x20));
	if (interrupt_handlers[registers->int_no] != 0){
		isr_t handler = interrupt_handlers[registers->int_no];
		handler(registers);
	}

	if (registers->int_no == 33) {
		 unsigned char scan_code = os::port::in_byte(0x60);
		 os::Term::kprintf("Scancode: %x\n", static_cast<unsigned int>(scan_code));
	}
}
