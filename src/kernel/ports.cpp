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

os::port::Port::Port(const unsigned short port): m_Port(port) {
}

void os::port::Port::Send(unsigned char data) const {
	os::port::out_byte(m_Port, data);
}

void os::port::Port::Send(unsigned short data) const {
	os::port::out_word(m_Port, data);
}

void os::port::Port::ReadByte() const {
	os::port::in_byte(m_Port);
}

void os::port::Port::ReadWord() const {
	os::port::in_word(m_Port);
}
