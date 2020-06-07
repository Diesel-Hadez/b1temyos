#ifndef PORTS_H
#define PORTS_H

namespace os
{
	namespace port
	{
		namespace TYPE{
			const short FAST_A20 = 0x92;

			namespace  DMA {
			}
			namespace PIC{
			}
			namespace PIT{
			}
			namespace PS2{
			}
			namespace CMOS{
			}
			namespace RTC{
			}

			//Might need to wrap in a namespace Primary and Secondary in future
			namespace ATA{
			}
			namespace VGA{
				const unsigned short INDEX 		= 0x3c0;
				const unsigned short DATA 		= 0x3c1;
				const unsigned short MISC_OUT		= 0x3c2;

				const unsigned short COMMAND_1		= 0x3d4;
				const unsigned short DATA_1		= 0x3d5;
				const unsigned short COMMAND_2		= 0x3c4;
				const unsigned short DATA_2		= 0x3c5;
				const unsigned short COMMAND_3		= 0x3ce;
				const unsigned short DATA_3		= 0x3cf;

				//Stands for Digital to Analog converter. No idea how these work.
				const unsigned short DAC_MASK_REGISTER 	= 0x3c6;

				const unsigned short COLOUR_INDEX 	= 0x3c8;
				const unsigned short COLOUR_DATA	= 0x3c9;
				const unsigned short COLOUR_READ 	= 0x3c7;

			}
			namespace FLOPPY{
			}
		}
		// Helper for example Port colourIndex(0x3c8)
		class Port {
		private:
			const unsigned short m_Port;
		public:
			// It's on the caller to make sure the right overloaded function is used
			// eg.: unsigned int data = 0;
			// port.Send(data);
			// Not sure what it would automatically use, but it's on them to cast it
			// to the right one
			void Send(unsigned char data) const;
			void Send(unsigned short data) const;
			void ReadByte() const;
			void ReadWord() const;
			Port(const unsigned short port);
		};
		unsigned char in_byte(unsigned short port);
		void out_byte(unsigned short port, unsigned char data); 
		unsigned char in_word(unsigned short port);
		void out_word(unsigned short port, unsigned short data); 
	}
}
#endif
