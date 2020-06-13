.DELETE_ON_ERROR:
.DEFAULT_GOAL := all
CC	= i686-elf-gcc
CXX	= i686-elf-g++
LD	= i686-elf-ld
NASM	= nasm
QEMU	= qemu-system-i386

CXX_FLAGS	= -Iinclude/kernel -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit

KERNEL_PATH	= src/kernel/
BOOT_PATH	= src/boot/
OBJ_PATH	= obj/

# Note that boot.asm has to be the first
BOOT_FILES	= boot.asm\
		  disk_utils.asm\
		  gdt.asm\
		  globals.asm\
		  pmode.asm\
		  screen_utils.asm\
		  screen_utils_pmode.asm

KERNEL_CPP_FILES= kernel_main.cpp\
		  ports.cpp\
		  utils.cpp\
		  terminal.cpp\
		  gdt.cpp\
		  idt.cpp\
		  isr.cpp\
		  irq.cpp\
		  timer.cpp\
		  interrupts.cpp

KERNEL_ASM_FILES= gdt_flush.asm\
		  idt_flush.asm\
		  isr_asm.asm\
		  irq_asm.asm

DEPS		= $(KERNEL_CPP_FILES:%.cpp=$(OBJ_PATH)/%.d)

# Completely useless. I don't know why I keep this
KERNEL_HEADERS	= ports.h utils.h common.h gdt.h terminal.h idt.h

CRTBEGIN_OBJ 	= $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ 	= $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)

CRTI_OBJ 	= crti.o
CRTN_OBJ 	= crtn.o

LINK_OBJS	= $(addprefix $(OBJ_PATH), kernel_entry.elf)\
	$(addprefix $(OBJ_PATH), $(CRTI_OBJ)) $(CRTBEGIN_OBJ)\
       	$(addprefix $(OBJ_PATH), $(KERNEL_CPP_FILES:.cpp=.elf))\
       	$(addprefix $(OBJ_PATH), $(KERNEL_ASM_FILES:.asm=.elf))\
	$(CRTEND_OBJ)\
	$(addprefix $(OBJ_PATH), $(CRTN_OBJ)) 

-include $(DEPS)

obj/kernel.bin: $(LINK_OBJS) 
	-@mkdir -p obj
	$(CXX) -T linker.ld -ffreestanding -O2 -nostdlib -lgcc $^

obj/%.elf: src/kernel/%.cpp
	-@mkdir -p obj
	$(CXX) -o $@ -c $< $(CXX_FLAGS) -MMD

obj/crti.o: src/kernel/crti.asm
	-@mkdir -p obj
	$(NASM) $< -f elf -o $@

obj/crtn.o: src/kernel/crtn.asm
	-@mkdir -p obj
	$(NASM) $< -f elf -o $@

obj/%.elf: src/kernel/%.asm
	-@mkdir -p obj
	$(NASM) $< -f elf -o $@

obj/kernel_entry.elf: src/kernel/kernel_entry.asm
	-@mkdir -p obj
	$(NASM) $< -f elf -o $@

#Overrides obj/%.bin
obj/boot_sect.bin: $(addprefix $(BOOT_PATH), $(BOOT_FILES))
	-@mkdir -p obj
	$(NASM) -I src/boot $< -f bin -o $@

rel/os-image.img: obj/boot_sect.bin obj/kernel.bin
	-@mkdir -p rel
#Concatenate the files into one raw image
	cat $^ > $@
# Qemu needs raw images (except for if=floppy) to be at least 32 sectors large
# Bochs needs raw images to be at least 10MB large
# Shamelessly stolen from https://unix.stackexchange.com/a/196727
# CHS 20_16_63
# 20160 (20*16*63) sectors should be rougly 10MB
# This specific CHS was chosen because it's the size which is automatically generated
# by Boch's 'bximage' utility for a 10MB raw image file, beats me as to why bximage's
# authors chose it.
	dd if=/dev/zero of=$@ bs=1 count=1 seek=$(shell expr 20 \* 16 \* 63 \* 512 - 1)

.PHONY: image
image: rel/os-image.img

.PHONY: all
all: clean rel/os-image.img

.PHONY: run
run: rel/os-image.img
	$(QEMU) -drive if=ide,format=raw,file=$<

.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf rel/*
