#ifndef IDT_H
#define IDT_H
#include <stdint.h>

namespace os{
	namespace IDT{
		// Really should be either private in a static
		// class or put into an anonymous namespace
		// in the idt.cpp file
		struct Entry{
			uint16_t base_low;
			uint16_t sel;
			uint8_t always0;
			uint8_t flags;
			uint16_t base_high;			
		} __attribute__((packed));

		struct Ptr{
			uint16_t limit;
			uint32_t base;
		} __attribute__((packed));
		
		void Flush();
		void Init();
		void CreateIDTEntry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags); 
	}
}
#endif
