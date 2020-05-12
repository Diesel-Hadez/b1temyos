#ifndef UTILS_H
#define UTILS_H

namespace os{
	namespace util{
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
