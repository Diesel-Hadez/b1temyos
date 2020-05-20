#ifndef ISR_H
#define ISR_H
#include <stdint.h>

namespace os{

	namespace ISR{
		struct Registers{
			uint32_t ds; // Data Segment
			uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; // Pushed by pusha
			uint32_t int_no, err_code; // int_no is pushed by us, err_code is either pushed by the CPU or substituted with a dummy by us
			uint32_t eip, cs, eflags, esp, ss; // Pushed automatically
		} __attribute__((packed));
	}
}
#endif
