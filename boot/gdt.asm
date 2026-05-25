; ============================================
; GDT - Global Descriptor Table (standalone reference)
; ============================================
; The GDT is currently defined inline in boot.asm
; This file is kept as a reference for the GDT structure.
;
; GDT Entry Format (8 bytes each):
;   Bits 0-15:   Segment limit (low)
;   Bits 16-31:  Base address (low)
;   Bits 32-39:  Base address (middle)
;   Bits 40-47:  Access byte
;   Bits 48-51:  Segment limit (high)
;   Bits 52-55:  Flags
;   Bits 56-63:  Base address (high)
;
; Access Byte:
;   Bit 7:    Present (must be 1)
;   Bits 5-6: Privilege level (0 = kernel, 3 = user)
;   Bit 4:    Descriptor type (1 = code/data)
;   Bit 3:    Executable (1 = code, 0 = data)
;   Bit 2:    Direction/Conforming
;   Bit 1:    Readable (code) / Writable (data)
;   Bit 0:    Accessed
