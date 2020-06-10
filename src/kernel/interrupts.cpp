#include "interrupts.h"
#include <stdint.h>
#include "idt.h"
#include "irq.h"

namespace{
	auto interrupts_initialised {false};
	// I mean, it's not entirely reliable since elsewhere in the code 
	// it is possible to call asm "sti" or "cli" directly
	auto interrupts_enabled{false}; 
}

void os::Interrupts::Init() {
	os::IDT::Init();
	os::IRQ::Init();
	os::IDT::Flush();
	os::Interrupts::EnableInterrupts();
	interrupts_enabled = true;
}

extern "C" void asm_enable_irq();
extern "C" void asm_disable_irq();

void os::Interrupts::EnableInterrupts() {
	interrupts_enabled = true;
	asm_enable_irq();
}

void os::Interrupts::DisableInterrupts() {
	interrupts_enabled = false;
	asm_disable_irq();
}

bool os::Interrupts::IsEnabled() {
	return interrupts_enabled;
}

bool os::Interrupts::IsInitialised() {
	return interrupts_initialised;
}
