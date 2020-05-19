#include "utils.h"
#include "common.h"
#include "ports.h"

//Don't have a heap yet, so I can't return a char*
//Because if I do, it would be a local variable for the function
//on the stack which is removed when the function returns.
//Be sure that ret has space for 10 bytes, plus the NULL Terminator
//So...11 bytes
void os::util::hex_to_str(const unsigned int num, char * ret, const bool trim) {
	static const char *  HEX_REF = "0123456789ABCDEF";
	os::util::memset(reinterpret_cast<byte *>(ret), 0, 11);
	os::util::memset(reinterpret_cast<byte *>(ret), '0', 10);
	ret[0] = '0';
	ret[1] = 'x';
	unsigned int write_pos = 2;
	for (int i=7; i >= 0;i--){
		char result = HEX_REF[(num >> (4*i)) & 0xF];
		if (trim && 
			result == '0'){
			continue;
		}
		ret[write_pos++]= result; 
	}
	ret[write_pos] = '\x00';
}

unsigned int os::util::pow(const unsigned int num, const unsigned int pow) {
	unsigned int ret = 1;
	for (unsigned int i=0; i<pow;i++){
		ret *= num;
	}
	return ret;
}

//Don't have a heap yet, so I can't return a char*
//Because if I do, it would be a local variable for the function
//on the stack which is removed when the function returns.
//Be sure that ret has space for 10 bytes, plus the NULL Terminator
//So...11 bytes
void os::util::int_to_str(const unsigned int num, char * ret, const bool trim) {
	static const char * INT_REF	= "0123456789";
	os::util::memset(reinterpret_cast<byte *>(ret), 0, 11);
	os::util::memset(reinterpret_cast<byte *>(ret), '-', 10);
	unsigned int write_pos=0;
	for (int i =0; i <= 9; i++) {
		unsigned int left 	= os::util::pow(10, 10-i);
		unsigned int right 	= os::util::pow(10, 9-i);
		unsigned int index 	= ((num % left) - (num % right))/right;
		if (trim &&
			num <= right){
			continue;
		}
		ret[write_pos++] = INT_REF[index];
	}
	//Null terminate
	ret[write_pos] = '\x00';
}
bool os::util::test_a20_enabled() {
	static byte * test_memory_1 = (byte*) 0x12345;
	static byte * test_memory_2 = (byte*) (0x12345 | 0x100000);
	// Is overwriting potentially important areas of memory just for the sake of checking if the A20
	// line is enabled fine? Probably.
	test_memory_1[0] = 'Z';
	test_memory_2[0] = 'Q';
	return test_memory_1[0] != test_memory_2[0];
}

//Unable to test this
void os::util::fast_enable_a20() {
	using namespace os::port;
	unsigned char a20 = in_byte(TYPE::FAST_A20);
	//Set 2nd bit (so OR with 0b10 or 2^1)
	a20 |= 2;
	os::port::out_byte(0x92, a20);
}


void os::util::memcpy(unsigned char * data, 
	const unsigned char * src,
	unsigned int count){
	for (unsigned int idx=0; idx < count; idx++){
		data[idx] = src[idx]; 
	}
}

void os::util::memset(unsigned char * data, 
	unsigned char val,
	unsigned int count){
	for (unsigned int idx=0; idx < count; idx++){
		data[idx] = val; 
	}
}

// I'm assuming little-endian...
void os::util::memsetw(unsigned short * data, 
	unsigned short val,
	unsigned int count){
	for (unsigned int idx=0; idx < count; idx+= 2){
		data[idx]	= val & 0xFF; 
		data[idx+1]	= (val >> 8) & 0xFF;
	}
}

unsigned int os::util::strlen(const char * str){
	unsigned int ret=0;
	while (str[ret] != '\x00'){ret++;}
	return ret;
}
