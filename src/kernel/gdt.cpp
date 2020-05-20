#include "gdt.h"
#include "terminal.h"

namespace{
	os::GDT::Entry gdt_entries[5];
	os::GDT::Ptr gdt_ptr;

	void CreateGDTEntry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
		gdt_entries[num].base_low 	= base & 0xFFFF;
		gdt_entries[num].base_middle 	= (base >> 16) & 0xFF;
		gdt_entries[num].base_high 	= (base >> 24) & 0xFF;

		gdt_entries[num].limit_low 	= limit & 0xFFFF;
		//First 4 bits are actually for the limit
		gdt_entries[num].granularity 	= (limit >> 16) & 0x0F;

		//Upper 4 bits are for the actual granularity
		//Assume that gran is already configured to the upper bits to have the correct flags
		gdt_entries[num].granularity 	|= gran & 0xF0;
		gdt_entries[num].access 	= access;
	}

	namespace Flags{
		enum class RW{
			ENABLE_READ,
			DISABLE_READ,
			ENABLE_WRITE,
			DISABLE_WRITE,
		};
		enum class DC{
			GROW_UP,
			GROW_DOWN,
			EXEC_LEQ_PRIV,
			EXEC_EQ_PRIV,
		};
		enum class Ex{
			//Code Selector
			EXECUTABLE,
			//Data Selector
			NON_EXECUTABLE,	
		};
		//Descriptor Type
		enum class Type{
			// Code and Data both get set
			CODE,
			DATA,

			//System (i.e: Task State Segment) does not get set
			SYSTEM,
		};

		enum class Privl{
			RING_0,
			RING_1,
			RING_2,
			RING_3,
		};

		namespace Index{
			static const uint8_t PRESENT 	= 7;
			static const uint8_t PRIVL 	= 5;
			static const uint8_t TYPE	= 4;
			static const uint8_t EXEC 	= 3;
			static const uint8_t DC		= 2;
			static const uint8_t RW		= 1;
			static const uint8_t AC		= 0;
		}
	}
	uint8_t AccessFlag(Flags::Privl priv, Flags::Type type, Flags::Ex exec, Flags::DC dc, Flags::RW read_write){
		using namespace Flags;
		uint8_t ret = 0;
		//Set Present bit
		ret |= 1 << Index::PRESENT;
		
		int ring = -1;
		if (priv == Privl::RING_0){
			ring = 0;
		}
		else if (priv == Privl::RING_1){
			ring = 1;
		}
		else if (priv == Privl::RING_2){
			ring = 2;
		}
		else if (priv == Privl::RING_3){
			ring = 3;
		}

		ret |= ring << Index::PRIVL;
		
		if (type == Type::CODE ||
			type == Type::DATA){
			ret |= 1 << Index::TYPE;
		}

		if (exec == Ex::EXECUTABLE){
			ret |= 1 << Index::EXEC;
		}

		if (dc == DC::GROW_DOWN ||
		  	dc == DC::EXEC_LEQ_PRIV ) {
			ret |= 1 << Index::DC;
		}
		
		// TBH I have no idea what to do if it's a system segment
		if (type == Type::CODE){
			// Readable bit
			// Write is never allowed
			if (read_write == RW::ENABLE_READ){
				ret |= 1 << Index::RW;
			}
			//Should never be ENABLE_WRITE or DISABLE_WRITE
		}
		else if (type == Type::DATA){
			// Writable bit
			// Read is always allowed
			if (read_write == RW::ENABLE_WRITE){
				ret |= 1 << Index::RW;
			}
			// Should never be ENABLE_READ or DISABLE_READ
		}

		// Access bit should always be set to 0
		// It's set by the CPU to 1 when accessed
		ret |= 0 << Index::AC;

		return ret;
	}
}

extern  "C" void asm_gdt_flush(uint32_t);

void os::GDT::Init(){
	gdt_ptr.limit 	= (sizeof(os::GDT::Entry)*5)-1;
	gdt_ptr.base 	= reinterpret_cast<uint32_t>(&gdt_entries);

	// Not sure why but the tutorial I use enables read for the code segments
	uint8_t kernel_code_access = AccessFlag(Flags::Privl::RING_0, 
			Flags::Type::CODE, 
			Flags::Ex::EXECUTABLE, 
			Flags::DC::EXEC_EQ_PRIV,
			Flags::RW::ENABLE_READ);

	uint8_t kernel_data_access = AccessFlag(Flags::Privl::RING_0, 
			Flags::Type::DATA, 
			Flags::Ex::NON_EXECUTABLE, 
			Flags::DC::GROW_UP,
			Flags::RW::ENABLE_WRITE);

	uint8_t user_code_access = AccessFlag(Flags::Privl::RING_3, 
			Flags::Type::CODE, 
			Flags::Ex::EXECUTABLE, 
			Flags::DC::EXEC_EQ_PRIV,
			Flags::RW::ENABLE_READ);

	uint8_t user_data_access = AccessFlag(Flags::Privl::RING_3, 
			Flags::Type::DATA, 
			Flags::Ex::NON_EXECUTABLE, 
			Flags::DC::GROW_UP,
			Flags::RW::ENABLE_WRITE);
	os::Term::kprintf("kernel_code_access	: %x\n", kernel_code_access);
	os::Term::kprintf("kernel_data_access	: %x\n", kernel_data_access);
	os::Term::kprintf("user_code_access 	: %x\n", user_code_access);
	os::Term::kprintf("user_data_access 	: %x\n", user_data_access);

	CreateGDTEntry(0, 0, 0, 0, 0);	// Null
	CreateGDTEntry(1, 0, 0xFFFFFFFF, kernel_code_access, 0xCF);	// Code
	CreateGDTEntry(2, 0, 0xFFFFFFFF, kernel_data_access, 0xCF);	// Data
	CreateGDTEntry(3, 0, 0xFFFFFFFF, user_code_access, 0xCF);	// User Code
	CreateGDTEntry(4, 0, 0xFFFFFFFF, user_data_access, 0xCF);	// User Data

	asm_gdt_flush(reinterpret_cast<uint32_t>(&gdt_ptr));
}

