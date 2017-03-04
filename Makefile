#!Makefile

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link update_image
	@echo Success!

%.o: %.c
	@echo Complie C Codes $< ...
	$(CC) $(C_FLAGS) $< -o $@

%.o: %.s
	@echo Complie ASM Codes $< ...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo Link Kernel Files...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o MortyOSKernel

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) MortyOSKernel

.PHONY:update_image
update_image:
	sudo mount floppy.img /mnt/kernel
	sudo cp MortyOSKernel /mnt/kernel/MortyOSKernel
	sudo cp scripts/imgConf/grub.conf /mnt/kernel/boot/grub
	sudo cp scripts/imgConf/menu.lst /mnt/kernel/boot/grub
	sudo cp initrd /mnt/kernel/initrd
	sleep 1
	sudo umount /mnt/kernel

.PHONY:mount_image
mount_image:
	sudo mount floppy.img /mnt/kernel

.PHONY:umount_image
umount_image:
	sudo umount /mnt/kernel

.PHONY:qemu
qemu:
	qemu -fda floppy.img -boot a -m 512

.PHONY:debug
debug:
	qemu -S -s -fda floppy.img -m 512 -boot a &
	sleep 1
	cgdb -x scripts/gdbinit