OBJECTS = loader.o drivers/io/io.o drivers/io/io_asm.o kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

VERSION ?= $(shell grep '^version=' ver.txt | cut -d'=' -f2)
ISONAME ?= $(shell grep '^isoname=' ver.txt | cut -d'=' -f2)


all: kernel.elf os.iso

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/kernel.elf
	cp iso/boot/grub/menu.lst iso/boot/grub/menu.lst 2>/dev/null || true
	genisoimage -R \
	            -b boot/grub/stage2_eltorito \
	            -no-emul-boot \
	            -boot-load-size 4 \
	            -A $(ISONAME) \
	            -input-charset utf8 \
	            -quiet \
	            -boot-info-table \
	            -o $(ISONAME).iso \
	            iso
	mkdir -p OS
	cp $(ISONAME).iso OS/

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o drivers/io/*.o kernel.elf *.iso iso/boot/kernel.elf releases
	rm -rf OS/*.iso

release: clean all
	mkdir -p releases
	cp $(ISONAME).iso releases/$(ISONAME)-v$(VERSION).iso

.PHONY: all clean release