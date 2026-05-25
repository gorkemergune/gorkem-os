# GorkemOS Makefile

# Tools
ASM      = nasm
CC       = i686-elf-gcc
LD       = i686-elf-ld
QEMU     = qemu-system-i386

# Flags
ASMFLAGS = -f elf32
CFLAGS   = -ffreestanding -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
           -nostartfiles -nodefaultlibs -Wall -Wextra -Iinclude -c
LDFLAGS  = -T linker.ld --oformat binary

# Output
OS_IMAGE = gorkem-os.bin

# Object files (order matters for linking!)
OBJS = kernel/kernel_entry.o \
       kernel/main.o \
       kernel/gdt.o \
       kernel/gdt_flush.o \
       kernel/idt.o \
       kernel/isr_stubs.o \
       kernel/pic.o \
       kernel/pmm.o \
       kernel/paging.o \
       kernel/heap.o \
       drivers/vga.o \
       drivers/timer.o \
       drivers/keyboard.o \
       lib/string.o \
       lib/print.o \
       shell/shell.o

# Build targets
all: $(OS_IMAGE)

# Assemble the bootloader (flat binary)
boot/boot.bin: boot/boot.asm
	$(ASM) -f bin $< -o $@

# Pattern rules for compiling
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Link kernel
kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Combine bootloader + kernel into final OS image
$(OS_IMAGE): boot/boot.bin kernel.bin
	cat $^ > $@
	@# Pad to at least 32 sectors (16 KB)
	@python3 -c "import os; size=os.path.getsize('$@'); f=open('$@','ab'); f.write(b'\0'*(16384-size) if size<16384 else b''); f.close()"

run: $(OS_IMAGE)
	$(QEMU) -m 32 -display cocoa,zoom-to-fit=on -drive format=raw,file=$(OS_IMAGE)

run-fullscreen: $(OS_IMAGE)
	$(QEMU) -m 32 -display cocoa,zoom-to-fit=on,full-screen=on -drive format=raw,file=$(OS_IMAGE)

run-nographic: $(OS_IMAGE)
	$(QEMU) -m 32 -drive format=raw,file=$(OS_IMAGE) -nographic

debug: $(OS_IMAGE)
	$(QEMU) -m 32 -display cocoa,zoom-to-fit=on -drive format=raw,file=$(OS_IMAGE) -s -S &
	@echo "Connect GDB with: gdb -ex 'target remote localhost:1234'"

clean:
	rm -f boot/*.bin
	rm -f kernel/*.o
	rm -f drivers/*.o
	rm -f lib/*.o
	rm -f shell/*.o
	rm -f kernel.bin
	rm -f $(OS_IMAGE)

.PHONY: all run run-nographic debug clean
