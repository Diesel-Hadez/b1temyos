CC	= i686-elf-gcc
CXX	= i686-elf-g++
LD	= i686-elf-ld
NASM	= nasm
QEMU	= qemu-system-i386

CXX_FLAGS	= -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

KERNEL_PATH	= src/kernel/
BOOT_PATH	= src/boot/
OBJ_PATH	= obj/

#Note that boot.asm has to be the first
BOOT_FILES	= boot.asm\
		  disk_utils.asm\
		  gdt.asm\
		  globals.asm\
		  pmode.asm\
		  screen_utils.asm\
		  screen_utils_pmode.asm

KERNEL_FILES	= kernel_main.cpp\
		  ports.cpp\
		  utils.cpp

DEPS		= $(KERNEL_FILES:%.cpp=$(OBJ_PATH)/%.d)
KERNEL_HEADERS	= ports.h


-include $(DEPS)

obj/kernel.bin: $(addprefix $(OBJ_PATH), kernel_entry.elf)\
       	$(addprefix $(OBJ_PATH), $(KERNEL_FILES:.cpp=.elf))
	mkdir -p obj
	$(CXX) -T linker.ld -ffreestanding -O2 -nostdlib -lgcc $^

obj/%.elf: src/kernel/%.cpp
	mkdir -p obj
	$(CXX) -o $@ -c $< $(CXX_FLAGS) -MMD

obj/kernel_entry.elf: src/kernel/kernel_entry.asm
	mkdir -p obj
	$(NASM) $< -f elf -o $@

#Overrides obj/%.bin
obj/boot_sect.bin: $(addprefix $(BOOT_PATH), $(BOOT_FILES))
	mkdir -p obj
	$(NASM) -I src/boot $< -f bin -o $@

rel/os-image.bin: obj/boot_sect.bin obj/kernel.bin
	mkdir -p rel
#Concatenate the files into one raw image
	cat $^ > $@
#Qemu needs raw images (except for if=floppy) to be at least 32 sectors large
#Shamelessly stolen from https://unix.stackexchange.com/a/196727
	dd if=/dev/zero of=$@ bs=1 count=1 seek=$(shell expr 512 \* 32 - 1)

.PHONY: image
image: rel/os-image.bin

.PHONY: all
all: rel/os-image.bin

.PHONY: run
run: rel/os-image.bin
	$(QEMU) -drive if=ide,format=raw,file=$<

.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf rel/*
