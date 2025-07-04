CC = i686-linux-gnu-gcc
LD = i686-linux-gnu-ld
CFLAGS = -ffreestanding -nostdlib -Wall -Wextra -Iinclude -std=gnu99 -m32
LDFLAGS = -T linker.ld -nostdlib -m elf_i386

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,build/%.o,$(SRC))

.PHONY: all clean iso run

all: kernel.bin

kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o build/kernel.bin $(OBJ)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o build/kernel.bin minios.iso isodir

iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp build/kernel.bin isodir/boot/
	cp boot/grub/grub.cfg isodir/boot/grub/
	grub-mkrescue -o minios.iso isodir

run: iso
	qemu-system-i386 -cdrom minios.iso
