TARGET := gleoos-v1.0.1
BUILD := build

CFLAGS := -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -fno-stack-protector
LDFLAGS := -m32 -T linker.ld -ffreestanding -O2 -nostdlib -no-pie

.PHONY: all clean run

all: $(BUILD)/$(TARGET).iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: boot/boot.asm | $(BUILD)
	nasm -f elf32 $< -o $@

$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/keyboard.o: kernel/keyboard.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.elf: $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/keyboard.o linker.ld
	gcc $(LDFLAGS) -o $@ $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/keyboard.o -lgcc

$(BUILD)/isodir/boot/$(TARGET).elf: $(BUILD)/kernel.elf grub.cfg
	mkdir -p $(BUILD)/isodir/boot/grub
	cp $(BUILD)/kernel.elf $@
	cp grub.cfg $(BUILD)/isodir/boot/grub/grub.cfg

$(BUILD)/$(TARGET).iso: $(BUILD)/isodir/boot/$(TARGET).elf
	grub-mkrescue -o $@ $(BUILD)/isodir

run: $(BUILD)/$(TARGET).iso
	qemu-system-i386 -cdrom $(BUILD)/$(TARGET).iso

clean:
	rm -rf $(BUILD)
