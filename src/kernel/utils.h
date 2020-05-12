#ifndef UTILS_H
#define UTILS_H

namespace os{
	namespace util{
		void hex_to_str(const unsigned int num, char * ret);
		bool test_a20_enabled();
		void fast_enable_a20();
		void kprint(const char * text);
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
	}
}

#endif
