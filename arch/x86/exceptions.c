/*
 * OpenOS - Exception Handler Implementation
 * Handles CPU exceptions with detailed error reporting
 */

#include "exceptions.h"
#include "idt.h"
#include <stddef.h>

/* Forward declarations */
void console_write(const char* s);
void console_put_char(char c);

/* Exception names for error reporting */
static const char* exception_messages[] = {
    "Divide by Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 FPU Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

/* Helper function to print a hexadecimal number */
static void print_hex(uint32_t value) {
    const char hex_digits[] = "0123456789ABCDEF";
    char buffer[11];  /* "0x" + 8 hex digits + null terminator */
    buffer[0] = '0';
    buffer[1] = 'x';
    
    for (int i = 9; i >= 2; i--) {
        buffer[i] = hex_digits[value & 0xF];
        value >>= 4;
    }
    buffer[10] = '\0';
    
    console_write(buffer);
}

/* Helper function to print a decimal number */
static void print_dec(uint32_t value) {
    if (value == 0) {
        console_put_char('0');
        return;
    }
    
    char buffer[11];  /* Max 10 digits + null terminator */
    int i = 10;
    buffer[i] = '\0';
    
    while (value > 0 && i > 0) {
        i--;
        buffer[i] = '0' + (value % 10);
        value /= 10;
    }
    
    console_write(&buffer[i]);
}

/*
 * Main exception handler called from assembly stubs
 */
void exception_handler(struct exception_registers *regs) {
    /* Just halt immediately without trying to print anything */
    (void)regs;  /* Suppress unused parameter warning */
    
    __asm__ __volatile__("cli");
    while(1) {
        __asm__ __volatile__("hlt");
    }
}

/*
 * Initialize exception handlers by installing them in the IDT
 */
void exceptions_init(void) {
    /* Install all 32 exception handlers in the IDT */
    /* Use kernel code segment (0x08) and interrupt gate flags (0x8E) */
    idt_set_gate(0, (uint32_t)exception_0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)exception_1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)exception_2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)exception_3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)exception_4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)exception_5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)exception_6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)exception_7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)exception_8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)exception_9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)exception_10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)exception_11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)exception_12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)exception_13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)exception_14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)exception_15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)exception_16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)exception_17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)exception_18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)exception_19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)exception_20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)exception_21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)exception_22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)exception_23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)exception_24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)exception_25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)exception_26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)exception_27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)exception_28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)exception_29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)exception_30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)exception_31, 0x08, 0x8E);
}
