#ifndef ISR_H
#define ISR_H
#include <stdint.h>

namespace os{
	namespace ISR{
		constexpr unsigned short IRQ0 = 32;
		constexpr unsigned short IRQ1 = 33;
		constexpr unsigned short IRQ2 = 34;
		constexpr unsigned short IRQ3 = 35;
		constexpr unsigned short IRQ4 = 36;
		constexpr unsigned short IRQ5 = 37;
		constexpr unsigned short IRQ6 = 38;
		constexpr unsigned short IRQ7 = 39;
		constexpr unsigned short IRQ8 = 40;
		constexpr unsigned short IRQ9 = 41;
		constexpr unsigned short IRQ10 = 42;
		constexpr unsigned short IRQ11 = 43;
		constexpr unsigned short IRQ12 = 44;
		constexpr unsigned short IRQ13 = 45;
		constexpr unsigned short IRQ14 = 46;
		constexpr unsigned short IRQ15 = 47;
		struct Registers{
			uint32_t ds; // Data Segment
			uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; // Pushed by pusha
			uint32_t int_no, err_code; // int_no is pushed by us, err_code is either pushed by the CPU or substituted with a dummy by us
			uint32_t eip, cs, eflags, esp, ss; // Pushed automatically
		} __attribute__((packed));
		typedef void (*isr_t)(os::ISR::Registers*);
		void register_interrupt_handler(uint8_t n, isr_t handler);
	}
}
#endif
