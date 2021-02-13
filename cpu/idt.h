#ifndef IDT_H
#define IDT_H

#include "types.h"

/* Segment selectors */
#define KERNEL_CS 0x08

/* Struct describing interrupt gate (handler) is defined */
struct idt_entry_struct
{
    u16 low_offset; /* Lower 16 bits of handler function address
                    i.e. address to jump when this interrup fires */

    u16 sel; /* Kernel segment selector */

    u8 always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */

    u8 flags;
    /*
    * The lower 5-bits should be constant at 01110b-14 in decimal.
    * The DPL describes the privilege level we expect to be called
    * from - in our case zero, 
    * but as we progress we'll have to change that to 3.
    * The P bit signifies the entry is present.
    * Any descriptor with this bit clear will cause
    * a "Interrupt Not Handled" exception.
    */

    u16 high_offset; /* Higher 16 bits of handler function address */
} __attribute__((packed));

typedef struct idt_entry_struct idt_gate_t;

/*
 * A struct describing a pointer to an array of interrupt handler
 * This is a format suitable for giving to 'lidt'
 */

struct idt_ptr_struct {
    u16 limit;  /* The total size of the idt array */
    u32 base;   /* The address of the first element in out idt array*/
} __attribute__((packed));

typedef struct idt_ptr_struct idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

/* Functions implemented in idt.c */
void set_idt_gate(int n, u32 handler);
void set_idt();

#endif