SRC_ASM := \
	src/asm/boot.asm \
	src/asm/low_level_functions.asm \
	src/asm/ex_handlers.asm src/asm/int_handlers.asm
SRC_C   := \
	src/c/entry.c \
	src/c/kernel/idt.c \
	src/c/kernel/gdt.c \
	src/c/kernel/exception_handler.c \
	src/c/kernel/interrupt_handler.c \
	src/c/drivers/keyboard/keyboard.c \
	src/c/drivers/timer/timer.c \
	src/c/drivers/file_system/file_system.c \
	src/c/drivers/timer_handler/timer_handler.c \
	src/c/bash/bash.c \
	src/c/screensaver/screensaver.c \
	src/c/bash/messages/messages.c \
	src/c/editor/editor.c \
	src/c/drivers/serial_port/serial_port.c \
	src/c/utils/cmp_string/cmp_string.c \
	src/c/utils/string_concat/string_concat.c \
	src/c/utils/string_copy/string_copy.c \
	src/c/utils/string_length/string_length.c \
	src/c/utils/random/random.c \
	src/c/utils/VGA_COLORS/VGA_COLORS.c \
	src/c/drivers/framebuffer/framebuffer.c \

OBJ_ASM := $(patsubst src/asm/%.asm, build/asm/%.o, $(SRC_ASM))
OBJ_C   := $(patsubst %.c, build/kernel/%.o, $(SRC_C))

all: kernel.bin

kernel.bin: $(OBJ_ASM) $(OBJ_C)
	@echo "Linking..."
	@mkdir -p $(@D)
	ld -m elf_i386 -T link.ld -Lbuild $(OBJ_ASM) $(OBJ_C) -o build/kernel.bin

build/asm/%.o: src/asm/%.asm
	@echo "Assembling $<..."
	@mkdir -p $(@D)
	nasm -f elf32 $< -o $@

build/kernel/%.o: %.c
	@echo "Compiling $<..."
	@mkdir -p $(@D)
	gcc -ffreestanding -m32 -fno-pie -Wall -Wextra -c $< -o $@

clean:
	rm -rf build

kernel.iso: kernel.bin
	cp build/kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o build/kernel.iso iso

boot: clean kernel.bin
	qemu-system-i386 -kernel build/kernel.bin

boot_stdio: clean kernel.bin
	qemu-system-i386 -monitor stdio -kernel build/kernel.bin

boot_iso: clean kernel.iso
	qemu-system-i386 -cdrom build/kernel.iso

.PHONY: all clean
