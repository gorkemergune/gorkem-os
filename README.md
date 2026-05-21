# GorkemOS

A hobby operating system built from scratch for the x86 architecture. Written in C and Assembly as a learning project to understand how computers work at the lowest level.

## Goals

- Write a bootloader that loads the kernel from disk
- Implement interrupt handling (keyboard, timer, exceptions)
- Build a physical and virtual memory manager
- Create a simple process scheduler with multitasking
- Design a basic file system
- Build an interactive shell

## Building

### Prerequisites

- `nasm` - assembler
- `qemu` - emulator for testing
- `i686-elf-gcc` - cross compiler
- `make` - build system

### Build & Run

```bash
make        # build the OS
make run    # build and launch in QEMU
make clean  # remove build artifacts
```

## Project Structure

```
boot/       - bootloader (assembly)
kernel/     - kernel core (GDT, IDT, memory, scheduler)
drivers/    - hardware drivers (VGA, keyboard, timer, disk)
fs/         - file system
lib/        - utility functions (string, print)
shell/      - command line interface
include/    - header files
```

## Resources

- [OSDev Wiki](https://wiki.osdev.org)
- Operating Systems: From 0 to 1 - Tu, Do Hoang
- Intel Software Developer Manual

