#include "timer.h"
#include "isr.h"
#include "terminal.h"
#include "ports.h"

namespace{
	unsigned int tick {0};
	unsigned int timer_frequency {0};
	unsigned int elapsed_seconds {0};
}
static void timer_callback(os::ISR::Registers * regs) {
	tick++;
	// Is called 1 time per second
	// Could also do tick % frequency == 0 if total time is needed
	if (tick >= timer_frequency) {
		elapsed_seconds++;
		os::Term::kprintf("Tick: %d\n", elapsed_seconds);
		tick = 0;
	}
}

void os::Timer::Init(unsigned int frequency) {
	// Bochs doesn't set tick to 0 for some reason even though
	// I initialised it
	tick = 0;
	timer_frequency = frequency;
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
