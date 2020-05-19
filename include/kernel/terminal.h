#ifndef TERMINAL_H
#define TERMINAL_H

namespace os{
	class Term{
		public:
			enum Colour{
				BLACK 		= 0,
				BLUE 		= 1,
				GREEN 		= 2,
				CYAN 		= 3,
				RED 		= 4,
				MAGENTA 	= 5,
				BROWN 		= 6,
				LIGHT_GREY 	= 7,
				DARK_GREY 	= 8,
				LIGHT_BLUE 	= 9,
				LIGHT_GREEN 	= 10,
				LIGHT_CYAN 	= 11,
				LIGHT_RED 	= 12,
				LIGHT_MAGENTA 	= 13,
				LIGHT_BROWN 	= 14,
				WHITE 		= 15,
			};

			struct PrintOptions{
				bool PAD_ZEROES;
				bool PREPEND_0X;
				bool HEX;
				bool LITTLE_ENDIAN;
				PrintOptions(const bool& pz, 
						const bool& p0, 
						const bool &h,
						const bool &le) : PAD_ZEROES(pz), PREPEND_0X(p0), HEX(h), LITTLE_ENDIAN(le){	}
			};

		private:
			static unsigned short m_CurRow;
			static unsigned short m_CurCol;
			static void puts(char c);
			static void Scroll();
			Term() = delete;
			~Term() = delete;
			Term(const Term &t) = delete;
			Term(Term &&t) = delete;
			Term& operator=(const Term &t) = delete;
			Term& operator=(Term &&t) = delete;
			static Colour m_Foreground;
			static Colour m_Background;

			static void wrapper_hex_to_str(const unsigned int num, char * ret);
		public:
			static void SetForeground(Colour c);
			static void SetBackground(Colour c);

			static Colour GetForeground();
			static Colour GetBackground();
			static void ClearScreen();

			static void EnableCursor();
			static void DisableCursor();
			//Shamelessly stolen from https://en.cppreference.com/w/cpp/language/parameter_pack
			static void kprintf(const char * text, PrintOptions p=PrintOptions(true,true,true,true));
			static void kprintf(unsigned int num, PrintOptions p=PrintOptions(true,true,true, true));

			template<typename T, typename... Targs>
			static void kprintf(const char * format, T value, Targs... Fargs) {
				while (*format != '\x00'){
					if (format[0] == '%'){
						//There is an obvious vuln here :)
						switch (format[1]){
							case 's':
								kprintf(value);
								break;
							case 'x':
								kprintf(value,PrintOptions(true,true,true,true));
								break;
							case 'd':
								kprintf(value,PrintOptions(true,true,false,true));
								break;
							default:
								break;
						};
						kprintf(format+2, Fargs...);
						return;
					}
					puts(*format);	
					format++;
				}
			}
	};
}

#endif
