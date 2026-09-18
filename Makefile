CC = gcc
AS = nasm
LD = ld
OBJCOPY = objcopy

CFLAGS = -std=gnu99 -m32 -ffreestanding -fno-stack-protector -fno-pie -nostdlib -Wall -Wextra -O2 -I./include

OBJS = build/kernel_entry.o build/switch.o build/interrupt.o build/kernel.o build/vga.o build/keyboard.o build/process.o build/idt.o build/timer.o build/pmm.o build/paging.o



all: seng21213-os.img

build/boot.bin: boot/boot.asm
	@mkdir -p build
	$(AS) -f bin boot/boot.asm -o build/boot.bin

build/kernel_entry.o: kernel/kernel_entry.asm
	@mkdir -p build
	$(AS) -f elf32 kernel/kernel_entry.asm -o build/kernel_entry.o

build/switch.o: kernel/switch.asm
	@mkdir -p build
	$(AS) -f elf32 kernel/switch.asm -o build/switch.o

build/interrupt.o: kernel/interrupt.asm
	@mkdir -p build
	$(AS) -f elf32 kernel/interrupt.asm -o build/interrupt.o

build/%.o: kernel/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.elf: $(OBJS)
	$(LD) -m elf_i386 -nostdlib -T linker.ld $(OBJS) -o build/kernel.elf

build/kernel.bin: build/kernel.elf
	$(OBJCOPY) -O binary build/kernel.elf build/kernel.bin

seng21213-os.img: build/boot.bin build/kernel.bin
	dd if=/dev/zero bs=512 count=2880 of=seng21213-os.img 2>/dev/null
	dd if=build/boot.bin conv=notrunc bs=512 count=1 of=seng21213-os.img 2>/dev/null
	dd if=build/kernel.bin conv=notrunc bs=512 seek=1 of=seng21213-os.img 2>/dev/null

clean:
	rm -rf build seng21213-os.img


run: seng21213-os.img
	qemu-system-i386 -fda seng21213-os.img -display curses
