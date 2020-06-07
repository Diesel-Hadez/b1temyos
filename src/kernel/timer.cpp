#include "timer.h"
#include "isr.h"
#include "terminal.h"
#include "ports.h"

static unsigned int tick = 0;
static void timer_callback(os::ISR::Registers * regs) {
	tick++;
	os::Term::kprintf("Tick: %d\n", tick);
}

void os::Timer::Init(unsigned int frequency) {
	os::ISR::register_interrupt_handler(os::ISR::IRQ0, &timer_callback);	

	unsigned int divisor = 1193180 / frequency;

	os::port::Port command(0x43);
	os::port::Port data(0x40);

	//command.Send(static_cast<unsigned char>(0x36));
	os::port::out_byte(0x43, 0x36);
	unsigned char low 	= static_cast<unsigned char>(divisor & 0xFF);
	unsigned char high 	= static_cast<unsigned char>((divisor >> 8) & 0xFF);

	os::port::out_byte(0x40, low);
	os::port::out_byte(0x40, high);
	//data.Send(low);
	//data.Send(high);
	os::Term::kprintf("Registered timer!\n");
}
