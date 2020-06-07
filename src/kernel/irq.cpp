#include "irq.h"
#include "idt.h"
#include "ports.h"

extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

// TO-DO, add check if IDT is already initialised?
// Maybe IDT is better as a singleton
void os::IRQ::Init() {
	// Might need to move this into some globals or something tbh
	static const uint16_t KERNEL_CODE_SEGMENT_SELECTOR = 0x08;

	// This reinits the Programmable Interval Timer
	os::port::out_byte(0x20, 0x11);
	os::port::out_byte(0xA0, 0x11);
	os::port::out_byte(0x21, 0x20);
	os::port::out_byte(0xA1, 0x28);
	os::port::out_byte(0x21, 0x04);
	os::port::out_byte(0xA1, 0x02);
	os::port::out_byte(0x21, 0x01);
	os::port::out_byte(0xA1, 0x01);
	os::port::out_byte(0x21, 0x0);
	os::port::out_byte(0xA1, 0x0);

	os::IDT::CreateIDTEntry(32, reinterpret_cast<uint32_t>(irq0), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(33, reinterpret_cast<uint32_t>(irq1), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(34, reinterpret_cast<uint32_t>(irq2), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(35, reinterpret_cast<uint32_t>(irq3), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(36, reinterpret_cast<uint32_t>(irq4), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(37, reinterpret_cast<uint32_t>(irq5), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(38, reinterpret_cast<uint32_t>(irq6), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(39, reinterpret_cast<uint32_t>(irq7), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(40, reinterpret_cast<uint32_t>(irq8), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(41, reinterpret_cast<uint32_t>(irq9), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(42, reinterpret_cast<uint32_t>(irq10), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(43, reinterpret_cast<uint32_t>(irq11), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(44, reinterpret_cast<uint32_t>(irq12), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(45, reinterpret_cast<uint32_t>(irq13), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(46, reinterpret_cast<uint32_t>(irq14), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);
	os::IDT::CreateIDTEntry(47, reinterpret_cast<uint32_t>(irq15), KERNEL_CODE_SEGMENT_SELECTOR, 0x8e);

	//Enable Keyboard interrupts only for testing purposes
	//os::port::out_byte(0x21,0xfd);
	//os::port::out_byte(0xa1,0xff);
}
