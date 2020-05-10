#include "ports.h"

unsigned char os::port::in_byte(unsigned short port) {
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port) );
	return result;
}

void os::port::out_byte(unsigned short port, unsigned char data){
	__asm__("out %%al, %%dx" : : "a" (data) , "d" (port) );
}

unsigned char os::port::in_word(unsigned short port) {
	unsigned char result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port) );
	return result;
}

void os::port::out_word(unsigned short port, unsigned short data) {
	__asm__("out %%ax, %%dx" : : "a" (data) , "d" (port) );
}
