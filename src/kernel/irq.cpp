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

namespace {
	// Besides the osdev wiki, I found this:-
	// https://www.eeeguide.com/8259-programmable-interrupt-controller/
	void RemapPIC(uint8_t master_offset, uint8_t slave_offset) {
		using namespace os::port;
		using namespace os::port::TYPE;

		Port pic_master(PIC::MASTER);
		Port pic_slave(PIC::SLAVE);

		Port pic_master_data(PIC::MASTER_DATA);
		Port pic_slave_data(PIC::SLAVE_DATA);

		// TO-DO, maybe a bitset for Initialisation words?
		// "Initialisation words"
		static const uint8_t ICW1_INIT 		= 0x10;
		static const uint8_t ICW1_ICW4_NEEDED 	= 0x01;

		// 8086/8088 mode, without it, it is set to 8085 mode
		static const uint8_t ICW4_8086 		= 0x01;

		// Don't know much about this, but maybe it would alleviate the 
		// pressure of having to send EOI in the IRQ code?
		static const uint8_t ICW4_AUTO_EOI 	= 0x02;

		// IRQ2
		static const uint8_t SLAVE_IRQ_NUM 	= 2;

		// Re-intialise the Programmable Interrupt Controller

		uint8_t initial_pic_master_data = pic_master_data.ReadByte();
		uint8_t initial_pic_slave_data	= pic_slave_data.ReadByte();
		
		// ICW1
		pic_master.Send(static_cast<uint8_t>(ICW1_INIT | ICW1_ICW4_NEEDED));
		pic_slave.Send(static_cast<uint8_t>(ICW1_INIT | ICW1_ICW4_NEEDED));

		// ICW2
		pic_master_data.Send(master_offset);
		pic_slave_data.Send(slave_offset);

		// ICW2
		//
		// Tell master there is a slave PIC in IRQ 2
		// tbh not sure why it takes an IRQ number and not a regular interrupt number
		pic_master_data.Send(static_cast<uint8_t>( 1 << SLAVE_IRQ_NUM));
		// Sets a slave ID
		// I suppose it was chosen arbitrarily? And 2 was chosen because it's in IRQ2
		// so it's easier to remember?
		pic_slave_data.Send(static_cast<uint8_t>(2));

		// ICW4
		pic_master_data.Send(ICW4_8086);
		pic_slave_data.Send(ICW4_8086);

		pic_master_data.Send(initial_pic_master_data);
		pic_slave_data.Send(initial_pic_slave_data);
	}
}

// TO-DO, add check if IDT is already initialised?
// Maybe IDT is better as a singleton
void os::IRQ::Init() {
	// Might need to move this into some globals or something tbh
	static const uint16_t KERNEL_CODE_SEGMENT_SELECTOR = 0x08;

	RemapPIC(0x20, 0x28);

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
}
