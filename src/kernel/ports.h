#ifndef PORTS_H
#define PORTS_H

namespace os
{
	namespace port
	{
		unsigned char in_byte(unsigned short port);
		void out_byte(unsigned short port, unsigned char data); 
		unsigned char in_word(unsigned short port);
		void out_word(unsigned short port, unsigned short data); 
	}
}
#endif
