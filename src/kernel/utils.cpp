#include "utils.h"

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
