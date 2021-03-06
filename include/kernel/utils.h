#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include <stddef.h>

namespace os{
	namespace util{
		size_t virtual_to_physical(const size_t address);
		void hex_to_str(const unsigned int num, char * ret, const bool trim=true);
		void int_to_str(const unsigned int num, char * ret, const bool trim=true);
		void disable_a20();
		bool test_a20_enabled();
		void fast_enable_a20();
		void memcpy(unsigned char * data, 
				const unsigned char * src,
				unsigned int count);
		void memset(unsigned char * data, 
				unsigned char val,
				unsigned int count);
		void memsetw(unsigned short * data, 
				unsigned short val,
				unsigned int count);
		unsigned int strlen(const char * str);
		unsigned int pow(const unsigned int num, const unsigned int pow);

		uint8_t ReadCMOSRegister(uint8_t reg);
	}
}

#endif
