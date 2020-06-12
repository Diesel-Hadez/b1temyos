#include "ports.h"
#include "utils.h"
#include "gdt.h"
#include "interrupts.h"
#include "terminal.h"
#include "timer.h"

typedef void (*Destructor)();
// If I'm not mistaken the standard says 32?
Destructor destructors[32]	= {0};
unsigned int num_destructors 	= 0;

extern "C"
int atexit(Destructor d){
	if (num_destructors+1 >= 32)
		return -1;
	destructors[num_destructors++] = d;	
	return 0;
}

// Needed because I currently don't have a heap and instead have an unusual way
// of addressing months with a fixed 3 letters per month and a 1 byte separator
// (for the null terminator)
// Note that here months start from 1 (January is the 1st month, December is the 12th month)
uint8_t month_to_index(uint8_t month) {
	return 4 * (month - 1);
}

extern "C"
void kernel_main(){
	os::GDT::Init();
	os::Interrupts::Init();
	os::Timer::Init(50);

	os::Term::ClearScreen();

	// 0x4e00 is at the time of writing at 0x4000 in the os-image.bin file
	/*volatile uint32_t* test = reinterpret_cast<uint32_t*>(0x4e00);
	while (*test == 0xdeadbabe) {
		test += 1;
	}
	os::Term::kprintf("Read: %x\n", *test);
	os::Term::kprintf("Addr: %x\n", reinterpret_cast<uint32_t>(test));
	*/


	// 0xC0001000 should be mapped to the start of kernel
	volatile uint32_t* test = reinterpret_cast<uint32_t*>(0xC0001000);
	unsigned int count=0;
	while (*test == 0) {
		test += 1;
		if (count++ == 1000) {
			os::Term::kprintf("Gave up\n");
			break;
		}
	}
	os::Term::kprintf("Read: %x\n", *test);

	// Since I still don't have a working heap... this is my solution
	// I tried manually adding in a null terminator after each month (JAN\x00FEB...),
	// but apparently that's not allowed 
	char MONTHS[] = "JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC";
	for (uint8_t i=3; i<= 43;i += 4){
		MONTHS[i] = 0;
	}

	// Note that this is in the UTC/GMT timezone
	uint8_t day 	= os::util::ReadCMOSRegister(0x7); //14;
	uint8_t month 	= os::util::ReadCMOSRegister(0x8); //3;
	uint8_t century = os::util::ReadCMOSRegister(0x32); //20;
	uint8_t year 	= os::util::ReadCMOSRegister(0x9); //45;


	// Shamelessly stolen from https://wiki.osdev.org/CMOS
	// Binary Coded Decimals to normal decimals
	day = (day & 0x0F) + ((day / 16) * 10);
	month = (month & 0x0F) + ((month / 16) * 10);
	year = (year & 0x0F) + ((year / 16) * 10);
	century = (century & 0x0F) + ((century / 16) * 10);

	os::Term::kprintf("Today is the %d", day);

	// workaround since I don't have a heap and call call kprintf("%s", os::util::number_with_indicator(number))
	// I really should implement a heap now since it's giving me so much trouble...
	// perhaps even a temp one with placement_address at the end of the program (at the end of the linker script)
	// and with no deallocating temporarily for simplicity.
	// BTW these are apparently called ordinal indicators
	
	// Hard-coding dates is easier for this small range of 1 to 31 rather than having code to extract
	// the rightmost digit.
	// Plus 11 up to and including 19 have exceptions to these rules anyway.
	if (day == 1 || day == 21 || day == 31) {os::Term::kprintf("st");}
	else if (day == 2 || day == 22) {os::Term::kprintf("nd");}
	else if (day == 3 || day == 23) {os::Term::kprintf("rd");}
	else {os::Term::kprintf("th");}

	os::Term::kprintf(" of %s, %d%d\n", reinterpret_cast<char*>(MONTHS+month_to_index(month)), century, year);

	os::Term::kprintf("%s has jumped %d times (In hex, that's %x times)\n", "James", 296, 296);
	while(true) {
		asm volatile ("hlt");
	}
	for (unsigned int i=0; i < num_destructors; i++) {
		destructors[i]();
	}
}
