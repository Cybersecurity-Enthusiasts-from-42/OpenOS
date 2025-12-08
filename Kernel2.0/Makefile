TARGET = openos

CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -fno-pic -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs
ASFLAGS = $(CFLAGS)

all: $(TARGET).bin

boot.o: boot.S
	$(CC) $(ASFLAGS) -c $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).bin: boot.o kernel.o linker.ld
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib boot.o kernel.o

run: $(TARGET).bin
	qemu-system-i386 -kernel $(TARGET).bin

clean:
	rm -f *.o $(TARGET).bin
