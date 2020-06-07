#include "idt.h"
#include "utils.h"
#include "irq.h"
namespace{
	os::IDT::Entry idt_entries[256];
	os::IDT::Ptr idt_ptr;
	uint8_t AccessFlag(uint8_t dpl) {
		//DPL should only be 2 bits large
		dpl &= 0b11;
		return 0x8e | (dpl << 5);
	}

}


void os::IDT::CreateIDTEntry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt_entries[num].base_low 	= base & 0xFFFF;
	idt_entries[num].base_high 	= (base >> 16) & 0xFFFF;
	
	idt_entries[num].sel 		= sel;
	idt_entries[num].always0 	= 0;

	// Uncomment when user-mode has been implemented
	idt_entries[num].flags 		= flags /* | 0x60 */;		
}

extern "C" void asm_idt_flush(uint32_t);

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

void os::IDT::Init(){
	static const uint16_t KERNEL_CODE_SEGMENT_SELECTOR = 0x08;

	idt_ptr.limit 	= sizeof(os::IDT::Entry) * 256 - 1;
	idt_ptr.base 	= reinterpret_cast<uint32_t>(&idt_entries); 

	os::util::memset(reinterpret_cast<uint8_t *>(&idt_entries), 0, sizeof(os::IDT::Entry) * 256);

	// This hurts my eyes. There's probably a better way to do this. An array of the ISRs?;
	CreateIDTEntry( 0, reinterpret_cast<uint32_t>(isr0) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 1, reinterpret_cast<uint32_t>(isr1) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 2, reinterpret_cast<uint32_t>(isr2) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 3, reinterpret_cast<uint32_t>(isr3) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 4, reinterpret_cast<uint32_t>(isr4) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 5, reinterpret_cast<uint32_t>(isr5) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 6, reinterpret_cast<uint32_t>(isr6) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 7, reinterpret_cast<uint32_t>(isr7) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 8, reinterpret_cast<uint32_t>(isr8) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry( 9, reinterpret_cast<uint32_t>(isr9) , KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(10, reinterpret_cast<uint32_t>(isr10), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(11, reinterpret_cast<uint32_t>(isr11), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(12, reinterpret_cast<uint32_t>(isr12), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(13, reinterpret_cast<uint32_t>(isr13), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(14, reinterpret_cast<uint32_t>(isr14), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(15, reinterpret_cast<uint32_t>(isr15), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(16, reinterpret_cast<uint32_t>(isr16), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(17, reinterpret_cast<uint32_t>(isr17), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(18, reinterpret_cast<uint32_t>(isr18), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(19, reinterpret_cast<uint32_t>(isr19), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(20, reinterpret_cast<uint32_t>(isr20), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(21, reinterpret_cast<uint32_t>(isr21), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(22, reinterpret_cast<uint32_t>(isr22), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(23, reinterpret_cast<uint32_t>(isr23), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(24, reinterpret_cast<uint32_t>(isr24), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(25, reinterpret_cast<uint32_t>(isr25), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(26, reinterpret_cast<uint32_t>(isr26), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(27, reinterpret_cast<uint32_t>(isr27), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(28, reinterpret_cast<uint32_t>(isr28), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(29, reinterpret_cast<uint32_t>(isr29), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(30, reinterpret_cast<uint32_t>(isr30), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	CreateIDTEntry(31, reinterpret_cast<uint32_t>(isr31), KERNEL_CODE_SEGMENT_SELECTOR, 0x8E);
	os::IRQ::Init();
	asm_idt_flush(reinterpret_cast<uint32_t>(&idt_ptr));	
	//asm volatile("sti");
}
