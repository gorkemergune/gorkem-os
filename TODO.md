# GorkemOS - Development Roadmap

## Project Goal
Build an x86 operating system from scratch in C and Assembly to learn low-level programming,
hardware-software interaction, and OS architecture.

---

## PHASE 0: PREREQUISITES AND ENVIRONMENT SETUP

- [✔] **0.1** Review C programming (pointers, structs, bitwise operations)
      - Write: simple linked list, bit manipulation examples
- [✔] **0.2** Learn basic x86 Assembly (NASM)
      - Write: "Hello World" in Assembly
      - Learn: `mov`, `jmp`, `cmp`, `int`, `push`, `pop` instructions
- [✔] **0.3** Set up development environment
      - Install:
        - `nasm` (assembler)
        - `qemu` (virtual machine for testing)
        - `i686-elf-gcc` (cross-compiler)
        - `make` (build system)
        - `gdb` (debugger)
      - Write: `Makefile` template
- [✔] **0.4** First QEMU test
      - Create an empty disk image, boot it in QEMU, observe what happens
- [✔] **0.5** Initialize git repo
      - Set up `.gitignore` (exclude *.o, *.bin, *.iso)
      - First commit: "Initial project setup"

---

## PHASE 1: THEORY — HARDWARE AND ABSTRACTION LAYERS
> Corresponds to book chapters 1-3

- [ ] **1.1** Study the physical implementation of a bit
      - Understand the chain: Transistor -> Logic Gate -> Machine Language
- [ ] **1.2** Understand digital logic gates (AND, OR, NOT, XOR)
      - Draw simple logic circuit diagrams on paper
- [ ] **1.3** Grasp the relationship between machine language and Assembly
      - Write: the same program in C, Assembly, and machine code side by side
- [ ] **1.4** Learn abstraction layers
      - Hardware -> ISA -> Assembly -> C -> OS relationship
- [ ] **1.5** Study x86 architecture
      - Registers (EAX, EBX, ECX, EDX, ESP, EBP, EIP, EFLAGS)
      - Memory addressing modes
      - Protection rings (Ring 0 — Ring 3)
      - Write: a reference diagram of x86 registers

---

## PHASE 2: x86 ASSEMBLY AND C IN PRACTICE
> Corresponds to book chapters 4-6

- [ ] **2.1** Learn to use objdump
      - Compile a simple C program, inspect it with objdump
- [ ] **2.2** Understand the anatomy of an Assembly instruction
      - Concepts: opcode, operand, addressing mode
- [ ] **2.3** Practice reading Intel manuals
      - Look up `jmp`, `mov`, `int` instructions in the official manual
- [ ] **2.4** Write practice programs in Assembly
      - Write: a program that adds two numbers
      - Write: a loop counting from 1 to 10
      - Write: a comparison and branching example
- [ ] **2.5** Inspect compiled C code
      - Write: simple C functions, then examine the Assembly output with `gcc -S`
- [ ] **2.6** Understand the ELF file format
      - ELF header, section header table, program header table
      - Inspect a binary with `readelf`
- [ ] **2.7** Learn debugging with GDB
      - Breakpoints, stepping, examining memory, inspecting registers

---

## PHASE 3: WRITING THE BOOTLOADER
> Corresponds to book chapter 7 — FIRST MAJOR MILESTONE

- [✔] **3.1** Understand the x86 boot process
      - BIOS -> MBR (first 512 bytes) -> Bootloader chain
      - The 0x7C00 address and its significance
      - Magic number: 0xAA55
- [✔] **3.2** Learn BIOS services
      - INT 0x10 (display), INT 0x13 (disk), INT 0x15 (memory)
- [✔] **3.3** Write the first bootloader (Assembly)
      - File: `boot/boot.asm`
      - A 512-byte bootloader that prints "Booting GorkemOS..."
      - Test in QEMU
- [✔] **3.4** Load a program from the bootloader
      - Bootloader that loads the kernel from disk into memory
      - File: `boot/boot.asm` (extended)
- [✔] **3.5** Transition from Real Mode to Protected Mode
      - Create a GDT (Global Descriptor Table)
      - Write the 16-bit to 32-bit mode switch code
      - File: `boot/gdt.asm`
- [✔] **3.6** Set up the build system
      - File: `Makefile`
      - `make run` compiles and launches QEMU automatically

---

## PHASE 4: LINKING AND BARE METAL PROGRAMMING
> Corresponds to book chapter 8

- [✔] **4.1** Understand relocations
      - Inspect relocation tables with `readelf`
- [✔] **4.2** Learn to write linker scripts
      - File: `linker.ld`
      - Define where the kernel gets loaded in memory
- [✔] **4.3** Understand freestanding C
      - Hosted vs Freestanding difference
      - Writing C code WITHOUT the standard library
      - `-ffreestanding`, `-nostdlib` flags
- [✔] **4.4** Write the first bare metal C program
      - File: `kernel/main.c`
      - Print a message by writing directly to the VGA text buffer
- [✔] **4.5** Bootloader + Kernel integration
      - Bootloader loads kernel and hands over control
      - Test the full boot chain end to end

---

## PHASE 5: KERNEL FUNDAMENTALS
> Corresponds to book chapter 9

- [✔] **5.1** Understand kernel-space vs user-space
      - Ring 0 (kernel) vs Ring 3 (user) separation
- [✔] **5.2** Implement segment descriptors
      - Rewrite GDT in C
      - Files: `kernel/gdt.c`, `include/gdt.h`
- [✔] **5.3** Write the display module (VGA driver)
      - Files: `drivers/vga.c`, `include/vga.h`
      - Functions: `vga_print()`, `vga_clear()`, `vga_newline()`
      - Color support
      - Screen scrolling
- [✔] **5.4** Write string utilities (your own libc)
      - Files: `lib/string.c`, `include/string.h`
      - `strlen`, `strcmp`, `strcpy`, `memset`, `memcpy`
- [✔] **5.5** Write number-to-string conversion
      - `itoa`, a simple `printf`-like function (`kprintf`)
      - File: `lib/print.c`

---

## PHASE 6: INTERRUPT HANDLING
> Corresponds to book chapter 11

- [✔] **6.1** Understand interrupts
      - Hardware interrupt vs Software interrupt
      - What is an IRQ, what is PIC (Programmable Interrupt Controller)
- [✔] **6.2** Create the IDT (Interrupt Descriptor Table)
      - Files: `kernel/idt.c`, `include/idt.h`
      - 256 interrupt entries
- [✔] **6.3** Write ISRs (Interrupt Service Routines)
      - Files: `kernel/idt.c`, `kernel/isr_stubs.asm`
      - CPU exception handlers (division by zero, page fault, etc.)
- [✔] **6.4** Program the PIC
      - Configure the 8259 PIC
      - Remap IRQs (0-15 -> 32-47)
      - File: `kernel/pic.c`
- [✔] **6.5** Set up the timer (PIT)
      - Configure the Programmable Interval Timer (100 Hz)
      - Tick counter for basic timekeeping
      - File: `drivers/timer.c`
- [✔] **6.6** Write the keyboard driver
      - File: `drivers/keyboard.c`
      - Scan code -> ASCII conversion (US layout)
      - Circular buffer for keyboard input
      - Shift key support

---

## PHASE 7: MEMORY MANAGEMENT
> Corresponds to book chapter 12

- [✔] **7.1** Understand physical memory management
      - Read the memory map from BIOS
      - Usable vs reserved memory regions
- [✔] **7.2** Write the physical memory manager (PMM)
      - Files: `kernel/pmm.c`, `include/pmm.h`
      - Bitmap-based page allocator
      - `pmm_alloc_page()` and `pmm_free_page()` functions
- [✔] **7.3** Set up paging
      - Page Directory and Page Table structures
      - Configure the CR3 register
      - Identity map first 16 MB
      - Files: `kernel/paging.c`, `include/paging.h`
- [✔] **7.4** Write the virtual memory manager (VMM)
      - Virtual address -> Physical address mapping (identity mapped)
      - Page fault handler with error reporting
- [✔] **7.5** Write the heap allocator (kmalloc)
      - File: `kernel/heap.c`
      - `kmalloc()`, `kfree()` functions
      - First-fit allocator with block coalescing

---

## PHASE 8: PROCESS MANAGEMENT
> Corresponds to book chapter 10

- [ ] **8.1** Understand the process concept
      - PCB (Process Control Block) structure
      - Process states: Ready, Running, Waiting, Terminated
- [ ] **8.2** Create the task/process structure
      - Files: `kernel/process.c`, `kernel/process.h`
      - Per process: pid, register state, memory map, status
- [ ] **8.3** Write context switching
      - Save/restore registers
      - Stack switching
      - File: `kernel/switch.asm`
- [ ] **8.4** Write a basic scheduler
      - Round-robin scheduling algorithm
      - File: `kernel/scheduler.c`
- [ ] **8.5** Multitasking demo
      - Show two simple tasks running in alternation

---

## PHASE 9: FILE SYSTEM
> Corresponds to book chapter 13

- [ ] **9.1** Understand file system concepts
      - Inode, superblock, directory entry
- [ ] **9.2** Design a simple file system (GorkemFS)
      - Define your own file system format
      - File: `fs/gorkemfs.h` (data structures)
- [ ] **9.3** Write the disk I/O driver
      - ATA/IDE disk read/write
      - File: `drivers/ata.c`
- [ ] **9.4** Implement file operations
      - File: `fs/gorkemfs.c`
      - `fs_open()`, `fs_read()`, `fs_write()`, `fs_close()`
      - Directory listing

---

## PHASE 10: USER SHELL
> Not in the book — your original contribution

- [✔] **10.1** Write the command line interface
       - File: `shell/shell.c`
       - Prompt: `gorkem> `
       - Read-parse-execute loop
- [✔] **10.2** Implement basic commands
       - `clear` — clear the screen
       - `help` — list available commands
       - `about` — show OS info
       - `mem` — display memory status (with visual bar)
       - `uptime` — show system uptime
       - `echo` — echo text
       - `reboot` — restart the system
       - `shutdown` — halt the system
- [ ] **10.3** Add file commands (once file system is ready)
       - `ls` — list directory contents
       - `cat <file>` — display file contents
       - `write <file>` — write to a file

---

## PHASE 11: EXTRAS AND POLISH

- [✔] **11.1** Design the boot screen
       - ASCII art "GorkemOS" logo
       - Show version number
       - Boot status messages with [OK] indicators
- [✔] **11.2** Colored terminal output
       - Support different text colors (16 VGA colors)
       - Error messages in red, success messages in green
       - Colored prompt (green username, cyan separator)
- [ ] **11.3** System info command
       - CPU info (via CPUID instruction)
       - Total/used memory
       - Uptime
- [ ] **11.4** Add a simple game
       - Snake or number guessing game
       - A mini game running inside the kernel
- [ ] **11.5** Sound output (PC Speaker)
       - Produce beep sounds
       - Play a simple melody (boot jingle)

---

## TARGET PROJECT STRUCTURE

```
gorkem-os/
├── boot/
│   ├── boot.asm          # Bootloader (Assembly)
│   └── gdt.asm           # GDT reference (Assembly)
├── kernel/
│   ├── kernel_entry.asm   # Kernel entry point (Assembly)
│   ├── main.c            # Kernel main
│   ├── gdt.c             # GDT (C side)
│   ├── gdt_flush.asm     # GDT reload (Assembly)
│   ├── idt.c             # Interrupt Descriptor Table + handlers
│   ├── isr_stubs.asm     # ISR/IRQ Assembly stubs
│   ├── pic.c             # PIC controller
│   ├── pmm.c             # Physical memory manager
│   ├── paging.c          # Paging
│   ├── heap.c            # Dynamic memory (kmalloc)
│   ├── process.c         # Process management
│   ├── scheduler.c       # Scheduler
│   └── switch.asm        # Context switch
├── drivers/
│   ├── vga.c             # Display driver
│   ├── keyboard.c        # Keyboard driver
│   ├── timer.c           # Timer (PIT)
│   └── ata.c             # Disk driver
├── fs/
│   ├── gorkemfs.h        # File system data structures
│   └── gorkemfs.c        # File system implementation
├── lib/
│   ├── string.c          # String functions
│   └── print.c           # printf-like function (kprintf)
├── shell/
│   └── shell.c           # Command line interface
├── include/              # Header files (.h)
│   ├── types.h           # Basic type definitions
│   ├── io.h              # I/O port access
│   ├── vga.h             # VGA driver header
│   ├── string.h          # String utilities header
│   ├── print.h           # Print utilities header
│   ├── gdt.h             # GDT header
│   ├── idt.h             # IDT header
│   ├── isr.h             # ISR declarations
│   ├── timer.h           # Timer header
│   ├── keyboard.h        # Keyboard header
│   ├── pmm.h             # PMM header
│   ├── paging.h          # Paging header
│   ├── heap.h            # Heap header
│   └── shell.h           # Shell header
├── linker.ld             # Linker script
├── makefile              # Build system
├── TODO.md               # This file
└── README.md             # Project overview
```

---

## RESOURCES

- [Book] Operating Systems: From 0 to 1 — Tu, Do Hoang (included in project)
- [Wiki] https://wiki.osdev.org — most comprehensive OS dev resource
- [Repo] https://github.com/cfenollosa/os-tutorial — step-by-step OS tutorial
- [Docs] Intel Software Developer Manual (x86 reference)
- [Forum] https://forum.osdev.org — for questions

---

## NOTES

- Commit after completing each phase. Track your progress.
- Test with QEMU constantly. Run after every new feature.
- Don't panic when you hit errors. Bugs are normal in OS dev — use GDB.
- Follow the book in parallel but you don't have to follow it 1:1.
- Add at least one original feature in each phase.
