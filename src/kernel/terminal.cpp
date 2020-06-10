#include "utils.h"
#include "common.h"
#include "terminal.h"
#include "ports.h"

namespace{
	struct Cell{
		uint8_t character;
		uint8_t colour;
	} __attribute__((packed));

	namespace COMMANDS {
		const unsigned short CURSOR_START	= 0x0a;
		const unsigned short CURSOR_END		= 0x0b;

		const unsigned short CURSOR_POS_LOW 	= 0x0f; 		
		const unsigned short CURSOR_POS_HIGH	= 0x0e; 		
	}

	const unsigned short VGA_WIDTH 	= 80;
	const unsigned short VGA_HEIGHT = 25;
	Cell * video_memory = reinterpret_cast<Cell*>(VIDEO_MEMORY);

	inline void move_cursor(unsigned short x, unsigned short y){
		using namespace os::port;
		unsigned short memory_pos = y * VGA_WIDTH + x;
		out_byte(TYPE::VGA::COMMAND_1, COMMANDS::CURSOR_POS_LOW);
		out_byte(TYPE::VGA::DATA_1, static_cast<uint8_t>(memory_pos & 0xFF));

		out_byte(TYPE::VGA::COMMAND_1, COMMANDS::CURSOR_POS_HIGH);
		out_byte(TYPE::VGA::DATA_1, static_cast<uint8_t>((memory_pos >> 8) & 0xFF));
	}

	inline void enable_cursor(){
		using namespace os::port;
		out_byte(TYPE::VGA::COMMAND_1, COMMANDS::CURSOR_START);
		out_byte(TYPE::VGA::DATA_1, 0);
		out_byte(TYPE::VGA::COMMAND_1, COMMANDS::CURSOR_END);
		out_byte(TYPE::VGA::DATA_1, 0);
	}

	inline void disable_cursor(){
		using namespace os::port;
		out_byte(TYPE::VGA::COMMAND_1, COMMANDS::CURSOR_START);
		out_byte(TYPE::VGA::COMMAND_1, COMMANDS::CURSOR_END);
	}

}

unsigned short os::Term::m_CurCol = 0;
unsigned short os::Term::m_CurRow = 0;
os::Term::Colour os::Term::m_Foreground = os::Term::Colour::CYAN;
os::Term::Colour os::Term::m_Background = os::Term::Colour::BLACK;

void os::Term::EnableCursor(){
	enable_cursor();
}

void os::Term::DisableCursor(){
	disable_cursor();
}

void os::Term::ClearScreen(){
	m_CurCol=0;
	m_CurRow=0;
	for (unsigned int i=0;i<VGA_WIDTH*VGA_HEIGHT;i++){
		puts(' ');
	}
	m_CurCol = 0;
	m_CurRow = 0;
}

os::Term::Colour os::Term::GetBackground(){
	return m_Background;
}
os::Term::Colour os::Term::GetForeground(){
	return m_Foreground;
}

void os::Term::SetBackground(os::Term::Colour c){
	if (c < 0 || c >= 16)
		return;
	m_Background = c;
}

void os::Term::SetForeground(os::Term::Colour c){
	if (c < 0 || c >= 16)
		return;
	m_Foreground = c;
}

void os::Term::Scroll(){
	//Move up
	
	for (unsigned int i = VGA_WIDTH;i < VGA_WIDTH*VGA_HEIGHT;i++){
		video_memory[i-VGA_WIDTH].character 	= video_memory[i].character;
		video_memory[i-VGA_WIDTH].colour 	= video_memory[i].colour;
	}
	//video_memory[0].character = '$';
	//Clear bottom
	m_CurRow = VGA_HEIGHT-1;
	m_CurCol = 0;
	for (unsigned int i=0; i < VGA_WIDTH-1;i++) {
		puts(' ');
	}

	//Reset Col Number
	m_CurRow = VGA_HEIGHT-1;
	m_CurCol = 0;
}

void os::Term::puts(char c){
	if (c == '\n'){
		if (m_CurRow+1 >= VGA_HEIGHT){
			//scroll
			Scroll();
			return;
		}
		m_CurRow++;
		m_CurCol = 0;
		return;
	}
	if (m_CurCol+1 >= VGA_WIDTH) {
		if (m_CurRow+1 >= VGA_HEIGHT){
			//scroll
			Scroll();
		}
		else {
			m_CurCol = 0;
			m_CurRow++;
		}
	}
	video_memory[m_CurRow*VGA_WIDTH+m_CurCol].character 	= c;
	video_memory[m_CurRow*VGA_WIDTH+m_CurCol].colour 	= m_Foreground | (m_Background << 4);
	m_CurCol++;
}

void os::Term::kprintf(const char * text, PrintOptions p){
	unsigned int len = os::util::strlen(text);
	for (unsigned int idx=0; idx < len; idx++) {
		puts(text[idx]);
	}
	move_cursor(m_CurCol, m_CurRow);
}


void os::Term::kprintf(unsigned int num, PrintOptions p){
	char result[11] = {0};
	if (p.HEX){
		os::util::hex_to_str(num, result, p.LITTLE_ENDIAN);
	}
	else {
		os::util::int_to_str(num, result);
	}
	kprintf(result);
}

